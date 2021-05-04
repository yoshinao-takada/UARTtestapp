# Test App for Studying Signals and Serialport Blocking I/O Operations
## Objective
I felt difficulties in writing multithreading communication program more in
Linux than in Windows. Especially the following two categories of APIs are
less intuitive than those of Windows.
* Low level file I/O like open, close, read, write; Similar to Windows CreateFile, ReadFile, etc.
* Signal APIs like sigaction and related timer APIs; Similar to Windows event and TimerQueueTimer  

Therefore I tried to design a small test program utilizing following components,
* Interval timer,
* Timer event dispatcher, and
* Low level blocking file I/O for serialports.

## App Overview
The sequence diagram is shown below.  
![sequence-diagram](docs/App-sequence.svg)  
Participants in the diagram are:
* IntervalTimer : Linux system timer set by setitimer(ITIMER_REAL,) to generate SIGALRM at each 10 ms.
* TimerEventDispatcher1 : A down counter and a callback function; send a string at each second.
* TimerEventDispatcher2 : Down counter and callback function; send SIGINT to the read thread.
* ReadThread : repeatedly call read() function.

## Intereval Timer
Releted APIs  
* settimer : setup an interval timer generating SIGALRM signal
* sigaction : set a signal handler

## Event Dispatcher
It is driven by the interval timer described above. Each dispatcher consists of
* downcounter : giving the first timeout,
* reload register : giving period of periodical timeout,
* callback function : called at each timeout
* parameter of the callback function
* mutex  

## Serialport I/O
Serialport settings are complicated and I studied detailed technique by
* [Serial Programming Guide for POSIX Operating Systems](https://www.cmrr.umn.edu/~strupp/serial.html)  
* [Github eRPC official repository](https://github.com/EmbeddedRPC/erpc)

## Sending Signal to A Thread
I carefully read Linux man pages and `signal.h` header file. Man pages, `settimer`, `sigaction` were useful.
It was not easy to understand the contents of the man pages and I have not yet well understood.

## Project Directories
* app : test apps main code
* base : base library functions
* docs : documents

## apps
They are built with `Makefile` in the project root directory. Run make as follows.  
&nbsp;&nbsp;&nbsp;&nbsp;`> make all`  
The executables are created in `debug/bin/` and `release/bin/`.