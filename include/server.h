#include "stdbool.h"
#include "inizio.h"

#define MAX 80
#define PORT 5000
#define SA struct sockaddr

extern int sockfd;
extern int connfd;
extern int len;
extern struct sockaddr_in servaddr;
extern struct sockaddr_in cli;