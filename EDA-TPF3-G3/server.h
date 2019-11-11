#pragma once
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <iostream>
#include <cstdio>
#include <cstring>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

/*******************************************************************************
 * NAMESPACES
 ******************************************************************************/
using namespace std;

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 *****************************************************************************/
typedef enum { BLOCK,TX, MERKLE, FILTER, GET, LAYOUT, PING, READY, NOTREADY, ERR } STATE;

class Server
{
public:
	Server(unsigned int port);
	~Server();

	/***********************************************************************************
		 SERVER NETWORKING
	***********************************************************************************/
	void startConnection();
	void listening();
	void receiveMessage();
	void sendMessage(const string& message);

	/***********************************************************************************
		 GETTERS
	***********************************************************************************/
	bool getDoneListening();
	bool getDoneDownloading();
	bool getDoneSending();
	STATE getState();
	string getMessage();

private:

	/***********************************************************************************
		 FLAGS
	***********************************************************************************/
	bool doneListening;
	bool doneDownloading;
	bool doneSending;

	/***********************************************************************************
		MESSAGES
	***********************************************************************************/
	string receiveMsg;
	string bodyMsg;
	
	/***********************************************************************************
		BOOST SH*T
	***********************************************************************************/
	boost::system::error_code error;
	boost::asio::io_service* IO_handler;
	boost::asio::ip::tcp::socket* socket;
	boost::asio::ip::tcp::acceptor* acceptor;

	/***********************************************************************************
		RESPONSE METHODS
	***********************************************************************************/
	STATE parseMessage();
	STATE state;
	jsonHandler JSON;
};