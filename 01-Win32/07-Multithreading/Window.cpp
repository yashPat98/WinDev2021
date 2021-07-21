//headers
#include <windows.h>
#include <tchar.h>
#include "Window.h"

//library linking
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "kernel32.lib")

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI ThreadProc1(LPVOID);
DWORD WINAPI ThreadProc2(LPVOID);

//entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    //variable declarations
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[] = TEXT("Multithreading");

    //code
    //initialize WNDCLASSEX
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName = NULL;
    wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

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
    //variable declaration
    static HANDLE hThread1 = NULL;
    static HANDLE hThread2 = NULL;

    //code
    switch(iMsg)
    {
        case WM_CREATE:
            hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc1, (LPVOID)hwnd, 0, NULL);
            if(hThread1 == NULL)
            {
                MessageBox(hwnd, TEXT("Failed To Create Thread 1"), TEXT("Error"), MB_OK | MB_ICONERROR);
                DestroyWindow(hwnd);
            }

            hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc2, (LPVOID)hwnd, 0, NULL);
            if(hThread2 == NULL)
            {
                MessageBox(hwnd, TEXT("Failed To Create Thread 2"), TEXT("Error"), MB_OK | MB_ICONERROR);
                DestroyWindow(hwnd);
            }
            break;

        case WM_LBUTTONDOWN:
            MessageBox(hwnd, TEXT("I am thread no 4"), TEXT("Message"), MB_OK);
            break;

        case WM_DESTROY:
            if(hThread1)
            {
                CloseHandle(hThread1);
                hThread1 = NULL;
            }

            if(hThread2)
            {
                CloseHandle(hThread2);
                hThread2 = NULL;
            }

            PostQuitMessage(0);
            break;
        
        default:
            break;
    }

    return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}

DWORD WINAPI ThreadProc1(LPVOID param)
{
    //variable declaration
    HDC hdc = NULL;
    TCHAR str[255];
    long i;

    //code
    hdc = GetDC((HWND)param);
    SetBkColor(hdc, RGB(0, 0, 0));
    SetTextColor(hdc, RGB(0, 255, 0));
    
    for(i = 0; i < 2147483647; i++)
    {
        wsprintf(str, TEXT("Incrementing Order : %ld"), i);
        TextOut(hdc, 5, 5, str, (int)_tcslen(str));
    }
    
    ReleaseDC((HWND)param, hdc);
    hdc = NULL;

    return (0);
}

DWORD WINAPI ThreadProc2(LPVOID param)
{
    //variable declaration
    HDC hdc = NULL;
    TCHAR str[255];
    long i;

    //code
    hdc = GetDC((HWND)param);
    SetBkColor(hdc, RGB(0, 0, 0));
    SetTextColor(hdc, RGB(255, 0, 0));
    
    for(i = 2147483647; i >= 0; i--)
    {
        wsprintf(str, TEXT("Decrementing Order : %ld"), i);
        TextOut(hdc, 5, 25, str, (int)_tcslen(str));
    }
    
    ReleaseDC((HWND)param, hdc);
    hdc = NULL;

    return (0);
}
