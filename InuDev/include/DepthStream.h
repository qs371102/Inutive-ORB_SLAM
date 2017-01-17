/*
* File - DepthStream.h
*
* This file is part of the Inuitive SDK
*
* Copyright (C) 2014 All rights reserved to Inuitive  
*
*/


#ifndef __DEPTHSTREAM_H__
#define __DEPTHSTREAM_H__

#include "ImageFrame.h"
#include "BaseStream.h"
#include <functional>

namespace InuDev 
{

    ///////////////////////////////////////////////////////////////////////
    /// \brief    All different Post Preprocessing algorithms.   
    ///////////////////////////////////////////////////////////////////////
    enum EPostProcessingMode
    {
        eDisabled,          ///<  No post processing algorithm is executed (best runtime)
        eCropOnlyMode,      ///<  Crops edges and disparities (very fast)     
        eFastMode,          ///<  Removes singular points (fast)
        eSmoothMode,        ///<  Removes singular points + deblocking filter (moderated runtime performance)
        eExtraSmoothMode    ///<  Global disparity erosion, this is the recommended mode
    };

    ////////////////////////////////////////////////////////////////////////
    /// \brief   Interface for Depth service.
    /// 
    /// Role: Controls depth images streaming service and provides depth image frames. 
    ///
    /// Responsibilities: 
    ///      1. Derives CBaseStream interface
    ///      2. Knows how to acquire one depth image frame (pull)
    ///      3. Knows how to provide a continuous stream of depth image frames (push)
    /// 
    ////////////////////////////////////////////////////////////////////////
    class CDepthStream : public CBaseStream
    {
    public:

        enum EOutputType
        {
            eDepth  = 0,
            eDisparity = 1,
            ePointCloud = 2,
            eRGB = 3,
            eDepthRegistration = 4,
            eRaw = 5
        };

        /// \brief    Prototype of callback function which is used by the Register method.
        ///
        /// This function is invoked any time a frame is ready, or if an error occurs. The parameters of this function are:
        /// Caller stream object, received depth frame and result code.
        typedef std::function<void(std::shared_ptr<CDepthStream> , const CImageFrame&,  CInuError)> CallbackFunction;

        virtual ~CDepthStream() {}

        /// \brief    Derived interface, Init without initial parameters
        using CBaseStream::Init;

        /// \brief    Service initialization.
        /// 
        /// Shall be invoked once before starting frames acquisition.
        /// \param[in] iWithConfidence    Acquire depth frames with confidence.
        /// \return InuDev::CInuError    Error code, InDev::eOK if operation successfully completed.
        virtual CInuError         Init(bool iWithConfidence) = 0;
        virtual CInuError         Init(EOutputType iOutputType,bool iWithConfidence = false) = 0;

        /// \brief    Retrieves new depth image frame (pull) 
        /// 
        /// This method returns when a new frame is ready (blocking) or if an input timeout has elapsed. 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        /// \param[out] oDepthFrame    The returned depth frame (Z-buffer).
        /// \param[in] iTimeout    Function is returned if timeout has elapsed even if no new frame is ready.
        /// \return CInuError    Error code, InDev::eOK if operation successfully completed.
        virtual CInuError  GetFrame(CImageFrame& oDepthFrame, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

        /// \brief    Registration/Unregistration for receiving stream of depth image frames (push) 
        /// 
        /// The provided callback function is called when a new frame is ready (non-blocking). 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        /// \param[in] iCallback    Callback function which is invoked when a new frame is ready. 
        ///                         Send nullptr to unregister for receiving frames.  
        /// \param[in] iTimeout    Callback is invoked if a timeout has elapsed even if no new frame is ready.
        /// \return CInuError    Error code, InDev::eOK if operation successfully completed.
        virtual CInuError Register(CallbackFunction iCallback, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

        /// \brief    Set Depth generation mode. 
        /// 
        /// It enables to set which post processing algorithm is invoked when calculating depth from disparity.
        /// There is a trade off between depth quality and performance and each application can select its preferable mode.
        /// All different modes are described in EPostProcessingMode declaration. 
        /// \param[in] iPostProcessingMose    Requested post processing mode.
        /// \return CInuError    Error code, InDev::eOK if operation successfully completed.
        virtual CInuError SetPostProcess(EPostProcessingMode iPostProcessingMose) = 0;
    };
}

#endif
