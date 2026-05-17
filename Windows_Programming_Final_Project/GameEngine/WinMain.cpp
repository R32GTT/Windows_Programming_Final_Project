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

//Input WinnMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)windProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
