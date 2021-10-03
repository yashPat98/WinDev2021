//Interfaces
class ISum:public IUnknown
{
    public:
        virtual HRESULT __stdcall SumOfTwoIntegers(int, int, int*) = 0;
};

class ISubtract:public IUnknown
{
    public:
        virtual HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int*) = 0;
};

class IMultiplication:public IUnknown
{
    public:
        virtual HRESULT __stdcall MultiplicationOfTwoIntegers(int, int, int*) = 0;
};

class IDivision:public IUnknown
{
    public:
        virtual HRESULT __stdcall DivisionOfTwoIntegers(int, int, int*) = 0;
};

//CLSID of SumSubtract Component {6805C5FA-3FBA-4B8E-B402-9944CCBA56AE}
const CLSID CLSID_SumSubtract = {0x6805c5fa, 0x3fba, 0x4b8e, 0xb4, 0x2, 0x99, 0x44, 0xcc, 0xba, 0x56, 0xae};

//IID of ISum Interface
const IID IID_ISum = {0x2c29ee4e, 0x83d7, 0x4034, 0xb5, 0x88, 0x89, 0x8c, 0x4c, 0x9f, 0x9e, 0x8b};

//IID of ISubtract Interface
const IID IID_ISubtract = {0x2239fc58, 0x6d8d, 0x4dbf, 0xac, 0x35, 0x0, 0x39, 0x7b, 0x31, 0x79, 0x1e};

//IID of IMultiplication Interface
const IID IID_IMultiplication = {0x897dd7f8, 0x9c59, 0x462b, 0xa0, 0x2a, 0x54, 0xcf, 0x4a, 0x27, 0xcf, 0x1e};

//IID of IDivision Interface
const IID IID_IDivision = {0xa639d256, 0xf368, 0x41d5, 0x9f, 0x66, 0x44, 0xd3, 0xad, 0xd3, 0xd8, 0xac};
