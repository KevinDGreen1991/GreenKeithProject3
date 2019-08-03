#include "server.h"

pthread_mutex_t socket_lock; 
int counter;
pthread_t tid[MAX_CLIENTS]; 

void *trythis(void *arg)
{
    int valread;
    char buffer[1024];
    pthread_mutex_lock(&socket_lock);
    //int *this_socket = malloc(sizeof((int*)arg);
    //this
    //int *this_socket = (int *) arg;
    int this_socket = *(int*)arg;
    counter++;
    int this_counter = counter;
    //printf("Thread: %d\n", this_socket);
    pthread_mutex_unlock(&socket_lock);
    valread = read(this_socket , buffer, 1024); 
    printf("%s\n",buffer ); 
    printf("Sending %d\n", this_counter);
    //sleep(5);
    snprintf(buffer, sizeof(buffer), "Your number is %d", this_counter);
    send(this_socket , buffer , strlen(buffer) , 0 ); 
    printf("Hello message sent %d %d\n", this_counter, this_socket);
    
    
    //exit(0); 

}
int main(void) 
{
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 
    counter = 0;
    int threadNumber = 0;
    //create lock for handling socket id's
    if (pthread_mutex_init(&socket_lock, NULL) != 0) 
    { 
        printf("\n mutex init has failed\n"); 
        return 1; 
    } 

    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    for(;;)
    {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket == -1)
        {
            fprintf(stderr, "accept() error: %s\n", strerror(errno));
            continue;
        }
        if(threadNumber >= MAX_CLIENTS)
            {
                printf("Maximum threads reached\n");
                char *error_msg = "Unable To Join: Maximum clients reached";
                send(new_socket, error_msg, strlen(error_msg), 0);
                continue;
            }
        //printf("Main: %d\n", new_socket);
        int error = pthread_create(&tid[threadNumber], NULL, &trythis, (void *)&new_socket);
        
        if (error != 0) 
            printf("\nThread can't be created :[%s]", strerror(error));
        else
            threadNumber++;
        /*switch (fork())
        {
        case -1: // error
            fprintf(stderr, "fork() can't create child: %s\n", strerror(errno));
            close(new_socket);
            break;
        case 0: // child
        {

            // child doesn't need listening socket
            close(server_fd);
            pthread_mutex_lock(&socket_lock);
            int this_socket = new_socket;
            counter = counter + 1;
            pthread_mutex_unlock(&socket_lock);
            valread = read( this_socket , buffer, 1024); 
            printf("%s\n",buffer ); 
            printf("Sending %d\n", counter);
            sleep(5);
            snprintf(buffer, sizeof(buffer), "Your number is %d", counter);
            send(this_socket , buffer , strlen(buffer) , 0 ); 
            printf("Hello message sent\n"); 
            counter++;
            exit(0);
            break;
        }
        default: // parent
            // parent doesn't need client socket
            close(new_socket);
            break;
        }*/
    }
    /*f ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }*/

    return 0; 
} 