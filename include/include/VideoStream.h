/*
* File - VideoStream.h
*
* This file is part of the Inuitive SDK
*
* Copyright (C) 2014 All rights reserved to Inuitive  
*
*/

#ifndef __VIDEOSTREAM_H__
#define __VIDEOSTREAM_H__

#include "StreamsExport.h"

#include "ImageFrame.h"

#include <functional>

namespace InuDev 
{

    ////////////////////////////////////////////////////////////////////////
    /// \brief    Video image frame. 
    ///
    /// Role: Represents Video image which is comprised of left and right sensor images.
    ///
    /// Responsibilities: 
    //      1. Access to Left and Right frames separately
    ///
    //////////////////////////////////////////////////////////////////////// 
    class CVideoFrame : public CBaseFrame
    {
    public:

        /// \brief    Constructs zero size left and right images
        INUSTREAMS_API CVideoFrame();

        /// \brief    Constructor.
        /// 
        /// \param[in] iWidth    Image width.
        /// \param[in] iHeight    Image height.
        /// \param[in] iFormat    Image Format.
        INUSTREAMS_API CVideoFrame(unsigned int iWidth, unsigned int iHeight, unsigned int iImageWidth, unsigned int iImageHeight, unsigned short iStartX, unsigned short iStartY, unsigned int  iFormat); 

        /// \brief    Copy constructor
        INUSTREAMS_API CVideoFrame(const CVideoFrame& input);

        /// \brief    Move constructor
        INUSTREAMS_API CVideoFrame(const CVideoFrame&& input);

        /// \brief    Assignment operator
        INUSTREAMS_API CVideoFrame& operator=(const CVideoFrame& input);

        /// \brief    move operator
        INUSTREAMS_API CVideoFrame& operator=(const CVideoFrame&& input);


        INUSTREAMS_API virtual ~CVideoFrame();

        const CImageFrame*   GetLeftFrame() const  {  return mLeftFrame;  }
        CImageFrame*         GetLeftFrame()        {  return mLeftFrame;  }

        const CImageFrame*   GetRightFrame() const {  return mRightFrame;  }
        CImageFrame*         GetRightFrame()       {  return mRightFrame;  }

    protected:

        /// \brief    Image received from left sensor. Equals to nullptr if left Sensor's image is not available.
        CImageFrame*  mLeftFrame;

        /// \brief    Image received from right sensor. Equals to nullptr if right Sensor's image is not available.
        CImageFrame*  mRightFrame;
    };


    ////////////////////////////////////////////////////////////////////////
    /// \brief   Interface for Video service.
    /// 
    /// Role: Controls video images streaming service and provides video image frames. 
    ///
    /// Responsibilities: 
    ///      1. Derives CBaseStream interface
    ///      2. Knows how to acquire one video image frame (pull)
    ///      3. Knows how to provide a continuous stream of video image frames (push)
    /// 
    ////////////////////////////////////////////////////////////////////////
    class CVideoStream : public CBaseStream
    {
    public:

        /// \brief    Prototype of callback function which is used by the Register method.
        ///
        /// This function is invoked any time a frame is ready, or if an error occurs. The parameters of this function are:
        /// Caller stream object, received video frame and result code.
        typedef std::function<void(std::shared_ptr<CVideoStream>, const CVideoFrame&,  CInuError)> CallbackFunction;

        virtual ~CVideoStream() {}

        /// \brief    Retrieves new video image frame (pull) 
        /// 
        /// This method returns when a new frame is ready (blocking) or if an input timeout has elapsed. 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        /// \param[out] oVideoFrame    The returned video frame (Z-buffer).
        /// \param[in] iTimeout    Function is returned if timeout has elapsed even if no new frame is ready.
        /// \return CInuError    Error code, InDev::eOK if operation successfully completed.
        virtual CInuError GetFrame(CVideoFrame& oVideoFrame, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

        /// \brief    Registration/Unregistration for receiving stream of video image frames (push) 
        /// 
        /// The provided callback function is called when a new frame is ready (non-blocking). 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        /// \param[in] iCallback    Callback function which is invoked when a new frame is ready.
        ///                         Send nullptr to unregister for receiving frames.  
        /// \param[in] iTimeout    Callback is invoked if a timeout has elapsed even if no new frame is ready.
        /// \return CInuError    Error code, InDev::eOK if operation successfully completed.
        virtual CInuError Register(CallbackFunction iCallback, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

    };
}

#endif
