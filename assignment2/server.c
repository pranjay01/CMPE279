// Server side C/C++ program to demonstrate Socket programming

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 80
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[102] = {0};
    char *hello = "Hello from server";
    
    printf("execve=0x%p\n", execve);
    // printf(" No. of arguments %s \n",argv[1]);
    // if (argv[1] =="CHILD")
    char *envvariable = getenv("isChild");
    printf(envvariable);
    if (envvariable && strcmp("true",envvariable)==0 )
    {
        printf("Re execution \n");
        goto readv;
    }
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attaching socket to port 80
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 80
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }


    // Forking a child process
    pid_t processId = fork();

    // if (processId > 0)
    // {
    //     // printf("parent entry");
    //     int returnStatus=0;
    //     // waitpid(processId, &returnStatus, 0);
    //     while ((wait(&returnStatus) > 0));
    //     printf("parent exit");
    // }

    if(processId == 0){
        printf("Child block\n");
        setuid(65534);

        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                        (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        char socket[20];
        sprintf(socket, "%d", new_socket);
        char *args[3] = {socket, "CHILD", NULL};
        char *env[] = {"isChild=true",NULL};
        // setenv("isChild", "true", 1);
        // setenv("socket", socket, 1);
        execve("./server", args,env);
        // execv("./server", args);
        printf("not re executed \n");
        // exit(0);
    }
    else if (processId > 0)
    // else
    {
        // printf("parent entry");
        int status;
        waitpid(processId, &status, 0);
        // while ((wait(&status) > 0));
        printf("parent exit \n");
        exit(0);
    }
    else
    {
        perror("Fork failed \n");
    }

readv:
    new_socket=atoi(argv[0]);
    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    return 0;
}
