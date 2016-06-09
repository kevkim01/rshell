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
#include <fcntl.h>


using namespace std;

class Command       //class command to make objects out of each command
{
    private:
        string command;     //the command to be executed
        string argument;    //argument for the command
        
        ////// edit //////
        string file;        //name of the specified file for I/O redirection
        string file2;
        
        Command* next;// = 0;  //used with ; connector
        Command* fail;// = 0;  //used with || connector
        Command* pass;// = 0;  //used with && connector
        Command* following; //any command with commands following it will have this
        
        bool open_paren;
        bool close_paren;
        
        ///// edit /////
        bool input;   //if '<' is found
        bool output;  //if '>' is found
        bool output1; //if '>>' is found
        bool has_pipe;
        
    public:
        Command()
        {
          next = NULL;
          fail = NULL;
          pass = NULL;
          following = NULL;
          //pipe = NULL;
          open_paren = false;
          close_paren = false;
          input = false;
          output = false;
          output1 = false;
          has_pipe = false;
        };
        Command(string c, string a)     //constructor
        {
            command = c; 
            argument = a;
            next = NULL;
            fail = NULL;
            pass = NULL;
            following = NULL;
            //pipe = NULL;
            open_paren = false;
            close_paren = false;
            input = false;
            output = false;
            output1 = false;
            has_pipe = false;
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
        
        void set_open_paren(bool b)
        {open_paren = b;}
        
        void set_close_paren(bool b)
        {close_paren = b;}
        
        ////////////////////////////////
        void set_file(string t)
        {file = t;}
        
        void set_file2(string t)
        {file2 = t;}
        
        void set_input(bool b)
        {input = b;}
        
        void set_output(bool b)
        {output = b;}
        
        void set_output1(bool b)
        {output1 = b;}
        
        void set_has_pipe(bool b)
        {has_pipe = b;}
        /////////////////////////////////
        
        bool get_close_paren()
        {return close_paren;}
        
        /////////////////////////////////
        string get_file()
        {return file;}
        
        string get_file2()
        {return file2;}
          
        bool get_input()
        {return input;}
        
        bool get_output()
        {return output;}
        
        bool get_output1()
        {return output1;}
        
        bool get_has_pipe()
        {return has_pipe;}
        ///////////////////////////////////
        
        bool run(string x, string y, string z, string z1, bool a, bool b, bool c)//, bool d)//, bool &istrue)//, bool &global)
        {
          //x = command, y = argument, z = file name, z1 = second file name, a = t/f for <, b = t/f for >, c = t/f for >>, d is t/f for |
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
              const char* f_name = z.c_str(); //f_name is the name of the file
              const char* f_name2 = z1.c_str();
              if(a == true && b == true)
              {
                int in, out;
                in = open(f_name ,O_RDONLY);
                out = open(f_name2 ,O_WRONLY|O_CREAT,0666); // Should also be symbolic values for access rights
                //out = open(f_name, O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR|S_IRGRP|S_IWGRP|S_IWUSR);
                
                dup2(in, 0);
                dup2(out,1);
                
                close(in);
                close(out);
              }
              else if(a == true)
              {
                int in = open(f_name ,O_RDONLY);
                dup2(in,STDIN_FILENO);
                close(in);
              }
              
              else if(b == true)
              {
                int out = open(f_name ,O_WRONLY|O_CREAT,0666); // Should also be symbolic values for access rights
                dup2(out,STDOUT_FILENO);
                close(out);
              }
              
              else if(c == true)
              {
                int out1 = open(f_name ,O_WRONLY|O_APPEND|O_CREAT,0666); // Should also be symbolic values for access rights
                dup2(out1,STDOUT_FILENO);
                close(out1);
              }
              
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
              //edit
              int errno;
              errno = execvp(command, args);
              if(errno < 0)
              {
                  perror("bash");
                  exit(1);
              }
              exit(0);
            }
            else if(pid > 0)     //parent from fork, will wait for child to finish running
            {
                int status = 0;
                waitpid(pid,&status, 0);
                if(status > 0)
                {
                  istrue = false;
                }
                return istrue;
            }
            
            return istrue;
        }
        
        //////////////////
        
        bool checkExists(const char *path)
        {
            struct stat info;
            bool b = false;
            if(stat(path,&info) == 0 )
            {
                if(info.st_mode & S_IFDIR)
                {
                    cout << "(True)" << endl;
                    b = true;
                }
                else if(info.st_mode & S_IFREG)
                {
                    cout<< "(True)" << endl;
                    b= true;
                }
            }
            else
            {
                cout << "(False)" << endl;
                b= false;
            }
            return b;
        }
        
        bool checkF(const char *path)
        {
            struct stat info;
            bool b = false;
            if(stat(path,&info) == 0 )
            {
                if(info.st_mode & S_IFDIR)
                {
                    cout << "(False)" << endl;
                    b = false;
                }
                else if(info.st_mode & S_IFREG)
                {
                    cout<< "(True)" << endl;
                    b = true;
                }
            }
            else
            {
                cout << "(False)" << endl;
                b = false;
            }
            return b;
        
        }
        bool checkD(const char *path)
        {
             struct stat info;
            bool b = false;
            if(stat(path,&info) == 0 )
            {
                if(info.st_mode & S_IFDIR)
                {
                    cout << "(True)" << endl;
                    b = true;
                }
                else if(info.st_mode & S_IFREG)
                {
                    cout<< "(False)" << endl;
                    b = false;
                }
            }
            else
            {
                cout << "(False)" << endl;
                b = false;
            }
            return b;
        
        }
        
        bool test(string line)
        {
            vector<string> strs;
            boost::split(strs,line,boost::is_any_of(" "));
            
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
                    cout << "unrecognized tag " << endl;
                    b = false;
                }
            }
            else if(strs.size() > 2)
            {
              cout << "unrecognized tag " << endl;
              b = false;
            }
            else
            {
                const char *test_command = strs.at(0).c_str();
                b = checkExists(test_command);
            }
            return b;
        }
        
        char** help(vector<Command> v, int index)
        {
            string x = v.at(index).get_cmd();
            string y = v.at(index).get_arg();
            string z = v.at(index).get_file();
            string z1 = v.at(index).get_file2();
            char* command = (char*)x.c_str();
            
            vector<string> argus;           //vector of argument strings
            
            istringstream ss(y);
            string temp;
            while(ss >> temp)
            {
                argus.push_back(temp);
            }
            int size = 2 + argus.size();    //size = command + NULL + # of arguments
            char** const args = new char*[size];
            args[0] = command;
            args[size - 1] = NULL;
            for (unsigned i = 1; i <= argus.size(); i++)
            {
              args[i] = (char*)argus.at(i - 1).c_str();
            }
            return args;
        }
        
        void pipe_execute_one(vector<Command> v)        // cmd | cmd
        {
          pid_t pid1 = fork();
          
          if(pid1 < 0)
          {
            perror("fork");
            exit(1);
          }
          else if(pid1 == 0)
          {
            int pipefd[2];
            pipe(pipefd);
          
            pid_t pid = fork();
            if(pid < 0)
            {
              perror("fork");
              exit(1);
            }
          
            if (pid == 0)
            {
              dup2(pipefd[1], 1);
              close(pipefd[0]);
              
              char ** cmd = help(v,0);
              char* command = (char*)v.at(0).get_cmd().c_str();
              
              string z = v.at(0).get_file();
              string z1 = v.at(0).get_file2();
              bool a = v.at(0).get_input();
              bool b = v.at(0).get_output();
              bool c = v.at(0).get_output1();
              const char* f_name = z.c_str(); //f_name is the name of the file
              const char* f_name2 = z1.c_str();
              if(a == true && b == true)
              {
                int in, out;
                in = open(f_name ,O_RDONLY);
                out = open(f_name2 ,O_WRONLY|O_CREAT,0666); // Should also be symbolic values for access rights
                //out = open(f_name, O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR|S_IRGRP|S_IWGRP|S_IWUSR);
                
                dup2(in, 0);
                dup2(out,1);
                
                close(in);
                close(out);
              }
              else if(a == true)
              {
                int in = open(f_name ,O_RDONLY);
                dup2(in,STDIN_FILENO);
                close(in);
              }
              
              else if(b == true)
              {
                int out = open(f_name ,O_WRONLY|O_CREAT,0666); // Should also be symbolic values for access rights
                dup2(out,STDOUT_FILENO);
                close(out);
              }
              
              else if(c == true)
              {
                int out1 = open(f_name ,O_WRONLY|O_APPEND|O_CREAT,0666); // Should also be symbolic values for access rights
                dup2(out1,STDOUT_FILENO);
                close(out1);
              }
              
              execvp(command, cmd);
              perror("execvp");
              exit(1);
            }
            else if(pid > 0)
            {
              int status = 0;
              waitpid(pid, &status, 0);
              
              dup2(pipefd[0], 0);
              close(pipefd[1]);
              
              string z = v.at(1).get_file();
              string z1 = v.at(1).get_file2();
              bool a = v.at(1).get_input();
              bool b = v.at(1).get_output();
              bool c = v.at(1).get_output1();
              const char* f_name = z.c_str(); //f_name is the name of the file
              const char* f_name2 = z1.c_str();
              
              char ** cmd1 = help(v,1);
              char* command1 = (char*)v.at(1).get_cmd().c_str();
              
              if(a == true && b == true)
              {
                int in, out;
                in = open(f_name ,O_RDONLY);
                out = open(f_name2 ,O_WRONLY|O_CREAT,0666); // Should also be symbolic values for access rights
                //out = open(f_name, O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR|S_IRGRP|S_IWGRP|S_IWUSR);
                
                dup2(in, 0);
                dup2(out,1);
                
                close(in);
                close(out);
              }
              else if(a == true)
              {
                int in = open(f_name ,O_RDONLY);
                dup2(in,STDIN_FILENO);
                close(in);
              }
              
              else if(b == true)
              {
                int out = open(f_name ,O_WRONLY|O_CREAT,0666); // Should also be symbolic values for access rights
                dup2(out,STDOUT_FILENO);
                close(out);
              }
              
              else if(c == true)
              {
                int out1 = open(f_name ,O_WRONLY|O_APPEND|O_CREAT,0666); // Should also be symbolic values for access rights
                dup2(out1,STDOUT_FILENO);
                close(out1);
              }
              
              execvp(command1, cmd1);
              perror("execvp");
              exit(1);
            }
          }
          else if(pid1 > 0)
          {
            int status = 0;
            waitpid(pid1, &status,0);
            return;
          }
        }

        
        void execute(int t, bool previous)
        {
          string x = this -> command;
          string y = this -> argument;
          string z = this -> file;
          string z1 = this -> file2;
          
          bool a = get_input();   //gets t/f for <
          bool b = get_output();  //gets t/f for >
          bool c = get_output1(); //gets t/f for  >>
          //bool d = false;
          
          bool executed = true;
          
          ////////////////////////////////////////////////////////////////////////////////////////////
          if(t == 0 && open_paren == true)
          {
            for(Command* curr = following; curr != NULL; curr = curr -> following)
            {
              if(curr -> get_close_paren() == true)
              {
                //cout << curr -> get_cmd() << endl;
                curr -> execute(0, false);
                break;
              }
            }
            return;
          }
          
          ////////////////////////////////////////////////////////////////////////////
          vector<Command> pipe_cmds;
          int count = 1;
          if(has_pipe == true)
          {
            pipe_cmds.push_back(*this);
            
            for(Command* curr = following; curr != NULL; curr = curr -> following)
            {
              pipe_cmds.push_back(*curr);
              ++count;
              if(curr -> get_has_pipe() == false)
              {
                if(count == 2)
                {
                    pipe_execute_one(pipe_cmds);
                }
                if(curr -> following != NULL)
                {
                  curr -> following -> execute(1, true);
                }
                break;
              }
            }
            return;
          }
          ////////////////////////////////////////////////////////////////////////////
          
          //edit------------------------------------------------------------------------
          if(x == "test")                 //handles the test command
          {
            executed = test(y);
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
          
          
          if(x == "")       //empty command, just returns
          {
            return;
          }
          
          if(t == 1)                //if the current command will execute
          {
            executed = run(x,y,z,z1,a,b,c);//,d);
          }   
          else                                //if current command wont execute
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