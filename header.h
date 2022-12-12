#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int PORT_A = 5000,
    PORT_B = 6000,
    MAX_CLIENT = 5,
    MAX_CLIENT_PER_SERVER = 1000,
    MAX_BUFFER = 100000;

char IP_SERVER_A[] = "127.0.0.1";
char IP_SERVER_B[] = "127.0.0.1";

// Function to connect to server
void connect_Server(int *server, char *ip, int portNumber)
{

    socklen_t len;
    struct sockaddr_in servAdd;

    if ((*server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "ERROR : Socket cannot be created\n");
        exit(1);
    }

    // configuration
    servAdd.sin_family = AF_INET;
    servAdd.sin_port = htons((uint16_t)portNumber);
    if (inet_pton(AF_INET, ip, &servAdd.sin_addr) < 0)
    {
        fprintf(stderr, "ERROR : Failed to run inet_pton()\n");
        exit(2);
    }

    if (connect(*server, (struct sockaddr *)&servAdd, sizeof(servAdd)) < 0)
    {
        fprintf(stderr, "ERROR : Connection Failed...\n");
        fprintf(stderr, "Verify Server port number and try again...\n");
        exit(3);
    }
}

int create_Server(int *server, int portNumber)
{
    socklen_t len;
    struct sockaddr_in servAdd;

    if ((*server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("ERROR : Socket cannot be created\n");
        exit(1);
    }

    servAdd.sin_family = AF_INET;
    servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
    servAdd.sin_port = htons((uint16_t)portNumber);

    bind(*server, (struct sockaddr *)&servAdd, sizeof(servAdd));
    if (listen(*server, MAX_CLIENT_PER_SERVER) < 0)
    {
        printf("ERROR : Server cannot be created\n");
        exit(1);
    }
}