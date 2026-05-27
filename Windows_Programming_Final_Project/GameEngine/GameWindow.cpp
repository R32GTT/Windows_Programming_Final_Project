#include "pch.h"
#include "GameWindow.h"
#include "GameEngine.h"


HINSTANCE hInst;
HWND g_hWnd;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    GameEngine GE;
    GE.Init(g_hWnd);


    MSG msg{};
    unsigned __int64  prevTick = 0;

    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        else
        {
            unsigned __int64 now = ::GetTickCount64();

            {
                GE.Update();
                GE.Render();

                prevTick = now;
            }
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex{};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style              = CS_VREDRAW | CS_HREDRAW;
    wcex.lpfnWndProc        = WndProc;
    wcex.cbClsExtra         = 0;
    wcex.cbWndExtra         = 0;
    //wcex.hIcon            = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hInstance          = hInstance;
    wcex.hCursor            = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground      = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName       = nullptr;
    wcex.lpszClassName      = L"WNPGFINAL";
    //wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    RECT windowRect{ 0,0,WinSizeX,WinSizeY };
    ::AdjustWindowRect(&windowRect, WS_SYSMENU|WS_CAPTION|WS_OVERLAPPED, false);

    HWND hWnd = CreateWindowW(L"WNPGFINAL", L"HL Clone", WS_SYSMENU | WS_CAPTION | WS_OVERLAPPED,
        CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, hInstance, nullptr);
    
    g_hWnd = hWnd;

    if (!hWnd)
    {
        DWORD errorCode = GetLastError();
        return FALSE;
    }

    ::ShowWindow(hWnd, nCmdShow);
    ::UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
        {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        /*
        case IDM_EXIT:
            ::DestroyWindow(hWnd);
            break;
        */
        default:
            return ::DefWindowProc(hWnd, msg, wParam, lParam);
        }
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT PS;

        HDC hdc = ::BeginPaint(hWnd, &PS);

        ::EndPaint(hWnd, &PS);
    }
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        break;
    default:
        return ::DefWindowProc(hWnd, msg, wParam, lParam);
        break;
    }
    return 0;
}