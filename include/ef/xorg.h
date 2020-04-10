#ifndef __EF_XORG_H__
#define __EF_XORG_H__

#include <ef/type.h>
#include <ef/image.h>
#include <ef/utf8.h>
#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <xcb/xcb_aux.h>
#include <xcb/xcb_image.h>
#include <xcb/xcb_atom.h>
#include <xcb/xcb_icccm.h>
#include <xcb/composite.h>
#include <xkbcommon/xkbcommon.h>

#ifdef XCB_ERROR_ENABLE
	#include <xcb/xcb_errors.h>
#endif

#define XKB_UTF_MAX 32

#define X_COLOR_MODE G2D_MODE_ARGB

#define X_WIN_EVENT XCB_EVENT_MASK_EXPOSURE |\
	XCB_EVENT_MASK_KEY_PRESS |\
	XCB_EVENT_MASK_KEY_RELEASE |\
	XCB_EVENT_MASK_BUTTON_PRESS |\
	XCB_EVENT_MASK_BUTTON_RELEASE |\
	XCB_EVENT_MASK_POINTER_MOTION |\
	XCB_EVENT_MASK_ENTER_WINDOW |\
	XCB_EVENT_MASK_LEAVE_WINDOW |\
	XCB_EVENT_MASK_VISIBILITY_CHANGE |\
	XCB_EVENT_MASK_STRUCTURE_NOTIFY |\
	XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |\
	XCB_EVENT_MASK_PROPERTY_CHANGE

#define XORG_MOUSE_CLICK_MS 200
#define XORG_MOUSE_DBLCLICL_MS 350

typedef enum {
	XORG_ATOM_NET_ACTIVE_WINDOW = 0,
	XORG_ATOM_NET_NUMBER_OF_DESKTOPS,
	XORG_ATOM_NET_CURRENT_DESKTOP,
	XORG_ATOM_NET_DESKTOP_NAMES,
	XORG_ATOM_NET_ACTIVE_DESKTOP,
	XORG_ATOM_NET_WM_DESKTOP, 
	XORG_ATOM_NET_WM_WINDOW_TYPE, 
	XORG_ATOM_NET_WM_STATE, 
	XORG_ATOM_NET_WM_VISIBLE_NAME, 
	XORG_ATOM_NET_WM_NAME,
	XORG_ATOM_NET_WM_STRUT,
	XORG_ATOM_NET_WM_STRUT_PARTIAL,
	XORG_ATOM_NET_WM_PID,
	XORG_ATOM_NET_WM_WINDOW_TYPE_DOCK,
	XORG_ATOM_XROOTPMAP_ID,
	XORG_ATOM_UTF8_STRING,
	XORG_ATOM_COUNT,
}xorgAtom_e;

typedef struct xkb{
	struct xkb_context* ctx;
	struct xkb_keymap* keymap;
	int device;
}xkb_s;

typedef struct xorgSurface{
	xcb_gcontext_t gc;
	xcb_image_t* ximage;
	g2dImage_s* img;
}xorgSurface_s;

typedef struct monitor{
	char* name;
	bool_t connected;
	g2dCoord_s size;
}monitor_s;

typedef struct xorg{
	xcb_connection_t* connection;
	xcb_screen_t* screen;
	monitor_s* monitor;
	size_t monitorCount;
	monitor_s* monitorCurrent;
	monitor_s* monitorPrimary;
	const char* display;
	xkb_s key;
#ifdef XCB_ERROR_ENABLE
	xcb_errors_context_t* err;
#endif
	xorgAtom_e atom[XORG_ATOM_COUNT];
	long clickms;
	long dblclickms;
	long _mousetime;
	unsigned _mousestate;
	int screenDefault;
	int screenCurrent;
}xorg_s;

#define XORG_WINDOW_HINTS_FLAGS_URGENCY(XWPTR)  ((XWPTR)->hints.flags & XCB_ICCCM_WM_HINT_X_URGENCY)
#define XORG_WINDOW_VISIBLE_UNMAP  XCB_MAP_STATE_UNMAPPED
#define XORG_WINDOW_VISIBLE_UNVIEW XCB_MAP_STATE_UNVIEWABLE
#define XORG_WINDOW_VISIBLE_MAP    XCB_MAP_STATE_VIEWABLE

struct xorgWindowStrut{
	unsigned left, right, top, bottom;
} __packed;
typedef struct xorgWindowStrut xorgWindowStrut_s;

struct xorgWindowStrutPartial{
	unsigned left, right, top, bottom;
	unsigned left_start_y, left_end_y;
	unsigned right_start_y, right_end_y;
	unsigned top_start_x, top_end_x;
	unsigned bottom_start_x,bottom_end_x;
} __packed;
typedef struct xorgWindowStrutPartial xorgWindowStrutPartial_s;

typedef struct xorgWindow{
	xcb_window_t idxcb;
	char* class;
	char* name;
	char* netname;
	char* title;
	xcb_icccm_wm_hints_t hints;
	xcb_size_hints_t size;
	unsigned x, y, w, h, border, visible;
	unsigned desktop;
	xcb_atom_t* type;
	size_t typeCount;
	xcb_atom_t* state;
	size_t stateCount;
	xorgWindowStrut_s strut;
	xorgWindowStrutPartial_s partial;
	pid_t pid;
}xorgWindow_s;

typedef enum {XORG_MOUSE_RELEASE, XORG_MOUSE_PRESS, XORG_MOUSE_MOVE, XORG_MOUSE_ENTER, XORG_MOUSE_LEAVE, XORG_MOUSE_CLICK, XORG_MOUSE_DBLCLICK} xorgMouse_e;
typedef enum {XORG_KEY_RELEASE, XORG_KEY_PRESS} xorgKey_e;
typedef enum {
	XORG_EVENT_CREATE = XCB_CREATE_NOTIFY,
	XORG_EVENT_DESTROY = XCB_DESTROY_NOTIFY,
	XORG_EVENT_DRAW = XCB_EXPOSE,
	XORG_EVENT_KEY_PRESS = XCB_KEY_PRESS,
	XORG_EVENT_KEY_RELEASE = XCB_KEY_RELEASE,
	XORG_EVENT_BUTTON_PRESS = XCB_BUTTON_PRESS,
	XORG_EVENT_BUTTON_RELEASE = XCB_BUTTON_RELEASE,
	XORG_EVENT_MOTION =	XCB_MOTION_NOTIFY,
	XORG_EVENT_ENTER = XCB_ENTER_NOTIFY,
	XORG_EVENT_LEAVE = XCB_LEAVE_NOTIFY,
	XORG_EVENT_FOCUS_IN = XCB_FOCUS_IN,
	XORG_EVENT_FOCUS_OUT = XCB_FOCUS_OUT,
	XORG_EVENT_MAP = XCB_MAP_NOTIFY,
	XORG_EVENT_UNMAP = XCB_UNMAP_NOTIFY,
	XORG_EVENT_MOVE = XCB_CONFIGURE_NOTIFY,
	XORG_EVENT_ATOM = XCB_PROPERTY_NOTIFY,
	XORG_EVENT_CLIENT = XCB_CLIENT_MESSAGE
}xorgEvent_e;

typedef struct xorgMouse{
	xorgMouse_e event;
	g2dPoint_s absolute;
	g2dPoint_s relative;
	unsigned button;
	unsigned key;
	long time;
}xorgMouse_s;

typedef struct xorgKeyboard{
	xorgKey_e event;
	g2dPoint_s absolute;
	g2dPoint_s relative;
	unsigned button;
	unsigned long keycode;
	unsigned long keysym;
	uint8_t utf8[XKB_UTF_MAX];
	long time;
}xorgKeyboard_s;

typedef struct xorgMove{
	g2dCoord_s coord;
	unsigned border;
}xorgMove_s;

typedef struct xorgCreate{
	int x;
	int y;
	int w;
	int h;
	int border;
}xorgCreate_s;

typedef struct xorgDraw{
	int x;
	int y;
	int w;
	int h;
}xorgDraw_s;

typedef struct xorgFocus{
	int outin;
}xorgFocus_s;

typedef struct xorgVisible{
	int visible;
}xorgVisible_s;

typedef struct xorgProperty{
	xcb_atom_t atom;
}xorgProperty_s;

typedef struct xorgClient{
	xcb_atom_t type;
	uint8_t format;
	uint8_t data[20];
}xorgClient_s;

typedef struct xorgEvent{
	int type;
	void* userdata;
	xorg_s* x;
	xcb_window_t win;
	union{
		xorgMouse_s mouse;
		xorgKeyboard_s keyboard;
		xorgMove_s move;
		xorgCreate_s create;
		xorgDraw_s draw;
		xorgFocus_s focus;
		xorgVisible_s visible;
		xorgProperty_s property;
		xorgClient_s client;
	};
}xorgEvent_s;	

#define xorg_root(XORG) ((XORG)->screen->root)
#define xorg_root_x(XORG) ((XORG)->monitorCurrent->size.x)
#define xorg_root_y(XORG) ((XORG)->monitorCurrent->size.y)
#define xorg_root_width(XORG) ((XORG)->monitorCurrent->size.w)
#define xorg_root_height(XORG) ((XORG)->monitorCurrent->size.h)
#define xorg_root_visual(XORG) ((XORG)->screen->root_visual)
#define xorg_fd(XORG) xcb_get_file_descriptor((XORG)->connection)

/** create new xorg client*/
xorg_s* xorg_client_new(const char* display, int defaultScreen);

/** free xorg client*/
void xorg_client_free(xorg_s* x);

/** initialized root to screen, this function is called inside client_new*/
err_t xorg_root_init(xorg_s* x, int onscreen);

/** flush a request, not really need*/
void xorg_client_flush(xorg_s* x);

/** sync connectoion, not really need*/
void xorg_client_sync(xorg_s* x);

#ifdef XCB_ERROR_ENABLE
const char* xorg_error_major(xorg_s* x, xcb_generic_error_t* err);
const char* xorg_error_minor(xorg_s* x, xcb_generic_error_t* err);
const char* xorg_error_string(xorg_s* x, xcb_generic_error_t* err, const char** extensionname);
#endif

/** return screen from idscreen*/
xcb_screen_t* xorg_screen_get(xorg_s* x, int idScreen);

/** refresh randr monitor*/
void xorg_randr_monitor_refresh(xorg_s* x);

/** set monitor by name*/
err_t xorg_monitor_byname(xorg_s* x, char const* name);

/** set monitor in position size*/
err_t xorg_monitor_bysize(xorg_s* x, g2dCoord_s* size);

/** set primary monitor*/
err_t xorg_monitor_primary(xorg_s* x);

/** get name from atom*/
const char* xorg_atom_name(xorg_s* x, xcb_atom_t atom);

/** get atom friom name*/
xcb_atom_t xorg_atom_id(xorg_s* x, const char* name);

/** create new atom*/
xcb_atom_t xorg_atom_new_id(xorg_s* x, const char* name);

/** load default atom*/
void xorg_atom_load(xorg_s* x);

/** xcp wrap*/
int xorg_xcb_attribute(xorg_s* x, xcb_get_window_attributes_cookie_t cookie);

/** xcb wrap*/
err_t xorg_xcb_geometry(xorg_s* x, xcb_get_geometry_cookie_t cookie, unsigned* X, unsigned* Y, unsigned* W, unsigned* H, unsigned* B);

/** xcb wrap*/
int xorg_xcb_property_cardinal(xorg_s* x, xcb_get_property_cookie_t cookie);

/** xcb wrap*/
xcb_get_property_cookie_t xorg_xcb_property_cookie_string(xorg_s* x, xcb_window_t win, xcb_atom_t atom);

/** xcb wrap*/
char* xorg_xcb_property_string(xorg_s* x, xcb_get_property_cookie_t cookie);

/** xcb wrap*/
err_t xorg_xcb_property_structure(void* out, xorg_s* x, xcb_get_property_cookie_t cookie, xcb_atom_t type, size_t size, size_t minsize);

/** xcb wrap*/
xcb_pixmap_t xorg_xcb_property_pixmap(xorg_s* x, xcb_get_property_cookie_t cookie);

/** send creat message*/
void xorg_send_creat(xorg_s* x, xcb_window_t parent, xcb_window_t win, int px, int py, int w, int h);

/** send destroy message*/
void xorg_send_destroy(xorg_s* x, xcb_window_t win);

/** send expose, redraw, message*/
void xorg_send_expose(xorg_s* x, xcb_window_t win, int px, int py, int w, int h);

/** send key press message*/
void xorg_send_key_press(xorg_s* x, xcb_window_t win, xcb_keycode_t keycode, xcb_timestamp_t time, int rx, int ry, int px, int py, int state, int samescreen);

/** send key release message*/
void xorg_send_key_release(xorg_s* x, xcb_window_t win, xcb_keycode_t keycode, xcb_timestamp_t time, int rx, int ry, int px, int py, int state, int samescreen);

/** send button press*/
void xorg_send_button_press(xorg_s* x, xcb_window_t win, xcb_button_t button, xcb_timestamp_t time, int rx, int ry, int px, int py, int state, int samescreen);

/** send button release*/
void xorg_send_button_release(xorg_s* x, xcb_window_t win, xcb_button_t button, xcb_timestamp_t time, int rx, int ry, int px, int py, int state, int samescreen);

/** send mouse move*/
void xorg_send_motion(xorg_s* x, xcb_window_t win, xcb_button_t button, xcb_timestamp_t time, int rx, int ry, int px, int py, int state, int samescreen);

/** send mouse enter*/
void xorg_send_enter(xorg_s* x, xcb_window_t win, xcb_button_t button, xcb_timestamp_t time, int rx, int ry, int px, int py, int state, int samescreen);

/** send mouse exit*/
void xorg_send_leave(xorg_s* x, xcb_window_t win, xcb_button_t button, xcb_timestamp_t time, int rx, int ry, int px, int py, int state, int samescreen);

/** send focus in*/
void xorg_send_focus_in(xorg_s* x, xcb_window_t win);

/** send focus out*/
void xorg_send_focus_out(xorg_s* x, xcb_window_t win);

/** send map/visible*/
void xorg_send_map(xorg_s* x, xcb_window_t win);

/** send unmap/hide*/
void xorg_send_unmap(xorg_s* x, xcb_window_t win);

/** send configure*/
void xorg_send_configure(xorg_s* x, xcb_window_t win, int px, int py, int w, int h, int border);

/** send property*/
void xorg_send_property(xorg_s* x, xcb_window_t win, xcb_atom_t atom);

/** send client*/
void xorg_send_client(xorg_s* x, xcb_window_t win, uint8_t type, xcb_atom_t atom, uint8_t* data, size_t len);

/** send client 32*/
void xorg_send_client32(xorg_s* x, xcb_window_t win, xcb_window_t dest, xcb_atom_t atom, const uint32_t* data, size_t len);

/** send active window*/
void xorg_send_active_window(xorg_s* x, xcb_window_t current, xcb_window_t activate);

/** send current desktop*/
void xorg_send_current_desktop(xorg_s* x, uint32_t desktop);

/** send set desktop*/
void xorg_send_set_desktop(xorg_s* x, xcb_window_t win, uint32_t desktop);

/** free window */
void xorg_window_release(xorgWindow_s* win);

/** tree of window*/
xorgWindow_s* xorg_query_tree(size_t* count, xorg_s* x, xcb_window_t root);

/** tree of window app*/
xorgWindow_s* xorg_window_application(xorg_s* x,  size_t nworkspace, xcb_window_t id, xorgWindow_s* stack, size_t* appCount);

/** return countr of workspace*/
unsigned xorg_workspace_count(xorg_s* x);

/** return active workspace*/
unsigned xorg_workspace_active(xorg_s* x);

/** get workspace name*/
char** xorg_workspace_name_get(xorg_s* x);

/** return buffer ximage*/
uint8_t* xorg_ximage_get_composite(unsigned* outW, unsigned* outH, unsigned* outV, unsigned* outD, xorg_s* x, xcb_window_t id);

/** get root pixmap*/
xcb_pixmap_t xorg_root_pixmap_get(xorg_s* x);

/** get pixel root image*/
uint8_t* xorg_ximage_root_get(unsigned* outW, unsigned* outH, unsigned* outV, unsigned* outD, xorg_s* x);

/** grab image of app*/
g2dImage_s* xorg_image_grab(xorg_s* x, xcb_window_t id);

/** grab root image*/
g2dImage_s* xorg_root_image_grab(xorg_s* x);

/** set window title*/
void xorg_win_title(xorg_s* x, xcb_window_t id, char const* name);

/** set window class*/
void xorg_win_class(xorg_s* x, xcb_window_t id, char const* name);

/** show windo*/
void xorg_win_show(xorg_s* x, xcb_window_t id, int show);

/** move window*/
void xorg_win_move(xorg_s* x, xcb_window_t id, unsigned X, unsigned y);

/** resize window*/
void xorg_win_resize(xorg_s* x, xcb_window_t id, unsigned w, unsigned h);

/** set window coordinate*/
void xorg_win_coord(xorg_s* x, xcb_window_t id, g2dCoord_s* pos);

/** set window size*/
void xorg_win_size(g2dCoord_s* out, unsigned* outBorder, xorg_s* x, xcb_window_t idxcb);

/** redraw surface*/
void xorg_win_surface_redraw(xorg_s* x, xcb_window_t id,  xorgSurface_s* surface);

/** set window as dock*/
void xorg_win_dock(xorg_s* x, xcb_window_t id);

/** reserve dock space*/
void xorg_wm_reserve_dock_space_on_top(xorg_s* x, xcb_window_t id, unsigned X, unsigned w, unsigned h);

/** reserve dock space*/
void xorg_wm_reserve_dock_space_on_bottom(xorg_s* x, xcb_window_t id, unsigned X, unsigned w, unsigned h);

/** register event on window*/
void xorg_register_events(xorg_s* x, xcb_window_t window, unsigned int eventmask);

/** create new window, if surface return new surface, remember to free*/
xcb_window_t xorg_win_new(xorgSurface_s** surface, xorg_s* x, xcb_window_t parent, g2dCoord_s* pos, unsigned border, g2dColor_t background);

/** resize surface*/
void xorg_surface_resize(xorgSurface_s* surface, unsigned w, unsigned h);

/** resize a surface, blitting img*/
void xorg_surface_resize_bitblt(xorgSurface_s* surface, unsigned w, unsigned h);

/** destroy/free sourface*/
void xorg_surface_destroy(xorg_s* x, xorgSurface_s* surface);

/** destroy window*/
void xorg_win_destroy(xorg_s* x, xcb_window_t id);

/** set focus on window*/
void xorg_win_focus(xorg_s* x, xcb_window_t id);

/** get new event, remember to release event*/
xorgEvent_s* xorg_event_new(xorg_s* x, int async);

/** free message*/
void xorg_event_free(xorgEvent_s* ev);

#endif 
