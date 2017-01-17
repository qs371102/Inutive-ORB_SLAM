#ifndef __FACEEXPRESSIONSSTREAM_H__
#define __FACEEXPRESSIONSSTREAM_H__

#include "StreamsExport.h"

#include "BaseStream.h"
#include <functional>

namespace InuDev 
{
    ////////////////////////////////////////////////////////////////////////
    /// \brief    Face expressions frame provided by  CFaceExpressionsStream
    ///
    /// Role: All expressions recognized by IAF for one face in current scene
    ////////////////////////////////////////////////////////////////////////  
    struct CFaceExpressionsFrame  : public CBaseFrame
    {
        /// \brief    All types of expressions that may be recognized by IAF 
        enum EExpressions {
            eAnger = 0,
            eContempt,
            eDisgust,
            eJoy,
            eFear,
            eSadness,
            eSurprise,
            eConfusion,
            eNumOfExpressions
        };   

        /// \brief    Intensity levels of all expressions recognized for one face.
        /// 
        /// Intensity level is in range [SCORE_MIN, SCORE_MAX]
        unsigned int Expressions[eNumOfExpressions];

        /// \brief    Recognized Gender
        ///
        /// Intensity level is in range [SCORE_MIN, SCORE_MAX], where 
        /// SCORE_MIN = female
        /// SCORE_MAX = male
        unsigned int Gender;

        /// \brief    Overall sentiment
        ///
        /// Intensity level is in range [SCORE_MIN, SCORE_MAX], where 
        /// SCORE_MIN = Negative
        /// SCORE_MAX = Positive
        /// (SCORE_MAX - SCORE_MIN)/2 = Neutral
        unsigned int Sentiment;

        /// \brief    Evidence for wearing glasses
        ///
        /// Intensity level is in range [SCORE_MIN, SCORE_MAX] 
        unsigned int  HasGlasses;
    };


    ////////////////////////////////////////////////////////////////////////
    /// \brief   Interface for Face expressions recognition service.
    /// 
    /// Role: Controls face expressions streaming service and provides expression frames.  
    ///
    /// Responsibilities: 
    ///      1. Knows how to control the service (Init, Terminate, Start and Stop).
    ///      2. Knows how to acquire one expressions frame (pull)
    ///      3. Knows how to provide a continuous stream of expressions frames (push)
    ///
    ////////////////////////////////////////////////////////////////////////
    class CFaceExpressionsStream : public CBaseStream 
    {
    public:

        /// \brief    Prototype of callback function which is used by the Register method.
        ///
        /// This function receive one expressions frame.
        typedef std::function<void(std::shared_ptr<CFaceExpressionsStream>, const CFaceExpressionsFrame&,  CInuError)> CallbackFunction;

        virtual ~CFaceExpressionsStream() {}

        /// \brief    Retrieve one face expressions frame (pull)
        /// 
        /// This method returns when a new frame is ready (blocking). 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        ///
        /// \param[out] oExpressionsFrame    Returned face expressions frame.
        /// \return InuDev::CInuError    Operation status which indicates on success or failure.
        virtual CInuError         GetFrame(CFaceExpressionsFrame& oExpressionsFrame, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

        /// \brief    Registration for receiving face expression frames (push) 
        /// 
        /// Provided callback function is called when a new frame is ready (non-blocking). 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        ///
        /// \param[in] iCallback    Callback function which is invoked when a new frame is ready
        /// \return CInuError    Operation status which indicates on success or failure.
        virtual CInuError         Register(CallbackFunction iCallback, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;
    };
}

#endif // __FACEEXPRESSIONSSTREAM_H__
