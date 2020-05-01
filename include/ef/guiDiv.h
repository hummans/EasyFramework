#ifndef __EF_GUI_DIV_H__
#define __EF_GUI_DIV_H__

#include <ef/gui.h>

typedef enum {
	GUI_DIV_NONE,
	GUI_DIV_VERTICAL,
	GUI_DIV_HORIZONTAL,
	GUI_DIV_TABLE
}guiDivMode_e;

#define GUI_DIV_TABLE_FIT 0x01

typedef struct guiDivCols{
	gui_s* gui;
	double propw;
	unsigned flags;
}guiDivCols_s;

typedef struct guiDivRow{
	double proph;
	guiDivCols_s* vcols;
}guiDivRow_s;

typedef struct guiDiv{
	guiDivMode_e mode;
	guiMargin_s padding;
	g2dPoint_s scroll;
	guiDivRow_s* vrows;
}guiDiv_s;

#define GUI_DIV_DEFAULT_PADDING 5

#define GUI_THEME_DIV_ALIGN "div.align"
#define GUI_THEME_DIV_SEP_X "div.sep.x"
#define GUI_THEME_DIV_SEP_Y "div.sep.y"


/** create new div*/
guiDiv_s* gui_div_new(guiDivMode_e mode);

/** attach div to gui*/
gui_s* gui_div_attach(gui_s* gui, guiDiv_s* div);

/** free div*/
void gui_div_free(guiDiv_s* div);

void gui_div_padding_top(gui_s* gui, int top);
void gui_div_padding_bottom(gui_s* gui, int bottom);
void gui_div_padding_left(gui_s* gui, int left);
void gui_div_padding_right(gui_s* gui, int right);
void gui_div_table_create_row(gui_s* tab, double raph, unsigned cols);
void gui_div_table_child_attach(gui_s* tab, unsigned idrow, unsigned idcol, gui_s* child);
void gui_div_table_row_prop(gui_s* tab, unsigned idrow, double prop);
void gui_div_table_col_prop(gui_s* tab, unsigned idrow, unsigned idcol, double prop);

/** realign div*/
void gui_div_align(gui_s* gui);

/** event free*/
int gui_div_event_free(gui_s* gui, __unused xorgEvent_s* ev);

/** event key*/
int gui_div_child_event_key(gui_s* gui, xorgEvent_s* event);


#endif
