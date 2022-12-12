#include "constants.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int total_Clients_A = 0, total_Clients_B = 0, total_Count_Clients = -1;

// Logic to choose Servers
int choose_Server_A_B()
{
    int choose_server;
    if (total_Clients_A < MAX_CLIENT)
        choose_server = 1;
    else if (total_Clients_B < MAX_CLIENT)
        choose_server = 0;
    else if (total_Count_Clients % 2 == 0)
        choose_server = 0;
    else
        choose_server = 1;

    return choose_server;
}

int run_command(char *buff, int length)
{
    
    if (fork() == 0)
    {
        system(buff);
        fprintf(stderr, "\nrRunning system command");
        exit(1);
    }
    else
    {
        wait(NULL);
    }
}

void ServeClient(int sd, const char *serverType)
{
    char message[MAX_BUFFER];
    int n;

    dup2(sd, STDOUT_FILENO);
    dup2(sd, STDIN_FILENO);
    dup2(sd, STDERR_FILENO);
    while (1)
    {
        fprintf(stderr, "\nmessage : Client input ");
        n = read(sd, message, MAX_BUFFER);
      
        //  'quit' to exit
        message[n] = '\0';
        fprintf(stderr, "\nmessage :  command -> %s", message);
        if (strncmp(message, "quit", 4) == 0)
        {
            fprintf(stderr, "Successfully closed : %s\n", message);
            close(sd);
            exit(0);
        }
        else
        {
            run_command(message, n);
            write(sd, "Success-", 9);
        }
    }
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("\nSyntax : ./Server A or B\n");
        exit(1);
    }

    int sd, server_a_id, client, n;
    char buffer[MAX_BUFFER];
    if (strcmp(argv[1], "A") == 0)
    {
        create_Server(&sd, PORT_A);
    }
    else
    {
        create_Server(&sd, PORT_B);
        connect_Server(&server_a_id, IP_SERVER_A, PORT_A);
        fprintf(stderr, "\nMessage : Server B -> A connected");
        write(server_a_id, "s", 1);
    }

    fprintf(stderr, "\n%s Server running---------", argv[1]);

    while (1)
    {
        client = accept(sd, (struct sockaddr *)NULL, NULL);

        if (strcmp(argv[1], "A") == 0)
        {
            total_Count_Clients++;
            n = read(client, buffer, MAX_BUFFER);
            buffer[n] = '\0';

            if (strncmp(buffer, "s", 1) == 0)
            {
                fprintf(stderr, "\nMessage : Server: %s connected", buffer);
                continue;
            }
            else
            {
                fprintf(stderr, "\nMessage : Client: %s connected", buffer);

                int choose_server_A = choose_Server_A_B();
                if (choose_server_A)
                {
                    total_Clients_A++;
                    write(client, "A", 1);
                }
                else
                {
                    total_Clients_B++;
                    write(client, "B", 1);
                    close(client);
                    continue;
                }
            }
        }

        else
            fprintf(stderr, "\n Recieved new client");

        if (fork() == 0)
            ServeClient(client, argv[1]);
    }
    return 0;
}