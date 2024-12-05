Banker's Algorithm Simulator
Overview
This is a C++ implementation of the Banker's Algorithm, a resource allocation and deadlock avoidance algorithm used in operating systems. 
The program helps simulate safe resource allocation among multiple processes.

Features
Reads system resource information from an input file
Tracks MAX, ALLOCATION, and NEED matrices
Validates resource requests
Checks for safe state before allocating resources

Input File Format
The input file (input.txt) should have the following structure:
<num_processes> <num_resources>
MAX matrix (processes x resources)
AVAILABLE resources vector
Example input.txt
5 3
7 5 3
3 2 2
9 0 2
2 2 2
4 3 3
10 5 7

How to Use
Compile the program
Prepare an input.txt file with system resource details
Run the executable
Follow on-screen prompts to:
Select a process
Enter resource request
View system state

Key Concepts
MAX: Maximum resources a process may need
ALLOCATION: Resources currently allocated to a process
NEED: Remaining resources a process might need
AVAILABLE: Unallocated resources in the system

Safety Algorithm
The program uses a safety algorithm to:
i) Prevent deadlocks
ii) Ensure resource allocation does not lead to an unsafe system state
