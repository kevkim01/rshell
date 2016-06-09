  The objective of this assignment was to create a program that emulates 
a command shell similar to the one found on the Linux operating system.
Our program was made to handle single commands, as well as commands linked
together by connectors (ie: ||, &&, ;). Based on the connectors or lack of,
our program would split up the command line implemented by the user and
execute what was appropriate. 
  
  To do this we implemented two classes. The Command_line class was created
to handle: displaying the prompt to the user, reading in the user's command,
and parsing the command to be executed. We were able to finish the extra
credit portion of this assignment, by making the prompt display the username
and location of where this program was run. Arguably the hardest part of
this assignment was implementing the parse. To do so we utilized the Command
class (will describe later) to create individual commands. Each command has
pointers to the commands that follow it. Based on the connectors, found in
the users input line, the pointers activated would either be fail(for ||),
pass(for &&), or next(for ;). This allowed us to more easily execute the 
commands because we could use conditions to see if the command would execute
based on which pointer was activated.
  
  The Command class was created to make individual commands. The parse
function of the command_line class was responsible for making these commands
which are linked together by pointers. To execute these commands, we used 
recursion. Essentially, when calling the base command, it would call all 
of its subsequent commands that it points to. The execution itself was taken
care of by the run() function (used inside of execute()) which used execvp 
to run the command with its proper arguments.
  
  The main bug we found is with the exit command. When the user enters
"exit", the program should execute any commands prior, and then break out
of the program, or if there are no prior commands, the program should just
end. However, when something like this is called:

          echo hi && echo a || ls ; sl || date
          date ; sl || echo a && echo b && echo c
          ls || date || echo hi; ls -a
          exit

in succession, the commands themselves would run, but for some reason,
when the exit is called, the program doesn't exit right away. Instead,
you need to continuously type exit, until the program ends. The problem
is caused where we call execvp. When execvp == -1 we should call exit(1)
however, we call return false instead. This is because the rest of our
execute relys on the return value. We couldn't figure out a way to extract
the false value when execvp fails before calling exit. We tried using
pass by reference, global variables, and extra member functions, but
none of that worked. Thus we decided that the proper functionality of our
exit function wasn't as important as the proper functionality of the rest
of our program.

  To go into more depth. The exit function will not work when execvp fails.
Thus if a command such as sl, which is not a command is called, exit will
become buggy. As I said before there is no exit(1) called when execvp fails,
which is the cause of this problem, but we need to return false instead for
the rest of the program to work properly.

// ASSIGNMENT 3 //

  In this assignment we were to implement the test command which would test 
for the existance of a file or directory. The user could use three tags to
do so: either -e, -f, or -d. -e would simply test for the existance of the 
specified file/directory, -f would test for the existance of the specified
file, and -d would test for the existance of the specified directory. We also
had to implement the [ ] command which is the equivalent to calling "test".
Both commands do the same thing when used in conjunction with the tags and the
specified file or directory. The user can specify paths to certain files or 
directories and if no tag is used, the -e tag is used as default.

  We also had to implement parenthesis logic into our existing code. This
changed the logic of how commands were called as commands within parenthesis
were executed or not executed as a group. To do so we edited the parse function
within our command_line.h to be able to identify the beginning and the end of 
the parenthesis and in our execute command, we had a loop that would traverse 
through the commands in parenthesis and either call them or not call them.

  One problem we ran into was determining how to distinguish between a file
and a directory. To solve it we found info.st_mode and built in functions
S_IFDIR, and S_IFREG which are part of the stat class. Basically these functions
were made to distinguish between files and directories, so upon using these,
running test became much easier and worked well.

  Also the way our parse was set up, we were able to parse parenthesis. But it 
was hard for us to parse for more than one set of parenthesis. We didn't want to
hardcode where the program should look for parenthesis so we only implemented the
functionality of handling single parenthesis. 

  We also fixed the exit command from our previous assignment. We had a problem
that didn't allow us to exit the program because we werent properly testing the
exit status of execvp. However, upon fixing this our program is now able to exit
properly. 

  In the specs we were confused about the test.sh files. We weren't sure if we were
supposed to keep the test files from the last assignment or remove them from the 
test directory. We ended up just keeping them and adding the test files from this
assignment to the folder to keep everything in one place. 

//ASSIGNMENT 4//


In this lab we had to implement input/output redirection and piping. The input output redirection part of the lab wasn’t too hard. To accomplish this we used the dup function. We were able to implement the functionality of < (input redirection), > (output redirection), and  >> (output redirection). The difference between the >, and >> functions are that if the file already exists, the >> function will append the specified information to the end, while the > will overwrite the preexisting file completely.

The piping part of the lab was a bit more difficult for us based on the way we had set up our previous portions of this project. We had to parse for piping and then store the commands which were to be piped into a vector. The vector was then passed into a function that specifically executed piping. 
One problem with our program is that it can’t handle pipes that are strung together. For example, in the example provided in the specs which consists of command | command | command | command, our program won’t be able to handle this. Due to finals and other time constraints we were only able to implement functionality for two commands and one pipe (ie: command | command). If we had more time this would likely be an easy fix but right now it’s the best we have. We also implemented the functionality where, if a user wants to pipe and then use connectors they can. 

(ie: command | command ; command) will work.

Another issue to address is, only one of us was able to push this assignment and commit to github. Kevin had to go home for his sister’s graduation and was not able to work on his linux computer at school. Instead we worked together on a collaborative workspace and Dillon was responsible for adding, pushing, and commiting to git.

We also added more test cases to the test.sh folder


