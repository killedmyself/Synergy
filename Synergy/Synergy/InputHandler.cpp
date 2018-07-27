#include "InputHandler.h"

InputHandler & InputHandler::Instance()
{
	static InputHandler handler;
	return handler;
}

int GetKeyBoardAction(WPARAM wParam)
{
	switch (wParam)
	{
	case WM_KEYDOWN:
		return 0;
	case WM_KEYUP:
		return KEYEVENTF_KEYUP;
	default:
		return -1;
		break;
	}
}

LRESULT CALLBACK KeyboardEventProcServer(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT *hooked_key = (KBDLLHOOKSTRUCT*)lParam;
	if (hooked_key != NULL)
	{
		std::string wparam = std::to_string(GetKeyBoardAction(wParam));
		std::string lparam = std::to_string(lParam);
		std::string kcode = std::to_string(hooked_key->vkCode);
		if (wparam != "2")
		{
			std::string message = "0 " + wparam + ' ' + lparam + ' ' + kcode + '\0';
			if (InputHandler::Instance().hasConnection)
			{
				InputHandler::Instance().sentMessage.push(message);
			}
		}
		return (InputHandler::Instance().isCurrentComputerDisabled ? 1 : CallNextHookEx(InputHandler::Instance().hKeyboardHook, nCode, wParam, lParam));
	}
}

bool MouseEventProcOutOfBorder(int nCode, WPARAM wParam, LPARAM lParam)
{
	MOUSEHOOKSTRUCT * pMouseStruct = (MOUSEHOOKSTRUCT *)lParam;
	if (pMouseStruct != NULL)
	{
		POINT P;
		GetCursorPos(&P);
		float yCoord, xCoord;
		if (pMouseStruct->pt.x < 0 && P.x == 0 && !InputHandler::Instance().isCurrentComputerDisabled)
		{
			InputHandler::Instance().isCurrentComputerDisabled = true;
			yCoord = (float)P.y / GetSystemMetrics(SM_CYSCREEN);
			InputHandler::Instance().sentMessage.push("3 " + std::to_string(yCoord));
			SetCursorPos(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			return true;
		}
		if (pMouseStruct->pt.x > 0 && P.x == GetSystemMetrics(SM_CXSCREEN) - 1 && !InputHandler::Instance().isCurrentComputerDisabled)
		{
			InputHandler::Instance().isCurrentComputerDisabled = true;
			yCoord = (float)P.y / GetSystemMetrics(SM_CYSCREEN);
			InputHandler::Instance().sentMessage.push("4 " + std::to_string(yCoord));
			SetCursorPos(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			return true;
		}
	}
	return false;
}

int GetMouseAction(WPARAM wParam)
{
	switch (wParam)
	{
	case WM_LBUTTONDOWN:
		return MOUSEEVENTF_LEFTDOWN;
		break;
	case WM_LBUTTONUP:
		return MOUSEEVENTF_LEFTUP;
		break;
	case WM_MBUTTONDOWN:
		return MOUSEEVENTF_MIDDLEDOWN;
		break;
	case WM_MBUTTONUP:
		return MOUSEEVENTF_MIDDLEUP;
		break;
	case WM_RBUTTONDOWN:
		return MOUSEEVENTF_RIGHTDOWN;
		break;
	case WM_RBUTTONUP:
		return MOUSEEVENTF_RIGHTUP;
		break;
	case WM_XBUTTONDOWN:
		return MOUSEEVENTF_XDOWN;
		break;
	case WM_XBUTTONUP:
		return MOUSEEVENTF_XUP;
		break;
	case WM_MOUSEWHEEL:
		return MOUSEEVENTF_WHEEL;
		break;
	case WM_MOUSEMOVE:
		return MOUSEEVENTF_MOVE;
		break;
	default:
		return -1;
		break;
	}
}

LRESULT CALLBACK MouseEventProcServer(int nCode, WPARAM wParam, LPARAM lParam)
{
	MOUSEHOOKSTRUCT * pMouseStruct = (MOUSEHOOKSTRUCT *)lParam;
	if (pMouseStruct != NULL)
	{
		if (InputHandler::Instance().hasConnection)
		{
			if (!MouseEventProcOutOfBorder(nCode, wParam, lParam))
			{
				std::string wparam = std::to_string(wParam);
				std::string lparam = std::to_string(lParam);
				short dx = pMouseStruct->pt.x - InputHandler::Instance().mousePosition.x;
				short dy = pMouseStruct->pt.y - InputHandler::Instance().mousePosition.y;
				int action = GetMouseAction(wParam);
				std::string delta;
				std::string message;
				if (action != -1 && action != MOUSEEVENTF_WHEEL)
				{
					delta = std::to_string(dx) + ' ' + std::to_string(dy);
					message = "1 " + std::to_string(action) + ' ' + lparam + ' ' + delta + '\0';
				}
				else if (action == MOUSEEVENTF_WHEEL)
				{
					delta = std::to_string(GET_WHEEL_DELTA_WPARAM(wParam)) + ' ';
					message = "2 " + std::to_string(action) + ' ' + lparam + ' ' + delta + '\0';
				}
				if (InputHandler::Instance().hasConnection && action != -1)
				{
					InputHandler::Instance().sentMessage.push(message);
				}
				GetCursorPos(&InputHandler::Instance().mousePosition);
			}
		}
		return (InputHandler::Instance().isCurrentComputerDisabled ? 1 : CallNextHookEx(InputHandler::Instance().hKeyboardHook, nCode, wParam, lParam));
	}
}

void InputHandler::MyMouseLogger()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)MouseEventProcServer, hInstance, NULL);
	MessageLoop();
	UnhookWindowsHookEx(hMouseHook);

}

void InputHandler::MyKeyboardLogger()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardEventProcServer, hInstance, NULL);
	MessageLoop();
	UnhookWindowsHookEx(hKeyboardHook);

}

void InputHandler::MessageLoop()
{
	MSG message;
	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

void InputHandler::Run()
{
}