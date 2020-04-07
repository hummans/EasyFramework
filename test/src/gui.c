#include "test.h"
#include <ef/ft.h>
#include <ef/image.h>
#include <ef/imageFiles.h>
#include <ef/xorg.h>
#include <ef/os.h>
#include <ef/utf8.h>
#include <ef/deadpoll.h>

/*@test -g --gui 'test gui'*/

typedef void (*imgDraw_f)(g2dImage_s* img);
typedef int (*xev_f)(xorgEvent_s* ev);

g2dImage_s* bkimg;

typedef struct xwin{
	xorg_s* x;
	xorgSurface_s* surf;
	xcb_window_t id;
	int state;
	int px,py;
	imgDraw_f redraw;
	xev_f draw;
	xev_f key;
	xev_f move;
	xev_f mouse;
	struct xwin* child;
}xwin_s;

__private int child_key(xorgEvent_s* ev){
	if( *ev->keyboard.utf8 == '\x1B' ){
		dbg_info("destroy simple app");
		xorg_send_destroy(ev->x, ev->win);
	}
	return 0;
}

__private int main_key(xorgEvent_s* ev){
	xwin_s* win = ev->userdata;

	if( *ev->keyboard.utf8 == '\x1B' ){
		dbg_info("destroy simple app");
		xorg_send_destroy(ev->x, ev->win);
	}
	else if( ev->keyboard.keycode == 113 ){
		dbg_info("left");
		win->child->px -= 5;
		xorg_win_move(win->x, win->child->id, win->child->px, win->child->py);
	}
	else if( ev->keyboard.keycode == 114 ){
		dbg_info("right");
		win->child->px += 5;
		xorg_win_move(win->x, win->child->id, win->child->px, win->child->py);
	}
	else if( ev->keyboard.keycode == 111 ){
		dbg_info("up");
		win->child->py -= 5;
		xorg_win_move(win->x, win->child->id, win->child->px, win->child->py);
	}
	else if( ev->keyboard.keycode == 116 ){
		dbg_info("down");
		win->child->py += 5;
		xorg_win_move(win->x, win->child->id, win->child->px, win->child->py);
	}
	return 0;
}

__private int main_mouse_move(xorgEvent_s* ev){
	xwin_s* win = ev->userdata;

	win->child->px = ev->mouse.relative.x;
	win->child->py = ev->mouse.relative.y;
	xorg_win_move(win->x, win->child->id, win->child->px, win->child->py);

	return 0;
}


__private int main_move(xorgEvent_s* ev){
	xwin_s* win = ev->userdata;
	if( ev->move.coord.w != win->surf->img->w || ev->move.coord.h != win->surf->img->h){
		dbg_info("redraw because: %d != %d && %d != %d",  ev->move.coord.w, win->surf->img->w, ev->move.coord.h, win->surf->img->h);
	   	xorg_surface_resize(win->surf, ev->move.coord.w, ev->move.coord.h);
		win->redraw(win->surf->img);
		xorg_win_surface_redraw(win->x, win->id, win->surf);
	}
	return 0;
}

__private int main_draw(xorgEvent_s* ev){
	xwin_s* win = ev->userdata;
	xorg_win_surface_redraw(ev->x, ev->win, win->surf);
	return 0;
}

__private void main_redraw(g2dImage_s* img){
	g2dColor_t bkcol = g2d_color_gen(X_COLOR_MODE, 255, 125,125,125); 
	g2dCoord_s gw = { .x = 0, .y = 0, .w = img->w, .h = img->h };
	g2d_clear(img, bkcol, &gw);
}

__private void child_redraw(g2dImage_s* img){
	__g2d_free g2dImage_s* resize = g2d_resize(bkimg, img->w, img->h);
	g2dCoord_s s = { .x = 0, .y =0, .w =img->w, .h = img->h };
	g2dCoord_s d = { .x = 0, .y =0, .w =img->w, .h = img->h };
	g2d_bitblt(img, &d, resize, &s);	
//	g2d_bitblt(img, &d, bkimg, &s);

}

xwin_s* main_win(xorg_s* x){
	xwin_s* win = mem_new(xwin_s);
	win->x = x;

	g2dCoord_s pos = { 
		.x = 100,
		.y = 100,
		.w = 800,
		.h = 600,
	};
	g2dColor_t bkcol = g2d_color_gen(X_COLOR_MODE, 255, 125,125,125); 
	win->id = xorg_win_new(&win->surf, win->x, xorg_root(win->x), &pos, 1, bkcol);
	win->redraw = main_redraw;
	win->move = main_move;
	win->key = main_key;
	win->draw = main_draw;
	win->mouse = main_mouse_move;
	win->child = NULL;

	win->child = mem_new(xwin_s);
	win->child->px = 30;
	win->child->py = 20;
	win->child->x = x;
	win->child->draw = main_draw;
	win->child->key = child_key;
	win->child->move = NULL;
	win->child->mouse = NULL;
	win->child->child = NULL;

	pos.x = win->child->px;
	pos.y = win->child->py;
	pos.w = 400;
	pos.h = 300;
	bkcol = g2d_color_gen(X_COLOR_MODE, 255, 0, 0, 0); 
	win->child->id = xorg_win_new(&win->child->surf, win->x, win->id, &pos, 0, bkcol);
	win->child->redraw = child_redraw;

	win->redraw(win->surf->img);
	win->child->redraw(win->child->surf->img);
	
	xorg_win_surface_redraw(x, win->id, win->surf);
	xorg_win_surface_redraw(x, win->child->id, win->child->surf);

	xorg_win_show(win->x, win->id, 1);
	xorg_win_show(win->x, win->child->id, 1);

//	xorg_client_flush(win->x);
//	xorg_client_sync(win->x);

	return win;
}

__private err_t x_events(__unused deadpoll_s* dp, __unused int ev, void* arg){
	xwin_s* win = arg;
	xwin_s* sel;

	xorgEvent_s* event;
	while( (event=xorg_event_new(win->x, 1)) ){
		if( event->win == win->id ){
			sel = win;
		}
		else if( event->win == win->child->id ){
			sel = win->child;
		}
		else{
			xorg_event_free(event);
			continue;
		}
		event->userdata = sel;

		switch( event->type ){
			case XCB_DESTROY_NOTIFY:
				return -1;
			break;

			case XCB_EXPOSE:
				if( sel->draw ) sel->draw(event);
			break;

			case XCB_KEY_RELEASE:
				if( sel->key ) sel->key(event);
			break;
		
			case XCB_MOTION_NOTIFY:
				if( sel->mouse ) sel->mouse(event);
			break;

			case XCB_CONFIGURE_NOTIFY:
				if( sel->move ) sel->move(event);
			break;

		}
		xorg_event_free(event);
	}
	xorg_client_flush(win->x);

	return 0;
}

/*@fn*/
void test_gui(__unused const char* argA, __unused const char* argB){
	err_enable();
	utf_begin();
	os_begin();	
	ft_begin();

	bkimg = g2d_load("/home/vbextreme/Immagini/180245.gif");
	if( !bkimg ){
		err_fail("loading image");
	}

	deadpoll_s* dp = deadpoll_new();

	xorg_s* x = xorg_client_new(NULL, 0);
	xorg_register_events(x, xorg_root(x), XCB_EVENT_MASK_PROPERTY_CHANGE);
	
	xwin_s* win = main_win(x);
	
	deadpoll_register(dp, xorg_fd(x), x_events, win, 0, NULL);
	deadpoll_loop(dp, -1);
	deadpoll_free(dp);
	
	ft_end();
	//xorg_surface_destroy(x, win.surf);
	xorg_client_free(x);
	err_restore();
}

