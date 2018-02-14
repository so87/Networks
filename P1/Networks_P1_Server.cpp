// Simon Owens
// Networks Project 1 Server

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>


using namespace std;

int port = 61500;

int main()
{
  
  int SocketD;
  SocketD = socket(AF_INET, SOCK_STREAM, 0);
  cout << "My Socket Discriptor is: " << SocketD << endl;
  cout << "Starting" << endl;
  
  struct sockaddr_in self;
  self.sin_family = AF_INET;
  self.sin_port = htons(port);            // assign port number use network byte order
  self.sin_addr.s_addr = INADDR_ANY;      // this is the ip address
  
  bind(SocketD, (struct sockaddr*)&self, sizeof(self));
  cout << "Server is Listening on port: " << port << endl;
  listen(SocketD, 20);
  struct sockaddr_in client_addr;
  int addrlen = sizeof(client_addr);
  int clientfd = accept(SocketD, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen);
  cout << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << "\n connected \n";
  
  while (1) {
    bool endofline = false;
    int received = 0;
    int iter = 0;
    char rebuffer[80];
    char buffer[1024];
    
    while (!endofline) {
      received = recv(clientfd, &rebuffer, 1024, 0);
      for (int i = 0; i < received; i++, iter++) {
	if (rebuffer[i] == '\n')
	  endofline = true;
	buffer[iter] = rebuffer[i];
	if (endofline) {
	  buffer[iter + 1] = 0;                   // null terminate string
	  send(clientfd, buffer, received, 0);
	  string arr = string(buffer);
	  cout << arr << endl;
	}
      }
    }
  }
  close(SocketD);
}
