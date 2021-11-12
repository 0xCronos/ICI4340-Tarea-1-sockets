#include <stdio.h> // perror, printf
#include <stdlib.h> // exit, atoi
#include <unistd.h> // write, read, close
#include <arpa/inet.h> // sockaddr_in, AF_INET, SOCK_STREAM,
                       // INADDR_ANY, socket etc...
#include <string.h> // strlen, memset

const char message[] = "Hello sockets world\n";

int main(int argc, char const *argv[]) {

  int serverFd;
  struct sockaddr_in6 server;
  socklen_t len;
  int port = 1234;
  char const *server_ip = "fe80::dea6:32ff:fe52:2005";
  char *buffer = "hello server";
  if (argc == 3) {
    server_ip = argv[1];
    port = atoi(argv[2]);
  }
  serverFd = socket(AF_INET6, SOCK_DGRAM, 0);
  if (serverFd < 0) {
    perror("Cannot create socket");
    exit(1);
  }
  server.sin6_family = AF_INET6;
  inet_pton(AF_INET6, server_ip, &server.sin6_addr);
  server.sin6_port = htons(port);
  len = sizeof(server);

  if (sendto(serverFd, buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &server, len) < 0) {
    perror("Cannot write");
    exit(3);
  }
  char recv[1024];
  memset(recv, 0, sizeof(recv));
  if (recvfrom(serverFd, recv, sizeof(recv), 0, (struct sockaddr *) &server, &len) < 0) {
    perror("cannot read");
    exit(4);
  }
  printf("Received %s from server\n", recv);
  close(serverFd);
  return 0;
}
