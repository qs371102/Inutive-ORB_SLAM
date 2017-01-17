/*
* File - WebCamStream.h
*
* This file is part of the Inuitive SDK
*
* Copyright (C) 2014 All rights reserved to Inuitive  
*
*/

#ifndef __WEBCAMSTREAM_H__
#define __WEBCAMSTREAM_H__

#include "StreamsExport.h"

#include "ImageFrame.h"

#include <functional>

namespace InuDev 
{

    ////////////////////////////////////////////////////////////////////////
    /// \brief   Interface for Web cam service.
    /// 
    /// Role: Controls web cam images streaming service and provides web cam image frames. 
    ///
    /// Responsibilities: 
    ///      1. Derives CBaseStream interface
    ///      2. Knows how to acquire one web cam image frame (pull)
    ///      3. Knows how to provide a continuous stream of web cam image frames (push)
    /// 
    ////////////////////////////////////////////////////////////////////////
    class CWebCamStream : public CBaseStream
    {
    public:

        enum EOutputType
        {
            eDefault= 0,
            eRegisteredWebcam = 1,
            eRectifiedWebcam = 2,
            eRaw = 3
        };

        /// \brief    Prototype of callback function which is used by the Register method.
        ///
        /// This function is invoked any time a frame is ready, or if an error occurs. The parameters of this function are:
        /// Caller stream object, received web cam frame and result code.
        typedef std::function<void(std::shared_ptr<CWebCamStream>, const CImageFrame&,  CInuError)> CallbackFunction;

        virtual ~CWebCamStream() {}

        /// \brief    Retrieves new web cam image frame (pull) 
        /// 
        /// This method returns when a new frame is ready (blocking) or if an input timeout has elapsed. 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        /// \param[out] oWebCamFrame    The returned web cam frame (Z-buffer).
        /// \param[in] iTimeout    Function is returned if timeout has elapsed even if no new frame is ready.
        /// \return CInuError    Error code, InDev::eOK if operation successfully completed.
        virtual CInuError GetFrame(CImageFrame& oWebCamFrame, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

        /// \brief    Registration/Unregistration for receiving stream of web cam image frames (push) 
        /// 
        /// The provided callback function is called when a new frame is ready (non-blocking). 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        /// \param[in] iCallback    Callback function which is invoked when a new frame is ready.
        ///                         Send nullptr to unregister for receiving frames.  
        /// \param[in] iTimeout    Callback is invoked if a timeout has elapsed even if no new frame is ready.
        /// \return CInuError    Error code, InDev::eOK if operation successfully completed.
        virtual CInuError Register(CallbackFunction iCallback, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

        /// \brief    Don't mask the default init function (which disables registration by default)
        using CBaseStream::Init;

        /// \brief    Overwrite the default Initialization method
        ///
        /// It enables to activate registration between Webcam and Depth. 
        /// It must be set to true in order to activate registration by SetParams method.
        /// The side effect of enabling registration is that Depth stream will be automatically started whenever Webcam stream is started. 
        /// \param[in] bool       True to enable registration.
        /// \return CInuError  

        virtual CInuError Init(EOutputType iOutputType) =0;
    };
}

#endif
