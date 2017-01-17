/*
* File - HandsStream.h
*
* This file is part of the Inuitive SDK
*
* Copyright (C) 2014 All rights reserved to Inuitive  
*
*/



#ifndef __HANDSSTREAM_H__
#define __HANDSSTREAM_H__

#include "StreamsExport.h"

#include "BaseStream.h"
#include <functional>


namespace InuDev 
{

    ////////////////////////////////////////////////////////////////////////
    /// \brief  Represents one Hand data which is part of the Hands frame 
    ////////////////////////////////////////////////////////////////////////
    struct CHandData 
    {
        /// \brief    Default constructor
        CHandData() : FingersTip() , PalmCenter(0.0f,0.0f,0.0f), Side(eNotValid)
        {
            for (unsigned int i=0; i<eNumOfFingers ; i++)
            {
                FingersTip[EFingers(i)] = CPoint3D(0.0f,0.0f,0.0f);
            }
        }

        /// \brief    Fingers identification
        enum EFingers
        { 
            eThumb = 0, 
            eGromming, 
            eMiddle,
            eRing, 
            eLittle,
            eNumOfFingers
        }; 

        /// \brief    Side of hand, eNotValid if hands data is not valid and can't be used  
        enum ESide
        {
            eLeft = 0,
            eRight, 
            eUnrecognized,          
            eNotValid               // hands data is not valid and can't be used 
        }; 

        /// \brief    3D position of each finger tip (in mm). (0,0,0) indicates that a finger is not recognized. 
        CPoint3D FingersTip[eNumOfFingers];

        /// \brief    3D position palm center (in mm). (0,0,0) indicates that a palm is not recognized. 
        CPoint3D PalmCenter;

        /// \brief    Hand side  
        ESide Side;
    };


    ////////////////////////////////////////////////////////////////////////
    /// \brief  Represents frames that are provided by CHandsStream
    ////////////////////////////////////////////////////////////////////////
    struct CHandsFrame : public CBaseFrame
    {
        /// \brief    Data for two hands  
        CHandData Hands[2];
    };


    ////////////////////////////////////////////////////////////////////////
    /// \brief   Interface for Hands service.
    /// 
    /// Role: Controls Hands streaming service and provides Hands frames. 
    ///
    /// Responsibilities: 
    ///      1. Derives CBaseStream interface
    ///      2. Knows how to acquire one Hands frame (pull)
    ///      3. Knows how to provide a continuous stream of Hands frames (push)
    ///
    ////////////////////////////////////////////////////////////////////////
    class CHandsStream : public CBaseStream
    {
    public:

        /// \brief    Prototype of callback function which is used by the Register method.
        ///
        /// This function is invoked any time a frame is ready, or if an error occurs. The parameters of this function are:
        /// Caller stream object, received Hands frame and result code.
        typedef std::function<void(std::shared_ptr<CHandsStream>, const CHandsFrame&,  CInuError)> CallbackFunction;

        virtual ~CHandsStream() {}

        /// \brief    Retrieve one Hands frame (pull)
        /// 
        /// This method returns when a new frame is ready (blocking) or if an input timeout has elapsed. 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        /// \param[out] oHandsFrame    The returned Hands frame.
        /// \param[in] iTimeout    Function is returned if timeout has elapsed even if no new frame is ready.
        /// \return CInuError    Error code, InDev::eOK if operation successfully completed.
        virtual CInuError GetFrame(CHandsFrame& oHandsFrame, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

        /// \brief    Registration/Unregistration for receiving Hands frames (push) 
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

#endif // __HANDSSTREAM_H__
