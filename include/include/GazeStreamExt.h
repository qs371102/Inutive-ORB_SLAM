
#ifndef __GAZESTREAMEXT_H__
#define __GAZESTREAMEXT_H__

#include "GazeStream.h"
#include "BaseStreamExt.h"
#include "BaseFrameExt.h"

namespace  InuDev
{
    struct CVideoInitParams;
    struct CDepthInitParams;
    struct CSensorParamsExt;

    ////////////////////////////////////////////////////////////////////////
    /// \brief Brief description, ends with empty line 
    ///
    /// Class: 
    ///
    /// Role: 
    ///
    /// Inherits: 
    ///
    /// Responsibilities: 
    ///      1. 
    ///      2. 
    ///
    /// Comments: 
    ////////////////////////////////////////////////////////////////////////
    struct CGazeFrameExt : public CGazeFrame, public CBaseFrameExt
    {
        struct CFaceData 
        {
            // in MMs
            bool        mValid;
            CPoint2D	mTopLeft;
            CPoint2D	mSize;
        };

        struct CEyeData 
        {
            // in MMs
            bool                mEyeValid;
            CPoint2D			mBoxTopLeft;
            CPoint2D	        mBoxSize;
            bool                mPupilValid;            
            CPoint2D			mPupilPos;
            CPoint2D            mGazeVector;
        };

        enum ESide
        {   
            eLeft = 0,
            eRight = 1
        }; 
        
        CFaceData    FaceData;
        CEyeData     EyesData[2];

        #pragma warning(disable : 4251) 
        CPoint2D     MidVector;

        // Status defined in "inu_cmd.h"
        int          GazeStatus;

        CGazeFrameExt() : CBaseFrameExt() {}
    };


    ////////////////////////////////////////////////////////////////////////
    /// \brief GazeStream extension class 
    ///
    /// Class: CGazeStreamExt
    ///
    /// Role: Adds extended features to the CGazeStream interface
    ///       Intended for internal use
    ///
    /// Inherits: CGazeStream
    ///
    /// Responsibilities: 
    ///      1. Add extended API interface
    ///
    /// Comments: 
    ////////////////////////////////////////////////////////////////////////
    class CGazeStreamExt : public CGazeStream, public CBaseStreamExt
    {

    public:
        using CGazeStream::GetFrame;

        CGazeStreamExt() {}
        virtual ~CGazeStreamExt() {}

        //Extended methods
        virtual CInuError  GetFrame(CGazeFrameExt& gazeInfo, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

        virtual CInuError  SetVideoParams(const CVideoInitParams& params) = 0;

    protected:


    private:

    };


    //////////////////////////////////////////////////////////////////////
    //                      INLINE FUNCTIONS                            //
    //////////////////////////////////////////////////////////////////////


}

#endif // __GAZESTREAMEXT_H__

