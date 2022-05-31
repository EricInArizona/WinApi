// Copyright Eric Chauvin 2022



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "Signals.h"
#include <signal.h>

// For Linux:
// #include <unistd.h>


// If you do fork() you'd want to set
// signal(SIGCHLD, SIG_IGN);
// Or wait() for the process.

// Send a signal to a process:
// int kill( __pid_t __pid, int __sig);

// Send a signal to this process.
// int raise( int __sig );


extern "C" {

// Declare it extern, then define it.
// extern volatile sig_atomic_t gotSigCtrlC;
// volatile sig_atomic_t gotSigCtrlC = 0;
static volatile sig_atomic_t gotSigCtrlC = 0;



// Erroneous arithmetic operation.
// SIGFPE is the floating point error signal.
// It is also used for integer errors, like
// with an integer overflow.  The default
// signal handler for that is to
// just close the program.


void catchFpeSignal( Int32 sigNum );


void catchFpeSignal( Int32 sigNum )
{
// Some systems need to reset this.  Does linux?
// signal( SIGFPE, catchFpeSignal );

// gotSigFpe++;

// All you have to do in increment a number,
// dothis++;
// and if you overflow it, the program shuts down
// because of this signal.


sigNum++; // Pretend to use this for the
          // compiler warnings.
if( sigNum < 1000000000 )
  throw "Floating point error signal.";

}



void catchControlCSignal( Int32 sigNum );


void catchControlCSignal( Int32 sigNum )
{
gotSigCtrlC++;

sigNum++;
if( sigNum == 1000000000 )
  throw "This is not going to happen.";

}


void catchIllegalOpSignal( Int32 sigNum );

void catchIllegalOpSignal( Int32 sigNum )
{
sigNum++;
if( sigNum < 1000000000 )
  throw "Got illegal op signal.";

}



void catchBadMemSignal( Int32 sigNum );

void catchBadMemSignal( Int32 sigNum )
{
sigNum++;
if( sigNum < 1000000000 )
  throw "Got bad memory signal.";

}




// In case I want to send signals.
// SIGUSR1 User-defined signal 1.
// SIGUSR2 User-defined signal 2.



  } // extern "C"




/*
 1) SIGHUP     Hang up.
 2) SIGINT     Control-C, like to stop the process.
 3) SIGQUIT
 4) SIGILL     Illegal instruction.
 5) SIGTRAP
 6) SIGABRT    Control-\  stops the process.
 7) SIGBUS     Bus error.  Memory invalid alignment.
 8) SIGFPE     Floating point error.  And integers too.
 9) SIGKILL    Kill the process.
 10) SIGUSR1   User defined 1.
 11) SIGSEGV   Segmentation Violation.  Illegal memory address.
 12) SIGUSR2   User defined 2.
 13) SIGPIPE
 14) SIGALRM
 15) SIGTERM   Terminate the process.  The nice way.
 // // Stack fault (obsolete).
 16) SIGSTKFLT  For the math coprocessor?
 17) SIGCHLD
 18) SIGCONT   Continue.  Resume execution.
 19) SIGSTOP   Suspend execution immediately.
 20) SIGTSTP   Control-Z to suspend execution. sleep.
 21) SIGTTIN
 22) SIGTTOU
 23) SIGURG
 24) SIGXCPU
 25) SIGXFSZ
 26) SIGVTALRM
 27) SIGPROF
 28) SIGWINCH
 29) SIGIO
 30) SIGPWR
 31) SIGSYS
 32
 33
 34) SIGRTMIN
 35) SIGRTMIN+1
 36) SIGRTMIN+2
 37) SIGRTMIN+3
 38) SIGRTMIN+4
 39) SIGRTMIN+5
 40) SIGRTMIN+6
 41) SIGRTMIN+7
 42) SIGRTMIN+8
 43) SIGRTMIN+9
 44) SIGRTMIN+10
 45) SIGRTMIN+11
 46) SIGRTMIN+12
 47) SIGRTMIN+13
 48) SIGRTMIN+14
 49) SIGRTMIN+15
 50) SIGRTMAX-14
 51) SIGRTMAX-13
 52) SIGRTMAX-12
 53) SIGRTMAX-11
 54) SIGRTMAX-10
 55) SIGRTMAX-9
 56) SIGRTMAX-8
 57) SIGRTMAX-7
 58) SIGRTMAX-6
 59) SIGRTMAX-5
 60) SIGRTMAX-4
 61) SIGRTMAX-3
 62) SIGRTMAX-2
 63) SIGRTMAX-1
 64) SIGRTMAX

*/



void Signals::setupFpeSignal( void )
{
// gotSigFpe = 0;

if( SIG_ERR == signal( SIGFPE, catchFpeSignal ))
  throw "Error setting the Fpe signal handler.";


/*
There is a problem with a recursive macro definition
when using this.  There is a sigaction struct and
a sigaction function.  Both with the same name.
You will never see macro definitions coming from my
header files.  This is like the poster child for why
you should never use macro definitions.

struct sigaction sa;

sa.sa_handler = catchFpeSignal;
sa.sa_flags = 0; // or SA_RESTART
sigemptyset( &sa.sa_mask );

if( sigaction( SIGFPE, &sa, NULL ) == -1 )
  {
  throw "Couldn't set sigAction.";
  }
*/
}


bool Signals::getControlCSignal( void )
{
if( gotSigCtrlC > 0 )
  return true;

return false;
}


void Signals::setupControlCSignal( void )
{
gotSigCtrlC = 0;

if( SIG_ERR == signal( SIGINT, catchControlCSignal ))
  throw "Error setting the Control C signal handler.";

}



// SIGILL Illegal instruction.
void Signals::setupIllegalOpSignal( void )
{
if( SIG_ERR == signal( SIGILL, catchIllegalOpSignal ))
  throw "Error setting the illegal op signal handler.";

}




// SIGSEGV Invalid memory reference.

void Signals::setupBadMemSignal( void )
{
if( SIG_ERR == signal( SIGSEGV, catchBadMemSignal ))
  throw "Error setting the bad mem signal handler.";

}
