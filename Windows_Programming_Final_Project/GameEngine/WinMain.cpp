#include "Input.h"
#include<fstream>

//윈도우 창 띄우기 용
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"My Window Class";
LPCTSTR lpszWindowName = L"Window Programming Final Assignment Game";


//맵의 가로 세로 타일 사이즈 변수
const int MAP_WIDTH = 40;
const int MAP_HEIGHT = 40;
const int TILE_SIZE = 50;

//맵 데이터를 불러와 저장할 2차원 배열
int mapData[MAP_HEIGHT][MAP_WIDTH];

//Load Map function
//void LoadMap() {

//}

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


