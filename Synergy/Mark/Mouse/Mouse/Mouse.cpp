#include "stdafx.h"
#include <iostream>
#include <windows.h>
using namespace std;

void MouseClicks() {
	while (true)
	{
		if (GetAsyncKeyState(VK_LBUTTON) < 0)
		{
			cout << "LBUTTON" << endl;
			Sleep(200);
		}
		if (GetAsyncKeyState(VK_RBUTTON) < 0)
		{
			cout << "RBUTTON" << endl;
			Sleep(200);
		}
	}
}
void MouseXY() {
	static POINT p;
	while(true)
	{
		GetCursorPos(&p);	
		cout << p.x << " " << p.y << endl;
	}
}

int main() {

	MouseClicks();
	return 0;
}