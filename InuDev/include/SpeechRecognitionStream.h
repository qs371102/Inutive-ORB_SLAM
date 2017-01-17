/*
 * File - SpeechRecognitionStream.h
 *
 * This file is part of Inuitive API Framework  and it contains declaration of classes CSpeechRecognitionFrame and CSpeechRecognitionStream.
 *
 */

#ifndef __SPEECHRECOGNITIONSTREAM_H__
#define __SPEECHRECOGNITIONSTREAM_H__

#include "StreamsExport.h"

#include "BaseStream.h"

#include <functional>
#include <vector>

namespace InuDev 
{

    ////////////////////////////////////////////////////////////////////////
    /// \brief    Speech Recognition frame provided by CSpeechRecognitionStream
    ////////////////////////////////////////////////////////////////////////
    struct CSpeechRecognitionFrame : public CBaseFrame
    {       
        /// \brief    UTF-8 characters string which represents the recognized word 
        std::string RecognizedWord;
    };


    ////////////////////////////////////////////////////////////////////////
    /// \brief   Interface for Speech Recognition service.
    /// 
    /// Role: Control speech recognition streaming service and provides speech recognition frames.  
    ///
    /// Responsibilities: 
    ///      1. Knows how to control the service (Init, Terminate, Start and Stop).
    ///      2. Knows how to acquire one speech recognition frame (pull)
    ///      3. Knows how to provide a continuous stream of speech recognition frames (push)
    ///
    ////////////////////////////////////////////////////////////////////////
    class CSpeechRecognitionStream : public CBaseStream
    {

    public:

        /// \brief    Prototype of callback function which is used by the Register method.
        ///
        /// This function receive one speech recognition frame.
        typedef std::function<void(std::shared_ptr<CSpeechRecognitionStream>, const CSpeechRecognitionFrame&,  CInuError)> CallbackFunction;

        virtual ~CSpeechRecognitionStream() {}

        /// \brief    Retrieve one speech recognition frame (pull)
        /// 
        /// This method returns when a new frame is ready (blocking). 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        ///
        /// \param[out] oRecgnizedWord    Returned speech recognition frame.
        /// \param[in] iTimeout    How long to wait for recognition, -1 ==> wait for ever.
        ///
        /// \return InuDev::CInuError    Operation status which indicates on success or failure.
        virtual CInuError         GetFrame(CSpeechRecognitionFrame& oRecgnizedWord, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

        /// \brief    Registration for receiving speech recognition frames (push) 
        /// 
        /// Provided callback function is called when a new frame is ready (non-blocking). 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        ///
        /// \param[in] iCallback    Callback function which is invoked when a new frame is ready
        ///
        /// \return CInuError    Operation status which indicates on success or failure.
        virtual CInuError         Register(CallbackFunction iCallback, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;
    };
}

#endif // __SPEECHRECOGNITION_H__
