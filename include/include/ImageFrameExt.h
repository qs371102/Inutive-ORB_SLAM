#ifndef __IMAGEFRAMEEXT_H__
#define __IMAGEFRAMEEXT_H__

#include "StreamsExport.h"

#include "ImageFrame.h"
#include "BaseFrameExt.h"

#include <stdint.h>

namespace InuDev
{

    struct CImageFrameParams
    {
        unsigned short  channelId;
        unsigned short  frameStartX;
        unsigned short  frameStartY;
        unsigned short  frameWidth;
        unsigned short  frameHeight;
    };

    ///////////////////////////////////////////////////////////////////////
    // Class: CImageFrameExt
    // Role: Represents Image (Frame) which is used internally by IAF
    // Inherits: 
    //      CImageStream - Stream which represents image and exported by IAF API 
    //      ISerializableFrame - Interface for serialization of IAF stream
    // Responsibilities: 
    //      1. Knows the Image format
    //      2. Knows Min/Max disparity of Depth Image.
    // Comments:
    ///////////////////////////////////////////////////////////////////////
    class  CImageFrameExt : public CImageFrame, public CBaseFrameExt
    {
    public:

        // Image format
        enum EFormatExt 
        { 
            eYUV = eEmpty+100,
            eY,    // 10 bit Y (the 11th bit indicates overflow)
            eUnpackedYUV,
            eYUVFloat,
            eBayerGRBG,
            eBayerRGGB,
            eBayerBGGR,
            eBayerGBRG,
            eDepthA0,      ///<  Depth from HW: 5 bits of confidence (MSB)
            eDepthB0,      ///<  Depth from HW: 2 bits for confidence (LSB)
            eY8,           ///<  8 bit Y (the 11th bit indicates overflow)
        };

        // Constructors
        // ------------
        INUSTREAMS_API CImageFrameExt() : CImageFrame(), CBaseFrameExt() {}
        INUSTREAMS_API CImageFrameExt(
            unsigned int iWidth,          // full buffer width
            unsigned int iHeight,
            unsigned int iImageWidth,     // real size of image inside the buffer
            unsigned int iImageHeight,
            unsigned short iStartX,
            unsigned short iStartY,
            unsigned int iFormat,         // input format
            unsigned int iMinDisparity = 0,
            unsigned int iMaxDisparity = 0 );


        INUSTREAMS_API CImageFrameExt(InuDev::byte*  iData,
            unsigned int iWidth,          // full buffer width
            unsigned int iHeight,
            unsigned int iImageWidth,     // real size of image inside the buffer
            unsigned int iImageHeight,
            unsigned short iStartX,
            unsigned short iStartY,
            unsigned int iFormat,
            bool iOwnData = true); 

        INUSTREAMS_API CImageFrameExt(const CImageFrameExt& input);
        INUSTREAMS_API CImageFrameExt(CImageFrameExt&& input);
        // Default operator = and copy
        //move assignment operator
        INUSTREAMS_API CImageFrameExt& operator=(const CImageFrameExt& input) { 
            CImageFrame::operator=(input);
            CBaseFrameExt::operator=(input);
            mMinDisparity = input.mMinDisparity;
            mMaxDisparity = input.mMaxDisparity;
            return *this; 
        }

        //move assignment operator
        INUSTREAMS_API CImageFrameExt& operator=(CImageFrameExt&& input) { 
            CImageFrame::operator=(std::move(input)); 
            CBaseFrameExt::operator=(std::move(input));
            mMinDisparity = input.mMinDisparity;
            mMaxDisparity = input.mMaxDisparity;
            return *this; 
        }

        // Access to private members
        // ------------
        unsigned int                    MinDisparity() const { return mMinDisparity; }
        void                            MinDisparity(unsigned int val) {  mMinDisparity = val; }

        unsigned int                    MaxDisparity() const { return mMaxDisparity; }
        void                            MaxDisparity(unsigned int val) {  mMaxDisparity = val; }

    protected:
        unsigned int mMinDisparity;
        unsigned int mMaxDisparity;
    };

}

#endif
