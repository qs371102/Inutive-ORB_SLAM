/*
* File - AuxStream.h
*
* This file is part of the Inuitive SDK
*
* Copyright (C) 2014 All rights reserved to Inuitive  
*
*/

/*
* File - AuxStream.h
*
* This file is part of the Inuitive SDK
*
* Copyright (C) 2014 All rights reserved to Inuitive  
*
*/


#ifndef __AUXSTREAM_H__
#define __AUXSTREAM_H__
#include <map>
#include "BaseStream.h"
#include "GeneralFrame.h"

#include <functional>

namespace InuDev 
{
    ////////////////////////////////////////////////////////////////////////
    /// \brief  Represents the type of the sensor which CImuData refer to.
    ////////////////////////////////////////////////////////////////////////
    enum ESensorTypes
    {
        eAccelarometer,
        eGyroscope,
        eMagnetometer
    };

 
    /// \brief    Represents IMU data that is provided from the sensor 
    //struct CImuData 
    //{
    //    ESensorTypes Type;
    //    CPoint3D Point;   
    //};

    /// \brief    Represents collection of IMU data. 
    class CImuFrame : public CBaseFrame
    {
        public:
        std::map<ESensorTypes,CPoint3D> SensorsData;
    };

   enum EPosSensorChannelType
   {
       eAccelarometerX,
       eAccelarometerY,
       eAccelarometerZ,
       eGyroscopeX,
       eGyroscopeY,
       eGyroscopeZ,
       eMagnetometerX,
       eMagnetometerY,
       eMagnetometerZ,
       eTimeStamp
   };
  

   struct CIioChannelInfo 
   {
       float Scale;
       float Offset;
       uint32_t Bytes;
       uint32_t BitsUsed;
       uint32_t Shift;
       uint64_t Mask;
       uint32_t Be;
       uint32_t IsSigned;
       uint32_t Location;
       EPosSensorChannelType ChannelType;
     };

    ////////////////////////////////////////////////////////////////////////
    /// \brief   Interface for Auxiliaries service.
    /// 
    /// Role: Controls auxiliaries streaming service and provides general frames. 
    ///       Auxiliaries frames are provided only if the connected device supports auxiliaries HW components.
    ///       The caller application should be familiar with provided frames and should know how to interpret them. 
    ///
    /// Responsibilities: 
    ///      1. Derives CBaseStream interface
    ///      2. Knows how to acquire one general frame (pull)
    ///      3. Knows how to provide a continuous stream of general frames (push)
    /// 
    ////////////////////////////////////////////////////////////////////////
    class CAuxStream : public CBaseStream
    {
    public:

        /// \brief    Prototype of callback function which is used by the Register method.
        ///
        /// This function is invoked any time a frame is ready, or if an error occurs. The parameters of this function are:
        /// Caller stream object, received general frame and result code.
        typedef std::function<void(std::shared_ptr<CAuxStream> , const CGeneralFrame&,  CInuError)> CallbackFunction;
        typedef std::function<void(std::shared_ptr<CAuxStream> , const CImuFrame&,  CInuError)>     ImuCallbackFunction;

        virtual ~CAuxStream() {}

        /// \brief    Retrieves iio data information (pull) 
        /// 
        /// This method returns when a new frame is ready (blocking) or if an input timeout has elapsed. 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        // In order to interpreted the data it should use the iio header information. 
        /// \param[out] oAuxFrame    The returned general frame (iio data).
        /// \param[in] iTimeout    Function is returned if timeout has elapsed even if no new frame is ready.
        /// \return CInuError    Error code, InDev::eOK if operation successfully completed.
        virtual CInuError GetFrame(CGeneralFrame& oAuxFrame, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

        /// \brief    Retrieves iio header information (pull) 
        /// 
        /// This method returns when a new iio header data is ready (blocking) or if an input timeout has elapsed. 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked. 
        /// \param[out] oAuxFrame    The returned iio header data.
        /// \param[in] iTimeout    Function is returned if timeout has elapsed even if no new frame is ready.
        /// \return CInuError    Error code, InDev::eOK if operation successfully completed.
        virtual CInuError GetHeader(std::map<ESensorTypes, std::vector<CIioChannelInfo>>& oAuxFrame, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;


        /// \brief    Retrieves CImuFrame frame (pull) 
        /// 
        /// This method returns when a new CImuFrame is ready (blocking) or if an input timeout has elapsed. 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        /// \param[out] oAuxFrame    Array of CImuData sensor values.
        /// \param[in] iTimeout    Function is returned if timeout has elapsed even if no new frame is ready.
        /// \return CInuError    Error code, InDev::eOK if operation successfully completed.

        virtual CInuError GetFrame(CImuFrame& oAuxFrame, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0; 


 

        /// \brief    Registration/Unregistration for receiving stream of general frames (push) 
        /// 
        /// The provided callback function is called when a new frame is ready (non-blocking). 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        /// \param[in] iCallback    Callback function which is invoked when a new frame is ready.
        ///                         Send nullptr to unregister for receiving frames.  
        /// \param[in] iTimeout    Callback is invoked if a timeout has elapsed even if no new frame is ready.
        /// \return CInuError    Error code, InDev::eOK if operation successfully completed.
        virtual CInuError Register(CallbackFunction iCallback, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

        /// \brief    Registration/Unregistration for receiving stream of general frames (push) 
        /// 
        /// The provided callback function is called when a new frame is ready (non-blocking). 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        /// \param[in] iCallback    Callback function which is invoked when a new frame is ready.
        ///                         Send nullptr to unregister for receiving frames.  
        /// \param[in] iTimeout    Callback is invoked if a timeout has elapsed even if no new frame is ready.
        /// \return CInuError    Error code, InDev::eOK if operation successfully completed.
        virtual CInuError Register(ImuCallbackFunction iCallback, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;
    };
}

#endif
