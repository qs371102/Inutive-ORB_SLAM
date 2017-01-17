/*
* File - DepthStream.h
*
* This file is part of Inuitive API Framework  and it contains declaration of class CDepthImage.
*
*/

#ifndef __BASESTREAMEXT_H__
#define __BASESTREAMEXT_H__

#include "InuError.h"
#include <limits>
#include <stdint.h>


namespace InuDev 
{
    ////////////////////////////////////////////////////////////////////////
    /// \brief   Interface for Depth service.
    /// 
    /// Role: Control depth images streaming service and provides depth image frames. 
    ///
    /// Responsibilities: 
    ///      1. Knows how to control the service (Init, Terminate, Start and Stop).
    ///      2. Knows how to acquire one depth image frame (pull)
    ///      3. Knows how to provide a continuous stream of depth image frames (push)
    /// 
    ////////////////////////////////////////////////////////////////////////
    class CBaseStreamExt
    {
    public:

        static const uint64_t RECORD_INFINITE = uint64_t(-1);

        virtual ~CBaseStreamExt() {}


        /// \brief Set the event callback buffer size for this stream
        /// \param[in]  iBufferSize the size of the queue buffer in the event callback. Can't be 0.
        /// \return CInuError eOK indicates the new buffer size was set properly
        virtual CInuError  SetEventBufferSize(uint32_t iBufferSize) = 0;

        /// \brief Record NU3000 streams
        /// \param[in]  iDestinationDirectory Destination directory for recording output. Send empty string to stop recording.
        /// \param[in]  iTemplateName string which will be concatenate to output file name.
        /// \param[in]  iDuration recording time in ms.
        /// \return CInuError eOK indicates that request was successfully processed but it doesn't indicate that recording is completed
        virtual CInuError  Record(const std::string& iDestinationDirectory,
                                  const std::string& iTemplateName = std::string(),
                                  uint64_t iDuration = CBaseStreamExt::RECORD_INFINITE) const  = 0;

        /// \brief Record NU3000 streams
        /// \param[in]  iDestinationDirectory Destination directory for recording output. Send empty string to stop recording.
        /// \param[in]  iTemplateName string which will be concatenate to output file name.
        /// \param[in]  iFileNameIndex string which will be concatenate to output file name. If no index is provided then frame index will be used.
        /// \return CInuError eOK indicates that request was successfully processed but it doesn't indicate that recording is completed
        virtual CInuError  Snapshot(const std::string& iDestinationDirectory,
                                    const std::string& iTemplateName = std::string(),
                                    uint64_t iFileNameIndex = CBaseStreamExt::RECORD_INFINITE) const  = 0;
    };
}

#endif // __BASESTREAM_H__
