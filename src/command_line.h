#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <iostream>
#include <cstring>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <sys/types.h>
#include <pwd.h>
#include "command.h"

using namespace std;
using namespace boost;

class Command_Line          //class that handles the command line input and parsing
{
    private:
        string line;        //line given by the user
    public:
        Command_Line(){};   //constructor
        
        string get_line()     //returns the line input by user
        {return line;}
        
        void read_in()      //will read in the line from the user
        {
            string x;
            getline(cin, x);
            line = x;
        }
        
        void display_prompt()   //displays the user prompt
        {
            /*extra credit, displays hostname and user*/
            char hostname[128];
            char *name; 
            struct passwd *pass; 
            pass = getpwuid(getuid()); 
            name = pass->pw_name;
            gethostname(hostname, sizeof hostname);
            cout<<"(precise)";
            printf("%s", name);
            cout<<"@";
            printf("%s", hostname);
            cout<<":$ ";
        }
        
        Command parse(string line)        //parses the command line
        {
            string command;
            string argument;
            string temp;
            
            if(line == "")      //if the line has no content
            {
                Command cmd(command, argument);
                return cmd;
            }
            
            istringstream split(line);      //split the line by whitespaces
            split >> temp;
            if(temp == "exit")
            {
              bool ex = true;
              ///////////////////    edit   /////////////////
              Command cmd;
              cmd.set_exit(ex);
              return cmd;
            }
            command = temp; //command will be the first part of the string
            if(temp.at(0) == '#')
            {
                Command cmd;
                return cmd;
            }
            if(command.at(command.size() -1) == ';')        //checks to see if there is a ; after the base command
            {
                command = temp.substr(0, temp.size()-1);
                string t;
                while(split >> temp)
                { t += temp + " ";}
                t = t.substr(0, t.size()-1);
                    
                Command cmd(command, argument);
                    
                Command* ptr = new Command;
                *ptr = parse(t);
                cmd.set_next(ptr);
                //edit
                cmd.set_following(ptr);
                //end edit
                return cmd;
            }
            
            while(split >> temp)    //continues to read in the string until a connector is met
            {
                if(temp.at(temp.size() -1) == ';')  //checks for ; at the end of an argument (no space)
                {
                    string s = temp.substr(0, temp.size()-1);
                    argument += s;
                    
                    string t;
                    while(split >> temp)
                    {
                        t += temp + " ";
                    }
                    t = t.substr(0, t.size()-1);
                    
                    Command cmd(command, argument);
                    
                    Command* ptr = new Command;
                    *ptr = parse(t);
                    cmd.set_next(ptr);
                    //edit
                    cmd.set_following(ptr);
                    //end edit

                    return cmd;
                }
                if(temp.at(0) == '#')       //checks for comments at beginning (no space)
                {
                    Command cmd(command, argument);
                    return cmd;
                }
                else if(temp == "#")       // checks for comments (space on both sides)
                {
                    Command cmd(command, argument);
                    return cmd;
                }
                else if(temp == "&&")       // checks for && (spaces on both sides)
                {
                    string t;
                    while(split >> temp)
                    {
                        t += temp + " ";
                    }
                    t = t.substr(0, t.size()-1);
                    
                    Command cmd(command, argument);
                    
                    Command* ptr = new Command;
                    *ptr = parse(t);
                    cmd.set_pass(ptr);
                    
                    //edit
                    cmd.set_following(ptr);
                    //end edit

                    return cmd;
                }
                else if(temp == "||")       //checks for || (spaces on both sides)
                {
                    string t;
                    while(split >> temp)
                    {
                        t += temp + " ";
                    }
                    t = t.substr(0, t.size()-1);
                    
                    Command cmd(command, argument);
                    
                    Command* ptr = new Command;
                    *ptr = parse(t);
                    cmd.set_fail(ptr);
                    
                    //edit
                    cmd.set_following(ptr);
                    //end edit

                    return cmd;
                }
                argument += temp + " ";     //if not a connector it is part of the argument
            }
        Command cmd(command, argument); //default, function needs to return at least a command
        return cmd;
        }
};

#endif
