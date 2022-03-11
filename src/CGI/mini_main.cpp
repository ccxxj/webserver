#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char **argv, char **env)
{
    char **argu;
    argu = (char **)malloc(2 * sizeof(char *));
    argu[0] = strdup("cgi_tester");
    argu[1] = NULL;

//pure array approach
    char *envp[4];
    char a[] = "REQUEST_METHOD=GE";
    char b[] = "SERVER_PROTOCOL=HTTP/1.0";
    char c[] = "PATH_INFO=/Users/Xiaojing/Desktop/Codam/webservMolly/src/CGI/cgi_tester";
    envp[0] = a;
    envp[1] = b;
    envp[2] = c;
    envp[3] = NULL;

//in case want to use vector approach
    // std::vector<std::string> envp1;
    // envp1.push_back("REQUEST_METHOD=GET");
    // envp1.push_back("SERVER_PROTOCOL=HTTP/1.0");
    // char **envp;
    // envp = (char **)malloc(3 * sizeof(char *));
    // for(int i = 0; i < 2; i++)
    //     envp[i] = strdup(envp1[i].c_str());
    //     //remember to free the memory space
    // envp[2] = NULL;
    execve(argu[0], argu, envp);//why the env is not passed
}