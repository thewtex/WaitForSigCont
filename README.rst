WaitForSigCont
==============

A small tool to debug long running code or difficult to attach processes with `gdb <https://www.gnu.org/software/gdb/>`_.


First, include the header file where you want to start debugging::

  #include "WaitForSigCont.h"

This header must be placed in the include directory path.

Then, call the `wait_for_sig_cont()` function where you want to start
debugging::

  // long running code....

  wait_for_sigcont();

Start gdb::

  $ gdb

Source the script that defines the `sigcontattach` command::

  (gdb) source ~/src/WaitForSigCont/sigcontattach.gdb

or add this line to *~/.gdbinit*.

Start the executable. When it reaches `wait_for_sigcont`, it will print::

  Waiting for SIGCONT on process 1234

In gdb, run `sigcontattach` with the process identifier::

  (gdb) sigcontattach 1234

Then let gdb figure out your issue :-).
