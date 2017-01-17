
#ifndef __HEADSTREAMEXT_H__
#define __HEADSTREAMEXT_H__

#include "HeadStream.h"
#include "BaseStreamExt.h"
#include "BaseFrameExt.h"

namespace  InuDev
{
    struct CVideoInitParams;
    struct CDepthInitParams;
    struct CSensorParamsExt;

    ///////////////////////////////////////////////////////////////////////
    // Class: CHeadFrameExt
    // Role: Represents a head orientation object which is used internally by IAF
    // Inherits: 
    //      CImageStream - Stream which represents image and exported by IAF API 
    //      ISerializableFrame - Interface for serialization of IAF stream
    // Responsibilities: 
    //      1. Knows the Image format
    //      2. Knows Min/Max disparity of Depth Image.
    // Comments:
    ///////////////////////////////////////////////////////////////////////
    struct  CHeadFrameExt : public CHeadFrame, public CBaseFrameExt
    {
        // lines to draw on screen 0-1, 2-3, 4-5
        static const int NUM_OF_LINES=6;
        float LinesX[NUM_OF_LINES];
        float LinesY[NUM_OF_LINES];

        //#pragma warning(suppress: 4251)
        CPoint3D   HeadBoundingBox[8];

        // 2 Roles:
        // Where to display the calibration icon (during calibration)
        // Intersection between vector that starts on nose tip and end screen plane
        //#pragma warning(suppress: 4251)
        CPoint2D   PointOnScreen;

        // True if the frame is part of calibration process
        bool       InCalibrationProcess;

        /// \brief    true if left eye (first) or right eye (second) are blinking.        
        //#pragma warning(suppress: 4251)
        std::pair<bool, bool>  Blinking; 


        // for debug use 
        // ---------------    
        // Error 
        float	      RegistrationError;

        // Face information
        //#pragma warning(suppress: 4251)
        CPoint2D      FaceRectPos;

        //#pragma warning(suppress: 4251)    
        CPoint2D      FaceRectSize;

        CHeadFrameExt() : CBaseFrameExt() {}

#ifndef _MSC_VER
        CHeadFrameExt(const CHeadFrameExt&) = default;
        CHeadFrameExt& operator=(const CHeadFrameExt&) = default;
#endif

    };

    ////////////////////////////////////////////////////////////////////////
    /// \brief HeadStream extension class 
    //.
    /// Role: Adds extended features to the CHeadStream interface
    ///       Intended for internal use
    ///
    /// Responsibilities: 
    ///      1. Add extended API interface
    ///
    ////////////////////////////////////////////////////////////////////////

    class CHeadStreamExt : public CHeadStream, public CBaseStreamExt
    {

    public:
        using CHeadStream::GetFrame;
        using CHeadStream::Init;

        CHeadStreamExt() {}
        virtual ~CHeadStreamExt() {}

        //Extended methods

        virtual CInuError  GetFrame(CHeadFrameExt& headInfo, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

        virtual CInuError  SetDepthParams(const CDepthInitParams& params) = 0;
        virtual CInuError  SetVideoParams(const CVideoInitParams& params) = 0;

        virtual CInuError  StartCalibration() = 0;

    protected:


    private:

    };


}

#endif // __HEADSTREAMEXT_H__

