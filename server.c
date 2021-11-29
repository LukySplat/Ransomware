#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){

 	char *server_ip = "127.0.0.1";
	int server_port = 8080;

	int server_sock, client_sock;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_size;
	char buffer[1024];
	int n;

	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock < 0){
	printf("[-]Socket error");
	exit(1);
	}
	printf("[+]TCP server socket created.\n");

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = server_port;
	server_addr.sin_addr.s_addr = inet_addr(server_ip);

	n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (n < 0){
		printf("Error during binding \n");
	exit(1);
	}
	printf("Server listening on %s : %d \n",server_ip,server_port);

	if ((listen(server_sock, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");

	addr_size = sizeof(client_addr);
	client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
	printf("[+]Client connected.\n");

	bzero(buffer, 1024);
	recv(client_sock, buffer, sizeof(buffer), 0);
	printf("Client: %s\n", buffer);

	bzero(buffer, 1024);
	strcpy(buffer, "HI, THIS IS SERVER. HAVE A NICE DAY!!!");
	printf("Server: %s\n", buffer);
	send(client_sock, buffer, strlen(buffer), 0);

	close(n);	
	close(server_sock);
	printf("[+]Client disconnected.\n\n");
	return 0;

}
