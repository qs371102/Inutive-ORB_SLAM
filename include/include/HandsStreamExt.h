
#ifndef __HandsSTREAMEXT_H__
#define __HandsSTREAMEXT_H__

#include "HandsStream.h"
#include "BaseStreamExt.h"
#include "BaseFrameExt.h"

namespace  InuDev
{
    struct CVideoInitParams;
    struct CDepthInitParams;
    struct CSensorParamsExt;

    ///////////////////////////////////////////////////////////////////////
    // Class: CHandsFrameExt
    // Role: Represents a hands information object which is used internally by IAF
    // Inherits: 
    //      CHandsFrame - Hands information class exported by IAF API 
    //      ISerializableFrame - Interface for serialization of IAF stream
    // Responsibilities: 
    // Comments:
    ///////////////////////////////////////////////////////////////////////
    struct CHandsFrameExt : public CHandsFrame, public CBaseFrameExt
    {
        // for debug use 
        // --------------- 
        #pragma warning(suppress: 4251)
        CPoint3D            	HandValleys[2][CHandData::eNumOfFingers+1];

        unsigned short	        HandRadius[2];

        CHandsFrameExt() : CBaseFrameExt(), HandValleys() {}

#ifndef _MSC_VER
        CHandsFrameExt(const CHandsFrameExt&) = default;
        CHandsFrameExt& operator=(const CHandsFrameExt&) = default;
#endif
    };


    ////////////////////////////////////////////////////////////////////////
    /// \brief HandsStream extension class 
    ///
    /// Class: CHandsStreamExt
    ///
    /// Role: Adds extended features to the CHandsStream interface
    ///       Intended for internal use
    ///
    /// Inherits: CHandsStream
    ///
    /// Responsibilities: 
    ///      1. Add extended API interface
    ///
    /// Comments: 
    ////////////////////////////////////////////////////////////////////////
    class CHandsStreamExt : public CHandsStream, public CBaseStreamExt
    {

    public:
        using CHandsStream::GetFrame;
        using CHandsStream::Init;

        CHandsStreamExt() {}
        virtual ~CHandsStreamExt() {}

        //Extended methods

        virtual CInuError  GetFrame(CHandsFrameExt& handsInfo, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

        virtual CInuError  SetDepthParams(const CDepthInitParams& params) = 0;
        virtual CInuError  SetVideoParams(const CVideoInitParams& params) = 0;

    protected:


    private:

    };


    //////////////////////////////////////////////////////////////////////
    //                      INLINE FUNCTIONS                            //
    //////////////////////////////////////////////////////////////////////


}

#endif // __HandsSTREAMEXT_H__

