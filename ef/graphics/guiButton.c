#include <ef/guiButton.h>
#include <ef/memory.h>
#include <ef/err.h>

guiButton_s* gui_button_new(guiLabel_s* lbl, guiEvent_f onclick){
	if( !lbl ) return NULL;
	guiButton_s* btn = mem_new(guiButton_s);
	if( !btn ) return NULL;
	btn->label = lbl;
	btn->onclick = onclick;
	btn->bkpress.color = 0 ;
	btn->bkpress.img = NULL;
	btn->bkpress.mode = GUI_BK_COLOR;
	return btn;
}

gui_s* gui_button_attach(gui_s* gui, guiButton_s* btn){
	if( !gui ) goto ERR;
	if( !btn ) goto ERR;
	gui->control = btn;
	gui->type = GUI_TYPE_BUTTON;
	gui->redraw = gui_button_event_redraw;
	gui->mouse = gui_button_event_mouse;
	gui->key = gui_button_event_key;
	gui->focus = gui_event_focus;
	gui->free = gui_button_event_free;
	return gui;
ERR:
	if( btn ) gui_button_free(btn);
	if( gui ) gui_free(gui);
	return NULL;
}

void gui_button_free(guiButton_s* btn){
	gui_label_free(btn->label);
	free(btn);
}

guiLabel_s* gui_button_label(guiButton_s* button){
	return button->label;
}

void gui_button_redraw(gui_s* gui, guiButton_s* btn, int press){
	if( press ){
		gui_label_redraw(gui, &btn->bkpress, btn->label);
	}
	else{
		gui_label_redraw(gui, &gui->background, btn->label);
	}
}

int gui_button_event_free(gui_s* gui, __unused xorgEvent_s* ev){
	iassert(gui->type == GUI_TYPE_BUTTON);
	gui_button_free(gui->control);
	return 0;
}

int gui_button_event_redraw(gui_s* gui, __unused xorgEvent_s* unset){
	iassert(gui->type == GUI_TYPE_BUTTON);
	gui_button_redraw(gui, gui->control, 0);
	return 0;
}

int gui_button_event_key(gui_s* gui, xorgEvent_s* event){
	iassert(gui->type == GUI_TYPE_BUTTON);

	if( event->keyboard.event == XORG_KEY_PRESS && event->keyboard.keysym == XKB_KEY_Return ){
		gui_button_redraw(gui, gui->control, 1);
		gui_draw(gui);
		return 0;
	}
	else if( event->keyboard.event == XORG_KEY_RELEASE && event->keyboard.keysym == XKB_KEY_Return ){
		gui_button_redraw(gui, gui->control, 0);
		gui_draw(gui);
		guiButton_s* btn = gui->control;
		if( btn->onclick ) btn->onclick(gui, event);
		return 0;
	}

	gui_event_key(gui, event);
	return 0;
}

int gui_button_event_mouse(gui_s* gui, xorgEvent_s* event){
	iassert(gui->type == GUI_TYPE_BUTTON);

	if( event->mouse.event == XORG_MOUSE_PRESS && event->mouse.button == 1 ){
		gui_button_redraw(gui, gui->control, 1);
		gui_draw(gui);
		return 0;
	}
	else if( (event->mouse.event == XORG_MOUSE_RELEASE || event->mouse.event == XORG_MOUSE_CLICK || event->mouse.event == XORG_MOUSE_DBLCLICK) 
			&& event->mouse.button == 1 )
	{
		gui_button_redraw(gui, gui->control, 0);
		gui_draw(gui);
		guiButton_s* btn = gui->control;
		if( btn->onclick ) btn->onclick(gui, event);
	}
	return 0;
}

int gui_event_focus(gui_s* gui, xorgEvent_s* event){
	if( event->focus.outin ){
		gui_border(gui, gui->bordersizefocused);
	}
	else{
		gui_border(gui, gui->bordersize);
	}
	return 0;
}


