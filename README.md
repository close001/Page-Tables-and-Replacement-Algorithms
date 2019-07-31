# Paging
**INTRODUCTION**\
Paging is a memory management scheme by which a computer stores and retrieves data from secondary storage for use in main memory. In this scheme, the operating system retrieves data from secondary storage in same-size blocks called pages. This repository contains programs that simulate the operation of paging using a page table and page replacement algorithms. _pagetable.c_ implements virtual-to-physical address translation and demands paging using a two-level page table and page remplacement algorithms which are implemented in _fifo.c_, _lru.c_, _clock.c_, _rand.c_ and _opt.c_.

**SOME NOTES ABOUT RUNNING THE PROGRAM**
- The instruction on how to use this prgoram is done in linux terminal. However, the steps should be similar in other OSes.
- Your system must be able to run C and Python files.
- _valgrind_ must be installed to run _runit_ file.

**SETUP**
0. Open up your command line 
1. _cd_ into the directory where you downloaded the files
2. Use the command `make` to generate executables
3. _cd_ into _traceprogs_ directory
4. Set _runit_ and _fastslim.py_ files' permission such that you can execute these files (use _chmod_ command) 
5. Generate the traces using the command `make traces`

**EXECUTING THE PROGRAM**\
The main driver for the memory simulator, _sim.c_ reads memory reference traces. For each line in the trace, the program asks for the simulated physical address that corresponds to the given virtual address by calling _find_physpage_, and then reads from that location. If the access type is a write ("M" for modify or "S" for store), it will also write to the location.

The simulator is exectued as

`./sim -f <tracefile> -m <memory size> -s <swapfile size> -a <replacement algorithm>`

where memory size and swapfile size are the number of frames of simulated physical memory and the number of pages that can be stored in the swapfile respectively. The swapfile size should be as large as the number of unique virtual pages in the trace.

Toy Example Command:

`./sim -f traceprogs/tr-test.ref -m 2 -s 5 -a fifo`


