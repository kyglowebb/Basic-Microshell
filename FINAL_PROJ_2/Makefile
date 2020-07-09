#/* Project Information */
#this project allows to create a smiple shell to excute files and make pipes between processes

# Assignment 2: MicroShell
#   Names: Kylee Webb, Martin Philip
#   Class: CS 570, Summer 2020 
#  Class Accounts: CSSC 2159, CSSC 2126
#   Emails: Kawebb@sdsu.edu, martin.phillip@gcccd.edu */






EXEC = msh 
FILES = msh.cpp shellFunctions.cpp shellFunctions.h  

LFLAGS = -g++ 
CFLAGS = -g++ -c
 
OBJECTS = $(FILES:.c=.o)
 
$(EXEC):$(OBJECTS) 
	$(LFLAGS) -o $(EXEC) $(OBJECTS) 
	rm -f *.o
 
c.o: 
	$(CC) $(CFLAGS) $<
 
clean: 
	rm -f *.o $(EXEC)

