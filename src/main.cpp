#include <iostream>
#include "command.h"
#include "command_line.h"

using namespace std;

int main()
{
    Command s;
    string line;
    while(1)
    { 
        Command_Line c;     //object that will handle reading in and parsing
        c.display_prompt();   //displays the prompt
        c.read_in();        //reads in the line from the user and stores it in the object c
        line = c.get_line();    //retrieve the line input
        s = c.parse(line);  //creates a command by parsing the line
        s.execute(1, true);
        s.clear();
    }
    return 0;
}
