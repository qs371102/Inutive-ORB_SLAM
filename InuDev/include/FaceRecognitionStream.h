#ifndef __FACERECOGNITIONSTREAM_H__
#define __FACERECOGNITIONSTREAM_H__



#include "StreamsExport.h"

#include "BaseStream.h"
#include <functional>

namespace InuDev 
{

    ////////////////////////////////////////////////////////////////////////
    /// \brief    Face frame provided by  CFaceRecognitionStream
    ///
    /// Role: Face position (User ID is defined in CBaseFrame)
    ////////////////////////////////////////////////////////////////////////  
    struct CFaceFrame  : public CBaseFrame
    {
        /// \brief    True if current recognized user is already enrolled.
        bool IsEnrolled;

        /// \brief    Top Left coordinates of recognized face.
        CPoint2D FaceRectPos;

        /// \brief    Size of recognized face.
        CPoint2D FaceRectSize;
    };

    ////////////////////////////////////////////////////////////////////////
    /// \brief   Interface for Face Recognition service.
    /// 
    /// Role: Controls Face Recognition service and provides Face frames.  
    ///
    /// Responsibilities: 
    ///      1. Knows how to control the service (Init, Terminate, Start and Stop).
    ///      2. Knows how to acquire one face frame (pull)
    ///      3. Knows how to provide a continuous stream of face frames (push)
    ///
    ////////////////////////////////////////////////////////////////////////
    class CFaceRecognitionStream  : public CBaseStream 
    {
    public:

        typedef std::function<void(std::shared_ptr<CFaceRecognitionStream>, const CFaceFrame&,  CInuError)> CallbackFunction;

        virtual ~CFaceRecognitionStream() {}

        virtual CInuError         GetFrame(CFaceFrame& oHeadFrame, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

        virtual CInuError         Register(CallbackFunction iCallback, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

        virtual CInuError         Enroll(std::string userId) = 0;

        virtual CInuError         Remove(std::string userId) = 0;

    };
}

#endif // __FACERECOGNITIONSTREAM_H__
