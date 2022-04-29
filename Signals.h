// Copyright Eric Chauvin 2022


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html




#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CppBase/RangeC.h"


class Signals
  {
  private:
  // Int32 testForCopy = 123;

  public:
  // Signals( void );
  // Signals( const Signals& in );
  // ~Signals( void );
  static void setupFpeSignal( void );
  static void setupControlCSignal( void );
  static void setupIllegalOpSignal( void );
  static void setupBadMemSignal( void );

  };
