#pragma once
#include "sSocket.h"
#include <iostream>
#include <string>
#include <curl/curl.h>

#define HTTP_OK "HTTP OK"
#define MSG_NETWORK_READY	"a"
#define MSG_NETWORK_NOT_READY	"B"

using namespace std;

typedef enum { POSTClient, GETClient } ClientType;

class Client
{
public:
	Client(sSocket socket);
	~Client();

	void sendRequest(void);
	void POST(string path, string& json);
	void GET(string path, string& json);
	Socket getSocket();
	string getResponse(void);
	int getRunning();
	ClientType getClientType() { return cType; };

private:
	ClientType cType;
	CURLM* curlm;
	CURL* curl;
	string host;
	string response;
	int running;
};