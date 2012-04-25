
#include <craftcore.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glew.h>
#include <GL/glx.h>

Display                 *dpy;
Window                  root;
GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo             *vi;
Colormap                cmap;
XSetWindowAttributes    swa;
Window                  win;
GLXContext              glc;
XWindowAttributes       gwa;
XEvent xev;
Cursor csr;

typedef int (*GLXSWAPINTERVALSGI)(int);
GLXSWAPINTERVALSGI glXSwapIntervalSGI;

void cc_system_poll()
{
	char cstr[128];
	int len = 0;
	KeySym sym;
	
	while (XPending(dpy) > 0)
	{
		XNextEvent(dpy, &xev);
		XKeyEvent *ke = (XKeyEvent*)&xev;
		XMotionEvent *me = (XMotionEvent*)&xev;
		XButtonEvent *be = (XButtonEvent*)&xev;
		int data = 0;
		switch (xev.type)
		{
		case ClientMessage: cc_emit_event(EVENT_CLOSEBUTTON, 0, 0, 0);break;
		case KeyPress:
			len = XLookupString(ke, cstr, 128, &sym, 0);
			for (int i = 0; i < len;i++){
				cc_emit_event(EVENT_KEYCHAR,cstr[i],0,0);
			}
			cc_emit_event(EVENT_KEYDOWN, ke->keycode, ke->x, ke->y);
			break;
		case KeyRelease:    cc_emit_event(EVENT_KEYUP, ke->keycode, ke->x, ke->y);break;
		case MotionNotify:  cc_emit_event(EVENT_MOUSEMOVE, 0, me->x, me->y);break;
		case ButtonPress:
			switch (be->button)
			{
			case Button1: data = 1;break;
			case Button3: data = 2;break;
			}
			cc_emit_event(EVENT_MOUSEDOWN, data, be->x, be->y);
			break;
		case ButtonRelease:
			switch (be->button)
			{
			case Button1: data = 1;break;
			case Button3: data = 2;break;
			}
			cc_emit_event(EVENT_MOUSEUP, data, be->x, be->y);
			break;
		}
	}
}

int cc_context_open(const char *title, int width, int height)
{
	dpy = XOpenDisplay(NULL);

	if (dpy == NULL)
		return 1;
		    
	root = DefaultRootWindow(dpy);

	vi = glXChooseVisual(dpy, 0, att);

	if (vi == NULL)
		return 2;

	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | FocusChangeMask;

	win = XCreateWindow(dpy, root, 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask | CWBorderPixel, &swa);

  Atom delWindow = XInternAtom(dpy, "WM_DELETE_WINDOW", 0);
  XSetWMProtocols(dpy, win, &delWindow, 1);
  
	XSizeHints* hints = XAllocSizeHints();
	/*hints->flags = PSize | PMinSize | PMaxSize;
	hints->min_width = (hints->max_width = (hints->base_width = width));
	hints->min_height = (hints->max_height = (hints->base_height = height));*/
	hints->win_gravity=CenterGravity;
	XSetWMNormalHints(dpy, win, hints);
	XFree(hints); 

	XMapWindow(dpy, win);
	XStoreName(dpy, win, title);
	
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);

	glXSwapIntervalSGI=(GLXSWAPINTERVALSGI)glXGetProcAddressARB((GLubyte*)"glXSwapIntervalSGI");

	glXSwapIntervalSGI(1);
		
	return 0;
}

void cc_context_size(int *width, int *height)
{
	XGetWindowAttributes(dpy, win, &gwa);
	*width = gwa.width;
	*height = gwa.height;
}

void cc_context_close()
{
	glXMakeCurrent(dpy, None, NULL);
	glXDestroyContext(dpy, glc);
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void cc_context_flip()
{
	glXSwapBuffers(dpy, win);
}

void cc_mouse_move(int x, int y)
{
	XWarpPointer(dpy,None,win,0,0,0,0,x,y);
}

void cc_mouse_visible(int visible)
{
	if (!win) return;

	if (visible)
	{
		XUndefineCursor(dpy,win);		
	}
	else
	{
		if (!csr)
		{
			XColor black;
			char bm[]={0,0,0,0,0,0,0,0};
			Pixmap pix=XCreateBitmapFromData(dpy,win,bm,8,8);
			memset(&black,0,sizeof(XColor));
			black.flags=DoRed|DoGreen|DoBlue;
			csr=XCreatePixmapCursor(dpy,pix,pix,&black,&black,0,0);
			XFreePixmap(dpy,pix);
		}
		XDefineCursor(dpy,win,csr);
	}
}
