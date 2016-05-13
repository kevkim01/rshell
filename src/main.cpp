#include <iostream>
#include "command.h"
#include "command_line.h"

using namespace std;

int main()
{
    while(1)
    {    
        string line;
        Command_Line c;     //object that will handle reading in and parsing
        c.display_prompt();   //displays the prompt
        c.read_in();        //reads in the line from the user and stores it in the object c
        line = c.get_line();    //retrieve the line input
        Command s = c.parse(line);  //creates a command by parsing the line
        s.execute();
    }
    return 0;
}
