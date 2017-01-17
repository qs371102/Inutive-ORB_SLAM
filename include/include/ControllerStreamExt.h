
#ifndef __CONTROLLERSTREAMEXT_H__
#define __CONTROLLERSTREAMEXT_H__

#include <map>
#include <string>
#include <vector>
#include <climits>
#include "InuSensorExt.h"


namespace  InuDev
{
    struct CControllerParams    
    {
        unsigned int mAddress;
        unsigned int mData;
        unsigned int mSize;
        EPhaseNumber mPhase;
        ESensorName   mSensor;
        char mFileName[MAX_PATH];      // Registers file
        bool mVerify;               // "true" - verify that the load operation succeeds (performance overhead)
    };

    ////////////////////////////////////////////////////////////////////////
    /// \brief   Gives basic control over the NU3000 
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
    class CControllerStreamExt : public CBaseStream, public CBaseStreamExt
    {

    public:
        CControllerStreamExt() {}
        virtual ~CControllerStreamExt() {}

        using   CBaseStream::Init;
        virtual CInuError  Init(const CSensorParamsExt& iParams) = 0;

        /// \brief SetRegister Sets a register value in the firmware
        /// \param[in]  iAddress register address
        /// \param[in]  iData new register value
        /// \return CInuError 
        virtual CInuError SetRegister(unsigned int iAddress, unsigned int iData) const  = 0;

        virtual CInuError SetPhaseRegister(unsigned int iAddress, unsigned int iData, EPhaseNumber iPhase) const = 0;

        virtual CInuError ActivatePhase() const = 0;
        virtual CInuError ClearPhase() const = 0;


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
        virtual CInuError LoadRegistersFromFile(const std::string &iFileName, bool iVerify) const  = 0;

        virtual CInuError GetTargetVersion(std::map<CEntityVersion::EEntitiesID,CEntityVersion>& version)  = 0;

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
        /// This data will be retrieved automatically and used anytime Controller service is initialized.  
        /// \param[in] iCameraFolder    input camera file.
        /// \param[in] iSensorID    input sensor ID.
        /// \return CInuError   eWriteToFlash 
        virtual CInuError WriteCameraData(const std::string& iCameraFolder, const std::vector<CEntityVersion>& iSensorInfo)  = 0; 

        /// \brief    Show (in log file) statistics data which is collected by the FW
        /// \return CInuError
        virtual CInuError ShowStatistics() = 0;

        /// \brief  Enable/Disable Calibration mode
        /// \return CInuError
        virtual CInuError EnableODPCalibrationMode(bool iEnabled) const = 0;
    };
}

#endif // __CONTROLLERSTREAMEXT_H__

