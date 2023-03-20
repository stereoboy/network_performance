/*
 * references
 *  - https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
 *
 *
 */

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <csignal>
#include <exception>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <chrono>
#include <thread>
#define MAX 1024
#define PORT 8080

// Function designed for chat between client and server.
void func(int connfd)
{
    char buff[MAX];
    int n;
    // infinite loop for chat
    for (;;) {
        bzero(buff, MAX);

        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        // print buffer which contains the client contents
        printf("From client: %s\t To client : ", buff);
//        bzero(buff, MAX);
//        n = 0;
//        // copy server message in the buffer
//        while ((buff[n++] = getchar()) != '\n')
//            ;

        std::this_thread::sleep_for(std::chrono::milliseconds(15));
        // and send that buffer to client
        write(connfd, buff, sizeof(buff));

        // if msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

class InterruptException : public std::exception
{
public:
    InterruptException(int s) : S(s) {}
    int S;
};

void signal_handler(int s)
{
    throw InterruptException(s);
}

// Driver function
int main()
{
    int sockfd = -1, connfd = -1;
    socklen_t len;
    struct sockaddr_in servaddr, cli;

    // init signal handler
    std::signal(SIGINT, signal_handler);

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
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) {
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
    connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");

    // reference
    //   * https://stackoverflow.com/questions/3060950/how-to-get-ip-address-from-sock-structure-in-c
    //   * http://man7.org/linux/man-pages/man3/inet_ntop.3.html
    struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&cli;
    struct in_addr ipAddr = pV4Addr->sin_addr;

    char str[INET_ADDRSTRLEN];
    const char *p = inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN );
    if (p != nullptr) {
        printf("  accepted from (addr=%s, port=%d)\n", str, ntohs(pV4Addr->sin_port)) ;
    } else {
        fprintf(stderr, "Error number: %s(%d)\n", strerror(errno), errno);
    }

    // reference
    //  * https://www.joinc.co.kr/w/man/2/getpeername
    struct sockaddr_in myaddr;
    len = sizeof(myaddr);
    getpeername(connfd, (struct sockaddr *)&myaddr, &len);
    printf("address : %s\n", inet_ntoa(myaddr.sin_addr));
    printf("Port    : %d\n", ntohs(myaddr.sin_port));

    // reference
    //  * https://stackoverflow.com/questions/46484240/getpeername-from-listeningserver-socket
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = clnt_addr_size=sizeof(clnt_addr);

    struct sockaddr_in addr1;
    struct sockaddr_in addr2;

    struct sockaddr_in addr3;
    struct sockaddr_in addr4;

    socklen_t serv_len = sizeof(serv_addr);
    int serv_peer_err = getpeername(sockfd, (struct sockaddr *)&addr1, &serv_len);
    int serv_sock_err = getsockname(sockfd, (struct sockaddr *)&addr3, &serv_len);

    printf("Server socket's peer ip : %s\n", inet_ntoa(addr1.sin_addr));
    printf("Server socket's peer port : %d\n", ntohs(addr1.sin_port));
    printf("Server socket's ip : %s\n", inet_ntoa(addr3.sin_addr));
    printf("Server socket's port : %d\n", ntohs(addr3.sin_port));
    printf("\n\n\n\n\n");

    int clnt_peer_err = getpeername(connfd, (struct sockaddr *)&addr2, &clnt_addr_size);
    int clnt_sock_err = getsockname(connfd, (struct sockaddr *)&addr4, &clnt_addr_size);

    printf("Client socket's peer ip : %s\n", inet_ntoa(addr2.sin_addr));
    printf("client socket's peer port %d\n", ntohs(addr2.sin_port));
    printf("Client socket's ip : %s\n", inet_ntoa(addr4.sin_addr));
    printf("Client socket's port : %d\n", ntohs(addr4.sin_port));

    // Function for chatting between client and server
    try {
        func(connfd);
    } catch (InterruptException &e) {
        fprintf(stderr, "Terminated by Interrupt %s\n", e.what());
    }

    // After chatting close the socket
    if (connfd > 0) close(connfd);
    if (sockfd > 0) close(sockfd);
}
