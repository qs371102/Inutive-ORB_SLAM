
#ifndef __PROVIDEREXT_H__
#define __PROVIDEREXT_H__

#include "StreamsExport.h"
#include "BaseStreamExt.h"
#include "InuSensor.h"
#include "InuDefs.h"

#include <limits>
#include <vector>

#ifndef MAX_PATH  
#define MAX_PATH 260
#endif

namespace InuDev
{
    enum EPhaseNumber
    {
        ePhase0 = 0,
        ePhase1 = 1,
        ePhase2 = 2,
        ePhaseA = 0xA,
        ePhaseB = 0xB,
    };

    enum ESensorName
    {
        eMaster = 0,
        eSlave = 1,
        eBoth = 2,
        eLeft = 3,
        eRight = 4,
    };

    enum ECameraName
    {
        eVideo = 0,
        eWebCam = 2,
    };


    struct CInjectionParams
    {   
        unsigned int        mCameraWidth;      
        unsigned int        mCameraHeight;     

        CInjectionParams() : 
            mCameraWidth(640), 
            mCameraHeight(480) { }
    };

    struct CAlternateParams
    {   
        /// \brief RatioA + RatioB must be a denominator of FPS
        unsigned int        mRatioA;      
        unsigned int        mRatioB;     

        CAlternateParams() : 
            mRatioA(1), 
            mRatioB(1) { }
    };

    struct CSensorParamsExt : public CSensorParams
    {
        bool                mIsInjectionMode; 
        bool                mIsSimulationMode;
        std::string         mSimulationInputFolder;

        /// \brief    if it is not empty then calibration data is loaded from this directory
        std::string         mCalibrationDirName;

        /// \brief     Injection settings
        CInjectionParams    mInjection;

        /// \brief     Alternate mode settings 
        CAlternateParams    mAlternate;

        CSensorParamsExt(const CSensorParams& iSensorParams=CSensorParams()) : 
            CSensorParams(iSensorParams), 
            mIsInjectionMode(false),
            mIsSimulationMode(false),
            mSimulationInputFolder(),
            mCalibrationDirName() {}
    };

    typedef unsigned char byte;


    struct CSensorRegisterParams
    {
        ESensorName mSensorNumber;
        unsigned int mAddress;
        unsigned int mData;
        unsigned int mSize;
        bool mVerify;               // "true" - verify that the load operation succeeds (performance overhead)
        std::string mFileName;  
    };

    struct CSensorI2CRegisterParams
    {
        unsigned int mI2CNumber;
        unsigned int mDeviceAddress;
        unsigned int mData;
        unsigned int mDataSize;
        unsigned int mRegAddress;
        unsigned int mRegAddressSize;

        CSensorI2CRegisterParams() {}

        CSensorI2CRegisterParams(unsigned int iI2CNumber, unsigned int iDeviceAddress,
                                 unsigned int iData, unsigned int iDataSize,
                                 unsigned int iRegAddress, unsigned int iRegAddressSize) :
            mI2CNumber(iI2CNumber), mDeviceAddress(iDeviceAddress), mData(iData), mDataSize(iDataSize), mRegAddress(iRegAddress),mRegAddressSize(iRegAddressSize) {}
    };

    struct CAgcParams 
    {
        enum EIntensityAlg
        {
            eNone = 0,      // No intensity balance algorithm
            eEqual,         // Equal weight of foreground and background
            eFG,            // Prefer foreground
            eBG,            // Prefer background
        };

        enum EIntensityLevel  {  eLow = 0, eMed,  eHigh};

        enum EPowerGridRate  {  e50 = 0, e60};

        CAgcParams()
        {
            // Set defaults
            mActivated = true;
            mAutoExposure = false;
            mIntensityAlg = eEqual;
            mForgroundLevel = eHigh; 
            mBackgroundLevel = eHigh;
            mAutoLumaTarget = false;
            mLumaTarget = 0x64;
            mEliminateFlickering = true;
            mPowerGridRate = e60;
            mGain[0] = mGain[1] = 0x1008;                 
            mShutter[0] =  mShutter[1] = 0xA7;
            mNewAlgorithm = 0;

        }

        bool            mActivated;         // True if filter is on

        // True for AE mode
        bool            mAutoExposure; 

        // Auto Exposure params
        EIntensityAlg   mIntensityAlg;
        EIntensityLevel mForgroundLevel;
        EIntensityLevel mBackgroundLevel;    
        bool            mAutoLumaTarget;
        byte            mLumaTarget;
        bool            mEliminateFlickering;
        EPowerGridRate  mPowerGridRate;    

        // Manual Exposure params
        unsigned short  mGain[2];       // Left, Right
        unsigned int    mShutter[2];    // Left, Right

        // Use the new algorithm
        int mNewAlgorithm;
    };

    struct COneSensorParams 
    {
        bool UseROI;
        CPoint<2, unsigned int> ROITopLeft;
        CPoint<2, unsigned int> ROIBottomRight;

        COneSensorParams () : UseROI(false), ROITopLeft(0,0), ROIBottomRight(0,0) {}
    };


    struct CSensorControlParamsExt :public CSensorControlParams
    {
  
        bool UseBasline;

        // for each sensor (use ESensorName to access them)
        COneSensorParams Params[2];

        CSensorControlParamsExt() : CSensorControlParams(), UseBasline(false)  { }
        CSensorControlParamsExt(const CSensorControlParams&  input) : CSensorControlParams(input), UseBasline(false)  { }
    };


    class CDepthStreamExt;
    class CVideoStreamExt;
    class CHeadStreamExt;
    class CHandsStreamExt;
    class CSpeechRecognitionExt;
    class CGazeStreamExt;
    class CSensorStreamExt;
    class CControllerStreamExt;
    class CFaceExpressionsStreamExt;
    class CWebCamStreamExt;
    class CAuxStreamExt;
    class CGeneralPurposeStreamExt;

    ///////////////////////////////////////////////////////////////////////
    // Class: CInuSensor
    // Role: Creation of IAF Services which provide NU3000 extended data streams
    // Inherits: 
    // Responsibilities: 
    //      1. Implementation of Singleton design pattern
    //      2. Knows how to generate all kinds of IAF Services
    // Comments:
    ///////////////////////////////////////////////////////////////////////
    class CInuSensorExt  : public CInuSensor
    {

    public:
        virtual ~CInuSensorExt() {}
        /// \brief    Access to the Singleton object
        INUSTREAMS_API static std::shared_ptr<CInuSensorExt>  Create(const std::string& iSensorID = std::string());

        // IAF Services
        virtual std::shared_ptr<CDepthStreamExt>            CreateDepthStreamExt(uint32_t iChannelID = 0) = 0; 
        virtual std::shared_ptr<CVideoStreamExt>            CreateVideoStreamExt(uint32_t iChannelID = 0) = 0;
        virtual std::shared_ptr<CHeadStreamExt>             CreateHeadStreamExt(uint32_t iChannelID = 0) = 0;
        virtual std::shared_ptr<CHandsStreamExt>            CreateHandsStreamExt(uint32_t iChannelID = 0) = 0;
        virtual std::shared_ptr<CGazeStreamExt>             CreateGazeStreamExt(uint32_t iChannelID = 0) = 0;
        virtual std::shared_ptr<CFaceExpressionsStreamExt>  CreateFaceExpressionsStreamExt(uint32_t iChannelID = 0) = 0;
        virtual std::shared_ptr<CWebCamStreamExt>           CreateWebCamStreamExt(uint32_t iChannelID = 0) = 0;
        virtual std::shared_ptr<CAuxStreamExt>              CreateAuxStreamExt(uint32_t iChannelID = 0) = 0;
        virtual std::shared_ptr<CGeneralPurposeStreamExt>   CreateGeneralPurposeStreamExt(uint32_t iChannelID = 0) = 0;

        using CBaseStream::Init;
        using CInuSensor::Init;
        virtual CInuError Init(const CSensorParamsExt& params) = 0;

        virtual CInuError  SetSensorParams(const CSensorParamsExt& params) = 0;
        virtual CInuError  GetSensorParams(CSensorParamsExt& params) const = 0;

        //Allow setting individual camera settings.
        virtual CInuError  SetCameraSettings(const CSensorParams& iParams, ECameraName iCameraNumber) = 0;
        virtual CInuError  GetCameraSettings(CSensorParams& oParams, ECameraName iCameraNumber) = 0;


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
        virtual CInuError SetSensorRegister(unsigned int iAddress, unsigned int iData, unsigned int size, ESensorName SensorNumber) const  = 0;

        /// \brief GetSensorRegister gets a register value from a sensor
        /// 
        /// \param[in]  iAddress register address
        /// \param[out]  iData register value
        /// \param[in]  size data size
        /// \param[in]  SensorNumber sensor number numb
        /// \return CInuError
        virtual CInuError GetSensorRegister(unsigned int iAddress, unsigned int &iData, unsigned int size, ESensorName SensorNumber) const  = 0;

        /// \brief SetRegister Sets a register value in the firmware
        /// \param[in]  iAddress register address
        /// \return CInuError 
        virtual CInuError SetI2CRegister(const CSensorI2CRegisterParams& iParams) const  = 0;

        /// \brief GetRegister Gets a register value from the firmware
        /// \param[in,out]  iAddress register address
        /// \return CInuError 
        virtual CInuError GetI2CRegister(CSensorI2CRegisterParams& ioParams) const  = 0; 
        
        virtual CInuError SetConfiguration(ESensorName iSensorNumber) const  = 0;

        /// \brief Sensor Load Registers from file
        /// 
        /// \param[in]  iFileName file name containing registers values
        /// \param[in]  iVerify - if it is true then the function checks that load succeeds (by reading the value of each register) and 
        ///                       then the loading time is increased. 
        /// \return CInuError
        virtual CInuError LoadSensorRegistersFromFile(const std::string &iFileName, bool iVerify) const  = 0;

        virtual CInuError LoadRegistersFromFile(const std::string &iFileName, bool iVerify) const = 0;

        virtual CInuError EnableODPCalibrationMode(bool iEnabled) const = 0;


        /// \brief Record NU3000 streams
        /// \param[in]  iDestinationDirectory Destination directory for recording output. Send empty string to stop recording.
        /// \param[in]  iTemplateName string which will be concatenate to output file name.
        /// \param[in]  iDuration recording time in ms.
        /// \return CInuError eOK indicates that request was successfully processed but it doesn't indicate that recording is completed
        virtual CInuError  Record(const std::string& iDestinationDirectory,
                                  const std::string& iTemplateName = std::string(),
                                  uint64_t iDuration = CBaseStreamExt::RECORD_INFINITE) const  = 0;

        /// \brief Record NU3000 streams
        /// \param[in]  iDestinationDirectory Destination directory for recording output. Send empty string to stop recording.
        /// \param[in]  iTemplateName string which will be concatenate to output file name.
        /// \return CInuError eOK indicates that request was successfully processed but it doesn't indicate that recording is completed
        virtual CInuError  Snapshot(const std::string& iDestinationDirectory,
                                    const std::string& iTemplateName = std::string(),
                                    uint64_t iFileNameIndex = CBaseStreamExt::RECORD_INFINITE) const  = 0;


        // AGC
        //----
        virtual CInuError GetSensorControlParamsExt(CSensorControlParamsExt& oParams, uint32_t iChannel) const = 0;
        virtual CInuError SetSensorControlParamsExt(const CSensorControlParamsExt& iParams, uint32_t iChannel) const = 0;



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


        virtual CInuError SetPhaseRegister(unsigned int iAddress, unsigned int iData, EPhaseNumber iPhase) const = 0;
        virtual CInuError SetSensorPhaseRegister(unsigned int iAddress, unsigned int iData, EPhaseNumber iPhase, unsigned int iSize, ESensorName iSensorNumber) const = 0;

        virtual CInuError ActivatePhase() const = 0;
        virtual CInuError ClearPhase() const = 0;


        /// \brief SetRegister Sets a register value in the firmware
        /// \param[in]  iAddress register address
        /// \param[in]  iData new register value
        /// \return CInuError 
        virtual CInuError SetRegister(unsigned int iAddress, unsigned int iData) const  = 0;

        /// \brief GetRegister Gets a register value from the firmware
        /// \param[in,out]  iAddress register address
        /// \param[out]  iData register value
        /// \return CInuError 
        virtual CInuError GetRegister(unsigned int iAddress, unsigned int &iData) const  = 0;

        /// \brief Load Registers from file 
        /// \param[in]  iFileName file name containing registers values
        /// \param[in]  iVerify - if it is true then the function checks that load succeeds (by reading the value of each register) and 
        ///                       then the loading time is increased. 
        /// \return CInuError
        virtual CInuError LoadControlRegistersFromFile(const std::string &iFileName, bool iVerify) const = 0;

        //virtual CInuError GetVersion(std::vector<CEntityVersion>& version)  = 0;

        /// \brief    Reload persistent configurable parameters
        /// \param[in] iClient    reload for client (IAFClient).
        /// \param[in] iServer    reload for server (IAFServer).
        /// \return CInuError
        virtual CInuError ReloadConfiguration(bool iClient, bool iServer) = 0;



        /// \brief   Performs a memory scan in the target.
        /// \return CInuError   
        virtual CInuError PerformMemoryScan(unsigned int& iMemoryScanResult) = 0; 

        /// \brief   Reads a Memory buffer from the target.
        /// 
        /// Reads a Memory buffer from the target and saves it to the file name.
        /// 
        /// \param[in,out] iStartAddress    Initial read address.
        /// \param[in,out] iSize            Buffer size to read.
        /// \param[in,out] iFileName        File that will store the read data.
        /// \return CInuError    
        virtual CInuError ReadMemoryBuffer(unsigned int iStartAddress, unsigned int iSize, std::string iFileName)  = 0; 

        /// \brief    Writes a Memory buffer to the target.
        /// 
        /// Sends a buffer that is contained in the file to the target.
        /// \param[in,out] iStartAddress    Initial write address.
        /// \param[in,out] iFileName        File that contains the buffer to send.
        /// \return CInuError    
        virtual CInuError WriteMemoryBuffer(unsigned int iStartAddress, std::string iFileName)  = 0; 

        /// \brief    Save camera data to Sensor's flash memory 
        /// 
        /// The folder should be organized in recognized format. The folder's contents is compressed and saved to Sensor's flash.
        /// This data will be retrieved automatically and used anytime InuSensor is connected.  
        /// \param[in] iCameraFolder    Folder which contains camera data that should be saved.
        /// \param[in] iSensorInfo    input sensor description.
        /// \return CInuError    Error if the save operation failed
        virtual CInuError WriteCameraData(const std::string& iCameraFolder, const std::vector<CEntityVersion>& iSensorInfo)  = 0; 

        /// \brief    Show (in log file) statistics data which is collected by the FW
        /// \return CInuError
        virtual CInuError ShowStatistics() = 0;



    protected:
        CInuSensorExt() {}
    };

}

#endif // __PROVIDEREXT_H__

