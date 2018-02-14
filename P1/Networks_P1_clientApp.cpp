// Simon Owens
// Networks Project 1 Client

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using namespace std;


int main(int argc, char *argv[]){ //receive a port number
  //enter the correct amount of args
  if(argc != 2){
    cout << "Please enter in two arguements only" << endl;
    return 0;
  }
  
  // information for building socket/connecting
  struct addrinfo hints, *res;
  int sockfd;

  // information for sending and receiving messages
  char msg[100];
  int len;
  string get_http = "GET / HTTP/1.1\r\nHost: " + url + "\r\nConnection: close\r\n\r\n";

  // get all of your address info
  memset(&hints, 0, sizeof hints);  // Set information for hints
  hints.ai_family = AF_INET;        // IPV4
  hints.ai_socktype = SOCK_STREAM;  // Any type of socket type

  struct hostent* ipaddress = gethostbyname(localhost);
  
  // basically same as gethostbyname, keeps things simpler
  getaddrinfo(ipaddress->h_name, argv[1] , &hints, &res);

  // make a socket with your info
  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  // connect to the server
  connect(sockfd, res->ai_addr, res->ai_addrlen);
  
  cout << "Type in something to be echo'ed to the server" << endl;

  while(1){
    // clear the message and get it from user
    bzero(msg, 100);
    fgets(msg, 100, stdin);
    len = strlen(msg);
    
    // send the message to the server then receive it back
    send(sockfd, msg, len, 0);
    recv(sockfd, msg, len, 0);
    string arr = string(msg);

    // print the received message back out to the screen
    cout << "Server echoed: " << arr << endl;
    bzero(msg, 100);
  }

  // close the socket when you are done communicating with the server
  close(sockfd);
  return 0;
}
