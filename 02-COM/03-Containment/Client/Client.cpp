//headers
#include <windows.h>
#include <process.h>
#include "ServerComponent.h"

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations
ISum *gpISum = NULL;
ISubtract *gpISubtract = NULL;
IMultiplication *gpIMultiplication = NULL;
IDivision *gpIDivision = NULL;

//entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    //variable declarations
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[] = TEXT("COM Client");
    HRESULT hr;

    //code
    //COM Initialization
    hr = CoInitialize(NULL);
    if(FAILED(hr))
    {
        MessageBox(NULL, TEXT("COM Library Can Not Be Initialized."), TEXT("Program Error"), MB_OK);
        exit(0);
    }

    //initialize WNDCLASSEX
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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

    //COM uninitialize
    CoUninitialize();
    return ((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //function declarations
    void safeInterfaceRelease(void);

    //variable declarations
    HRESULT hr;
    int num1, num2, sum, sub, mul, div;
    TCHAR str[255]; 

    //code
    switch(iMsg)
    {
        case WM_CREATE:
            MessageBox(NULL, TEXT("Inside WndProc"), TEXT("Error"), MB_OK);

            hr = CoCreateInstance(CLSID_SumSubtract, NULL, CLSCTX_INPROC_SERVER, IID_ISum, (void **)&gpISum);
            if(FAILED(hr))
            {
                MessageBox(hwnd, TEXT("ISum Interface Can Not Be Obtained"), TEXT("Error"), MB_OK);
                DestroyWindow(hwnd);
            }

            //ISum 
            num1 = 65;
            num2 = 45;
            gpISum->SumOfTwoIntegers(num1, num2, &sum);
            
            wsprintf(str, TEXT("Sum of %d and %d = %d"), num1, num2, sum);
            MessageBox(hwnd, str, TEXT("Result"), MB_OK);

            hr = gpISum->QueryInterface(IID_ISubtract, (void**)&gpISubtract);
            if(FAILED(hr))
            {
                MessageBox(hwnd, TEXT("ISubtract Interface Can Not Be Obtained"), TEXT("Error"), MB_OK);
                DestroyWindow(hwnd);
            }

            gpISum->Release();
            gpISum = NULL;

            //ISubtract 
            num1 = 155;
            num2 = 55;
            gpISubtract->SubtractionOfTwoIntegers(num1, num2, &sub);
            
            wsprintf(str, TEXT("Subtraction of %d and %d = %d"), num1, num2, sub);
            MessageBox(hwnd, str, TEXT("Result"), MB_OK);

            hr = gpISubtract->QueryInterface(IID_IMultiplication, (void**)&gpIMultiplication);
            if(FAILED(hr))
            {
                MessageBox(hwnd, TEXT("IMultiplication Interface Can Not Be Obtained"), TEXT("Error"), MB_OK);
                DestroyWindow(hwnd);
            }

            gpISubtract->Release();
            gpISubtract = NULL;

            //IMultiplication
            num1 = 30;
            num2 = 25;
            gpIMultiplication->MultiplicationOfTwoIntegers(num1, num2, &mul);
            
            wsprintf(str, TEXT("Multiplication of %d and %d = %d"), num1, num2, mul);
            MessageBox(hwnd, str, TEXT("Result"), MB_OK);

            hr = gpIMultiplication->QueryInterface(IID_IDivision, (void**)&gpIDivision);
            if(FAILED(hr))
            {
                MessageBox(hwnd, TEXT("IDivision Interface Can Not Be Obtained"), TEXT("Error"), MB_OK);
                DestroyWindow(hwnd);
            }

            gpIMultiplication->Release();
            gpIMultiplication = NULL;

            //IDivision
            num1 = 200;
            num2 = 25;
            gpIDivision->DivisionOfTwoIntegers(num1, num2, &div);
            
            wsprintf(str, TEXT("Division of %d and %d = %d"), num1, num2, div);
            MessageBox(hwnd, str, TEXT("Result"), MB_OK);

            gpIDivision->Release();
            gpIDivision = NULL;

            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            safeInterfaceRelease();
            PostQuitMessage(0);
            break;
        
        default:
            break;
    }

    return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void safeInterfaceRelease(void)
{
    //code
    if(gpISum)
    {
        gpISum->Release();
        gpISum = NULL;
    }

    if(gpISubtract)
    {
        gpISubtract->Release();
        gpISubtract = NULL;
    }

    if(gpIMultiplication)
    {
        gpIMultiplication->Release();
        gpIMultiplication = NULL;
    }

    if(gpIDivision)
    {
        gpIDivision->Release();
        gpIDivision = NULL;
    }
}

