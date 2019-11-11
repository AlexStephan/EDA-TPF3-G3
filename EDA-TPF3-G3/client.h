#pragma once
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "NodeData.h"
#include "errorType.h"
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

	/****************************************************************************
		 NETWOTK CONFIGURATON
	*****************************************************************************/
	void POST(string path, string& json);
	void GET(string path, string& json);

	/****************************************************************************
		COMMUNICATION
	*****************************************************************************/
	errorType sendRequest(void);

	/****************************************************************************
		 GETTERS
	*****************************************************************************/
	Socket getSocket();
	string getResponse(void);
	int getRunning();
	ClientType getClientType();

private:

	ClientType cType;
	CURLM* curlm;
	CURL* curl;
	string ip;
	string port;
	string response;
	int running;

	ip_t crackIp();
};