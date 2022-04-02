// Copyright Eric Chauvin 2022
// This copyright notice has to stay at the top.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "X11Main.h"
#include "../LinuxApi/Casting.h"
#include "../LinuxApi/Threads.h"


// The order of these matters.
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

// Color management: <X11/Xcms.h>





extern "C" {



Int32 X11ErrorHandler( Display* disp,  XErrorEvent* errEvent );

Int32 X11ErrorHandler( Display* disp,  XErrorEvent* errEvent )
{
// Make the compiler think that I am using these
// parameters.

if( disp == nullptr )
  throw "Got an X11 error with disp == nullptr.";

if( errEvent == nullptr )
  throw "Got an X11 error with errEvent == nullptr.";

throw "Got an X11 error.";
}


/*

typedef int (*XErrorHandler) (
// WARNING, this type not in Xlib spec
    Display*  ,
    XErrorEvent* );

*/

  } // extern "C" 








// static: It is only in this compilation unit, as opposed
// to being extern.

// There is only one of these variables for all windows.
static Display *display;
static Int32 screen;
static Uint64 colorBlack;
static Uint64 colorWhite;




bool X11Main::init( FileIO& mainIO )
{
mainIO.appendChars( "X11 init().\n" );

XSetErrorHandler( X11ErrorHandler );


// XOpenDisplay: connect to the X server.

// Display* XOpenDisplay( char* display_name );

// If you pass nullptr then it defaults to what is in
// the DISPLAY environment variable.
// My DISPLAY variable is set to
// DISPLAY=:0
// Use printenv to show all environment variables.

// protocol/hostname:number.screen_number

display = XOpenDisplay( nullptr );

if( display == nullptr )
  {
  mainIO.appendChars( "Display was null.\n" );
  return false;
  }

screen = DefaultScreen( display );

colorBlack = XBlackPixel( display, screen );
colorWhite = XWhitePixel( display, screen );

return true;
}




Uint64 X11Main::createSimpleWindow( const Uint64 parent,
                                    const Int32 x,
                                    const Int32 y,
                                    const Int32 width,
                                    const Int32 height,
                                    const Int32 borderW )
{
if( width < 1 )
  throw "createSimpleWindow width < 1";

if( height < 1 )
  throw "createSimpleWindow height < 1";

if( x < 0 )
  throw "createSimpleWindow x < 0";

if( y < 0 )
  throw "createSimpleWindow y < 0";

if( borderW < 0 )
  throw "createSimpleWindow border < 0";


// Is this right?
// The coordinates inside a window start at 0,0 which
// is inside the border at the upper-left corner.



// Window XCreateSimpleWindow( Display *display,
                            // Window parent,
                            // int x,
                            // int y,
                            // unsigned int width,
                            // unsigned int height,
                            // unsigned int border_width,
                            // unsigned long border,
                            // unsigned long background );


// GC gc; Graphics context.


// Window window;

// Coordinates and sizes like width and height are
// truncated to 16 bits.
Uint64 window = XCreateSimpleWindow( display,
                   parent, // RootWindow( display, screen ),
                   x, y,
                   Casting::i32ToU32( width ),
                   Casting::i32ToU32( height ),
                   Casting::i32ToU32( borderW ),
                   colorWhite, colorBlack );


// An atom is an identifier of a constant string,
// So this asks the server to return an ID for this
// string.

// Xatom.h has predefined atoms.


// That zero at the end is for a bool false.
// If it was true then it would return it only if it
// already exists.
// It returns zero if no atom is returned.
Atom delWindow = XInternAtom( display,
                               "WM_DELETE_WINDOW", 0 );


// Status is an integer.
// If Status is zero it didn't work.

XSetWMProtocols( display, window, &delWindow, 1 );

// The events I want for this particular window.
XSelectInput( display, window, ExposureMask |
                               KeyPressMask |
                               ButtonPressMask ); // |
                               // KeymapStateMask );

/*
NoEventMask
KeyReleaseMask
ButtonReleaseMask
EnterWindowMask
LeaveWindowMask
PointerMotionMask
PointerMotionHintMask
Button1MotionMask
Button2MotionMask
Button3MotionMask
Button4MotionMask
Button5MotionMask
ButtonMotionMask
VisibilityChangeMask
StructureNotifyMask
ResizeRedirectMask
SubstructureNotifyMask
SubstructureRedirectMask
FocusChangeMask
PropertyChangeMask
ColormapChangeMask
OwnerGrabButtonMask
*/



// Show it.
XMapWindow( display, window );


// XSync blocks until all previously buffered asynchronous
// events have been sent and acted on.

// Flush it but don't wait like XSync.
XFlush( display );

return window;
}



Int32 X11Main::checkWindowEvent( Uint64 window )
{
XEvent event;

if( !XCheckWindowEvent( display, window, ExposureMask |
                                         KeyPressMask |
                                         ButtonPressMask,
                                      // KeymapStateMask
                                         &event ))
  return X11Main::EventNothing;

/*
KeyPress  2
KeyRelease  3
ButtonPress  4
ButtonRelease  5
MotionNotify  6
EnterNotify  7
LeaveNotify  8
FocusIn   9
FocusOut  10
KeymapNotify  11
Expose   12
GraphicsExpose  13
NoExpose  14
VisibilityNotify 15
CreateNotify  16
DestroyNotify  17
UnmapNotify  18
MapNotify  19
MapRequest  20
ReparentNotify  21
ConfigureNotify  22
ConfigureRequest 23
GravityNotify  24
ResizeRequest  25
CirculateNotify  26
CirculateRequest 27
PropertyNotify  28
SelectionClear  29
SelectionRequest 30
SelectionNotify  31
ColormapNotify  32
ClientMessage  33
MappingNotify  34
GenericEvent  35

*/


// if( KeyPress != X11Main::EventKeyPress )
  // It doesn't matter if they are the same or not.

if( event.type == KeyPress )
  return X11Main::EventKeyPress;

if( event.type == Expose )
  return X11Main::EventExpose;

 

// What was the event?
return X11Main::EventNothing;
}


======
/* 
void X11fillRectangle( const Uint64 win )
{
Read about graphics context.

I need the graphics context for this window.

      XFillRectangle( display, window,
                      DefaultGC( display, screen ),
                      RECT_X, RECT_Y,
                      RECT_WIDTH, RECT_HEIGHT );
}


*/




// If you destroy a top window then this will
// destroy any subwindows that it has.
// So be careful that you don't destroy a window and
// then try to use it later.
void X11Main::destroyWindow( const Uint64 window )
{
XDestroyWindow( display, window );
}



void X11Main::closeConnect( void )
{
if( display != nullptr )
  {
  // This frees any resources you haven't already freed.
  XCloseDisplay( display );
  display = nullptr;
  }
}



void X11Main::test( FileIO& mainIO )
{
mainIO.appendChars( "X11 test().\n" );

if( !init( mainIO ))
  return;

mainIO.appendChars( "init() is OK.\n" );

// A window manager can override your choice of size,
// border width, and position for a top-level window.
// Your program must be prepared to use the actual size
// and position of the top window.

Int32 dWidth = DisplayWidth( display, screen );
Int32 dHeight = DisplayHeight( display, screen );


// In millimeters:
// DisplayWidthMM(dpy, scr)
// DisplayHeightMM(dpy, scr)

mainIO.appendChars( "dWidth: " );
Str showW( dWidth );
mainIO.appendStr( showW );
mainIO.appendChars( "\n" );

if( dWidth < 800 )
  dWidth = 800;

if( dHeight < 600 )
  dHeight = 600;


Uint64 newWin = createSimpleWindow(
                      DefaultRootWindow( display ),
                      1,
                      1,
                      // minus two borders - 2
                      dWidth - (2 * 5) - 2,
                      // If dHeight is too big then it will
                      // push the taskbar down so it is
                      // hidden or partially pushed down.
                      dHeight - 110,
                      5 );


Threads::sleep( 5000 );

destroyWindow( newWin );

closeConnect();

mainIO.appendChars( "After close.\n" );
}
