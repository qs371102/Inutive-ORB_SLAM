/*
* File - IAFFrames.h
*
* This file is part of Inuitive API Framework  and it defines base class of all provided frames
*
*/


#ifndef __BASEFRAMEEXT_H__
#define __BASEFRAMEEXT_H__

namespace InuDev
{

    ////////////////////////////////////////////////////////////////////////
    /// \brief    Common behavior of all IAF NUI frames. 
    ///
    /// Role: Base class of all InuStreams' frames.
    ///
    /// Responsibilities: 
    ///      1. Common members. 
    ///      2. Construction is enabled only from derived classes.
    ///
    ////////////////////////////////////////////////////////////////////////
    class  CBaseFrameExt 
    {
    public:
        /// \brief    true if this frame was recorded by InuService
        bool WasRecorded; 
        
        CBaseFrameExt() : WasRecorded(false),ServiceTimestamp(0),StreamTimestamp(0) {}
        
        /// \brief    Frame receive time in nanoseconds relates to host's system clock.
        ///           It should be used for frames synchronization. 
        uint64_t ServiceTimestamp;
        uint64_t StreamTimestamp;
        
        virtual ~CBaseFrameExt() {}
    };
}

#endif // __BASEFRAMEEXT_H__
