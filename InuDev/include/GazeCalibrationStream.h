/*
* File - GazeCalibrationFrame.h
*
* This file is part of Inuitive API Framework  and it contains declaration of classes CGazeCalibrationFrame and CGazeCalibrationStream.
*
*/

#ifndef __GAZECALIBRATIONSTREAM_H__
#define __GAZECALIBRATIONSTREAM_H__

#include "StreamsExport.h"

#include "BaseStream.h"

#include <functional>

namespace InuDev 
{

    ////////////////////////////////////////////////////////////////////////
    /// \brief  Gaze calibration frame provided by CGazeCalibrationStream
    ///
    /// Role: Data of one gaze calibration frame
    ////////////////////////////////////////////////////////////////////////
    struct CGazeCalibrationFrame : public CBaseFrame
    { 
        // Upper and lower limits of Progress 
        static const unsigned int PROGRESS_MIN=0;
        static const unsigned int PROGRESS_MAX=100;

        /// \brief    Client application should place an icon on screen at this position.
        ///
        /// User should look at the icon in order to calibrate his gaze parameters.
        CPoint2D Position;

        /// \brief    Gaze calibration progress indicator from PROGRESS_MIN up to PROGRESS_MAX.
        unsigned int Progress;
    };


    ////////////////////////////////////////////////////////////////////////
    /// \brief   Interface for Gaze Calibration service.
    /// 
    /// Role: Control gaze calibration process and provides appropriate frames. 
    ///
    /// Responsibilities: 
    ///      1. Knows how to control the service (Init, Terminate, Start and Stop).
    ///      2. Knows how to acquire one gaze calibration frame (pull)
    ///      3. Knows how to provide a continuous stream of gaze calibration frames (push)
    ///      4. Knows if there is previous calibration data for specific user that can be used.
    /// 
    /// Comments: 
    ///      Gaze calibration is required in order to receive accurate gaze indication.
    ///      Calibration should be done for each user and the resulted calibrated data is saved
    ///      persistently by IAF for future use.
    ///
    ///      Calibration process is performed by placing any visual target on screen for some time
    ///      and the user should look at this target. The progress of the process can be monitored
    ///      by reading the value of CGazeCalibrationFrame::Progress.
    ///
    ///      At the end of the process a score is given to calibration results, and the client may
    ///      repeat the process for insufficient value. Score can be accessed through CBaseFrame class.
    ///
    ////////////////////////////////////////////////////////////////////////
    class CGazeCalibrationStream : public CBaseStream
    {

    public:

        /// \brief    Prototype of callback function which is used by the Register method.
        ///
        /// This function receive one gaze calibration frame.
        typedef std::function<void(std::shared_ptr<CGazeCalibrationStream>, const CGazeCalibrationFrame&,  CInuError)> CallbackFunction;

        virtual ~CGazeCalibrationStream() {}


        /// \brief    Check if gaze has been calibrated for input user ID.
        /// 
        /// \param[in] iUid    Unique ID of user.
        ///
        /// \return bool    true if and only if gaze has been calibrated for input user ID.
        virtual bool              IsCalibrated(std::string iUid) const = 0;

        /// \brief    Retrieve one gaze calibration frame (pull)
        /// 
        /// This method returns when a new frame is ready (blocking). 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        ///
        /// \param[out] oGazeCalibrationFrame    Returned gaze calibration frame.
        ///
        /// \return InuDev::CInuError    Operation status which indicates on success or failure.
        virtual CInuError         GetFrame(CGazeCalibrationFrame& oGazeCalibrationFrame, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

        /// \brief    Registration for receiving gaze calibration frames (push) 
        /// 
        /// Provided callback function is called when a new frame is ready (non-blocking). 
        /// It shall be called only after a Start() was is invoked and but before any invocation of a Stop() is invoked.
        ///
        /// \param[in] iCallback    Callback function which is invoked when a new frame is ready
        ///
        /// \return CInuError    Operation status which indicates on success or failure.
        virtual CInuError         Register(CallbackFunction iCallback, unsigned int iTimeout = FPS_BASED_TIMEOUT) = 0;

    };

}


#endif // __GAZECALIBRATION_H__
