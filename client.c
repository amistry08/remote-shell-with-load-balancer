#include "header.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    char buff[MAX_BUFFER];
    int server, n;

    // Connecting to Server A
    connect_Server(&server, IP_SERVER_A, PORT_A);
    write(server, "c", 1);

    n = read(server, buff, 1);
    buff[n] = '\0';

    if (strncmp(buff, "A", 1) == 0)
    {
        fprintf(stderr, "\nServer connected to %s", buff);
    }
    else
    {
        connect_Server(&server, IP_SERVER_B, PORT_B);
        fprintf(stderr, "\nServer connected to %s", buff);
    }

    while (1)
    {
        
        write(STDOUT_FILENO, "--------------------------\n\n\n>>>", 32);
        n = read(STDIN_FILENO, buff, MAX_BUFFER);
        buff[n] = '\0';

        write(server, buff, strlen(buff) + 1);
        
        //  'quit' to exit
        if (strncmp(buff, "quit", 4) == 0)
        {
            printf("Successfull closed connection\n");
            close(server);
            exit(0);
        }
        do
        {
            if (n = read(server, buff, MAX_BUFFER))
            {
                buff[n] = '\0';
                write(STDOUT_FILENO, buff, n + 1);
            }
        } while (strncmp(buff, "Success-", 8));
    }

    return 0;
}
