#include "BoostClient.h"

BClient::BClient(boost::asio::io_service& io_service) : m_Socket(io_service)
{
}


BClient::~BClient()
{
}

void BClient::Connect(boost::asio::ip::tcp::endpoint& endpoint)
{
	m_Socket.async_connect(endpoint,
		boost::bind(&BClient::handle_connect,
			this,
			boost::asio::placeholders::error));
}

void BClient::PostReceive()
{
	if (!Messenger::Instance().sentMessages.empty())
	{
		try {
			std::cout << Messenger::Instance().sentMessages.front() << std::endl;
			boost::asio::write(m_Socket, boost::asio::buffer(Messenger::Instance().sentMessages.front()));
			Messenger::Instance().sentMessages.pop();
		}
		catch (std::exception&) {
			return;
		}
	}
	m_Socket.async_read_some(boost::asio::buffer(buff),
		boost::bind(&BClient::handle_receive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}


void BClient::handle_connect(const boost::system::error_code& error)
{
	if (error)
	{
		InputHandler::Instance().hasConnection = false;
		InputHandler::Instance().isCurrentComputerDisabled = false;
		std::cout << "handle_connect connect failed error No: " << error.value() << " error Message: " << error.message() << std::endl;
	}
	else
	{
		InputHandler::Instance().hasConnection = true;
		InputHandler::Instance().isCurrentComputerDisabled = true;
		std::cout << "connected" << std::endl;
		PostReceive();
	}
}


void BClient::handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (error)
	{
		InputHandler::Instance().hasConnection = false;
		InputHandler::Instance().isCurrentComputerDisabled = false;
		if (error == boost::asio::error::eof)
		{
			std::cout << "Disconnect" << std::endl;
		}
		else
		{
			std::cout << "handle_receive error No: " << error.value() << " error Message: " << error.message() << std::endl;
		}
	}
	else
	{
		strRecvMessage = buff;
		Emulator::ParseMSG(strRecvMessage);
		//std::cout << "Message from server : " << strRecvMessage<< std::endl;
		PostReceive();
	}
}


