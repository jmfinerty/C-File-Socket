#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <libgen.h> // basename()

#define READ_LEN 3000

void error(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(0);
}

int main(int argc, char *argv[])
{

    // Check that command contained the correct number of parameters
    if (argc != 4) {
        fprintf(stderr, "USAGE: %s hostname port filepath\n", argv[0]);
        exit(0);
    }

    // Read in command-line parameters
    int port = atoi(argv[2]);
    char *filetosend = basename(argv[3]); // basename was needed so same dir was not written at server
    struct hostent *server = gethostbyname(argv[1]);
    if (server == NULL) 
        error("ERROR: no such host exists.");

    // Create socket: AF_INET : ipv4, SOCK_STREAM : TCP, 0 is protocol value for IP
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) 
        error("ERROR: error opening socket.");

    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(port);
    if (connect(sock,(struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        error("ERROR: error connecting to server.");
    printf("Connected to server...\n");

    // Open file
    FILE *f = fopen(argv[3], "rb");
    if (f == NULL)
        error("ERROR: error opening file.");

    // Send file name
    char buffer[READ_LEN];
    strncpy(buffer, filetosend, strlen(filetosend));
    if (send(sock, buffer, READ_LEN, 0) < 0)
        error("ERROR: error sending file name.");
    
    // Send file
    char sendline[READ_LEN] = {0};
    int nread = fread(sendline, sizeof(char), READ_LEN, f);
    while (nread > 0) {
        if (send(sock, sendline, nread, 0) < 0)
            error("ERROR: error sending file.");
        memset(sendline, 0, READ_LEN);
        nread = fread(sendline, sizeof(char), READ_LEN, f);
    }
    printf("File sent.............\n");

    // End sending
    fclose(f);
    close(sock);
    printf("Connection ended......\n");
    return 0;

}