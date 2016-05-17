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
#include <sys/types.h>
#include <pwd.h>
#include <vector>


using namespace std;

class Command       //class command to make objects out of each command
{
    private:
        string command;     //the command to be executed
        string argument;    //argument for the command
        Command* next;// = 0;  //used with ; connector
        Command* fail;// = 0;  //used with || connector
        Command* pass;// = 0;  //used with && connector
        Command* following; //any command with commands following it will have this
    public:
        Command()
        {
          next = NULL;
          fail = NULL;
          pass = NULL;
          following = NULL;
        };
        Command(string c, string a)     //constructor
        {
            command = c; 
            argument = a;
            next = NULL;
            fail = NULL;
            pass = NULL;
            following = NULL;
        }

        //edit made: destructor
        void clear()
        {
          for(Command* curr = following; curr != NULL; curr = curr -> following)
          {
            delete curr;
          }
        }

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

        void set_following(Command* c)
        {following = c;}

        bool run(string x, string y)//, bool &global)
        {
          if(x == "exit")
          {
            exit(0);
          }
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

                istringstream ss(y);
                string temp;
                while(ss >> temp)
                {
                  argus.push_back(temp);
                }
                //edit
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
                    perror("bash");
                    return false;
                    //exit(1);
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
        
        void execute(int t, bool previous)
        {
          string x = this -> command;
          string y = this -> argument;

          if(x == "")
          {
            return;
          }
          bool executed;
          if(t == 1)
          {
            executed = run(x,y);
          }
          else
          {
            if(fail && previous == true)
            {
              fail -> execute(0, false);
            }
            if(fail && previous == false)
            {
              fail -> execute(1, true);
            }
            if(next)
            {
              next -> execute(1, true);
            }
            if(pass)
            {
              pass -> execute(1, true);
            }
            return;
          }
          if(next)
          {
            next -> execute(1, true);
          }
          if(pass && executed == true)
          {
            pass -> execute(1, true);
          }
          if(pass && executed == false)
          {
            pass -> execute(0, false);
          }
          if(fail && executed == false)
          {
            fail -> execute(1, false);
          }
          if(fail && executed == true)
          {
            fail -> execute(0, true);
          }
          return;
        }
};

#endif
