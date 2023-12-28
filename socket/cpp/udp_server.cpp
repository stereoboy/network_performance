/*
 * references
 *  - https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
 *  - https://www.geeksforgeeks.org/udp-server-client-implementation-c/
 *
 */

#include <cstdio>
#include <netdb.h>
#include <netinet/in.h>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <csignal>
#include <exception>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cctype>
#include <chrono>
#include <thread>
#include <getopt.h>

#include "logging.hpp"
#include "config.hpp"

static struct option long_options[] = {
    {"help",                no_argument,        nullptr,  'h' },
    {"hostname",            required_argument,  nullptr,  's' },
    {"port",                required_argument,  nullptr,  'p' },
    {"buffer-size",         required_argument,  nullptr,  'b' },
    {"debug",               no_argument,        nullptr,  'd' },
    {nullptr,               0,                  nullptr,  0 },
};

void print_help(void) {
    LOG_INFO("usage: tcp_client [-h] [-H HOST] [-p PORT]\n\n");
    LOG_INFO("This is toy udp client to test comm latency\n\n");
    LOG_INFO("options:\n");

    LOG_INFO("  -h,      --help                  show this help message and exit\n");
    LOG_INFO("  -s HOST, --server-hostname HOST  set server hostname\n");
    LOG_INFO("  -p PORT, --port            PORT  set server port number\n");
    LOG_INFO("  -b SIZE, --buffer-size     SIZE  set message buffer-size\n");
    LOG_INFO("  -d,      --debug                 enable debug messages\n");
}

// Function designed for chat between client and server.
int func(int connfd, size_t buffer_size, bool debug)
{
    int ret = EXIT_SUCCESS;

    char *buff = (char*) std::malloc(buffer_size);

    struct sockaddr_in cliaddr;
    std::memset(&cliaddr, 0x0, sizeof(cliaddr));
    socklen_t len = sizeof(cliaddr);

    // int n;
    // infinite loop for chat
    bool printed = false;
    for (;;) {
        std::memset(buff, 0x0, buffer_size);

        // read the message from client and copy it in buffer
        ssize_t size = recvfrom(connfd, buff, buffer_size, UDP_RECV_FLAG, (struct sockaddr *) &cliaddr, &len);
        if (size <= 0) {
            LOG_ERR("recvfrom failed: %s(%d)\n", strerror(errno), errno);
            LOG_INFO("Server closed\n");
            break;
        } else if (size < (ssize_t) buffer_size) {
            LOG_WARN("data from recvfrom is broken: %s(%d)\n", strerror(errno), errno);
        }


        char *cliaddr_str = inet_ntoa(cliaddr.sin_addr);
        int cliaddr_port = ntohs(cliaddr.sin_port);
        // print buffer which contains the client contents
        if (debug) if(!printed) LOG_DEBUG("message from %s:%d\n", cliaddr_str, cliaddr_port);
        printed = true;
        // bzero(buff, MAX);
        // n = 0;
        // // copy server message in the buffer
        // while ((buff[n++] = getchar()) != '\n')
        //     ;

        if (std::strncmp(buff, MESSAGE_BEGIN_STRING, sizeof(MESSAGE_BEGIN_STRING) - 1) != 0) {
            LOG_ERR("message is broken!\n");
            ret = EXIT_FAILURE;
            break;
        }

        if (std::strncmp(buff + buffer_size - sizeof(MESSAGE_END_STRING) - 1, MESSAGE_END_STRING, sizeof(MESSAGE_END_STRING) - 1) != 0) {
            LOG_ERR("message is broken!\n");
            ret = EXIT_FAILURE;
            break;
        }

        //std::this_thread::sleep_for(std::chrono::milliseconds(15));
        // and send that buffer to client
        if (sendto(connfd, buff, buffer_size, UDP_SEND_FLAG, (const struct sockaddr *) &cliaddr, len) < 0) {
            LOG_ERR("sendto failed: %s(%d)\n", strerror(errno), errno);
            break;
        }

        // if msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0) {
            LOG_INFO("Server Exit...\n");
            break;
        }
    }

    std::free(buff);
    return ret;
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
int main(int argc, char *argv[])
{
    int sockfd = -1;
    // int connfd = -1;
    // socklen_t len;
    struct sockaddr_in servaddr;
    // struct sockaddr_in cli;

    // init signal handler
    std::signal(SIGINT, signal_handler);

    //
    // reference
    //  - https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
    //  - https://man7.org/linux/man-pages/man3/getopt.3.html
    //
    const char *server_hostname_default = "127.0.0.1";
    char *server_hostname = (char *)server_hostname_default;
    int port            = PORT;
    size_t buffer_size  = MAX_BUFFER_SIZE;
    bool debug = false;
    int c;
    opterr = 0;

    int option_index = 0;
    while ((c = getopt_long (argc, argv, "hs:p:b:d", long_options, &option_index)) != -1)
        switch (c)
        {
            case 0:
                LOG_INFO("option %s", long_options[option_index].name);
                if (optarg)
                       LOG_INFO(" with arg %s", optarg);
                LOG_INFO("\n");
                break;
            case 'h':
                print_help();
                return EXIT_SUCCESS;
            case 'p':
                port = atoi(optarg);
                break;
            case 'b':
                buffer_size = (size_t)atoi(optarg);
                break;
            case 'd':
                debug = true;
                break;
            case '?':
                if (optopt == 'r')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (std::isprint (optopt))
                    fprintf (stderr, "Unknown option or no argument `-%c'.\n", optopt);
                else
                    fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                return EXIT_FAILURE;
            default:
                LOG_INFO("return=%c(%x)\n", c, c);
                print_help();
                return EXIT_FAILURE;
        }

    LOG_INFO("\tserver_hostname: %s\n", server_hostname);
    LOG_INFO("\tport           : %d\n", port);
    LOG_INFO("\tbuffer-size    : %ld\n", buffer_size);

    if (buffer_size < 64) {
        LOG_INFO("buffer-size is too small\n");
        std::exit(EXIT_FAILURE);
    }

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        LOG_INFO("socket creation failed...\n");
        std::exit(EXIT_FAILURE);
    }
    else
        LOG_INFO("Socket successfully created..\n");

    // assign IP, PORT
    std::memset(&servaddr, 0x0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    // int one = 1;
    // // int zero = 0;
    // if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0) {
    //     LOG_ERR("setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) failed: %s\n", strerror(errno));
    //     close(sockfd);
    //     std::exit(EXIT_FAILURE);
    // }

    int retbuf = 0;
    socklen_t optlen;
    optlen = sizeof(retbuf);
    if (getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &retbuf, &optlen) < 0) {
        LOG_ERR("getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &retbuf, &optlen) failed: %s(%d)\n", strerror(errno), errno);
        close(sockfd);
        std::exit(EXIT_FAILURE);
    }
    LOG_INFO("SNDBUF send buffer size = %d\n", retbuf);

    optlen = sizeof(retbuf);
    if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &retbuf, &optlen) < 0) {
        LOG_ERR("getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &retbuf, &optlen) failed: %s(%d)\n", strerror(errno), errno);
        close(sockfd);
        std::exit(EXIT_FAILURE);
    }
    LOG_INFO("RCVBUF recv buffer size = %d\n", retbuf);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) {
        LOG_INFO("socket bind failed...\n");
        close(sockfd);
        std::exit(EXIT_FAILURE);
    }
    else
        LOG_INFO("Socket successfully binded..\n");

    // // Now server is ready to listen and verification
    // if ((listen(sockfd, 5)) != 0) {
    //     LOG_INFO("Listen failed...\n");
    //     close(sockfd);
    //     std::exit(EXIT_FAILURE);
    // }
    // else
    //     LOG_INFO("Server listening..\n");
    // len = sizeof(cli);

    int ret = EXIT_SUCCESS;
    try {
        while(true) {
            // // Accept the data packet from client and verification
            // connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
            // if (connfd < 0) {
            //     LOG_INFO("server accept failed...\n");
            //     close(sockfd);
            //     std::exit(EXIT_FAILURE);
            // }
            // else
            //     LOG_INFO("server accept the client...\n");

            // if (setsockopt(connfd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one)) < 0) {
            //     LOG_ERR("setsockopt(connfd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one)) failed: %s\n", strerror(errno));
            //     close(sockfd);
            //     close(connfd);
            //     std::exit(EXIT_FAILURE);
            // }

            // reference
            //   * https://stackoverflow.com/questions/3060950/how-to-get-ip-address-from-sock-structure-in-c
            //   * http://man7.org/linux/man-pages/man3/inet_ntop.3.html
            // struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&cli;
            // struct in_addr ipAddr = pV4Addr->sin_addr;

            // char str[INET_ADDRSTRLEN];
            // const char *p = inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN );
            // if (p != nullptr) {
            //     LOG_INFO("  accepted from (addr=%s, port=%d)\n", str, ntohs(pV4Addr->sin_port)) ;
            // } else {
            //     LOG_ERR("Error number: %s(%d)\n", strerror(errno), errno);
            // }

            LOG_INFO("############################################################################################\n");
            // reference
            //  * https://www.joinc.co.kr/w/man/2/getpeername
            // struct sockaddr_in myaddr;
            // len = sizeof(myaddr);
            // getpeername(connfd, (struct sockaddr *)&myaddr, &len);
            // LOG_INFO("address : %s\n", inet_ntoa(myaddr.sin_addr));
            // LOG_INFO("Port    : %d\n", ntohs(myaddr.sin_port));

            // reference
            //  * https://stackoverflow.com/questions/46484240/getpeername-from-listeningserver-socket
            struct sockaddr_in serv_addr;
            // struct sockaddr_in clnt_addr;
            // socklen_t clnt_addr_size =sizeof(clnt_addr);

            // struct sockaddr_in addr1;
            // struct sockaddr_in addr2;

            struct sockaddr_in addr3;
            // struct sockaddr_in addr4;

            socklen_t serv_len = sizeof(serv_addr);
            // if (getpeername(sockfd, (struct sockaddr *)&addr1, &serv_len) < 0) {
            //     LOG_ERR("getpeername(): failed, %s(%d)\n", strerror(errno), errno);
            //     std::exit(EXIT_FAILURE);
            // }
            if (getsockname(sockfd, (struct sockaddr *)&addr3, &serv_len) < 0) {
                LOG_ERR("getsockname(): failed, %s(%d)\n", strerror(errno), errno);
                std::exit(EXIT_FAILURE);
            }

            // LOG_INFO("Server socket's peer ip : %s\n", inet_ntoa(addr1.sin_addr));
            // LOG_INFO("Server socket's peer port : %d\n", ntohs(addr1.sin_port));
            LOG_INFO("Server socket's ip : %s\n", inet_ntoa(addr3.sin_addr));
            LOG_INFO("Server socket's port : %d\n", ntohs(addr3.sin_port));
            LOG_INFO("\n");
            LOG_INFO("\n");

            // if (getpeername(connfd, (struct sockaddr *)&addr2, &clnt_addr_size) < 0) {
            //     LOG_ERR("getpeername(): failed, %s(%d)\n", strerror(errno), errno);
            //     std::exit(EXIT_FAILURE);
            // }
            // if (getsockname(connfd, (struct sockaddr *)&addr4, &clnt_addr_size) < 0) {
            //     LOG_ERR("getsockname(): failed, %s(%d)\n", strerror(errno), errno);
            //     std::exit(EXIT_FAILURE);
            // }

            // LOG_INFO("   Client socket's peer ip : %s\n", inet_ntoa(addr2.sin_addr));
            // LOG_INFO("   client socket's peer port %d\n", ntohs(addr2.sin_port));
            // LOG_INFO("   Client socket's ip : %s\n", inet_ntoa(addr4.sin_addr));
            // LOG_INFO("   Client socket's port : %d\n", ntohs(addr4.sin_port));

            // Function for chatting between client and server
            {
                ret = func(sockfd, buffer_size, debug);
            }

            // close sockets
            // if (connfd > 0) close(connfd);
            // connfd = -1;
        }
    } catch (InterruptException &e) {
        LOG_ERR("Terminated by Interrupt %s\n", e.what());
    }

    // close sockets
    // if (connfd > 0) close(connfd);
    if (sockfd > 0) close(sockfd);

    return ret;
}
