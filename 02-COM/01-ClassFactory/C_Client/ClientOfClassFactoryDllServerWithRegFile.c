//headers
#include <windows.h>
#include "ClassFactoryDllServerWithRegFileC.h"

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations
ISum *gpISum = NULL;
ISubtract *gpISubtract = NULL;

//entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    //variable declarations
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[] = TEXT("Com Client");
    HRESULT hr;

    //code
    //COM initialization (internally maps ole32.dll oleaut32.dll to process address space)
    hr = CoInitialize(NULL);
    if(FAILED(hr))
    {
        MessageBox(NULL, TEXT("COM Library Can Not Be Initialized.\nProgram Will Now Exit."), TEXT("Program Error"), MB_OK | MB_ICONERROR);
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

    CoUninitialize();
    return ((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //variable declarations
    HRESULT hr;
    int num1, num2, sum, subtract;
    TCHAR str[255];

    //code
    switch(iMsg)
    {
        case WM_CREATE:
            hr = CoCreateInstance(&CLSID_SumSubtract, NULL, CLSCTX_INPROC_SERVER, &IID_ISum, (void**)&gpISum);
            if(FAILED(hr))
            {
                MessageBox(hwnd, TEXT("ISum Interface Can Not Be Obtained"), TEXT("Error"), MB_OK | MB_ICONERROR);
                DestroyWindow(hwnd);
            }

            //initialize args hardcoded
            num1 = 55;
            num2 = 45;

            //request service from dll
            gpISum->lpVtbl->SumOfTwoIntegers(gpISum, num1, num2, &sum);

            //display result 
            wsprintf(str, TEXT("Sum of %d and %d = %d"), num1, num2, sum);
            MessageBox(hwnd, str, TEXT("Result"), MB_OK);

            hr = gpISum->lpVtbl->QueryInterface(gpISum, &IID_ISubtract, (void**)&gpISubtract);
            if(FAILED(hr))
            {
                gpISum->lpVtbl->Release(gpISum);
                gpISum = NULL;  
                MessageBox(hwnd, TEXT("ISubtract Interface Can Not Be Obtained"), TEXT("Error"), MB_OK | MB_ICONERROR);
                DestroyWindow(hwnd);
            }

            //release ISum interface
            gpISum->lpVtbl->Release(gpISum);
            gpISum = NULL;

            num1 = 155;
            num2 = 145;

            //request service from dll
            gpISubtract->lpVtbl->SubtractionOfTwoIntegers(gpISubtract, num1, num2, &subtract);

            //release ISubtract
            gpISubtract->lpVtbl->Release(gpISubtract);
            gpISubtract = NULL;

            //display result 
            wsprintf(str, TEXT("Subtraction of %d and %d = %d"), num1, num2, subtract);
            MessageBox(hwnd, str, TEXT("Result"), MB_OK);

            //exit the application
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        
        default:
            break;
    }

    return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}

 /*                         CLSID_SumSubtract, NULL, INPROC, IID_ISum, &gpISum
  * HRESULT CoCreateInstace(REFCLSID rclsid, IUnknown *pUnkOuter, DWORD dwClsContext, REFIID riid, void **ppv)
  * {
  *     IClassFactory *pIClassFactory = NULL;
  *     HRESULT hr;    
  * 
  *     hr = CoGetClassObject(rclsid , dwClsContext, NULL, IID_IClassFactory, (void**)&pIClassFactory);
  *     if(SUCCEEDED(hr))
  *     {
  *         pIClassFactory->CreateInstance((IUnknown *pUnkOuter)NULL, riid, ppv);
  *         pIClassFactory->Release();
  *         PIClassFactory = NULL;
  *     }
  *     
  *     return (hr);
  * }
  *                          CLSID_SumSubtract, INPROC, struct CoServerInfo, IID_IClassFactory, &pIClassFactory
  * HRESULT CoGetClassObject(REFCLSID rclsid, DWORD dwClsContext, NanterBolu, REFIID riid, void **ppv)
  * {
  *     // .. takes CLSID_SumSubtract goes to registry and return the path of dll which we provided in reg file
  *     
  *     //get hdll from load library
  *     HINSTANCE hDll = CoLoadLibrary(DllPath, TRUE);  ->  LoadLibrary()
  *     
  *     //get global functions from dll
  *     pfn1 = GetProcAddress(hDll, "DllGetCLassObject")
  *     pfn2 = GetProcAddress(hDdll, "DllCanUnloadNow")
  * 
  *     //call global functions from dll
  *     pfn1(rclsid, riid, ppv);
  * 
  *     //if TRUE -> give pfn2 to thread which pokes application to remove dll(COM thread, dll kadu ka)
  *     //if FALSE -> CoUninitialize() in WinMain will call CoFreeLibrary() to free all the loaded libraries
  * }
  *
  * TRUE = poke application periodically to remove dll
  * FALSE = dont poke application frequently to remove dll
  * HINSTANCE CoLoadLibrary(DllPath, BOOl)
  * {
  *     //loads/maps the dll in clients address space
  *     return (LoadLibrary(DllPath));
  * }
  * 
  * CoFreeUnusedLibraries()
  * {
  *     CoFreeLibrary()
  *     {
  *         FreeLibrary();
  *     }
  * }
  */
