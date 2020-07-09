 /* Project Information */
//this project allows to create a smiple shell to excute files and make pipes between processes

/* Assignment 2: MicroShell
   Names: Kylee Webb, Martin Philip
   Class: CS 570, Summer 2020 
   Class Accounts: CSSC 2159, CSSC 2126
   Emails: Kawebb@sdsu.edu, martin.phillip@gcccd.edu */


 int command(int input, int first, int last);
 int run(char* cmd, int input, int first, int last);
 char* skipspace(char* s);
 void waiting(int temp_n);
  void tokenize(char* cmd);
 int dowork();
