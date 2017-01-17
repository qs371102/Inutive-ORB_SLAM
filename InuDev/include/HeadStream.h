/*
* File - HeadStream.h
*
* This file is part of the Inuitive SDK
*
* Copyright (C) 2014 All rights reserved to Inuitive  
*
*/

#ifndef __HEADSTREAM_H__
#define __HEADSTREAM_H__

#include "StreamsExport.h"

#include "BaseStream.h"
#include <functional>


namespace InuDev 
{
    ////////////////////////////////////////////////////////////////////////
    /// \brief  Represents frames that are provided by CHeadStream
    ////////////////////////////////////////////////////////////////////////
    struct CHeadFrame : public CBaseFrame
    {
        /// \brief    Rotation represented by quaternion (3 axes + amount of rotation) 
        CPoint4D Quaternion;

        // \brief    Rotation angles in Radians
        CPoint3D RotationAngles;

        // \brief    Head location (nose tip) in mm, coordinates in 3D space  
        CPoint3D Translation3D;

        // \brief    Head location (nose tip) in pixels, coordinates in image frame
        CPoint2D Translation2D;
    };


    ////////////////////////////////////////////////////////////////////////
    /// \brief   Interface for Head service.
    /// 
    /// Role: Controls Head streaming service and provides Head frames. 
    ///
    /// Responsibilities: 
    ///      1. Derives CBaseStream interface
    ///      2. Knows how to acquire one Head frame (pull)
    ///      3. Knows how to provide a continuous stream of Head frames (push)
    ///
    ////////////////////////////////////////////////////////////////////////
    class CHeadStream : public CBaseStream
    {
    public:

        /// \brief    Prototype of callback function which is used by the Register method.
        ///
        /// This function is invoked any time a frame is ready, or if an error occurs. The parameters of this function are:
        /// Caller stream object, received Head frame and result code.
        typedef std::function<void(std::shared_ptr<CHeadStream>, const CHeadFrame&,  CInuError)> CallbackFunction;

        virtual ~CHeadStream() {}

        /// \brief    Retrieve one Head frame (pull)
        /// 
        /// This method returns when a new frame is ready (blocking) or if an input timeout has elapsed. 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        /// \param[out] oHeadFrame    The returned Head frame.
        /// \param[in] iTimeout    Function is returned if timeout has elapsed even if no new frame is ready.
        /// \return CInuError    Error code, InDev::eOK if operation successfully completed.
        virtual CInuError GetFrame(CHeadFrame& oHeadFrame, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

        /// \brief    Registration/Unregistration for receiving Head frames (push) 
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
