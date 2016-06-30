#ifndef WaitForSigCont_h
#define WaitForSigCont_h

#include <iostream>

#include <signal.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

int debugger_attached();

/* A convenience function for setting a breakpoint target in the debugger. */
void debug_break() {}

/* Pause the process, and wait for SIGCONT to be sent to the process before
 * continuing.
 *
 * A process that takes a long time
 * to run in a debugger can be started outside the debugger.
 *
 *   wait_for_sigcont();
 *
 * can be added at the point where we want to start debugging. When the process
 * pauses, this function will print out the process ID (PID) to
 * stdout, which can be used to then attach to the process with a debugger.
 */
void wait_for_sigcont()
{
  if( debugger_attached() )
    {
    return;
    }

  sigset_t waitset;
  int sig;
  int result = 0;
  sigemptyset( &waitset );
  sigaddset( &waitset, SIGCONT );
  sigprocmask( SIG_BLOCK, &waitset, NULL );
  std::cout << "Waiting for SIGCONT on process " << getpid() << std::endl;
  result = sigwait( &waitset, &sig );
  if( result == 0 )
    {
    std::cout << "Signal caught, continuing..." << std::endl;
    }
  debug_break();
}

/* Determine if we're running under a debugger by attempting to attach using pattach
 *
 * Only works on Linux.
 *
 * Source:
 * http://stackoverflow.com/questions/3596781/how-to-detect-if-the-current-process-is-being-run-by-gdb
 */
int debugger_attached()
{
  int pid = fork();
  int status;
  int res;

  if (pid == -1)
    {
      perror("fork");
      return -1;
    }

  if (pid == 0)
    {
      int ppid = getppid();

      /* Child */
      if (ptrace(PTRACE_ATTACH, ppid, NULL, NULL) == 0)
        {
          /* Wait for the parent to stop and continue it */
          waitpid(ppid, NULL, 0);
          ptrace(PTRACE_CONT, NULL, NULL);

          /* Detach */
          ptrace(PTRACE_DETACH, getppid(), NULL, NULL);

          /* We were the tracers, so gdb is not present */
          res = 0;
        }
      else
        {
          /* Trace failed so gdb is present */
          res = 1;
        }
      exit(res);
    }
  else
    {
      waitpid(pid, &status, 0);
      res = WEXITSTATUS(status);
    }
  return res;
}

#endif
