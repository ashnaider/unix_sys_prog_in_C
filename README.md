# unix_sys_prog_in_C
System programming topics for UNIX-based systems in C

### Task 2
Create some Makefiles
In bin directory You can find Makefile with automatic dependency building

### Task 3
Program gets POSIX command line options and arguments (which are filters), and searches for PATH variables, that are match the conditions.
You can get list of available options by calling program with key --help or without arguments.

### Task 4
Program copies (appends) bytes from source file to destination file in reverse order

### Task 5
Program recursively searches for broken symbolic links in given directory and delete them, if there are any.
There are two variants of program: one of them uses low level UNIX function, and second one uses nftw function.

### Task 6
If signal SIGUSR1 is received, main program spawns two new processes.
If signal SIGUSR1 is received again, program terminates one of the processes.
If signal SIGUSR2 is received, program kills second process, and you can send signal SIGUSR1 again.

### Task 7
It is a client-server program. Server becomes daemon.
You can send file name to server using client, and server will return information about last time file was changed and etc.
Server writes log information in syslog journal: /var/log/syslog
