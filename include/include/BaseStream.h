/*
* File - BaseStream.h
*
* This file is part of the Inuitive SDK
*
* Copyright (C) 2014 All rights reserved to Inuitive  
*
*/

#ifndef __BASESTREAM_H__
#define __BASESTREAM_H__

#include "BaseFrame.h"
#include <memory>

namespace InuDev 
{
    /// \brief    Stream's state is always one of EStreamState
    enum EStreamState
    {
        eUninitialized,      
        eInitialized,
        eStarted,
        eSensorDisconnected       /// Sensor is disconnected               
    };

    ////////////////////////////////////////////////////////////////////////
    /// \brief   Common interface for all InuDev NUI streams.
    /// 
    /// Role: Base class for all streams classes. It defines an interface that should be implemented by all derived classes.  
    ///
    /// Responsibilities: 
    ///      1. Knows how to control the service (Init, Terminate, Start and Stop)
    ///      2. Implements a state machine (more details in Reference Manual)
    ///      3. Controls the frame acquisition time out
    ////////////////////////////////////////////////////////////////////////
    class CBaseStream
    {
    public:

        /// \brief    Default timeout for frames acquisition.
        ///
        /// Default is FPS based, which means that timeout is derived from the configured number of frames per second 
        static const unsigned int FPS_BASED_TIMEOUT=0;

        virtual ~CBaseStream() {}

        /// \brief    Service initialization.
        /// 
        /// Shall be invoked once before starting frames acquisition.
        /// \return CInuError    Operation status which indicates on success or failure.
        virtual CInuError Init() = 0;

        /// \brief    Service termination. 
        /// 
        /// Shall be invoked when the service is no longer in use and after frames acquisition has stopped.
        /// \return CInuError    Operation status which indicates success or failure. 
        virtual CInuError Terminate() = 0;

        /// \brief    Start acquisition of frames.
        /// 
        /// Shall be invoked only after the service is successfully initialized and before any request
        /// for new frame (push or pull).
        /// \param[in] iUserID    Unique ID of the user who should be tracked.
        /// \return CInuError    Operation status. 
        virtual CInuError Start(const std::string& iUserID = std::string()) = 0;

        /// \brief    Stop acquisition of frames. 
        /// 
        /// Shall be invoked after requests for frames are no longer sent and before service termination
        /// (only if Start() was invoked).
        /// \return CInuError    Operation status which indicates success or failure. 
        virtual CInuError Stop() = 0;

        /// \brief    brief Set the default timeout for frames acquisition.
        /// 
        /// If this timeout is elapsed then the frame acquisition method returns with timeout error
        /// \param[in] iTimeout    requested timeout.
        /// \return void    description.
        virtual void SetFrameTimeout(unsigned int iTimeout) = 0;

        /// \brief    Stream state information. 
        /// \return EStreamState    Stream's current state.
        virtual EStreamState State() const = 0;

        /// \brief    Stream's channel ID 
        /// \return uint8_t   The channel ID that was assigned when the 
        virtual uint32_t ChannelID() const = 0;
    };
}

#endif // __BASESTREAM_H__
