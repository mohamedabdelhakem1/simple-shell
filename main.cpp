#include <bits/stdc++.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string>
#include <cstring>
#include <signal.h>
#include <ctime>
#include <chrono>
using namespace std;


vector <string> parseInput (string input) {

    vector <string> tokens;
    int curr = 0;
    //regex reg("([^\\s]+)(\\s*)");
    regex reg("([\\']{1}.+[\\']{1}|[^\\s]+)(\\s*)");
    int n = input.size();
    while(curr < n) {
        smatch m;
        regex_search(input, m, reg);
        if(m.str(1)[0] == '\'' && m.str(1)[m.str(1).size()-1] == '\'') {
            string sub = m.str(1);
            sub = sub.substr(1, sub.size()-2);
            tokens.push_back(sub);
        } else {
            tokens.push_back(m.str(1));
        }
        curr += m.str(0).size();

        input = input.substr(m.str(0).size());
    }
    return tokens;
}
char ** convertToCstring( vector <string>tokens) {

    char ** args_list = new char*[tokens.size()+1];
    args_list[tokens.size()] = NULL;
    for(int i = 0; i < tokens.size() ; i++) {
        args_list[i]= strdup(tokens[i].c_str());

    }
    return args_list;
}
void signal_stop_handler(int signal_number) {
    if(signal_number == SIGCHLD || signal_number == SIGCLD) {
        ofstream myfile;
        time_t ltime = time(NULL);;
        string output = asctime( localtime(&ltime) ) ;
        output += "Child process was terminated"  ;
        myfile.open ("log", std::ios_base::app);
        myfile  <<output<<endl;
        myfile.close();
    }
}

int main() {
    string username = getenv("USER");
    char dir[1000];
    int bg_processes = 0;
    pid_t pid;

    while(1) {
        // print the username and directory shell opened in
        cout<<username<<"> "<<getcwd( dir, 1000)<<": ";
        // get command line and parse input
        string input;
        getline(cin,input);
        if( input == "" ) {
            continue;
        }
        // parse the input into tokens
        vector <string> tokens = parseInput(input);
        // convert the tokens from vector of strings to char**
        char** args_list = convertToCstring(tokens);
        // flipping bg_processes to stop the program from waiting to
        // the child process
        if(strcmp(args_list[tokens.size()-1],"&") == 0) {
            bg_processes = 1;
            args_list[tokens.size()-1] = NULL;
        } else {
            bg_processes = 0;
        }


        if (strcmp(args_list[0],"cd") == 0) {
            if (chdir(args_list[1]) < 0) {
                cout<<"no such command or directory"<<endl;
                exit(1);
            }
        } else if(strcmp(args_list[0],"exit") == 0) {
            exit(0);
        } else {
            // create child process
            pid = fork();

            if(pid == 0) {
                if (execvp(args_list[0],args_list) < 0) {
                    cout<<"no such command or directory"<<endl;
                    exit(1);
                }

            } else {
                signal(SIGCHLD, signal_stop_handler);
                if(bg_processes == 0) {
                    wait(NULL);
                }
            }

        }

    }
    return 0;
}
