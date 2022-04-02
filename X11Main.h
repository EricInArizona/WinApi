// Copyright Eric Chauvin 2022
// This copyright notice has to stay at the top.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html




#pragma once



// This is a good reference and guide:
// Xlib - C Language X Interface
// www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html



#include "../LinuxApi/BasicTypes.h"
#include "../LinuxApi/FileIO.h"



class X11Main
  {
  private:

  public:
  static const Int32 EventNothing = 0;
  static const Int32 EventKeyPress = 2;
  static const Int32 EventExpose = 3;

  static bool init( FileIO& mainIO );
  static void closeConnect( void );
  static void test( FileIO& mainIO );
  static Uint64 createSimpleWindow( const Uint64 parent,
                             const Int32 x,
                             const Int32 y,
                             const Int32 width,
                             const Int32 height,
                             const Int32 borderW );

  static void destroyWindow( const Uint64 window );
  static Int32 checkWindowEvent( Uint64 window );


  };
