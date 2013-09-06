#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>

const char *hello_string = "Hello World";

int main(int argc, char **argv)
{
	Display *display = XOpenDisplay(NULL);
	Visual *visual = DefaultVisual(display, 0);
	int depth = DefaultDepth(display, 0);

	XSetWindowAttributes frame_attributes;
	frame_attributes.background_pixel = XWhitePixel(display, 0);

	Window frame_window = XCreateWindow(display, XRootWindow(display, 0),
						0, 0, 400, 400, 5, depth,
						InputOutput, visual, CWBackPixel,
						&frame_attributes);

	XStoreName(display, frame_window, "Hello World Example");
	XSelectInput(display, frame_window, ExposureMask | StructureNotifyMask);

	XFontStruct *fontinfo = XLoadQueryFont(display, "10x20");

	XGCValues gc_values;
	gc_values.font = fontinfo->fid;
	gc_values.foreground = XBlackPixel(display, 0);

	GC gc = XCreateGC(display, frame_window, GCFont + GCForeground, &gc_values);
	XMapWindow(display, frame_window);

	XKeyEvent event;
	for(;;)
	{
		XNextEvent(display, (XEvent *)&event);

		if(event.type == Expose)
		{
			int font_direction, font_ascent, font_descent;
			XCharStruct char_struct;

			XTextExtents(fontinfo, hello_string, strlen(hello_string),
					&font_direction, &font_ascent, &font_descent,
					&char_struct);

			XWindowAttributes window_attribs;
			XGetWindowAttributes(display, frame_window, &window_attribs);

			int text_x = (window_attribs.width - char_struct.width) / 2;
			int text_y = (window_attribs.height -
					(char_struct.ascent + char_struct.descent)) / 2;

			XDrawString(display, frame_window, gc, text_x, text_y,
					hello_string, strlen(hello_string));
		}
	}
}
