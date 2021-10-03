//interface
class ISum:public IUnknown
{
    public:
        //ISum specific method declarations 
        virtual HRESULT __stdcall SumOfTwoIntegers(int, int, int*) = 0;
};

class ISubtract:public IUnknown
{
    public:
        //ISubtract specific method declarations
        virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*) = 0;
};

//CLSID of ISumSubtract Component {0DAFBEAF-48D2-4BF7-93E3-9346E9771D80}
const CLSID CLSID_SumSubtract = {0xdafbeaf, 0x48d2, 0x4bf7, 0x93, 0xe3, 0x93, 0x46, 0xe9, 0x77, 0x1d, 0x80};

//IID of ISum Interface {24F86022-DA6C-4DD7-8647-A1152CAE6552}
const IID IID_ISum = {0x24f86022, 0xda6c, 0x4dd7, 0x86, 0x47, 0xa1, 0x15, 0x2c, 0xae, 0x65, 0x52};

//IID of ISubtract Interface {DA436042-E9B1-4B2E-8E83-3F88002B1D61}
const IID IID_ISubtract = {0xda436042, 0xe9b1, 0x4b2e, 0x8e, 0x83, 0x3f, 0x88, 0x0, 0x2b, 0x1d, 0x61};
