/*
* File - GeneralFrame.h
*
* This file is part of the Inuitive SDK
*
* Copyright (C) 2014 All rights reserved to Inuitive  
*
*/

#ifndef __GENERALFRAME_H__
#define __GENERALFRAME_H__

#include "BaseStream.h"
#include <stdint.h>

namespace InuDev
{

    ////////////////////////////////////////////////////////////////////////
    /// \brief    Represents generic buffer that is provided by some of InuDev streams 
    ///
    /// Responsibilities: 
    ///      1. Buffer attributes: data and size.
    ///      2. Memory management control.
    ///
    /// Comment: The interpretation of provided buffers should be done by the caller application.
    ///          The caller application should be familiar with the internal format of 
    ///          each provided frame.
    ////////////////////////////////////////////////////////////////////////
    class CGeneralFrame : public CBaseFrame
    {

    public:
        
        /// \brief    Constructs empty (zero size) buffer
        INUSTREAMS_API CGeneralFrame();

        /// \brief    Constructor.
        /// \param[in] bufferSize    generated buffer size.
        INUSTREAMS_API CGeneralFrame(uint32_t bufferSize);

        /// \brief    Copy constructor - deep Copy.
        INUSTREAMS_API CGeneralFrame(const CGeneralFrame& input);

        /// \brief    Move constructor. 
        INUSTREAMS_API CGeneralFrame(CGeneralFrame&& input);

        /// \brief    Assignment operator - deep copy.
        INUSTREAMS_API CGeneralFrame& operator=(const CGeneralFrame& input);

        /// \brief    Move assignment operator.
        INUSTREAMS_API CGeneralFrame& operator=(CGeneralFrame&& input);

        /// \brief    Destructor.
        INUSTREAMS_API virtual ~CGeneralFrame();

        INUSTREAMS_API unsigned int BufferSize() const  {  return mBufferSize;  }

        INUSTREAMS_API const InuDev::byte* GetData() const {  return mData;  }
        INUSTREAMS_API InuDev::byte*  GetData() {  return mData;  }

    protected:

        ///  \brief     Data buffer
        InuDev::byte* mData;

        ///  \brief     Buffer size
        unsigned int mBufferSize;

        ///  \brief    If data was extracted (by using ExtractDataBuffer) then this flag becomes false and data will not be released.
        bool mReleaseData;
    };

}

#endif // __GENERALFRAME_H__
