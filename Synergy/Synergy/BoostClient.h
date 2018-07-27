#pragma once
#include "InputHandler.h"
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <bitset>
#include <boost/asio/read_until.hpp>
#include "Emulator.h"
#include <string>
#include <locale>
#include <iostream>
#include <deque>

using  boost::asio::ip::tcp;

class BClient
{
public:
	BClient(boost::asio::io_service& io_service);
	~BClient();

	void Connect(boost::asio::ip::tcp::endpoint& endpoint);
private:
	void PostReceive();
	void handle_connect(const boost::system::error_code& error);
	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_write(const boost::system::error_code& error, size_t bytes_transferred);
	void ParseMSG();
	boost::asio::ip::tcp::socket m_Socket;

	int m_nSeqNumber;
	char buff[1024];
	std::string m_WriteMessage;
	std::string strRecvMessage;

};

