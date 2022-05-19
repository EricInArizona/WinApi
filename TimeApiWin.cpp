// Copyright Eric Chauvin 2022


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#include "TimeApiWin.h"
// #include "TimeApiLinux.h"
#include "../CppBase/CharBuf.h"
// #include <chrono>
#include <time.h>


TimeApi::TimeApi( void )
{
setNow();
}



void TimeApi::setNow( void )
{
struct tm newtime;
__time64_t long_time;
// char timebuf[26];
errno_t err;

// Get time as 64-bit integer.
_time64( &long_time );

timeSeconds = long_time;

// Convert to local time.
err = _localtime64_s( &newtime, &long_time );
if( err != 0 )
  throw "Error with time set up.";

seconds = newtime.tm_sec;
minutes = newtime.tm_min;
hour = newtime.tm_hour;
day = newtime.tm_mday;
month = newtime.tm_mon;
year = 1900 + newtime.tm_year;
weekDay = newtime.tm_wday; // Sunday = 0
yearDay = newtime.tm_yday; // day of year to 365.
dayLightSavings = newtime.tm_isdst;

//     if( newtime.tm_hour > 12 )
}



/*
For Linux.
void TimeApi::setNow( void )
{
time_t rawtime;
struct tm * timeinfo;

time( &rawtime );
timeSeconds = rawtime;

// For Linux.
timeinfo = localtime (&rawtime);

localtime_s( timeinfo, &rawtime );




// StIO::printFS( "Year:\n" );
// StIO::printFUD( 1900 + buf.tm_year );
// StIO::printFS( "\n" );

seconds = timeinfo->tm_sec;
minutes = timeinfo->tm_min;
hour = timeinfo->tm_hour;
day = timeinfo->tm_mday;
month = timeinfo->tm_mon;
year = 1900 + timeinfo->tm_year;
weekDay = timeinfo->tm_wday; // Sunday = 0
yearDay = timeinfo->tm_yday; // day of year to 365.
dayLightSavings = timeinfo->tm_isdst;
}
*/



Int64 TimeApi::diffSec( TimeApi& in )
{
return timeSeconds - in.timeSeconds;
}



Str TimeApi::timeStr( void )
{
CharBuf cBuf;

Str colon( ":" );
Str zero( "0" );
Str secS( seconds );
Str minS( minutes );
Str hourS( hour );

// Close enough for now.
cBuf.appendStr( hourS );
cBuf.appendStr( colon );
if( minS.getSize() < 2 )
  cBuf.appendStr( zero );

cBuf.appendStr( minS );
cBuf.appendStr( colon );

if( secS.getSize() < 2 )
  cBuf.appendStr( zero );

cBuf.appendStr( secS );

return cBuf.getStr();
}
