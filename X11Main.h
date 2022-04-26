// Copyright Eric Chauvin 2022



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html




#pragma once



// This is a good reference and guide:
// Xlib - C Language X Interface
// www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html

// And also:
// https://arcturus.su/~alvin/docs/xlpm/ch05.html


#include "../LinuxApi/BasicTypes.h"
#include "../LinuxApi/FileIO.h"

// These are safe-to-use static constants.
#include "X11Const.h"



class X11Main
  {
  private:

  public:
  static bool init( FileIO& mainIO );
  static void closeConnect( void );
  static Uint64 createSimpleWindow( const Int32 x,
                             const Int32 y,
                             const Int32 width,
                             const Int32 height,
                             const Int32 borderW );

  static void destroyWindow( const Uint64 window );
  static Int32 checkWindowEvent( Uint64 window );
  static void createGraphicsContext( Uint64 window );
  static Int32 getDisplayWidth( void );
  static Int32 getDisplayHeight( void );
  static char getKeyChar( void );
  static void drawRectangle( Uint64 window,
                             Int32 x, Int32 y,
                             Int32 width, Int32 height );

  static void setFont( void );
  static void listFonts( FileIO& mainIO );
  static Uint64 loadFont( const char* fontName );
  static void drawString( Uint64 window, Int32 x, Int32 y,
                          const char* pStr );


  };
