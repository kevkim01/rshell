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
  
  One of the bugs we found is with the exit command. When the user enters
"exit", the program should execute any commands prior, and then break out
of the program, or if there are no prior commands, the program should just
end. However, we can't seem to find the source of our error when something
like this is called:

          echo hi && echo a || ls ; sl || date
          date ; sl || echo a && echo b && echo c
          ls || date || echo hi; ls -a
          exit

in succession, the commands themselves would run, but for some reason,
when the exit is called, the program doesn't exit right away. Instead,
you need to continuously type exit, until the program ends. (I don't 
actually know if the case above will do this, but when a lot of commands
have been processed and run, this will happen). We think that the problem 
lies in the nesting created by the for loop. We can't really pinpoint the 
cause since we created a destructor that deletes the dynamically allocated
data and don't have any nested loops.



