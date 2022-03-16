// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include <iostream>

#include <string>

#define PORT 80

int main(int argc, char const *argv[])
{
    int sock = 0; long valread;
    struct sockaddr_in serv_addr;
    std::string http_request_mes = "POST /cgi-bin/process.cgi HTTP/1.1\r\n"
                            "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
                            "Host: www.tutorialspoint.com\r\n"
                            "Content-Type: application/x-www-form-urlencoded\r\n"
                            "Content-Length: 49\r\n"
                            "Accept-Language: en-us\r\n"
                            "Accept-Encoding: gzip, deflate\r\n"
                            "Connection: Keep-Alive\r\n\r\n"

                            "licenseID=string&content=string&/paramsXML=string";

    char buffer[2048] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }


    send(sock , http_request_mes.c_str() , 1, 0 );
    sleep(1);
    send(sock , http_request_mes.c_str() + 1 , 1, 0 );
    sleep(1);
    send(sock , http_request_mes.c_str() + 2 , 1, 0 );
    sleep(1);
    send(sock , http_request_mes.c_str() + 3 , 1, 0 );

    send(sock , http_request_mes.c_str() + 4, http_request_mes.size() - 4 , 0 );

    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}