// Simon Owens
// Networks Project 2 Client

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
#include "json.hpp"

using namespace std;


int main(int argc, char *argv[]){ //receive a port number
  //enter the correct amount of args
  if(argc != 2){
    cout << "Please enter in two arguements only" << endl;
    return 0;
  }
  // information for sending and receiving messages
  char msg[1000000];
  char json_msg[1000000];
  
  // information for building socket/connecting
  struct addrinfo hints, *res;
  int sockfd;

  // get all of your address info
  memset(&hints, 0, sizeof hints);  // Set information for hints
  hints.ai_family = AF_INET;        // IPV4
  hints.ai_socktype = SOCK_STREAM;  // Any type of socket type

  struct hostent* ipaddress = gethostbyname("localhost");
  
  // basically same as gethostbyname, keeps things simpler
  getaddrinfo(ipaddress->h_name, argv[1], &hints, &res);

  // make a socket with your info
  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  // connect to the server
  connect(sockfd, res->ai_addr, res->ai_addrlen);

  string user_choice = "-1";
  do{
    
  // prompt user for information 
  cout << endl << "Select one of the three places below for their weather data! (0 to quit)" << endl;
  cout << "1 Indianapolis" << endl;
  cout << "2 Evansville" << endl;
  cout << "3 Lexington" << endl << endl;
  
  // must be correct input
  bzero(msg, 1);
  do{
    cin >> user_choice;
    if(user_choice == "0")
      break;
  }while((user_choice > "3") || (user_choice < "0"));
  
  // send the data(1-3) to the server
  msg[0] = user_choice[0];
  send(sockfd, msg, 1, 0);
  
  // receive the data from the server
  int rec = recv(sockfd, &json_msg, 1000000, 0);
  if(rec < 0)
    cout << "Failed to receive data from server" << endl;

  string json_file_string = string(json_msg);
  json_file_string = json_file_string.substr(json_file_string.find("{"), json_file_string.size());

  // parse the received json file
  nlohmann::json js = nlohmann::json::parse(json_file_string);
  cout << "Right now it feels like: " << js["current_observation"]["feelslike_f"] << endl << " degrees fahrenheit" << endl;
  js = js["forecast"]["simpleforecast"]["forecastday"];
  
  // display the data to the user
  cout << "Here is your four day forecast" << endl << endl;
  for(int day = 0; day < 4; day++){
    cout << "Date: "<< js[day]["date"]["month"].dump(2) <<","
	 << js[day]["date"]["day"].dump(2) << ","
	 << js[day]["date"]["year"].dump(2) << endl;
    cout << "------------------------------------------" << endl;
    cout << "High: " << js[day]["high"]["fahrenheit"].dump(2) << endl;
    cout << "Low: " << js[day]["low"]["fahrenheit"].dump(2) << endl;
    cout << "Conditions: " << js[day]["conditions"].dump(2) << endl << endl;
  }
  bzero(json_msg, 1000000);
  }while(user_choice != "0");
  // close the socket when you are done communicating with the server
  close(sockfd);
  
  return 0;
}
