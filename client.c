#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){

  char *server_ip = "127.0.0.1";
  int server_port = 8080;

  int sock;
  struct sockaddr_in client_addr;
  socklen_t addr_size;
  char buffer[1024];
  int n;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    printf("[-]Socket error");
    exit(1);
  }
  printf("[+]TCP server socket created.\n");

  // assign IP, PORT
  memset(&client_addr, '\0', sizeof(client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_port = server_port;
  client_addr.sin_addr.s_addr = inet_addr(server_ip);

  connect(sock, (struct sockaddr*)&client_addr, sizeof(client_addr));
  printf("Connected to the server.\n");

  bzero(buffer, 1024);
  strcpy(buffer, "HELLO, THIS IS CLIENT.");
  printf("Client: %s\n", buffer);
  send(sock, buffer, strlen(buffer), 0);

  bzero(buffer, 1024);
  recv(sock, buffer, sizeof(buffer), 0);
  printf("Server: %s\n", buffer);

  close(sock);
  printf("Disconnected from the server.\n");

  return 0;

}