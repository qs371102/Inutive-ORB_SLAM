
#ifndef __WEBCAMSTREAMEXT_H__
#define __WEBCAMSTREAMEXT_H__

#include "WebCamStream.h"
#include "ImageFrameExt.h"
#include "BaseStreamExt.h"

namespace InuDev
{

     ////////////////////////////////////////////////////////////////////////
    /// \brief WebCamStream extension class 
    ///
    ///
    /// Role: Adds extended features to the WebCam interface
    ///       Intended for internal use
    ///
    ///
    /// Responsibilities: 
    ///      1. Add extended API interface
    ///
    /// Comments: 
    ////////////////////////////////////////////////////////////////////////
    class CWebCamStreamExt : public CWebCamStream, public CBaseStreamExt
    {

    public:

        typedef std::function<void(std::shared_ptr<CWebCamStreamExt>, const CImageFrame&,  CInuError)> CallbackFunctionExt;

        CWebCamStreamExt() {}
        virtual ~CWebCamStreamExt() {}

        using CWebCamStream::Init;
       
        //Extended methods
        virtual CInuError GetFrame(CImageFrame& oWebcamFrame, unsigned int iTimeout = FPS_BASED_TIMEOUT, bool iCopyLocal = true) = 0;
        virtual CInuError Register(CallbackFunctionExt iCallback, unsigned int iTimeout = FPS_BASED_TIMEOUT, bool iCopyLocal = true) = 0;

    protected:


    private:

    };
}

#endif // __DEPTHSTREAMEXT_H__

