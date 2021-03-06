#pragma once
#include <mutex>
#include <queue>
#include <string.h>
#include <string>
#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <windows.h>

class Messenger
{
public:
	std::queue<std::string> sentMessages;
	static Messenger & Instance();
	void AddKeyboardMessage(WPARAM wParam,const int& vkCode);
	void AddMouseMessage(WPARAM wParam, LPARAM lParam, POINT p);
	std::string SetMessageMouseScroll(const std::string& action, const short& delta);
	std::string SetMessageMouseAction(const std::string& action, const int& dx, const int&dy);
	void AddOutOfBorderMessage(const std::string & side, float ratio);
	int GetKeyBoardAction(WPARAM wParam);
	int GetMouseAction(WPARAM wParam);
	char recievedMessage[1024];
private:
	Messenger() {};
	~Messenger() {};
	static std::mutex mutex_;
	Messenger(Messenger const&) = delete;
	Messenger(Messenger const&&) = delete;
	Messenger operator=(Messenger const&) = delete;
	Messenger operator=(Messenger const&&) = delete;
};

