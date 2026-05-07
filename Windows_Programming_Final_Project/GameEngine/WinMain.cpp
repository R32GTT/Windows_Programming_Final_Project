#include "pch/pch.h"
#include "Input.h"

struct WinStuff
{
	bool running = true;
	Input input;

};

WinStuff windowsStuff;

LRESULT windProc(HWND hWnd, UINT iMsg, WPARAM wp, LPARAM lp)
{
	LRESULT result = 0;

	bool pressed = 0;

	switch (iMsg)
	{
	case WM_CLOSE:
		windowsStuff.running = false;
		break;
	case WM_SYSKEYDOWN: case WM_KEYDOWN:
		pressed = true;
	case WM_SYSKEYUP: case WM_KEYUP:

	}

}