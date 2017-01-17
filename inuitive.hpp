#ifndef __INUTIVE_H_
#define __INUTIVE_H_

#include "InuSensor.h"
#include "InuSensorExt.h"
#include "DepthStream.h"
#include "VideoStream.h"
#include "WebCamStream.h"
#include "AuxStream.h"
#include "HeadStream.h"
#include "HandsStream.h"
#include "GazeStream.h"
#include "GeneralPurposeStream.h"

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#define Rad2Deg 57.2957795130823208767981548141052
#define Deg2Rad 0.0174532922222222

#define Kp 2.0f   // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.01f   // integral gain governs rate of convergence of gyroscope biases


using namespace std;
using namespace cv;

class Inuitive
{
public:
	Inuitive();
	~Inuitive();

	bool Stop();
    bool Start();
	bool StartDepth();
	bool StartCamera();
    bool StartAux();
	bool StartVideo();
    bool StartHead();
    bool StartHands();
    bool StartGaze();
    bool StartGPP();

    void SaveDepthFrams(std::shared_ptr<InuDev::CDepthStream> iStream, 
                    const InuDev::CImageFrame& iFrame, InuDev::CInuError retCode);
	void SaveCameraFrams(std::shared_ptr<InuDev::CWebCamStream> iStream, 
                    const InuDev::CImageFrame& iFrame, InuDev::CInuError retCode);

    void SaveAuxFrams(std::shared_ptr<InuDev::CAuxStream> iStream, 
                    const InuDev::CImuFrame& iFrame, InuDev::CInuError retCode);
    void SaveVideoFrams(std::shared_ptr<InuDev::CVideoStream> iStream,
		            const InuDev::CVideoFrame &iFrame, InuDev::CInuError retCode);
    void SaveHeadFrams(std::shared_ptr<InuDev::CHeadStream> iStream,
		            const InuDev::CHeadFrame &iFrame, InuDev::CInuError retCode);
    void SaveHandsFrams(std::shared_ptr<InuDev::CHandsFrame> iStream, 
                    const InuDev::CHandsFrame& iFrame, InuDev::CInuError retCode);
    void SaveGazeFrams(std::shared_ptr<InuDev::CGazeStream> iStream, 
                    const InuDev::CGazeFrame& iFrame, InuDev::CInuError retCode);
    void SaveGeneralPurposeFrams(std::shared_ptr<InuDev::CGeneralPurposeStream> iStream, 
                    const InuDev::CGeneralFrame& iFrame, InuDev::CInuError retCode); 

    int getRGB_Depth(cv::Mat& rgb, cv::Mat& depth) {
        if (m_rgb_flag && m_depth_flag && (m_rgb_depth_index[0] == m_rgb_depth_index[1])) {        
			rgb = m_rgb;
            depth = m_depth;
			m_rgb_flag = false;
            m_depth_flag = false;
			return 0; 
        }
        return -1;
    };

    int getImuData(std::vector<double>& imu_data) {
        if (m_imu_flag) {
            imu_data = m_imu_data;
            m_imu_flag = false;
            return 0;
        }
        return -1; 
    };


private:
    cv::Mat m_rgb;
    cv::Mat m_depth;

    std::vector<unsigned int> m_rgb_depth_index;
	double  m_FPS;
    std::vector<double> m_imu_data;
	double DigitalGainRight_, DigitalGainLeft_, AnalogGainRight_,
			AnalogGainLeft_, ExposureTimeLeft_, ExposureTimeRight_;

	InuDev::COpticalData OpticalData;
	std::shared_ptr<InuDev::CInuSensor>     inuSensor;
	std::shared_ptr<InuDev::CDepthStream>   depthStream;
	std::shared_ptr<InuDev::CWebCamStream>  cameraStream;
	std::shared_ptr<InuDev::CAuxStream>     auxStream;
	std::shared_ptr<InuDev::CVideoStream>   videoStream;
	std::shared_ptr<InuDev::CHeadStream>    headStream;
	std::shared_ptr<InuDev::CHandsStream>   handsStream;
	std::shared_ptr<InuDev::CGazeStream>    gazeStream;
	std::shared_ptr<InuDev::CGeneralPurposeStream>  generalPurposeStream;

	bool initSuccessful;
	void getOpticalParam();

    bool m_rgb_flag, m_depth_flag, m_gp_flag;
    bool m_gyro_flag, m_acc_flag, m_imu_flag;

    bool depth_enable, video_enable, camera_enable, IMU_enable;
	bool head_enable, hands_enable, gaze_enable, generalPurpose_enable;
};

#endif /* #define __INUTIVE_H_ */
