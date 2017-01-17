#ifndef __FACEENROLLMENTSTREAM_H__
#define __FACEENROLLMENTSTREAM_H__

#include "StreamsExport.h"

#include "BaseStream.h"
#include <functional>

namespace InuDev 
{
    ////////////////////////////////////////////////////////////////////////
    /// \brief    Enrollment frame provided by CFaceEnrollmentStream
    ///
    /// Role: Empty
    ////////////////////////////////////////////////////////////////////////  
    struct CFaceEnrollment  : public CBaseFrame
    {
    };

    ////////////////////////////////////////////////////////////////////////
    /// \brief   Interface for Face Enrollment service.
    /// 
    /// Role: Controls Face Recognition service and provides Face frames.  
    ///
    /// Responsibilities: 
    ///      1. Knows how to control the service (Init, Terminate, Start and Stop).
    ///      2. Knows how to acquire one face frame (pull)
    ///      3. Knows how to provide a continuous stream of face frames (push)
    ///
    ////////////////////////////////////////////////////////////////////////
    class CFaceEnrollmentStream : public CBaseStream
    {
    public:

        /// \brief    Prototype of callback function which is used by the Register method.
        ///
        /// This function receive one head frame.
        typedef std::function<void(std::shared_ptr<CFaceEnrollmentStream>, const CFaceEnrollment&,  CInuError)> CallbackFunction;

        virtual ~CFaceEnrollmentStream() {}


        virtual CInuError         GetFrame(CFaceEnrollment& oHeadFrame, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;


        virtual CInuError         Register(CallbackFunction iCallback, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

    };
}

#endif // __FACEENROLLMENTSTREAM_H__
