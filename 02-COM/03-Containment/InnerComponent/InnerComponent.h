//Interfaces 
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

//CLSID of MultiplicationDivision Component {2A2D52A6-91D6-40FF-BF16-C7124AE71E01}
const CLSID CLSID_MultiplicationDivision = {0x2a2d52a6, 0x91d6, 0x40ff, 0xbf, 0x16, 0xc7, 0x12, 0x4a, 0xe7, 0x1e, 0x1};

//IID of IMultiplication Interface
const IID IID_IMultiplication = {0x897dd7f8, 0x9c59, 0x462b, 0xa0, 0x2a, 0x54, 0xcf, 0x4a, 0x27, 0xcf, 0x1e};

//IID of IDivision Interface
const IID IID_IDivision = {0xa639d256, 0xf368, 0x41d5, 0x9f, 0x66, 0x44, 0xd3, 0xad, 0xd3, 0xd8, 0xac};

