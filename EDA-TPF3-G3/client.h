#pragma once
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "Socket.h"
#include <iostream>
#include <string>
#include <curl/curl.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define HTTP_OK "HTTP OK"
#define MSG_NETWORK_READY	"a"
#define MSG_NETWORK_NOT_READY	"B"

/*******************************************************************************
 * NAMESPACES
 ******************************************************************************/
using namespace std;

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 *****************************************************************************/
typedef enum { POSTClient, GETClient } ClientType;


class Client
{
public:
	Client(NodeData data);
	Client(Socket socket);
	~Client();

	errorType sendRequest(void);
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