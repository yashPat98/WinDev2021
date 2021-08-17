//headers
#include <windows.h>
#include "Window.h"

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    //variable declarations
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[] = TEXT("Explicit DLL");

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
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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
    //variable declarations
    HMODULE hLib = NULL;
    typedef int (*PFNMAKECUBE)(int);
    PFNMAKECUBE pfnMakeCube = NULL;
    int num1, num2;
    TCHAR str[255];

    //code
    switch(iMsg)
    {
        case WM_CREATE:
            hLib = LoadLibrary(TEXT("MyMath.dll"));
            if(hLib == NULL)
            {
                MessageBox(hwnd, TEXT("Failed to load library"), TEXT("Error"), MB_OK | MB_ICONERROR);
                DestroyWindow(hwnd);
            }

            pfnMakeCube = (PFNMAKECUBE)GetProcAddress(hLib, "MakeCube");
            if(pfnMakeCube == NULL)
            {
                MessageBox(hwnd, TEXT("Failed to get MakeCube procedure"), TEXT("Error"), MB_OK | MB_ICONERROR);
                DestroyWindow(hwnd);
            }

            num1 = 15;
            num2 = pfnMakeCube(num1);
            wsprintf(str, TEXT("The cube of %d is %d."), num1, num2);
            MessageBox(hwnd, str, TEXT("cube"), MB_OK);
            break;
        
        case WM_DESTROY:
            FreeLibrary(hLib);
            hLib = NULL;
            PostQuitMessage(0);
            break;
        
        default:
            break;
    }

    return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}
