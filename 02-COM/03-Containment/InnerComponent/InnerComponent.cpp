//headers
#include <windows.h>
#include "InnerComponent.h"

//class declarations
class CMultiplicationDivision:public IMultiplication, IDivision 
{
    private:
        long m_cRef;
    public:
        CMultiplicationDivision(void);
        ~CMultiplicationDivision(void);

        //IUnknown specific methods
        HRESULT __stdcall QueryInterface(REFIID, void**);
        ULONG __stdcall AddRef(void);
        ULONG __stdcall Release(void);

        //IMultiplication specific methods
        HRESULT __stdcall MultiplicationOfTwoIntegers(int, int, int*);

        //IDivision specific methods
        HRESULT __stdcall DivisionOfTwoIntegers(int, int, int*);
};

class CMultiplicationDivisionClassFactory:public IClassFactory
{
    private:
        long m_cRef;
    public:
        CMultiplicationDivisionClassFactory(void);
        ~CMultiplicationDivisionClassFactory(void);

        //IUnknown specific methods
        HRESULT __stdcall QueryInterface(REFIID, void**);
        ULONG __stdcall AddRef(void);
        ULONG __stdcall Release(void);

        //IClassFactory specific methods
        HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
        HRESULT __stdcall LockServer(BOOL);
};

//global variable declarations
long glNumberOfActiveComponents = 0;
long glNumberOfServerLocks = 0;

//DllMain
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved)
{
    //code
    switch(dwReason)
    {
        case DLL_PROCESS_ATTACH:
            break;

        case DLL_PROCESS_DETACH:
            break;
    }

    return (TRUE);
}

//Implementation of CMultiplicationDivision 
CMultiplicationDivision::CMultiplicationDivision(void)
{
    //code
    m_cRef = 1;
    InterlockedIncrement(&glNumberOfActiveComponents);
}

CMultiplicationDivision::~CMultiplicationDivision(void)
{
    //code
    InterlockedDecrement(&glNumberOfActiveComponents);
}

HRESULT CMultiplicationDivision::QueryInterface(REFIID riid, void **ppv)
{
    //code
    if(riid == IID_IUnknown)
        *ppv = static_cast<IMultiplication*>(this);
    else if(riid == IID_IMultiplication)
        *ppv = static_cast<IMultiplication*>(this);
    else if(riid == IID_IDivision)
        *ppv = static_cast<IDivision*>(this);
    else 
    {
        *ppv = NULL;
        return (E_NOINTERFACE);
    }

    reinterpret_cast<IUnknown*>(*ppv)->AddRef();
    return (S_OK);
}

ULONG CMultiplicationDivision::AddRef(void)
{
    //code
    InterlockedIncrement(&m_cRef);
    return (m_cRef);
}

ULONG CMultiplicationDivision::Release(void)
{
    //code
    InterlockedDecrement(&m_cRef);
    if(m_cRef == 0)
    {
        delete (this);
        return (0);
    }

    return (m_cRef);
}

HRESULT CMultiplicationDivision::MultiplicationOfTwoIntegers(int num1, int num2, int *pMultiplication)
{
    //code
    *pMultiplication = num1 * num2;
    return (S_OK);
}

HRESULT CMultiplicationDivision::DivisionOfTwoIntegers(int num1, int num2, int *pDivision)
{
    //code
    *pDivision = num1 / num2;
    return (S_OK);
}

//Implementation of CMultiplicationDivisionClassFactory
CMultiplicationDivisionClassFactory::CMultiplicationDivisionClassFactory(void)
{
    //code
    m_cRef = 1;
}

CMultiplicationDivisionClassFactory::~CMultiplicationDivisionClassFactory(void)
{
    //code
}

HRESULT CMultiplicationDivisionClassFactory::QueryInterface(REFIID riid, void **ppv)
{
    //code
    if(riid == IID_IUnknown)
        *ppv = static_cast<IClassFactory*>(this);
    else if(riid == IID_IClassFactory)
        *ppv = static_cast<IClassFactory*>(this);
    else 
    {
        *ppv = NULL;
        return (E_NOINTERFACE);
    }

    reinterpret_cast<IUnknown*>(*ppv)->AddRef();
    return (S_OK);
}

ULONG CMultiplicationDivisionClassFactory::AddRef(void)
{
    //code
    InterlockedIncrement(&m_cRef);
    return (m_cRef);
}

ULONG CMultiplicationDivisionClassFactory::Release(void)
{
    //code 
    InterlockedDecrement(&m_cRef);
    if(m_cRef == 0)
    {
        delete (this);
        return (0);
    }

    return (m_cRef);
}

HRESULT CMultiplicationDivisionClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
    //variable declarations 
    CMultiplicationDivision *pCMultiplicationDivision = NULL;
    HRESULT hr;

    //code
    MessageBox(NULL, TEXT("Inside Inner CreateInstance"), TEXT("Error"), MB_OK);
    if(pUnkOuter != NULL)
        return (CLASS_E_NOAGGREGATION);

    pCMultiplicationDivision = new CMultiplicationDivision;
    if(pCMultiplicationDivision == NULL)
        return (E_OUTOFMEMORY);

    hr = pCMultiplicationDivision->QueryInterface(riid, ppv);
    pCMultiplicationDivision->Release();

    return (hr);
}

HRESULT CMultiplicationDivisionClassFactory::LockServer(BOOL fLock)
{
    //code
    if(fLock)
        InterlockedIncrement(&glNumberOfServerLocks);
    else 
        InterlockedDecrement(&glNumberOfServerLocks);
    
    return (S_OK);
}

//Implementation of Exported Functions From This Dll
HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
    //variable declarations
    CMultiplicationDivisionClassFactory *pCMultiplicationDivisionClassFactory = NULL;
    HRESULT hr;

    //code
    if(rclsid != CLSID_MultiplicationDivision)
        return (CLASS_E_CLASSNOTAVAILABLE);

    pCMultiplicationDivisionClassFactory = new CMultiplicationDivisionClassFactory;
    if(pCMultiplicationDivisionClassFactory == NULL)
        return (E_OUTOFMEMORY);

    hr = pCMultiplicationDivisionClassFactory->QueryInterface(riid, ppv);
    pCMultiplicationDivisionClassFactory->Release();

    return (hr);
}

HRESULT __stdcall DllCanUnloadNow(void)
{
    //code
    if((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
        return (S_OK);
    else 
        return (S_FALSE);
}
