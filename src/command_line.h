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
#include <sys/types.h>
#include <pwd.h>
#include "command.h"

using namespace std;

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
            //--------------------------------------------
            bool p_begin = false;
            bool p_end = false;
            //--------------------------------------------
            
            if(line == "")      //if the line has no content
            {
                Command cmd;
                return cmd;
            }
            
            istringstream split(line);      //split the line by whitespaces
            split >> temp;
            
            if(temp.at(0) == '#')
            {
                Command cmd;
                return cmd;
            }
            /////////////////////////////////////
            if(temp == "(")
            {
                p_begin = true;
                split >> temp;
            }
            
            if(temp.at(0) == '(')
            {
                p_begin = true;
                //in_paren = true;        //sets in_paren to true if beginning of parenthesis is recognized
                temp = temp.substr(1, temp.size()-1);
            }
            
            if(temp.at(temp.size()-1) == ')')
            {
                p_end = true;
                //in_paren = false;       //sets in_paren back to false when end of () is reached
                temp = temp.substr(0, temp.size()-1);
            }
            /////////////////////////////////////
            
            command = temp; //command will be the first part of the string
            if(command.at(command.size() -1) == ';')        //checks to see if there is a ; after the base command
            {
                command = temp.substr(0, temp.size()-1);
                if(command.at(command.size() -1) == ')')
                {
                    p_end = true;
                    //in_paren = false;
                    command = command.substr(0, command.size()-1);
                }
                string t;
                while(split >> temp)
                {
                  t += temp + " ";
                }
                t = t.substr(0, t.size()-1);
                    
                Command cmd(command, argument);
                    
                Command* ptr = new Command;
                *ptr = parse(t); //edit---------------------------
                cmd.set_next(ptr);
                cmd.set_following(ptr);
                if(p_begin == true)
                {
                    cmd.set_open_paren(true);
                }
                if(p_end == true)
                {
                    cmd.set_close_paren(true);
                }
                return cmd;
            }
            if(command == "[")
            {
                command = "test";
            }
            
            while(split >> temp)    //continues to read in the string until a connector is met
            {
                if(temp == ")")
                {
                    p_end = true;
                    continue;
                }
                if(temp.at(temp.size() -1) == ')')
                {
                    p_end = true;
                    temp = temp.substr(0, temp.size()-1);
                    //cout << temp << endl;
                    //in_paren = false;
                }
                
                if(temp.at(temp.size() -1) == ';')  //checks for ; at the end of an argument (no space)
                {
                    string s = temp.substr(0, temp.size()-1);
                    if(s.size() != 0 && s.at(s.size() -1) == ')')
                    {
                        p_end = true;
                        //in_paren = false;
                        s = s.substr(0, s.size()-1);
                    }
                    argument += s;
                    
                    string t;
                    while(split >> temp)
                    {
                        t += temp + " ";
                    }
                    t = t.substr(0, t.size()-1);
                    
                    Command cmd(command, argument);
                    
                    Command* ptr = new Command;
                    *ptr = parse(t);  //edit-------------------------
                    cmd.set_next(ptr);
                    cmd.set_following(ptr);
                    if(p_begin == true)
                    {
                        cmd.set_open_paren(true);
                    }
                    if(p_end == true)
                    {
                        cmd.set_close_paren(true);
                    }
                    return cmd;
                }
                if(temp.at(0) == '#' || temp == "#")       //checks for comments at beginning (no space)
                {
                    Command cmd(command, argument);
                    return cmd;
                }
                else if(temp == "&&" || temp == "||" || temp == ";")       // checks for && (spaces on both sides)
                {
                    string connector = temp;
                    string t;
                    while(split >> temp)
                    {
                        t += temp + " ";
                    }
                    t = t.substr(0, t.size()-1);
                    
                    Command cmd(command, argument);
                    
                    Command* ptr = new Command;
                    *ptr = parse(t);  //edit-------------------------
                    if(connector == "&&")
                    {
                      cmd.set_pass(ptr);
                    }
                    else if(connector == "||")
                    {
                      cmd.set_fail(ptr);
                    }
                    else if(connector == ";")
                    {
                      cmd.set_next(ptr);
                    }
                    cmd.set_following(ptr);
                    //////////////////////////////////////////////////////////////////////////////
                    if(p_begin == true)
                    {
                        cmd.set_open_paren(true);
                    }
                    if(p_end == true)
                    {
                        cmd.set_close_paren(true);
                    }
                    return cmd;
                }
                if(temp == "]")
                {
                    continue;
                }
                argument += temp + " ";     //if not a connector it is part of the argument
            }
        Command cmd(command, argument); //default, function needs to return at least a command
        return cmd;
        }
};

#endif