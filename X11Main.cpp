// Written by Eric Chauvin 2022


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.



#include "X11Main.h"


#include <X11/Xlib.h>



// static: It is only in this compilation unit, as opposed
// to being extern.

static Display *display;



enum {
        RECT_X = 20,
        RECT_Y = 20,
        RECT_WIDTH = 10,
        RECT_HEIGHT = 10,

        WIN_X = 10,
        WIN_Y = 10,
        WIN_WIDTH = 100,
        WIN_HEIGHT = 100,
        WIN_BORDER = 1
};



bool X11Main::init( FileIO& mainIO )
{
mainIO.appendChars( "X11 init().\n" );

// Window window;
Uint64 window;

XEvent event;
Int32 screen;

display = XOpenDisplay( nullptr );

if( display == nullptr )
  {
  mainIO.appendChars( "Display was null.\n" );
  return false;
  }

screen = DefaultScreen( display );

// create window
window = XCreateSimpleWindow( display,
                   RootWindow( display, screen ),
                   WIN_X, WIN_Y, WIN_WIDTH, WIN_HEIGHT,
                   WIN_BORDER, BlackPixel( display,
                   screen ),
                   WhitePixel( display, screen ));

// process window close event through event
// handler so XNextEvent does not fail.
Atom del_window = XInternAtom( display,
                               "WM_DELETE_WINDOW", 0 );

XSetWMProtocols( display, window, &del_window, 1 );

XSelectInput( display, window, ExposureMask | KeyPressMask );

// display the window
XMapWindow( display, window );

// event loop
// while( true )
for( Int32 count = 0; count < 5; count++ )
  {
  XNextEvent( display, &event );

  switch( event.type )
    {
    case KeyPress:
    // FALLTHROUGH
    case ClientMessage:
      break;

    case Expose:
      XFillRectangle( display, window,
                      DefaultGC( display, screen ),
                      RECT_X, RECT_Y,
                      RECT_WIDTH, RECT_HEIGHT );

    // NO DEFAULT
    }
  }

// breakout:


XDestroyWindow( display, window );

XCloseDisplay( display );

return true;
}
