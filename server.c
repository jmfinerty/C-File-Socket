#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define READ_LEN 3000

void error(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(0);
}

int main(int argc, char *argv[])
{

    // Check that command contained the correct number of parameters
    if (argc != 2) {
        fprintf(stderr, "usage: %s port\n", argv[0]);
        exit(0);
    }
    int port = atoi(argv[1]);

    // Create socket: AF_INET : ipv4, SOCK_STREAM : TCP, 0 is protocol value for IP
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) 
        error("ERROR: error opening socket.");
    
    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    if (bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) 
        error("ERROR: error binding.");

    listen(sock, 5); // 5 = size of queue of connection, shouldn't ever need > 1
    printf("Server started........\n");

    // Continuously receive files as they are sent
    while(1) {
        
        struct sockaddr_in client_addr;
        int len_client_addr = sizeof(client_addr);
        int newsock = accept(sock, (struct sockaddr *) &client_addr, &len_client_addr);    
        if (newsock < 0)
            error("ERROR: error connecting.");
        printf("Client connected......\n");

        // Receive name of file
        char filetoreceive[READ_LEN] = {0};
        if (recv(newsock, filetoreceive, READ_LEN, 0) < 0)
            error("ERROR: error receiving file name.");
        
        // Open file to write to
        FILE *f = fopen(filetoreceive, "wb");
        if (f == NULL)
            error("ERROR: error opening file.");

        // Receive file
        char buffer[READ_LEN] = {0};
        int n = recv(newsock, buffer, READ_LEN, 0);
        while (n > 0) {
            if (fwrite(buffer, sizeof(char), n, f) != n)
                error("ERROR: error writing file.");
            memset(buffer, 0, READ_LEN);
            n = recv(newsock, buffer, READ_LEN, 0);
        }
        printf("File received.........\n");

        fclose(f);
        close(newsock);
        printf("Connection ended......\n");

    }
    
    return 0; 

}