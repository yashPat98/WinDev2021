//headers
#include <windows.h>
#include "ClassFactoryDllServerWithRegFile.h"

//class declarations
class CSumSubtract:public ISum, ISubtract
{
    private:
        long m_cRef;
    public:
        //constructors
        CSumSubtract(void);

        //destructor
        ~CSumSubtract(void);

        //IUnknown specific method declarations (inherited)
        HRESULT __stdcall QueryInterface(REFIID, void**);
        ULONG __stdcall AddRef(void);
        ULONG __stdcall Release(void);

        //ISum specific method declarations (inherited)
        HRESULT __stdcall SumOfTwoIntegers(int, int, int*);

        //ISubtract specific method declarations (inherited)
        HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*);
};

class CSumSubtractClassFactory:public IClassFactory
{
    private:
        long m_cRef;
    public:
        //constructors
        CSumSubtractClassFactory(void);

        //destructor
        ~CSumSubtractClassFactory(void);

        //IUnknown specific method declarations (inherited)
        HRESULT __stdcall QueryInterface(REFIID, void**);
        ULONG __stdcall AddRef(void);
        ULONG __stdcall Release(void);

        //IClassFactory specific method declarations (inherited)
        HRESULT __stdcall CreateInstance(IUnknown *, REFIID, void**);
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
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
    }

    return (TRUE);
}

//Implementation of CSumSubtract 
CSumSubtract::CSumSubtract(void)
{
    //code
    m_cRef = 1;
    InterlockedIncrement(&glNumberOfActiveComponents);
}

CSumSubtract::~CSumSubtract(void)
{
    //code
    InterlockedDecrement(&glNumberOfActiveComponents);
}

//IID_ISum, &gpISum
//IID_ISubtract, &gpISubtract
HRESULT CSumSubtract::QueryInterface(REFIID riid, void **ppv)
{
    //code
    if(riid == IID_IUnknown)
    {
        *ppv = static_cast<ISum*>(this);
    }
    else if(riid == IID_ISum)
    {
        *ppv = static_cast<ISum*>(this);
    }
    else if(riid == IID_ISubtract)
    {
        *ppv = static_cast<ISubtract*>(this);
    }
    else
    {
        *ppv = NULL;
        return (E_NOINTERFACE);
    }

    reinterpret_cast<IUnknown*>(*ppv)->AddRef();
    return (S_OK);
}

ULONG CSumSubtract::AddRef(void)
{
    //code
    InterlockedIncrement(&m_cRef);
    return (m_cRef);
}

ULONG CSumSubtract::Release(void)
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

HRESULT CSumSubtract::SumOfTwoIntegers(int num1, int num2, int *pSum)
{
    //code
    *pSum = num1 + num2;
    return (S_OK);
}

HRESULT CSumSubtract::SubtractionOfTwoIntegers(int num1, int num2, int *pSubtract)
{
    //code
    *pSubtract = num1 - num2;
    return (S_OK);
}

//Implementation of CSumSubtractClassFactory
CSumSubtractClassFactory::CSumSubtractClassFactory(void)
{
    //code
    m_cRef = 1;
}

CSumSubtractClassFactory::~CSumSubtractClassFactory(void)
{
    //code
}

//IID_IClassFactory, &pIClassFactory
HRESULT CSumSubtractClassFactory::QueryInterface(REFIID riid, void **ppv)
{
    //code
    if(riid == IID_IUnknown)
    {
        *ppv = static_cast<IClassFactory*>(this);
    }
    else if(riid == IID_IClassFactory)
    {
        *ppv = static_cast<IClassFactory*>(this);
    }
    else 
    {
        *ppv = NULL;
        return (E_NOINTERFACE);
    }

    reinterpret_cast<IUnknown*>(*ppv)->AddRef();
    return (S_OK);
}

ULONG CSumSubtractClassFactory::AddRef(void)
{
    //code 
    InterlockedIncrement(&m_cRef);
    return (m_cRef);
}

ULONG CSumSubtractClassFactory::Release(void)
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

//NULL, IID_ISum, &gpISum;
HRESULT CSumSubtractClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
    //variable declarations
    CSumSubtract *pCSumSubtract = NULL;
    HRESULT hr;

    //code
    if(pUnkOuter != NULL)
        return (CLASS_E_NOAGGREGATION);
    
    //create instance of component CSumSubtract
    pCSumSubtract = new CSumSubtract;
    if(pCSumSubtract == NULL)
        return (E_OUTOFMEMORY);
    
    //get the requested interface 
    hr = pCSumSubtract->QueryInterface(riid, ppv);
    pCSumSubtract->Release();
    return (hr);
}

HRESULT CSumSubtractClassFactory::LockServer(BOOL fLock)
{
    //code
    if(fLock)
    {
        InterlockedIncrement(&glNumberOfServerLocks);
    }
    else
    {
        InterlockedDecrement(&glNumberOfServerLocks);
    }

    return (S_OK);
}

//Implementation of exported functions from this Dll
//CLSID_SumSubtract , IID_IClassFactory, &pIClassFactory(inside CoCreateInstance) 
HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
    //variable declarations
    CSumSubtractClassFactory *pCSumSubtractClassFactory = NULL;
    HRESULT hr; 

    //code
    if(rclsid != CLSID_SumSubtract)
        return (CLASS_E_CLASSNOTAVAILABLE);

    //create class factory
    pCSumSubtractClassFactory = new CSumSubtractClassFactory;
    if(pCSumSubtractClassFactory == NULL)
        return (E_OUTOFMEMORY);
    
    hr = pCSumSubtractClassFactory->QueryInterface(riid, ppv);
    pCSumSubtractClassFactory->Release();

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
