
#ifndef __GENERALSTREAMEXT_H__
#define __GENERALSTREAMEXT_H__

#include "GeneralFrame.h"
#include "BaseFrameExt.h"

namespace InuDev
{

    class CGeneralFrameExt : public CGeneralFrame, public CBaseFrameExt
    {
    public:
        INUSTREAMS_API CGeneralFrameExt (uint32_t bufferSize) : CGeneralFrame(bufferSize) {}
        INUSTREAMS_API CGeneralFrameExt () : CGeneralFrame() {}

        /// \brief    Copy constructor - deep Copy.
        INUSTREAMS_API CGeneralFrameExt(const CGeneralFrameExt& input) : CGeneralFrame(input) {}

        //assignment operator
        INUSTREAMS_API CGeneralFrameExt& operator=(const CGeneralFrameExt& input) {
        	CGeneralFrame::operator=(input);
            return *this;
        }

        //move assignment operator
        INUSTREAMS_API CGeneralFrameExt& operator=(CGeneralFrameExt&& input) {
        	CGeneralFrame::operator=(std::move(input));
            return *this;
        }

        /// \brief    Move constructor. 
        INUSTREAMS_API CGeneralFrameExt(CGeneralFrame&& input) : CGeneralFrame(input) {}
        virtual ~CGeneralFrameExt() {}
    };
}

#endif // __GENERALSTREAMEXT_H__

