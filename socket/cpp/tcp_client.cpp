/*
 * references
 *  - https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
 *
 *
 */

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <csignal>
#include <exception>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <ctype.h>
#include <chrono>
#include <thread>
#include <getopt.h>

#include <queue>
#include <vector>

#include "logging.hpp"
#include "config.hpp"

static struct option long_options[] = {
    {"help",                no_argument,        nullptr,  'h' },
    {"server-hostname",     required_argument,  nullptr,  's' },
    {"port",                required_argument,  nullptr,  'p' },
    {nullptr,               0,                  nullptr,  0 },
};

void print_help(void) {
    LOG_INFO("usage: tcp_client [-h] [-H HOST] [-p PORT]\n\n");
    LOG_INFO("This is toy tcp client to test comm latency\n\n");
    LOG_INFO("options:\n");

    LOG_INFO("  -h,      --help                  show this help message and exit\n");
    LOG_INFO("  -s HOST, --server-hostname HOST  set server hostname\n");
    LOG_INFO("  -p PORT, --port            PORT  set server port number\n");
}

int func(int sockfd)
{
    int ret = EXIT_SUCCESS;
    std::priority_queue<double, std::vector<double>, std::greater<double>> min_heap;

    char buff[MAX_BUF] = {'H', 'E', 'L', 'L', 'O', '\n'};
    int n;
    uint64_t count = 0;
    auto b = std::chrono::high_resolution_clock::now();
    for (;;) {
        auto b_frame = std::chrono::high_resolution_clock::now();
        //bzero(buff, sizeof(buff));
        //LOG_INFO("Enter the string : ");
        n = 0;
//        while ((buff[n++] = getchar()) != '\n') {
//            LOG_INFO("<%c>\n", buff[n-1]);
//        }
        // buff[MAX-1] = count;
        count++;
        if (send(sockfd, buff, sizeof(buff), 0) < 0) {
            LOG_ERR("send failed: %s\n", strerror(errno));
            break;
        }

        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        bzero(buff, sizeof(buff));


        if (recv(sockfd, buff, sizeof(buff), 0) <= 0) {
            LOG_ERR("recv failed: %s\n", strerror(errno));
            LOG_INFO("Client exit...\n");
            break;
        }
        // LOG_INFO("From Server : %s(%d)", buff, buff[MAX-1]);
        if ((strncmp(buff, "HELLO", 5)) != 0) {
            LOG_ERR("ERROR: message is broken!\n");
            ret = EXIT_FAILURE;
            break;
        }

        auto e_frame = std::chrono::high_resolution_clock::now();
        double elapsed_frame = std::chrono::duration<double, std::milli>(e_frame - b_frame).count();

        if (min_heap.size() < 10) {
            min_heap.push(elapsed_frame);
        } else {
            if (elapsed_frame > min_heap.top()) {
                min_heap.pop();
                min_heap.push(elapsed_frame);
            }
        }

        // if (elapsed_frame > 30.0) LOG_INFO(" - %f milliseconds\n", elapsed_frame);

        auto e = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double, std::milli>(e - b).count();
        if (elapsed > PERF_MONITOR_INTERVAL) {
            float fps = 1000.0*count/elapsed;
            LOG_INFO("\t--------------------------------------------------------------------------------\n");
            LOG_INFO("\t    fps: %6f hz\telapsed= %9f milliseconds/ %ld\n", fps, elapsed, count);
            LOG_INFO("\t    average elapsed time per frame: %3.3f millisecs\n", elapsed/count);
            LOG_INFO("\t    [");
            while (!min_heap.empty()) {
                std::printf("%.3f, ", min_heap.top());
                min_heap.pop();
            }
            std::printf("]\n");
            LOG_INFO("\t--------------------------------------------------------------------------------\n");
            //reset
            b = std::chrono::high_resolution_clock::now();
            count = 0;
        }
    }
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

int main(int argc, char *argv[])
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // init signal handler
    std::signal(SIGINT, signal_handler);

    //
    // reference
    //  - https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
    //  - https://man7.org/linux/man-pages/man3/getopt.3.html
    //
    const char *server_hostname_default = "127.0.0.1";
    char *server_hostname = (char *)server_hostname_default;
    int port = PORT;
    int c;
    opterr = 0;

    int option_index = 0;
    while ((c = getopt_long (argc, argv, "hs:p:", long_options, &option_index)) != -1)
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
                return 0;
            case 's':
                server_hostname = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case '?':
                if (optopt == 'r')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option or no argument `-%c'.\n", optopt);
                else
                    fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                LOG_INFO("return=%c(%x)\n", c, c);
                abort ();
        }

    // LOG_INFO("optind=%d\n", optind);

    LOG_INFO("\tserver_hostname: %s\n", server_hostname);
    LOG_INFO("\tport           : %d\n", port);

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        LOG_INFO("socket creation failed...\n");
        std::exit(EXIT_FAILURE);
    }
    else
        LOG_INFO("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));


    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(server_hostname);
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
        LOG_INFO("connection with the server failed...\n");
        std::exit(EXIT_FAILURE);
    }
    else
        LOG_INFO("connected to the server..\n");

    //get host name
    char hostname[16];
    memset(hostname, 0x0, sizeof(hostname));
    gethostname(hostname, sizeof(hostname));
    LOG_INFO("Device Name (=hostname): %s\n", hostname);

    // reference
    //  - https://stackoverflow.com/questions/49335001/get-local-ip-address-in-c
    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    int err = getsockname(sockfd, (struct sockaddr*)&name, &namelen);

    char buffer[INET_ADDRSTRLEN];
    const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, INET_ADDRSTRLEN);
    if(p != nullptr) {
        LOG_INFO("Local IP address is: %s\n", buffer);
    } else {
        LOG_ERR("Error number: %s(%d)\n", strerror(errno), errno);
    }

    // reference
    //  * https://stackoverflow.com/questions/46484240/getpeername-from-listeningserver-socket
    struct sockaddr_in addr1;
    struct sockaddr_in addr2;

    int clnt_sock_err, clnt_peer_err;
    socklen_t serv_len = sizeof(addr1);
    clnt_peer_err = getpeername(sockfd, (struct sockaddr *)&addr1, &serv_len);
    clnt_sock_err = getsockname(sockfd, (struct sockaddr *)&addr2, &serv_len);

    LOG_INFO("Client socket's ip : %s\n", inet_ntoa(addr2.sin_addr));
    LOG_INFO("client socket's port %d\n", ntohs(addr2.sin_port));
    LOG_INFO("Client socket's peer ip : %s\n", inet_ntoa(addr1.sin_addr));
    LOG_INFO("Client socket's peer port : %d\n", ntohs(addr1.sin_port));

    // LOG_INFO("client socket error=%d peer_error=%d\n", clnt_sock_err, clnt_peer_err);

    // function for chat
    try {
        func(sockfd);
    } catch (InterruptException &e) {
        LOG_ERR("Terminated by Interrupt %s\n", e.what());
    }

    // close the socket
    close(sockfd);

    return EXIT_SUCCESS;
}

