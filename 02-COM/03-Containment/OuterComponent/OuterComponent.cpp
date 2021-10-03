//headers
#include <windows.h>
#include "InnerComponent.h"
#include "OuterComponent.h"

//class declarations
class CSumSubtract:public ISum, ISubtract, IMultiplication, IDivision 
{
    private:
        long m_cRef;
        IMultiplication *m_pIMultiplication;
        IDivision *m_pIDivision;
    public:
        CSumSubtract(void);
        ~CSumSubtract(void);

        //IUnknown specific methods
        HRESULT __stdcall QueryInterface(REFIID, void **);
        ULONG __stdcall AddRef(void);
        ULONG __stdcall Release(void);

        //inherited interface methods
        HRESULT __stdcall SumOfTwoIntegers(int, int, int*);
        HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*);
        HRESULT __stdcall MultiplicationOfTwoIntegers(int, int, int*);
        HRESULT __stdcall DivisionOfTwoIntegers(int, int, int*);

        //custom method for inner component creation
        HRESULT __stdcall InitializeInnerComponent(void);
};

class CSumSubtractClassFactory:public IClassFactory
{
    private:
        long m_cRef;
    public:
        CSumSubtractClassFactory(void);
        ~CSumSubtractClassFactory(void);

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

//Implementation of CSumSubtract
CSumSubtract::CSumSubtract(void)
{
    //code
    m_pIMultiplication = NULL;
    m_pIDivision = NULL;
    m_cRef = 1;
    InterlockedIncrement(&glNumberOfActiveComponents);
}

CSumSubtract::~CSumSubtract(void)
{
    //code
    InterlockedDecrement(&glNumberOfActiveComponents);
    if(m_pIMultiplication)
    {
        m_pIMultiplication->Release();
        m_pIMultiplication = NULL;
    }

    if(m_pIDivision)
    {
        m_pIDivision->Release();
        m_pIDivision = NULL;
    }
}

HRESULT CSumSubtract::QueryInterface(REFIID riid, void **ppv)
{
    if(riid == IID_IUnknown)
        *ppv = static_cast<ISum*>(this);
    else if(riid == IID_ISum)
        *ppv = static_cast<ISum*>(this);
    else if(riid == IID_ISubtract)
        *ppv = static_cast<ISubtract*>(this);
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

HRESULT CSumSubtract::MultiplicationOfTwoIntegers(int num1, int num2, int *pMultiplication)
{
    //code
    m_pIMultiplication->MultiplicationOfTwoIntegers(num1, num2, pMultiplication);
    return (S_OK);
}

HRESULT CSumSubtract::DivisionOfTwoIntegers(int num1, int num2, int *pDivision)
{
    //code
    m_pIDivision->DivisionOfTwoIntegers(num1, num2, pDivision);
    return (S_OK);
}

HRESULT CSumSubtract::InitializeInnerComponent(void)
{
    //variable declarations
    HRESULT hr;

    //code
    hr = CoCreateInstance(CLSID_MultiplicationDivision, NULL, CLSCTX_INPROC_SERVER, IID_IMultiplication, (void**)&m_pIMultiplication);
    if(FAILED(hr))
    {
        MessageBox(NULL, TEXT("IMultiplication Interface Can Not Be Obtained From Inner Component."), TEXT("Error"), MB_OK);
        return (E_FAIL);
    }

    hr = m_pIMultiplication->QueryInterface(IID_IDivision, (void**)&m_pIDivision);
    if(FAILED(hr))
    {
        MessageBox(NULL, TEXT("IDivision Interface Can Not Be Obtained From Inner Component."), TEXT("Error"), MB_OK);
        return (E_FAIL);
    }

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

HRESULT CSumSubtractClassFactory::QueryInterface(REFIID riid, void **ppv)
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

HRESULT CSumSubtractClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
    //variable declarations
    CSumSubtract *pCSumSubtract = NULL;
    HRESULT hr;

    //code
    MessageBox(NULL, TEXT("Inside Outer CreateInstance"), TEXT("Error"), MB_OK);
    if(pUnkOuter != NULL)
        return (CLASS_E_NOAGGREGATION);

    pCSumSubtract = new CSumSubtract;
    if(pCSumSubtract == NULL)
        return (E_OUTOFMEMORY);

    hr = pCSumSubtract->InitializeInnerComponent();
    if(FAILED(hr))
    {
        MessageBox(NULL, TEXT("Failed To Initialize Inner Component"), TEXT("Error"), MB_OK);
        pCSumSubtract->Release();
        return (hr);
    }

    hr = pCSumSubtract->QueryInterface(riid, ppv);
    pCSumSubtract->Release();
    return (hr);
}

HRESULT CSumSubtractClassFactory::LockServer(BOOL fLock)
{
    //code
    if(fLock)
        InterlockedIncrement(&glNumberOfServerLocks);
    else 
        InterlockedDecrement(&glNumberOfServerLocks);

    return (S_OK);
}

HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
    //variable declarations
    CSumSubtractClassFactory *pCSumSubtractClassFactory = NULL;
    HRESULT hr;

    //code
    MessageBox(NULL, TEXT("Inside DllGetClassObject()"), TEXT("Error"), MB_OK);
    if(rclsid != CLSID_SumSubtract)
        return (CLASS_E_CLASSNOTAVAILABLE);

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

