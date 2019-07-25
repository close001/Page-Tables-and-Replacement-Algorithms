# Page-Tables-and-Replacement-Algorithms
**INTRODUCTION**\

**SETUP**\
Go to _traceprogs_ folder then compile the trace programs and generate the traces using the command\
`make`\
**EXECUTING THE PROGRAM**\

The main driver for the memory simulator, _sim.c_ reads memory reference traces. For each line in the trace, the program asks for the simulated physical address that corresponds to the given virtual address by calling _find_physpage_, and then reads from that location. If the access type is a write ("M" for modify or "S" for store), it will also write to the location.\
The simulator is exectued as\
`./sim -f <tracefile> -m <memory size> -s <swapfile size> -a <replacement algorithm>`\
where memory size and swapfile size are the number of frames of simulated physical memory and the number of pages that can be stored in the swapfile respectively. 


