#ifndef COMMAND_H
#define COMMAND_H

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

using namespace std;
using namespace boost;

class Command       //class command to make objects out of each command
{
    private:
        string command;     //the command to be executed
        string argument;    //argument for the command
        Command* next;// = 0;  //used with ; connector
        Command* fail;// = 0;  //used with || connector
        Command* pass;// = 0;  //used with && connector
        bool is_exit; //tells whether the command is execute or not
    public:
        Command()
        {
          next = NULL;
          fail = NULL;
          pass = NULL;
          is_exit = false;
        };
        Command(string c, string a)     //constructor
        {
            command = c; 
            argument = a;
            next = NULL;
            fail = NULL;
            pass = NULL;
            is_exit = false;
        }
        //edit made
        void set_exit(bool x)
        {
          is_exit = x;
        }
        //edit end
        string get_cmd()            //returns the command
        {return command;}
        
        string get_arg()            //returns the argument
        {return argument;}
        
        void set_next(Command* c)        //sets pointer to next command in case of ;
        {next = c;}
        
        void set_fail(Command* c)        //sets pointer to next command in case of ||
        {fail = c;}
        
        void set_pass(Command* c)        //sets pointer to next command in case of &&
        {pass = c;}
        
        bool run(string x, string y)
        {
            pid_t pid = fork();
            if(pid == -1)       //if fork doesnt run correctly - error
            {
                perror("fork");
                exit(0);
            }
            if(pid == 0)            //child function of fork()
            {
                char* command = (char*)x.c_str();
                
                vector<string> argus;           //vector of argument strings
                char_separator<char> sep(" ");
                
                tokenizer<char_separator<char> > tokens(y, sep);
                
                BOOST_FOREACH(string t, tokens)
                {
                  argus.push_back(t);
                } //splitting the argument into separate strings
                
                int size = 2 + argus.size();    //size = command + NULL + # of arguments
                char** const args = new char*[size];
                args[0] = command;
                args[size - 1] = NULL;
                for (unsigned i = 1; i <= argus.size(); i++)
                {
                    args[i] = (char*)argus.at(i - 1).c_str();
                }
                if(execvp(command, args) < 0)
                {
                    return false;
                    exit(1);
                }
                return true;
            }
            if(pid > 0)     //parent from fork, will wait for child to finish running
            {
                int status;
                waitpid(pid,&status,0);
                return true;
            }
            return false;
        }
        
        void execute()        //executes command 
        {
            string x = this -> command;
            string y = this -> argument;
            bool exi = this -> is_exit;

            if(x == "")     //if the command recieved is essentially nothing, the program will do nothing
            {
                return;
            }
            if(exi == true)
            {
              exit(0);
            }
            
            bool executed = run(x,y);
            if(next)  //if the command has a next pointer execute next command regardless
            {
                if(executed == false)
                {
                     cout << "bash: invalid command: " << command << " or argument: " << argument << endl;
                }
                next -> execute();
            }
            if(pass && executed == true)  //if the command has a pass pointer and the previous executed
            {
                pass -> execute();
            }
            if(fail && executed == false) //if the command has a fail pointer and the previous executed
            {
                cout << "bash: invalid command: " << command << " or argument: " << argument << endl;
                fail -> execute();
            }
            return;
        }
};

#endif
