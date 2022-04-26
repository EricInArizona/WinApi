// Copyright Eric Chauvin 2022



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



// Use OpenGL.
// #include <gl/gl.h>
// #include<GL/glx.h>
// #include<GL/glu.h>

// https://en.wikipedia.org/wiki/GLX

// https://www.khronos.org/opengl/wiki/
//              Programming_OpenGL_in_Linux:_GLX_and_Xlib
// the GLX extension to the X windows system.



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
static Display *display = nullptr;
static Int32 screen;
static Uint64 colorBlack;
static Uint64 colorWhite;
static GC gc = nullptr; // Graphics context.
static char keyBuf[256];
static Uint64 keySymbol;





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



Int32 X11Main::getDisplayWidth( void )
{
return DisplayWidth( display, screen );
}



Int32 X11Main::getDisplayHeight( void )
{
return DisplayHeight( display, screen );
}




Uint64 X11Main::createSimpleWindow( const Int32 x,
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


// Window window;

// Coordinates and sizes like width and height are
// truncated to 16 bits.
Uint64 window = XCreateSimpleWindow( display,
                  DefaultRootWindow( display ),
                  // RootWindow( display, screen ),
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


// There is only one graphics context for now.
createGraphicsContext( window );


// XClearWindow( display, window );
// XMapRaised( display, window );


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
  return X11Const::EventNothing;

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


// if( KeyPress != X11Const::EventKeyPress )
  // It doesn't matter if they are the same or not.

if( event.type == KeyPress )
  {
  XLookupString( &event.xkey, keyBuf, 254, &keySymbol,
                                           nullptr );
  return X11Const::EventKeyPress;
  }


if( event.type == Expose )
  return X11Const::EventExpose;

if( event.type == ButtonPress )
  return X11Const::EventButtonPress;



// What was the event?
return X11Const::EventNothing;
}



char X11Main::getKeyChar( void )
{
return keyBuf[0];
}




void X11Main::drawRectangle( Uint64 window,
                             Int32 x, Int32 y,
                             Int32 width, Int32 height )
{
XDrawRectangle( display, window, gc, x, y,
                           Casting::i32ToU32( width ),
                           Casting::i32ToU32( height ));
}





/*

XDrawLine(dis,win,gc, x1,y1, x2,y2);
XDrawArc(dis,win,gc,x,y, width, height, arc_start, arc_stop);
XFillArc(dis,win, gc, x, y, width, height, arc_start, arc_stop);
XFillRectangle(dis,win,gc, x, y, width, height);
*/



// https://arcturus.su/~alvin/docs/xlpm/ch05.html


void X11Main::createGraphicsContext( Uint64 window )
{
XGCValues values;
Uint64 valuemask = 0;

// There is only one current graphics context.
gc = XCreateGC( display, window, valuemask, &values );

XSetForeground( display, gc, colorWhite );
XSetBackground( display, gc, colorBlack );
}





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
if( gc != nullptr )
  XFreeGC( display, gc );

if( display != nullptr )
  {
  // This frees any resources you haven't already freed.
  XCloseDisplay( display );
  display = nullptr;
  }
}



void X11Main::setFont( void )
{
Uint64 font = 0; // Temporary fake number.

XSetFont( display, gc, font );
}





void X11Main::listFonts( FileIO& mainIO )
{
// #include <gl/gl.h>

Int32 numFonts;
char** fontList;

//  fontlist = XListFonts (dpy, "*helv*", 1000, &num_fonts);
fontList = XListFonts( display, "*", 5000, &numFonts );


// Name is: -misc-fixed-medium-r-semicondensed--13-100-100-
//                    100-c-60-iso8859-1
// Name is: -misc-fixed-medium-r-semicondensed--13-120-75-75-
//                 c-60-iso8859-1

// Name is: 6x13
// Name is: cursor
// Name is: fixed
// Name is: -misc-fixed-medium-r-semicondensed--0-0-75-75-
// c-0-iso8859-1


mainIO.appendChars( "\n\nFonts:\n\n" );

for( Int32 count = 0; count < numFonts; count++ )
  {
  mainIO.appendChars( "Name is: " );
  mainIO.appendChars( fontList[count] );
  mainIO.appendChars( "\n" );
  }

XFreeFontNames( fontList );
}



// XLoadQueryFont()


Uint64 X11Main::loadFont( const char* fontName )
{
return XLoadFont( display, fontName );
}




void X11Main::drawString( Uint64 window, Int32 x, Int32 y,
                          const char* pStr )
{
const char* sizePoint = pStr;
Int32 strSize = 0;
for( Int32 count = 0; count < 1000; count++ )
  {
  char c = *sizePoint;
  if( c == 0 )
    break;

  sizePoint++;
  strSize++;
  }


XDrawString( display, window, gc, x, y, pStr, strSize );
}
