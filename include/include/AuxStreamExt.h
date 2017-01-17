
#ifndef __AUXSTREAMEXT_H__
#define __AUXSTREAMEXT_H__

#include "AuxStream.h"
#include "BaseStreamExt.h"

namespace InuDev
{

    class CAuxStreamExt : public CAuxStream, public CBaseStreamExt
    {

    public:
        using CAuxStream::Init;
        using CAuxStream::GetFrame;

        CAuxStreamExt() {}
        virtual ~CAuxStreamExt() {}

    protected:


    private:

    };
}

#endif // __AUXSTREAMEXT_H__

