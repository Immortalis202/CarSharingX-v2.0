#include "include/server.h"
#include "include/commons.h"
#include <string.h>

char buffer[32];
int cityAdded = 0;
int sockfd;
int connfd;
int len;
struct sockaddr_in servaddr;
struct sockaddr_in cli;

int createSocket(int sockfd, int connfd,struct sockaddr_in servaddr, struct sockaddr_in cli ){
    int len;
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");

    return connfd;
}

void receiveMessage(int connfd){
    char buffer1[2];
    read(connfd, buffer1, 2);
    printf("arrived %s1\n",buffer1);
    strcpy(buffer, buffer1);    
}

// Driver function
int main()
{

    
    populateGraph();
    connfd =createSocket(sockfd,  connfd, servaddr,  cli);

    receiveMessage(connfd);
    if(switchChoice(buffer,connfd) == -1){
        write(connfd, "Wrong Input", 11);
    }
    // Function for chatting between client and server


    // After chatting close the socket
    close(sockfd);
}
