/*
* File - GazeStream.h
*
* This file is part of the Inuitive SDK
*
* Copyright (C) 2014 All rights reserved to Inuitive  
*
*/

#ifndef __GAZESTREAM_H__
#define __GAZESTREAM_H__

#include "StreamsExport.h"

#include "BaseStream.h"

#include <functional>

namespace InuDev 
{

    ////////////////////////////////////////////////////////////////////////
    /// \brief  Represents frames that are provided by CGazeStream
    ////////////////////////////////////////////////////////////////////////
    struct CGazeFrame : public CBaseFrame 
    {
        enum ECalibrationState
        {
            eNotCalibrated,
            eInCalibration,
            eCalibrated
        };

        ECalibrationState       CalibrationState;

        /// \brief    Point on screen where user is looking at.
        CPoint2D GazeCoord;
    };
 

    ////////////////////////////////////////////////////////////////////////
    /// \brief   Interface for Gaze service.
    /// 
    /// Role: Controls Gaze streaming service and provides Gaze frames. 
    ///
    /// Responsibilities: 
    ///      1. Derives CBaseStream interface
    ///      2. Knows how to acquire one Gaze frame (pull)
    ///      3. Knows how to provide a continuous stream of Gaze frames (push)
    ///
    ////////////////////////////////////////////////////////////////////////
    class CGazeStream : public CBaseStream
    {

    public:

        /// \brief    Prototype of callback function which is used by the Register method.
        ///
        /// This function is invoked any time a frame is ready, or if an error occurs. The parameters of this function are:
        /// Caller stream object, received Gaze frame and result code.
        typedef std::function<void(std::shared_ptr<CGazeStream>, const CGazeFrame&,  CInuError)> CallbackFunction;

        virtual ~CGazeStream() {}

        /// \brief    Retrieve one gaze frame (pull)
        /// 
        /// This method returns when a new frame is ready (blocking) or if an input timeout has elapsed. 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        /// \param[out] oGazeFrame    The returned Gaze frame.
        /// \param[in] iTimeout    Function is returned if timeout has elapsed even if no new frame is ready.
        /// \return CInuError    Error code, InDev::eOK if operation successfully completed.
        virtual CInuError GetFrame(CGazeFrame& oGazeFrame, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

        /// \brief    Registration/Unregistration for receiving Gaze frames (push) 
        /// 
        /// The provided callback function is called when a new frame is ready (non-blocking). 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        /// \param[in] iCallback    Callback function which is invoked when a new frame is ready.
        ///                         Send nullptr to unregister for receiving frames.  
        /// \param[in] iTimeout    Callback is invoked if a timeout has elapsed even if no new frame is ready.
        /// \return CInuError    Error code, InDev::eOK if operation successfully completed.
        virtual CInuError Register(CallbackFunction iCallback, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;


        /// \brief    Start gaze position calibration 
        /// 
        /// Starts the process of calibrating the gaze screen position
        /// \return CInuError    Error code, InDev::eOK if operation successfully completed.
        virtual CInuError StartCalibration() = 0;

    };

}

#endif // __GAZESTREAM_H__
