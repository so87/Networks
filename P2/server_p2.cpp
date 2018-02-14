// Simon Owens
// Networks Project 2 Server

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using namespace std;

struct weather_now {
  string city, reg, cou,
    chl, dir, spd,
    hum, vis, pre, ris,
    sunrise, sunset,
    text, code, temp, date;
};

struct weather_fore {
  string day, date, text, low, high;
};

string get_data(const string line, int &i);


int port = 61500;

int main(int argc, char *argv[])
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


  string user_input;
  do{
  char msg[10];
  char json_file[1000000];
    
  // make a new socket for connecting to the api
  int weather_sock;
  struct addrinfo hints, *res;
  // get all of your address info
  memset(&hints, 0, sizeof hints);  // Set information for hints
  hints.ai_family = AF_INET;        // IPV4
  hints.ai_socktype = SOCK_STREAM;  // Any type of socket type
  struct hostent* ipaddress = gethostbyname("api.wunderground.com");
  // basically same as gethostbyname, keeps things simpler
  getaddrinfo(ipaddress->h_name, argv[1], &hints, &res);
    
  // make a socket with your info
  weather_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    
  //connect to weather server
  int con = connect(weather_sock, res->ai_addr, res->ai_addrlen);
  if(con < 0)
    cout << "Did not connect to weather underground" << endl;

  // receive from our client
  recv(clientfd, &msg, 1, 0);
  user_input = string(msg);
  bzero(msg, 1);
  
  // accept the numbers 1-3 from the client app for different places
  string place;
  if(user_input == "1"){
    place = "IN/indianapolis";
  }
  else if(user_input == "2"){
    place = "IN/evansville";
  }
  else if(user_input == "3"){
    place = "KY/lexington";
  }
  string weather_domain = "host: api.wunderground.com\n\n";
  string key = "0dad506abcda17f7";
  string get_request = "GET http://api.wunderground.com/api/"+key+"/forecast/conditions/q/" + place  + ".json HTTP/1.1\n";
  

  // make get and http request from api to receive data file
  send(weather_sock, get_request.c_str(), strlen(get_request.c_str()), 0);
  send(weather_sock, weather_domain.c_str(), strlen(weather_domain.c_str()), 0);
  int rec = recv(weather_sock, &json_file, 1000000, 0);
  
  // give file back to the client app
  send(clientfd, json_file, 1000000, 0);
  bzero(json_file, 1000000);
  close(weather_sock);
  }while(user_input !="0");
  close(SocketD);
}
