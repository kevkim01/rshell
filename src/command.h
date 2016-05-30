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
#include <pwd.h>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <string>
#include <sys/stat.h>


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
        bool open_paren;  //if the command is the begining of parenthesis
        bool close_paren; //if the command is the end of the parenthesis
    public:
        Command()
        {
          next = NULL;
          fail = NULL;
          pass = NULL;
          following = NULL;
          open_paren = false;
          close_paren = false;
        };
        Command(string c, string a)     //constructor
        {
            command = c; 
            argument = a;
            next = NULL;
            fail = NULL;
            pass = NULL;
            following = NULL;
            open_paren = false;
            close_paren = false;
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

        void set_open_paren(bool b)   //sets the open parenthesis data
        {open_paren = b;}

        void set_close_paren(bool b)  //sets the closed parenthesis data
        {close_paren = b;}

        bool get_close_paren()      //returns the value of the close_paren data member
        {return close_paren;}

        bool checkExists(const char *path)
        {
          struct stat info;
          bool b = false;
          if(stat(path, &info) == 0)
          {
            if(info.st_mode & S_IFDIR)
            {
              cout << "(TRUE)" << endl;
              cout << "Exists" << endl;
              b = true;
            }
            else if(info.st_mode & S_IFREG)
            {
              cout << "(TRUE)" << endl;
              cout << "Exists" << endl;
              b = true;
            }
          }
          else
          {
            cout << "(FALSE)" << endl;
            cout << "Not a file/directory" << endl;
            b = false;
          }
          return b;
        }

        bool checkF(const char *path)
        {
          struct stat info;
          bool b = false;
          if(stat(path, &info) == 0)
          {
            if(info.st_mode & S_IFDIR)
            {
              cout << "(FALSE)" << endl;
              cout << "It's a directory " << endl;
              b = false;
            }
            else if(info.st_mode & S_IFREG)
            {
              cout << "(TRUE)" << endl;
              cout << "It's a file " << endl;
              b = true;
            }
          }
          else
          {
            cout << "(FALSE)" << endl;
            cout << "Not a file/directory" << endl;
            b = false;
          }
          return b;
        }

        bool checkD(const char *path)
        {
          struct stat info;
          bool b = false;
          if(stat(path, &info) == 0)
          {
            if(info.st_mode & S_IFDIR)
            {
              cout << "(TRUE)" << endl;
              cout << "It's a directory" << endl;
              b = true;
            }
            else if(info.st_mode & S_IFREG)
            {
              cout << "(FALSE)" << endl;
              cout << "It's a file" << endl;
              b = false;
            }
          }
          else
          {
            cout << "(FALSE)" << endl;
            cout << "Not a file/directory" << endl;
            b = false;
          }
          return b;
        }

        bool test(string line)
        {
          vector<string> strs;
          boost::split(strs, line, boost::is_any_of(" "));

          bool b = false;
          if(strs.at(0).at(0) == '-')
          {
            const char *test_command = strs.at(1).c_str();
            if(strs.at(0) == "-e")
            {
              b = checkExists(test_command);
            }
            else if(strs.at(0) == "-d")
            {
              b = checkD(test_command);
            }
            else if(strs.at(0) == "-f")
            {
              b = checkF(test_command);
            }
            else if(strs.at(0).size() > 2)
            {
              cout << "unrecognized tag" << endl;
              b = false;
            }
            else
            {
              cout << "unrecognized tag" << endl;
              b = false;
            }
          }
          else if(strs.size() > 2)
          {
            cout << "unrecognized tag" << endl;
            b = false;
          }
          else
          {
            const char *test_command = strs.at(0).c_str();
            b = checkExists(test_command);
          }
          return b;
        }
    

        bool run(string x, string y)//, bool &global)
        {
          bool istrue = true;
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
            else if(pid == 0)            //child function of fork()
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
                int errno;
                errno = execvp(command, args);
                if(errno < 0)
                {
                    perror("bash");
                    //return false;
                    exit(1);
                }
                exit(0);
            }
            else if(pid > 0)     //parent from fork, will wait for child to finish running
            {
                int status = 0;
                waitpid(pid,&status,0);
                if(status > 0)
                {
                  istrue = false;
                }
                return istrue;
            }
            return istrue;
        }
        
        void execute(int t, bool previous)
        {
          string x = this -> command;
          string y = this -> argument;
          bool executed = true;

          if(t == 0 && open_paren == true)
          {
            for(Command* curr = following; curr != NULL; curr = curr -> following)
            {
              if(curr -> get_close_paren() == true)
              {
                curr -> execute(0, false);
                break;
              }
            }
            return;
          }


          if(x == "")
          {
            return;
          }
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
