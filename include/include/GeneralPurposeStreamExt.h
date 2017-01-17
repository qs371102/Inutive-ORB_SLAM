#ifndef __GENERALPURPOSESTREAMEXT_H__  
#define __GENERALPURPOSESTREAMEXT_H__

#include "GeneralPurposeStream.h"
#include "BaseStreamExt.h"

namespace InuDev
{

    class CGeneralPurposeStreamExt : public CGeneralPurposeStream, public CBaseStreamExt
    {

    public:
       // using CGeneralPurposeStreamExt::WasRecorded=true;
        using CGeneralPurposeStream::Init;
        using CGeneralPurposeStream::GetFrame;

        CGeneralPurposeStreamExt() { }
        virtual ~CGeneralPurposeStreamExt() {}

    protected:


    private:

    };
}

#endif // __GENERALPURPOSESTREAMEXT_H__

