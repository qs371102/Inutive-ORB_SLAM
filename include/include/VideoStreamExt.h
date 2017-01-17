
#ifndef __VIDEOSTREAMEXT_H__
#define __VIDEOSTREAMEXT_H__

#include "VideoStream.h"
#include "ImageFrameExt.h"
#include "BaseStreamExt.h"

namespace  InuDev
{
    struct CSensorParamsExt;

    struct CVideoInitParams : CImageFrameParams
    {
		bool isInterleaved;
    };

    struct CVideoFrameExt : public CVideoFrame, public CBaseFrameExt
    {
        INUSTREAMS_API CVideoFrameExt() : CVideoFrame(), CBaseFrameExt() {}
        /// \brief    Constructs zero size left and right images

        /// \brief    Constructor.
        /// 
        /// \param[in] iWidth    Image width.
        /// \param[in] iHeight    Image height.
        /// \param[in] iFormat    Image Format.
        /// \param[in] iTimestamp    Frame ID (acquisition time).
        INUSTREAMS_API CVideoFrameExt(unsigned int iWidth, unsigned int iHeight,unsigned int iImageWidth, unsigned int iImageHeight, unsigned short iStartX, unsigned short iStartY, unsigned int  iFormat) : 
            CVideoFrame(iWidth, iHeight,iImageWidth, iImageHeight,iStartX, iStartY, iFormat), CBaseFrameExt() {} 

        /// \brief    Copy constructor
        INUSTREAMS_API CVideoFrameExt(const CVideoFrameExt& input) : CVideoFrame(input), CBaseFrameExt(input) {} 

        /// \brief    Move constructor
        INUSTREAMS_API CVideoFrameExt(const CVideoFrameExt&& input) : CVideoFrame(input), CBaseFrameExt(input) {} 

        /// \brief    Assignment operator
        INUSTREAMS_API CVideoFrameExt& operator=(const CVideoFrameExt& input);

        /// \brief    move operator
        INUSTREAMS_API CVideoFrameExt& operator=(const CVideoFrameExt&& input);

        INUSTREAMS_API virtual ~CVideoFrameExt() {}
    };


    ////////////////////////////////////////////////////////////////////////
    /// \brief VideoStream extension class 
    ///
    /// Class: CVideoStreamExt
    ///
    /// Role: Adds extended features to the CVideoStream interface
    ///       Intended for internal use
    ///
    /// Inherits: CVideoStream
    ///
    /// Responsibilities: 
    ///      1. Add extended API interface
    ///
    /// Comments: 
    ////////////////////////////////////////////////////////////////////////
    class CVideoStreamExt : public CVideoStream, public CBaseStreamExt
    {

    public:
        typedef std::function<void(std::shared_ptr<CVideoStreamExt>, const CVideoFrameExt&,  CInuError)> CallbackFunctionExt;

        enum EOutputType
        {
            eDefaultDisplay  = 0,
            eRaw = 1,
            eBGRA,
            eRGBA,
            eABGR,
            eARGB
        };

        using CVideoStream::Init;
        using CVideoStream::GetFrame;

        CVideoStreamExt() {}
        virtual ~CVideoStreamExt() {}

        virtual CInuError         GetFrame(CVideoFrameExt& oVideoFrame, unsigned int iTimeout = FPS_BASED_TIMEOUT, bool iCopyLocal = true) = 0;
        virtual CInuError         Register(CallbackFunctionExt iCallback, unsigned int iTimeout = FPS_BASED_TIMEOUT, bool iCopyLocal = true) = 0;

        //Extended methods
        virtual CInuError         Init(bool iInterleaved) = 0;

        virtual CInuError         Init(bool iInterleaved, EOutputType iOutputType) = 0;

        virtual CInuError         Init(EOutputType iOutputType) = 0;

        virtual CInuError         SetParams(const CImageFrameParams& params) = 0;
        virtual CInuError         GetParams(CVideoInitParams& params) = 0;

    protected:


    private:

    };

}

#endif // __VIDEOSTREAMEXT_H__

