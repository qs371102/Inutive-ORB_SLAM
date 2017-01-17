/*
* File - ImageFrame.h
*
* This file is part of the Inuitive SDK
*
* Copyright (C) 2014 All rights reserved to Inuitive  
*
*/

#ifndef __IMAGEFRAME_H__
#define __IMAGEFRAME_H__

#include "StreamsExport.h"

#include "BaseStream.h"
#include "InuSensor.h"

namespace InuDev
{

    ////////////////////////////////////////////////////////////////////////
    /// \brief    Represents  an image that is  provided  by InuDev streams 
    ///
    /// Responsibilities: 
    ///      1. Image attributes: format, scale, width, height and number of bytes per pixel.
    ///      2. Knows how to manage the image buffer.
    ///
    ////////////////////////////////////////////////////////////////////////
    class CImageFrame : public CBaseFrame
    {

    public:

        ///  \brief     Supported formats
        enum EFormat 
        { 
            eEmpty = 0,     ///<  Empty buffer      
            eDepth,         ///<  Z-Buffer (16 bits per pixel)  
            eBGR,           ///<  3 Bytes per pixel: B, G and R 
            eBGRA,          ///<  4 Bytes per pixel: B, G, R and Alpha (which is always 0)
            eDisparity,     ///<  2 Bytes per pixel: 4 MSB for confidence and 12 LSB for Disparities 
            eRGB565,        ///<  Standard RGB565 format (2 bytes per pixel)  
            eDisparityB0,   ///<  2 Bytes per pixel: 4 LSB for confidence and 4 LSB for Disparities 
            eRGBA,          ///<  4 Bytes per pixel: R, G, B and Alpha (which is always 0)
            eDepthWithConfidence, ///<  Z-Buffer: 14 MSBits are depth and 2 LSBits are for confidence (The higher the better)
            ePointCloud,    ///<  XYZ float point cloud buffer
            eYUV422,         ///<  2 bytes of compressed Y, U, V 
            eABGR,          ///<  4 Bytes per pixel: Alpha, B, G, R (Alpha which is always 0)
            eARGB,          ///<  4 Bytes per pixel: Alpha, R, G, B (Alpha which is always 0)
            eRGB,           ///<  3 Bytes per pixel: R, G and B 
        };

        /// \brief    Scale factor (relevant only to eDepth format)
        enum EScale
        { 
            eMM = 0,    ///< Default scale (mm)
            eCM 
        };

        /// \brief    Constructs empty (zero size) frame
        INUSTREAMS_API CImageFrame();

        /// \brief    Constructor.
        /// \param[in] iWidth         Image width.       (full buffer width)
        /// \param[in] iHeight        Image height.
        /// \param[in] iImageWidth    Image width.       (real size of image inside the buffer)
        /// \param[in] iImageHeight   Image height.
        /// \param[in] iStartX        X offset inside the buffer
        /// \param[in] iStartY        Y offset inside the buffer
        /// \param[in] iFormat        Image format (which defines  the number of bytes  per pixel).
        INUSTREAMS_API CImageFrame(unsigned int iWidth,       
            unsigned int iHeight,
            unsigned int iImageWidth,  
            unsigned int iImageHeight,
            unsigned short iStartX,
            unsigned short iStartY,
            unsigned int iFormat); 

        /// \brief    Constructor.
        /// \param[in] iData          Data buffer.
        /// \param[in] iWidth         Image width.      (full buffer width)
        /// \param[in] iHeight        Image height.
        /// \param[in] iImageWidth    Image width.      (real size of image inside the buffer)
        /// \param[in] iImageHeight   Image height.
        /// \param[in] iStartX        X offset inside the buffer
        /// \param[in] iStartY        Y offset inside the buffer
        /// \param[in] iFormat    Image format (which defines  the number of bytes  per pixel).
        /// \param[in] iOwnData    If this flag is true then this CImageFrame object is responsible for freeing the buffer
        INUSTREAMS_API CImageFrame(InuDev::byte*  iData,
            unsigned int iWidth,
            unsigned int iHeight,
            unsigned int iImageWidth, 
            unsigned int iImageHeight,
            unsigned short iStartX,
            unsigned short iStartY,
            unsigned int iFormat,
            bool iOwnData = true); 

        /// \brief    Copy constructor - deep Copy.
        INUSTREAMS_API CImageFrame(const CImageFrame& input);

        /// \brief    Move constructor. 
        INUSTREAMS_API CImageFrame(CImageFrame&& input);

        /// \brief    Assignment operator - deep copy.
        INUSTREAMS_API CImageFrame& operator=(const CImageFrame& input);

        /// \brief    Move assignment operator.
        INUSTREAMS_API CImageFrame& operator=(CImageFrame&& input);

        /// \brief    Destructor.
        INUSTREAMS_API virtual ~CImageFrame();

        unsigned int BufferSize() const  { return ImageSize() * BytesPerPixel(); } 
        unsigned int ImageSize() const { return mWidth * mHeight; } 

        unsigned int Width() const     { return mWidth; }
        unsigned int Height() const    { return mHeight; }

        unsigned int ImageWidth() const     { return mImageWidth; }
        unsigned int ImageHeight() const    { return mImageHeight; }

        unsigned short StartX() const     { return mStartX; }
        unsigned short StartY() const    { return mStartY; }


        unsigned int Format() const    { return mFormat; }

        /// \brief    Number of bytes that are used to represent each pixel.
        INUSTREAMS_API unsigned int BytesPerPixel() const;

        EScale Scale() const { return mScale; }
        EScale& Scale() { return mScale; }

        const InuDev::byte* GetData() const  { return mData; }
        InuDev::byte*  GetData() { return mData; }

        /// \brief    ExtractDataBuffer() - Extracts the data buffer from the ImageFrame 
        /// 
        /// Returns the ImageFrame's data buffer, and removes the ownership.
        /// The ImageFrame won't hold the buffer anymore, and is no longer responsible for freeing the buffer.
        /// \return InuDev::byte*     data buffer pointer
        INUSTREAMS_API InuDev::byte*   ExtractDataBuffer();

        
        CExposureParams GetExposureParams() const { return mExposureParams; }
    protected:

        ///  \brief     Image width (full buffer width) 
        unsigned int mWidth;

        ///  \brief     Image height (full buffer height)
        unsigned int mHeight;

        ///  \brief     Data buffer
        InuDev::byte* mData;

        ///  \brief     Image Format
        unsigned int mFormat;

        ///  \brief     Scale (relevant only to eDepth format)
        EScale mScale;

        ///  \brief    If data was extracted (by using ExtractDataBuffer) then this flag becomes false and data will not be released.
        bool mReleaseData;

        ///  \brief     Image width  (real size of image inside the buffer)
        unsigned int mImageWidth;

        ///  \brief     Image height (real size of image inside the buffer)
        unsigned int mImageHeight;

        ///  \brief     Image X offset inside the buffer 
        unsigned short mStartX;

        ///  \brief     Image Y offset inside the buffer 
        unsigned short mStartY;

        ///  \brief     holds the ET, digital and analog gain L/F
        CExposureParams mExposureParams;


    };
}

#endif // __IMAGEFRAME_H__
