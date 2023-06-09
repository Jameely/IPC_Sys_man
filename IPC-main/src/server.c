#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#define SSA  (struct sockaddr *)
#define PORT 5777
#define BUFFER_SIZE 1024
#define LARGE_BUFFER_SIZE  104857600
#define MAX_MESSAGE_LENGTH 1024
#define UDS_PATH "/tmp/mysocket"


void chat_server_TCP_IPV4(int port)
{
    int server_fd , client_fd;
    struct sockaddr_in server_addr , client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char msg[MAX_MESSAGE_LENGTH];
    struct pollfd fds[2];

    // create socket file descriptor 
    server_fd = socket(AF_INET,SOCK_STREAM,0);
    if(server_fd == 0){
        perror("Server -> Falied socket ");
        exit(EXIT_FAILURE);
    }

    //set socket options
    int opt = 1;
    if(setsockopt(server_fd, SOL_SOCKET , SO_REUSEADDR , &opt , sizeof(opt))){
        perror("Server -> Falied setsocketopt ");
        exit(EXIT_FAILURE);
    }

    memset (&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // bind socket to port 
    if(bind(server_fd,SSA &server_addr,sizeof(server_addr))<0){
        perror("Server -> Falied bind ");
        exit(EXIT_FAILURE);
    }

    //listen for incoming connections(1) 
    if(listen(server_fd,1)<0){
        perror("Server -> Falied listen ");
        exit(EXIT_FAILURE);
    }

    // accept incoming connection
    if((client_fd=accept(server_fd,SSA&client_addr,&client_addr_len))<0){
        perror("Accept -> Falied listen ");
        exit(EXIT_FAILURE);
    }

    // use poll to handle incoming messages from client and keyboard input
    fds[0].fd = client_fd;
    fds[0].events = POLLIN;
    fds[1].fd = STDIN_FILENO;
    fds[1].events = POLLIN;

    // send and receive messages
    while(1){
        if(poll(fds,2,-1) < 0){
            perror("Server -> Poll error");
            exit(EXIT_FAILURE);
        }

        if(fds[0].revents & POLLIN){
            //recv message from client
            if(recv(client_fd,msg ,MAX_MESSAGE_LENGTH,0)<0){
                perror("Server ->  recv Falied ");
                exit(EXIT_FAILURE);
            }
            printf("Client: %s",msg);
            strncpy(msg,"",1);
        }

        if(fds[1].revents & POLLIN){
            // read message from keyboard
            fgets(msg,MAX_MESSAGE_LENGTH,stdin);

            //send message to client
            if(send(client_fd,msg,strlen(msg),0)<0){
                perror("Server ->  send Falied ");
                exit(EXIT_FAILURE);
            }
        }
    }
   
}


//part B
void start_server_TCP_IPv4() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char *hello = "Hello from server";

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    // Send 100MB (104857600 bytes) of data to client
    int total_sent = 0;
    while (total_sent < LARGE_BUFFER_SIZE) {
        int bytes_sent = send(new_socket, hello, strlen(hello), 0);
        if (bytes_sent < 0) {
            perror("send failed");
            exit(EXIT_FAILURE);
        }
        total_sent += bytes_sent;
    }

    printf("Sent %d bytes to client\n", total_sent);
    close(new_socket);
    close(server_fd);
}


void start_server_TCP_IPv6() {
    int server_fd, new_socket, valread;
    struct sockaddr_in6 address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char *hello = "Hello from server";

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET6, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    address.sin6_family = AF_INET6;
    address.sin6_addr = in6addr_any;
    address.sin6_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    // Send 100MB (104857600 bytes) of data to client
    int total_sent = 0;
    while (total_sent < LARGE_BUFFER_SIZE) {
        int bytes_sent = send(new_socket, hello, strlen(hello), 0);
        if (bytes_sent < 0) {
            perror("send failed");
            exit(EXIT_FAILURE);
        }
        total_sent += bytes_sent;
    }

    printf("Sent %d bytes to client\n", total_sent);
    close(new_socket);
    close(server_fd);
}
void error(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
void start_server_UDP_IPv4(int port) {
    struct sockaddr_in server_addr, client_addr;
    int sockfd, recv_len, total_recv = 0;
    socklen_t client_len;
    char buf[BUFFER_SIZE];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        error("Error creating socket");
    }

    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        error("Error binding socket");
    }

    printf("Server started on port %d...\n", port);

    while (total_recv < 100000000) { // receive 100MB of data
        memset(buf, 0, BUFFER_SIZE);
        client_len = sizeof(client_addr);

        if ((recv_len = recvfrom(sockfd, buf, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len)) == -1) {
            error("Error receiving data");
        }

        if (sendto(sockfd, buf, recv_len, 0, (struct sockaddr *)&client_addr, client_len) == -1) {
            error("Error sending data");
        }

        total_recv += recv_len;
        printf("Received %d bytes of data\n", total_recv);

    }


    close(sockfd);
}


void start_server_UDP_IPv6(int port) {
    struct sockaddr_in6 server_addr, client_addr;
    int sockfd, recv_len, total_recv = 0;
    socklen_t client_len;
    char buf[BUFFER_SIZE];

    if ((sockfd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        error("Error creating socket");
    }

    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_addr = in6addr_any;
    server_addr.sin6_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        error("Error binding socket");
    }

    printf("Server started on port %d...\n", port);

    while (total_recv < LARGE_BUFFER_SIZE) { // receive 100MB of data
        memset(buf, 0, BUFFER_SIZE);
        client_len = sizeof(client_addr);

        if ((recv_len = recvfrom(sockfd, buf, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len)) == -1) {
            error("Error receiving data");
        }

        char client_addr_str[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &(client_addr.sin6_addr), client_addr_str, INET6_ADDRSTRLEN);

        if (sendto(sockfd, buf, recv_len, 0, (struct sockaddr *)&client_addr, client_len) == -1) {
            error("Error sending data");
        }

        total_recv += recv_len;
        printf("Received %d bytes of data\n", total_recv);

    }

    close(sockfd);
}

//must check the function..
void start_server_UDS_dgram() {
    int server_fd, valread;
    struct sockaddr_un address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char *hello = "Hello from server";

    // Create socket file descriptor
    if ((server_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to UDS path
    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    #define UDS_PATH "/tmp/my_unix_socket" //must define the relevant path
    strncpy(address.sun_path, UDS_PATH, sizeof(address.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Receive message from client
    char buffer[BUFFER_SIZE] = {0};
    int bytes_received = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&address, &addrlen);
    if (bytes_received < 0) {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    }

    printf("Received message from client: %s\n", buffer);

    // Send response to client
    int total_sent = 0;
    while (total_sent < LARGE_BUFFER_SIZE) {
        int bytes_sent = sendto(server_fd, hello, strlen(hello), 0, (struct sockaddr *)&address, addrlen);
        if (bytes_sent < 0) {
            perror("sendto failed");
            exit(EXIT_FAILURE);
        }
        total_sent += bytes_sent;
    }

    printf("Sent %d bytes to client\n", total_sent);
    close(server_fd);
    unlink(UDS_PATH);
}

void start_server_UDS_stream() {
    int server_fd, new_socket, valread;
    struct sockaddr_un address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char *hello = "Hello from server";

    // Create socket file descriptor
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to UDS path
    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, UDS_PATH, sizeof(address.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    // Receive message from client
    char buffer[BUFFER_SIZE] = {0};
    int bytes_received = recv(new_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        perror("recv failed");
        exit(EXIT_FAILURE);
    }

    printf("Received message from client: %s\n", buffer);

    // Send response to client
    int total_sent = 0;
    while (total_sent < LARGE_BUFFER_SIZE) {
        int bytes_sent = send(new_socket, hello, strlen(hello), 0);
        if (bytes_sent < 0) {
            perror("send failed");
            exit(EXIT_FAILURE);
        }
        total_sent += bytes_sent;
    }

    printf("Sent %d bytes to client\n", total_sent);
    close(new_socket);
    close(server_fd);
    unlink(UDS_PATH);
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FILENAME "example.txt" //change the file name.
#define FILESIZE 4096

void start_server_mmap() {
    int fd;
    char *mapped;

    // Create file and memory map
    if ((fd = open(FILENAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, FILESIZE) == -1) {
        perror("ftruncate failed");
        exit(EXIT_FAILURE);
    }

    if ((mapped = mmap(NULL, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    // Read from memory map
    printf("Waiting for client to write to memory map...\n");
    while (mapped[0] == '\0') {
        usleep(1000);
    }

    printf("Received message from client: %s\n", mapped);

    // Write to memory map
    char *response = "Hello from server";
    strncpy(mapped, response, strlen(response));

    printf("Sent message to client: %s\n", mapped);

    // Clean up
    if (munmap(mapped, FILESIZE) == -1) {
        perror("munmap failed");
        exit(EXIT_FAILURE);
    }

    close(fd);
    unlink(FILENAME);
}

void start_server_pipe() {
    int fd;
    char buffer[BUFFER_SIZE];

    // Create named pipe
    if (mkfifo(FILENAME, 0666) == -1) {
        perror("mkfifo failed");
        exit(EXIT_FAILURE);
    }

    // Open pipe for reading
    if ((fd = open(FILENAME, O_RDONLY)) == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    // Read from pipe
    int bytes_received = read(fd, buffer, BUFFER_SIZE);
    if (bytes_received == -1) {
        perror("read failed");
        exit(EXIT_FAILURE);
    }

    printf("Received message from client: %s\n", buffer);

    // Write to pipe
    char *response = "Hello from server";
    int bytes_sent = write(fd, response, strlen(response));
    if (bytes_sent == -1) {
        perror("write failed");
        exit(EXIT_FAILURE);
    }

    printf("Sent message to client: %s\n");

    // Clean up
    close(fd);
    unlink(FILENAME);
}


int main(){
    //start_server_TCP_IPv4();
    //start_server_TCP_IPv6();
    //start_server_UDP_IPv4(7777);
    //start_server_UDP_IPv6(6666);

    return 0;
}