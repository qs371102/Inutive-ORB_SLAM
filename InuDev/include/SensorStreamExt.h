
#ifndef __SENSORSTREAMEXT_H__
#define __SENSORSTREAMEXT_H__

#include "BaseStream.h"
#include "InuSensorExt.h"

#include <string>

namespace  InuDev
{
    ////////////////////////////////////////////////////////////////////////
    /// \brief   Grants data injection capabilities to the NU3000
    ///
    /// Role: 
    ///
    /// Responsibilities: 
    ///      1. 
    ///      2. 
    ///
    /// Comments: 
    ///
    ////////////////////////////////////////////////////////////////////////
    class CSensorStreamExt : public CBaseStream
    {

    public:
        CSensorStreamExt() {}
        virtual ~CSensorStreamExt() {}

        using   CBaseStream::Init;
        
        /// \brief Init() - Service initalization
        /// 
        /// \param[in] params     parameters which are required in order to start the service
        ///
        /// \return InuDev::CInuError     one of NU3000 Driver Errors (see in IAFExceptions.h)
        virtual CInuError       Init(const CSensorParamsExt& iParams) = 0;

        /// \brief Write() - Inject buffer
        /// 
        /// Detailed description: Send to InuService the buffer that should be injected 
        ///
        /// \param[in] iBuffer     Buffer data 
        /// \param[in] iBufferLen     Buffer Length
        ///
        /// \return InuDev::CInuError     one of NU3000 Driver Errors (see in IAFExceptions.h)
        virtual CInuError       Write(const unsigned char* iBuffer, unsigned int iBufferLen) const = 0;

        /// \brief SetSensorRegister Sets a register value in a sensor
        /// 
        /// \param[in]  iAddress register address
        /// \param[in]  iData new register value
        /// \param[in]  size data size
        /// \param[in]  SensorNumber sensor number numb
        /// \return CInuError
        virtual CInuError SetRegister(unsigned int iAddress, unsigned int iData, unsigned int size, ESensorName SensorNumber) const  = 0;

        /// \brief GetSensorRegister gets a register value from a sensor
        /// 
        /// \param[in]  iAddress register address
        /// \param[out]  iData register value
        /// \param[in]  size data size
        /// \param[in]  SensorNumber sensor number numb
        /// \return CInuError
        virtual CInuError GetRegister(unsigned int iAddress, unsigned int &iData, unsigned int size, ESensorName SensorNumber) const  = 0;

        /// \brief SetSensorRegister Sets a register value in a sensor
        /// 
        /// \param[in]  iAddress register address
        /// \param[in]  iData new register value
        /// \param[in]  size data size
        /// \param[in]  SensorNumber sensor number numb
        /// \return CInuError
        virtual CInuError SetI2CRegister(const CSensorI2CRegisterParams& iParams) const  = 0;

        /// \brief GetSensorRegister gets a register value from a sensor
        /// 
        /// \param[in]  iAddress register address
        /// \param[out]  iData register value
        /// \param[in]  size data size
        /// \param[in]  SensorNumber sensor number numb
        /// \return CInuError
        virtual CInuError GetI2CRegister(CSensorI2CRegisterParams& ioParams) const  = 0;

        virtual CInuError SetSensorPhaseRegister(unsigned int iAddress, unsigned int iData, EPhaseNumber iPhase, unsigned int iSize, ESensorName iSensorNumber) const = 0;

        virtual CInuError SetConfiguration(ESensorName iSensorNumber) const  = 0;

        /// \brief Sensor Load Registers from file
        /// 
        /// \param[in]  iFileName file name containing registers values
        /// \param[in]  iVerify - if it is true then the function checks that load succeeds (by reading the value of each register) and 
        ///                       then the loading time is increased. 
        /// \return CInuError
        virtual CInuError LoadRegistersFromFile(const std::string &iFileName, bool iVerify) const  = 0;

        /// \brief    Loads a file containing target register settings
        /// \param[in] iFileName    file path.
        /// \return CInuError
        virtual CInuError LoadRegistersConfigurationFile(std::string iFileName) const = 0;


        // AGC
        //----
        
        virtual CInuError GetSensorControlParams(CSensorControlParamsExt&, uint32_t iChannel = 0) const = 0;
        virtual CInuError SetSensorControlParams(const CSensorControlParamsExt&, uint32_t iChannel = 0) const = 0;

        virtual CInuError GetSensorConfiguration(CSensorConfiguration& iConfiguration) const = 0;

        /// \brief    Define parameters for AGC (data is sent to chip).
        /// 
        /// \param[in,out] ioParams   Input structure that may be changed by the service in order to make them coherent.
        ///
        /// \return CInuError    eIllegalAGCValue.
        virtual CInuError       SetAgcParams(CAgcParams& ioParams) const = 0;

        /// \brief    Get parameters that are currently used by AGC.
        /// 
        /// \param[in] oParams    Output parameters.
        virtual CInuError       GetAgcParams(CAgcParams& oParams) const = 0;

        /// \brief    Start manual calibration process.
        virtual CInuError       ManualCalibration() const = 0;

        /// \brief    Report on calibration progress.
        /// 
        /// \return CInuError    eSatusOK, eInCalibrationProcess, eClibarationError.
        virtual CInuError       ManualCalibrationStatus() const = 0;

        /// \brief    Start tuning sensor register to improve image quality
        /// 
        /// \return CInuError    eSatusOK
        virtual CInuError       StartTuning() const = 0;

        /// \brief Get Optical data information 
        /// \param[out]  oOpticalData   Output optical data information
        /// \return CInuError   If InuSensor isn't initialized then eStateError is returned 
        virtual CInuError GetOpticalData(COpticalData& oOpticalData) const  = 0;

    protected:


    private:

    };
}

#endif // __SENSORSTREAMEXT_H__

