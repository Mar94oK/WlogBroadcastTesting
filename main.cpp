#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <string>

#define MAXRECVSTRING 255  /* Longest string to receive */


void LogAsWirelessLoggerDaemon(const std::string &text)
{
    std::string result = "===WirelessLoggerDaemon=== :: " + text + "\n";
    fprintf(stderr, "%s", result.c_str());
}


int main()
{
    LogAsWirelessLoggerDaemon("Hello, World!");

    int sock;                         /* Socket */
    struct sockaddr_in broadcastAddr; /* Broadcast Address */
    unsigned short broadcastPort;     /* Port */
    char recvString[MAXRECVSTRING+1]; /* Buffer for received string */
    int recvStringLen;                /* Length of received string */


    broadcastPort = 9009;   /* First arg: broadcast port */

    /* Create a best-effort datagram socket using UDP */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        LogAsWirelessLoggerDaemon("socket() failed");

    /* Construct bind structure */
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));   /* Zero out structure */
    broadcastAddr.sin_family = AF_INET;                 /* Internet address family */
    broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);  /* Any incoming interface */
    broadcastAddr.sin_port = htons(broadcastPort);      /* Broadcast port */

    /* Bind to the broadcast port */
    if (bind(sock, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) < 0)
        LogAsWirelessLoggerDaemon("bind() failed");

    for (;;)
    {
        /* Receive a single datagram from the server */
        if ((recvStringLen = recvfrom(sock, recvString, MAXRECVSTRING, 0, NULL, 0)) < 0)
            LogAsWirelessLoggerDaemon("recvfrom() failed");

        recvString[recvStringLen] = '\0';
        printf("Received: %s\n", recvString);    /* Print the received string */
    }

    close(sock);
    exit(0);
}