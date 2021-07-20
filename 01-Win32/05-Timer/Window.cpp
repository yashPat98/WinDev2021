//headers
#include <windows.h>

//macros
#define ID_TIMER1 101

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations

//entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    //variable declarations
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[] = TEXT("Timer");

    //code
    //initialize WNDCLASSEX
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName = NULL;
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    //register above class
    RegisterClassEx(&wndclass);

    //create the window in memory
    hwnd = CreateWindow(szAppName,
        szAppName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL, 
        NULL,
        hInstance,
        NULL);
    
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    //message loop
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return ((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //variable declarations
    HDC hdc;
    RECT rc;
    PAINTSTRUCT ps;
    HBRUSH hbrush;
    static int iPaintFlag = 0;

    //code
    switch(iMsg)
    {
        case WM_CREATE:
            SetTimer(hwnd, ID_TIMER1, 1000, NULL);
            break;

        case WM_TIMER:
            KillTimer(hwnd, ID_TIMER1);
            iPaintFlag++;
            if(iPaintFlag >= 8)
                iPaintFlag = 0;
            InvalidateRect(hwnd, NULL, TRUE);
            SetTimer(hwnd, ID_TIMER1, 1000, NULL);
            break;

        case WM_PAINT:
            GetClientRect(hwnd, &rc);
            hdc = BeginPaint(hwnd, &ps);
            
            if(iPaintFlag == 1)
            {
                hbrush = CreateSolidBrush(RGB(255, 0, 0));
            }
            else if(iPaintFlag == 2)
            {
                hbrush = CreateSolidBrush(RGB(0, 255, 0));
            }
            else if(iPaintFlag == 3)
            {
                hbrush = CreateSolidBrush(RGB(0, 0, 255));
            }
            else if(iPaintFlag == 4)
            {
                hbrush = CreateSolidBrush(RGB(0, 255, 255));
            }
            else if(iPaintFlag == 5)
            {
                hbrush = CreateSolidBrush(RGB(255, 0, 255));
            }
            else if(iPaintFlag == 6)
            {
                hbrush = CreateSolidBrush(RGB(255, 255, 0));
            }
            else if(iPaintFlag == 7)
            {
                hbrush = CreateSolidBrush(RGB(0, 0, 0));
            }
            else if(iPaintFlag == 8)
            {
                hbrush = CreateSolidBrush(RGB(255, 255, 255));
            }
            else
            {
                hbrush = CreateSolidBrush(RGB(0, 0, 0));
            }

            SelectObject(hdc, hbrush);
            FillRect(hdc, &rc, hbrush);
            DeleteObject(hbrush);
            EndPaint(hwnd, &ps);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        
        default:
            break;
    }

    return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}
