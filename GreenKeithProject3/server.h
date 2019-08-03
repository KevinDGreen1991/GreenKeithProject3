#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <time.h>
#include<pthread.h> 

#define PORT 9001
#define BACKLOG 10
#define MAX_LEN 1024
#define MAX_FILE_LEN 10240
#define MAX_CLIENTS 5