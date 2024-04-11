#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define PORT 5000
#define SA struct sockaddr


typedef struct {
    char *key; 
    int val;
}t_symstruct;


static t_symstruct lookuptable[] = {
    {"1", 1}, {"2",2}, {"3", 3},{"4", 4},{"5", 5},{"6", 6},{"7", 7},{"8", 8},{"9",9}
    };

#define NKEYS (sizeof(lookuptable)/sizeof(t_symstruct))

int keyFromString(char * key){
    for(int i = 0; i< NKEYS; i++){
        t_symstruct *sym = &lookuptable[i];
        if(strcmp(sym->key, key) == 0)
            return sym->val;
    }
    return -1;
}

void func(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int sendInput(char input[], int sockfd, int size){
    if(write(sockfd, input, size) == -1){
        printf("Failed to write to server");
        return -1;
    };
    return 1;
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    char choice[32];
    char bufferClient[256];

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
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
        }
    else
        printf("connected to the server..\n");

    system("sh client/title.sh");
INSERT:
    printf("INSERT\n");
    fgets(choice, 2, stdin);
    if(choice[strlen(choice) -1] != '\n'){
        int dropped = 0;
        while(fgetc(stdin) != '\n')
            dropped++;
        if(dropped > 0){
            printf("Input over the limit \n");
            fflush(stdout);
            goto ENDWHILE;
        }
    }
    if(sendInput(choice, sockfd, sizeof(choice)) == -1){
        goto INSERT;
    }

    switch(keyFromString(choice)){
        case 1:
            while(-12){
            bzero(bufferClient,sizeof(bufferClient));
            read(sockfd,bufferClient,sizeof(bufferClient));
            if(strcmp(bufferClient,"gabbopower") == 0){
                goto ENDWHILE;
            }
            printf("%s",bufferClient);
            }
        case 2: 
            
INPUT2:
            printf("INPUT2 \n");
            bzero(bufferClient, 256);
            read(sockfd,bufferClient,sizeof(bufferClient));
            
            printf("%s\n",bufferClient); //* INSERT A CITY
            
            bzero(bufferClient, 256);
            printf("\n");
            char city[32];
            fgets(city, 32, stdin);
            if(strcmp(city, "exit") == 0 || strcmp(city, "quit") == 0){
                exit(1);
            }
            write(sockfd, city, 32); // WRITE CITY
            read(sockfd,bufferClient,sizeof(bufferClient));
            read(sockfd,bufferClient,sizeof(bufferClient));

            
            if(strcasecmp(bufferClient,"overLimit") == 0){
                printf("Over the limit\n");
                goto ENDWHILE;
            } 
            if(strcasecmp(bufferClient,"onlyLetter") == 0){
                printf("Insert only letter\n");
                goto INPUT2;
            }  
            if(strcasecmp(bufferClient,"City not found") == 0){
                //printf("City not found\n");
                printf("%s\n",bufferClient);
                //printf("Reinsert \n");
                fflush(stdout);
                // read(sockfd,bufferClient,sizeof(bufferClient));
            }
            while(-12){
                bzero(bufferClient,sizeof(bufferClient));
                read(sockfd,bufferClient,sizeof(bufferClient));
                if(strcmp(bufferClient,"gabbopower") == 0){
                    goto ENDWHILE;
                }
                printf("%s",bufferClient);
                }
            }































    if(sendInput(choice, sockfd, sizeof(choice)) == -1){
        goto INSERT;
    }

    
ENDWHILE:
    printf("ENDWHILE\n");
    printf("\nWhat do you wanna do now?\n");
    goto INSERT;

    sleep(10);
    // close the socket
    close(sockfd);
}
