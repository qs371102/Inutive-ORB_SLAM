/*
* File - BaseFrames.h
*
* This file is part of the Inuitive SDK 
*
* Copyright (C) 2014 All rights reserved to Inuitive  
*
*/


#ifndef __BASEFRAME_H__
#define __BASEFRAME_H__

#include "StreamsExport.h"

#include "InuDefs.h"

#include <time.h> 
#include <string>
#include <vector>
#include <stdint.h>

namespace InuDev
{

    ////////////////////////////////////////////////////////////////////////
    /// \brief    Common behavior of all InuDev NUI frames. 
    ///
    /// Role: Base class of all InuStreams' frames.
    ///
    /// Responsibilities: 
    ///      1. Common members. 
    ///      2. Construction is enabled only from derived classes.
    ///
    ////////////////////////////////////////////////////////////////////////
    class  CBaseFrame 
    {
    public:
        // Upper and lower limits of Score 
        static const unsigned int SCORE_MIN=0;
        static const unsigned int SCORE_MAX=100;

        /// \brief    Frame acquisition time in nanoseconds relates to host's system clock.
        ///           It should be used for frames synchronization. 
        uint64_t Timestamp;

        /// \brief    Frame acquisition unique index, should be used for synchronization. 
        unsigned int FrameIndex;

        /// \brief    Indicates if this frame is valid data or not. 
        bool Valid;

        /// \brief    Confidence of current frame; range is from SCORE_MIN up to SCORE_MAX.
        unsigned int Score; 

        /// \brief    Unique ID of the user who is the source of this NUI frame. String is empty when user is not detected. 
        std::string UserID;

        virtual ~CBaseFrame() {}

    protected:

        /// \brief    Constructor
        /// \param[in] iTime    Frame acquisition time.
        /// \param[in] iFrameIndex    Frame acquisition unique index.
        /// \param[in] iScore    Confidence.
        /// \param[in] iValid    Validity bit.
        /// \param[in] iUserID    Unique User ID.
        CBaseFrame(uint64_t iTime = 0,
                   unsigned int iFrameIndex = 0,
                   unsigned int iScore = SCORE_MAX,
                   bool iValid = false,
                   std::string iUserID = std::string()) : 
            Timestamp(iTime),
            FrameIndex(iFrameIndex),
            Score(SCORE_MAX), 
            Valid(iValid),
            UserID(iUserID) {}
    };
}

#endif // __BASEFRAME_H__
