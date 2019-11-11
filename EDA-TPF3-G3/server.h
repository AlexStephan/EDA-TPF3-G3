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
typedef enum { TX, BLOCK, MERKLE, FILTER, GET, ERR } STATE;

class Server
{
public:
	Server(unsigned int port);
	~Server();
	void startConnection();
	void listening();
	void receiveMessage();
	void sendMessage(const string& message);
	bool getDoneListening();
	bool getDoneDownloading();
	bool getDoneSending();
	STATE getState();
	string getMessage();

private:

	bool doneListening;
	bool doneDownloading;
	bool doneSending;
	STATE state;
	string receiveMsg;
	string bodyMsg;
	boost::system::error_code error;
	boost::asio::io_service* IO_handler;
	boost::asio::ip::tcp::socket* socket;
	boost::asio::ip::tcp::acceptor* acceptor;

	STATE parseMessage();
	bool validateBlock(string blck);
	bool validateTx(string tx);
	bool validateFilter(string filter);
};