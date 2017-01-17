#include "inuitive.hpp"
#include <sys/time.h>


#define TEST_VIDEO 0

struct  timeval start;

Inuitive::Inuitive():m_imu_data(10, 0.0), m_rgb_depth_index(2, 0), m_rgb_flag(false), 
			m_depth_flag(false), m_gyro_flag(false), m_acc_flag(false), m_imu_flag(false),
			m_gp_flag(false)
{
	depth_enable	= true;
#if TEST_VIDEO
	video_enable	= true;
#else
	video_enable	= false;
#endif
	camera_enable	= true;
	IMU_enable		= false;
	head_enable		= false;
	hands_enable	= false;
	gaze_enable		= false;
	generalPurpose_enable	= false;

	m_FPS = 15.0;
	DigitalGainRight_ = 15.0;
    DigitalGainLeft_ = 15.0;
    AnalogGainRight_ = 0.0;
    AnalogGainLeft_ = 0.0;
    ExposureTimeLeft_ = 1000.0;
    ExposureTimeRight_ = 1000.0;
	initSuccessful = true;
#if TEST_VIDEO
	cv::namedWindow("videoL", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("videoR", cv::WINDOW_AUTOSIZE);
#endif
}

Inuitive::~Inuitive()
{
}


void Inuitive::SaveCameraFrams(std::shared_ptr<InuDev::CWebCamStream> iStream, const InuDev::CImageFrame &iFrame, InuDev::CInuError retCode)
{
    if (retCode != InuDev::eOK) {
        std::cout << "Error in camera receiving frame: " << std::hex << int(retCode) << " " << std::string(retCode) << std::endl;
        return;
    }

	if (iFrame.Valid == false) {
        std::cout << "Frame(camera)  " << iFrame.FrameIndex << " is invalid" << std::endl;
        return;
    }
	m_rgb_depth_index[0] = iFrame.FrameIndex;
	cv::Mat video_rgb8;
    if(iFrame.Format()==InuDev::CImageFrame::eBGR){
    	cv::Mat camera_raw(iFrame.Height(), iFrame.Width(), CV_8UC3, (void *)iFrame.GetData(), iFrame.Width()*3);		
		cv::cvtColor(camera_raw, video_rgb8, CV_BGR2RGB);//CV_BGR5652RGB		
		m_rgb = camera_raw.clone();
		m_rgb_flag = true;
		//cout << "eBGR" << endl;
    }
    if(iFrame.Format()==InuDev::CImageFrame::eRGB565){
    	cv::Mat camera_raw(iFrame.Height(), iFrame.Width(), CV_8UC2, (void *)iFrame.GetData(), iFrame.Width()*2);
		cv::cvtColor(camera_raw, video_rgb8, CV_BGR5652RGB);//CV_BGR5652RGB
		m_rgb = video_rgb8.clone();
		m_rgb_flag = true;
		//cout << "eRGB565" << endl;    
	}
    if(iFrame.Format()==InuDev::CImageFrame::eBGRA){
    	cv::Mat camera_raw(iFrame.Height(), iFrame.Width(), CV_8UC4, (void *)iFrame.GetData(), iFrame.Width()*4);
		cv::cvtColor(camera_raw, video_rgb8, CV_BGRA2RGB);//CV_BGR5652RGB
		m_rgb = camera_raw.clone();
		m_rgb_flag = true;
		//cout << "eBGRA" << endl;    
	}
	if(iFrame.Format()==InuDev::CImageFrame::eYUV422){
    }
	//cout << "size:(HxW) " << iFrame.Height() << "\t" << iFrame.Width() << endl;
	//cout << iFrame.Format()<< endl;    
/*
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
*/
	//cv::imshow("rgb", video_rgb8);
	//cv::waitKey(1);
}

void Inuitive::SaveDepthFrams(std::shared_ptr<InuDev::CDepthStream> iStream, const InuDev::CImageFrame&  iFrame, InuDev::CInuError retCode)
{
    if (retCode != InuDev::eOK) {
        std::cout << "Error in depth receiving frame: " << std::hex << int(retCode) << " " << std::string(retCode) << std::endl;
        return;
    }

    if (iFrame.Valid == false) {
        std::cout << "Frame(depth)  " << iFrame.FrameIndex << " is invalid" << std::endl;
        return;
    }

	cv::Mat depth(iFrame.Height(), iFrame.Width(), CV_16UC1, (void *)iFrame.GetData(), iFrame.Width() * 2);
	m_depth = depth.clone();
	m_depth_flag = true;
	m_rgb_depth_index[1] = iFrame.FrameIndex;
}

bool Inuitive::StartDepth()
{
	depthStream=inuSensor->CreateDepthStream();
    if (depthStream == nullptr) {
        std::cout << "Unexpected error, failed to get Depth Stream" << std::endl;
        return false;
    }
	InuDev::CInuError retCode = depthStream->Init();//InuDev::CDepthStream::eDepthRegistration);
    if (retCode != InuDev::eOK) {
        std::cout << "Depth initiation error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    }
	std::cout << "Depth Stream is initialized" << std::endl;

	InuDev::CSensorControlParams cam_para;
	inuSensor->GetSensorControlParams(cam_para, 0);
	cam_para.DigitalGainRight = (unsigned int)DigitalGainRight_;
	cam_para.DigitalGainLeft = (unsigned int)DigitalGainLeft_;
	cam_para.AnalogGainRight = (unsigned int)AnalogGainRight_;
	cam_para.AnalogGainLeft = (unsigned int)AnalogGainLeft_;
	cam_para.ExposureTimeLeft = (unsigned int)ExposureTimeLeft_;
	cam_para.ExposureTimeRight = (unsigned int)ExposureTimeRight_;
	inuSensor->SetSensorControlParams(cam_para, 0);
    
	retCode = depthStream->SetPostProcess(InuDev::eDisabled);
	if (retCode != InuDev::eOK) {
        std::cout << "Depth SetPostProcess error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    }
	
	retCode = depthStream->Start();
    if (retCode != InuDev::eOK) {
        std::cout << "Depth start error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    }
    std::cout << "Depth frames acquisition started" << std::endl;

	retCode = depthStream->Register(std::bind(&Inuitive::SaveDepthFrams, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    if (retCode != InuDev::eOK) {
        std::cout << "Depth register error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    } 
	return true;

}
bool Inuitive::StartCamera()
{
	cameraStream = inuSensor->CreateWebCamStream();
	if (cameraStream == nullptr) {
        std::cout << "Unexpected error, failed to get Camera Stream" << std::endl;
        return false;
    }

	InuDev::CInuError retCode = cameraStream->Init(InuDev::CWebCamStream::eDefault);
	 if (retCode != InuDev::eOK) {
        std::cout << "Camera initiation error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    }
	std::cout << "Camera Stream is initialized" << std::endl;

	retCode = cameraStream->Start();
	if (retCode != InuDev::eOK) {
        std::cout << "Camera start error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    }
	std::cout << "Camera frames acquisition started" << std::endl;

	retCode = cameraStream->Register(
		          std::bind(&Inuitive::SaveCameraFrams, this, std::placeholders::_1,
		                    std::placeholders::_2, std::placeholders::_3));

	if (retCode != InuDev::eOK) {
		printf("Camera register error: %d\n", (int)retCode);
		return false;
	}
	return true;
}

bool Inuitive::Start()
{
    inuSensor = InuDev::CInuSensor::Create();

	InuDev::CSensorParams iSensorParams;
	iSensorParams.FPS = m_FPS;
	iSensorParams.SensorRes = InuDev::eFull;//eFull eBinning;
	InuDev::CInuError retCode = inuSensor->Init(iSensorParams);

    if (retCode != InuDev::eOK) {
        std::cout << "Failed to connect to Inuitive Sensor. Error: " << std::hex  << int(retCode) << " - " << std::string(retCode) << std::endl;
        return false;
    }
    std::cout << "Connected to Sensor" << std::endl;
	retCode = inuSensor->Start();
    if (retCode != InuDev::eOK) {
        std::cout << "Failed to start to Inuitive Sensor." << std::endl;
        return false;
    }
    std::cout << "Sensor is started" << std::endl;
	getOpticalParam();

	if (depth_enable){
		if (!StartDepth()) {
			initSuccessful = false;
			return false;
		}
	}
	if (camera_enable) {
		if (!StartCamera()){
			initSuccessful = false;
			return false;
		}
	}
	if (video_enable){
		if (!StartVideo()) {
			initSuccessful = false;
			return false;
		}
	}
	if (IMU_enable) {
		if (!StartAux()){
			initSuccessful = false;
			return false;
		}
	}
	if (head_enable){
		if (!StartHead()) {
			initSuccessful = false;
			return false;
		}
	}
	if (hands_enable) {
		if (!StartHands()){
			initSuccessful = false;
			return false;
		}
	}
	if (gaze_enable){
		if (!StartGaze()) {
			initSuccessful = false;
			return false;
		}
	}
	if (generalPurpose_enable) {
		if (!StartGPP()){
			initSuccessful = false;
			return false;
		}
	}

    return true;
}

bool Inuitive::Stop()
{
	::sleep(5000);
	InuDev::CInuError retCode(0);
	if (depth_enable) {
		retCode = depthStream->Register(nullptr);
		if (retCode != InuDev::eOK) {
			std::cout << "Depth unregister error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
			return false;
		}
		retCode = depthStream->Stop();
		if (retCode != InuDev::eOK) {
		    std::cout << "Depth Stop error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
		    return false;
		}
		std::cout << "Depth frames acquisition stopped" << std::endl;
		retCode = depthStream->Terminate();
		if (retCode != InuDev::eOK) {
		    std::cout << "Depth Terminate error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
		    return false;
		}
		std::cout << "Depth Stream was finalized" << std::endl;
	}
	if (camera_enable) {
		retCode = cameraStream->Register(nullptr);
		if (retCode != InuDev::eOK) {
			std::cout << "Camera unregister error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
			return false;
		}
		retCode = cameraStream->Stop();
		if (retCode != InuDev::eOK) {
			std::cout << "Camera unregister error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
			return false;
		}
		std::cout << "Camera frames acquisition stopped" << std::endl;
		retCode = cameraStream->Terminate();
		if (retCode != InuDev::eOK) {
		    std::cout << "Camera Terminate error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
		    return false;
		}
		std::cout << "Camera Stream was finalized" << std::endl;
	}

	if(video_enable){
    	videoStream->Register((InuDev::CVideoStream::CallbackFunction)nullptr);
		if (retCode != InuDev::eOK) {
			std::cout << "Video unregister error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
			return false;
		}
    	videoStream->Stop();
		if (retCode != InuDev::eOK) {
			std::cout << "Video unregister error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
			return false;
		}
		std::cout << "Camera frames acquisition stopped" << std::endl;
    	videoStream->Terminate();
		if (retCode != InuDev::eOK) {
		    std::cout << "Video Terminate error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
		    return false;
		}
		std::cout << "Video Stream was finalized" << std::endl;
	}

    if(IMU_enable){
		auxStream->Register((InuDev::CAuxStream::ImuCallbackFunction) nullptr);
		if (retCode != InuDev::eOK) {
			std::cout << "Aux unregister error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
			return false;
		}
		auxStream->Stop();
		if (retCode != InuDev::eOK) {
			std::cout << "Aux unregister error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
			return false;
		}
		std::cout << "Aux frames acquisition stopped" << std::endl;
		auxStream->Terminate();
    	if (retCode != InuDev::eOK) {
		    std::cout << "Aux Terminate error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
		    return false;
		}
		std::cout << "Aux Stream was finalized" << std::endl;
    }

    if(head_enable){
    	headStream->Register((InuDev::CHeadStream::CallbackFunction) nullptr);
		if (retCode != InuDev::eOK) {
			std::cout << "Head unregister error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
			return false;
		}
    	headStream->Stop();
		if (retCode != InuDev::eOK) {
			std::cout << "Head unregister error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
			return false;
		}
		std::cout << "Head frames acquisition stopped" << std::endl;
    	headStream->Terminate();
    	if (retCode != InuDev::eOK) {
		    std::cout << "Heads Terminate error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
		    return false;
		}
		std::cout << "Heads Stream was finalized" << std::endl;
	}

    if(hands_enable){
    	handsStream->Register((InuDev::CHandsStream::CallbackFunction) nullptr);
		if (retCode != InuDev::eOK) {
			std::cout << "Hands unregister error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
			return false;
		}
    	handsStream->Stop();
		if (retCode != InuDev::eOK) {
			std::cout << "Hands unregister error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
			return false;
		}
		std::cout << "Hands frames acquisition stopped" << std::endl;
    	handsStream->Terminate();
    	if (retCode != InuDev::eOK) {
		    std::cout << "Hands Terminate error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
		    return false;
		}
		std::cout << "Hands Stream was finalized" << std::endl;
    }

    if(gaze_enable){
    	gazeStream->Register((InuDev::CGazeStream::CallbackFunction) nullptr);
		if (retCode != InuDev::eOK) {
			std::cout << "Gaze unregister error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
			return false;
		}
    	gazeStream->Stop();
		if (retCode != InuDev::eOK) {
			std::cout << "Gaze unregister error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
			return false;
		}
		std::cout << "Gaze frames acquisition stopped" << std::endl;
    	gazeStream->Terminate();
    	if (retCode != InuDev::eOK) {
		    std::cout << "Gaze Terminate error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
		    return false;
		}
		std::cout << "Gaze Stream was finalized" << std::endl;
    }

    if(generalPurpose_enable){
    	generalPurposeStream->Register((InuDev::CGeneralPurposeStream::CallbackFunction) nullptr);
		if (retCode != InuDev::eOK) {
			std::cout << "GP unregister error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
			return false;
		}    	
		generalPurposeStream->Stop();
		if (retCode != InuDev::eOK) {
			std::cout << "GP unregister error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
			return false;
		}
		std::cout << "GP frames acquisition stopped" << std::endl;
    	generalPurposeStream->Terminate();
    	if (retCode != InuDev::eOK) {
		    std::cout << "GP Terminate error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
		    return false;
		}
		std::cout << "GP Stream was finalized" << std::endl;
    }


	retCode = inuSensor->Stop();
    if (retCode != InuDev::eOK) {
        std::cout << "Failed to stop CInuSensor, error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    }
    std::cout << "Sensor was stopped" << std::endl;
	retCode = inuSensor->Terminate();
    if (retCode != InuDev::eOK) {
        std::cout << "Failed to terminate CInuSensor, error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    }
    std::cout << "Disconnected from Sensor" << std::endl;
	return true;
}


void Inuitive::SaveAuxFrams(std::shared_ptr<InuDev::CAuxStream> iStream, const InuDev::CImuFrame &iFrame, InuDev::CInuError retCode)  // Error code (eOK if frame was successfully acquired)
{

    if (retCode != InuDev::eOK) {
        std::cout << "Error in receiving frame: " << std::hex << int(retCode) << " "
                  << std::string(retCode) << std::endl;
        return;
    }
	//std::cout << std::endl << "AUX:[" <<  iFrame.Timestamp << "]" << std::endl;
    std::map<InuDev::ESensorTypes, InuDev::CPoint3D> SensorsData = iFrame.SensorsData;
    for (std::map<InuDev::ESensorTypes, InuDev::CPoint3D>::iterator it =
                SensorsData.begin();
            it != SensorsData.end(); ++it) {
		if (it->first == InuDev::ESensorTypes::eAccelarometer){
			//std::cout << "Acce: " << it->second.X() << " " << it->second.Y() << " " << it->second.Z() << std::endl;
			m_imu_data[0] = (iFrame.Timestamp);
			m_imu_data[1] = it->second.X();
			m_imu_data[2] = it->second.Y();
			m_imu_data[3] = it->second.Z();
			m_acc_flag = true;
		}
		if (it->first == InuDev::ESensorTypes::eGyroscope) {
			//std::cout <<  "Gyro: " << it->second.X() << " " << it->second.Y() << " " << it->second.Z() << std::endl;
			m_imu_data[0] = (iFrame.Timestamp);
			m_imu_data[4] = it->second.X();
			m_imu_data[5] = it->second.Y();
			m_imu_data[6] = it->second.Z();
			m_gyro_flag = true;
		}
		if (it->first == InuDev::ESensorTypes::eMagnetometer) {
			//std::cout <<  "Magn: " << it->second.X() << " " << it->second.Y() << " " << it->second.Z() << std::endl;
			m_imu_data[7] = it->second.X();
			m_imu_data[8] = it->second.Y();
			m_imu_data[9] = it->second.Z();
		}

        if (m_gyro_flag & m_acc_flag) {
            m_gyro_flag = false;
            m_acc_flag = false;
			m_imu_flag = true;
        }
    }
}
void Inuitive::SaveHeadFrams(std::shared_ptr<InuDev::CHeadStream> iStream,
                    const InuDev::CHeadFrame &iFrame, InuDev::CInuError retCode){
	//std::cout<<iFrame.UserID<<std::endl;
	//std::cout<<"head orient:[%lf" << "," << iFrame.Quaternion.W() << "," << iFrame.Quaternion.X() << "," << iFrame.Quaternion.Y() << "," << iFrame.Quaternion.Z()"]" << std::endl;
}

void Inuitive::SaveHandsFrams(std::shared_ptr<InuDev::CHandsFrame> iStream, 
                    const InuDev::CHandsFrame& iFrame, InuDev::CInuError retCode){

}

void Inuitive::SaveGazeFrams(std::shared_ptr<InuDev::CGazeStream> iStream,
                    const InuDev::CGazeFrame &iFrame, InuDev::CInuError retCode){
}

void Inuitive::SaveGeneralPurposeFrams(std::shared_ptr<InuDev::CGeneralPurposeStream> iStream,
                    const InuDev::CGeneralFrame &iFrame, InuDev::CInuError retCode){

}
	
void Inuitive::SaveVideoFrams(
    std::shared_ptr<InuDev::CVideoStream> iStream,  // Parent Stream
    const InuDev::CVideoFrame &iFrame,               // Acquired frame
    InuDev::CInuError
    retCode)  // Error code (eOK if frame was successfully acquired)
{
#if TEST_VIDEO
    if (retCode != InuDev::eOK) {
        std::cout << "Error in receiving frame: " << std::hex << int(retCode) << " "
                  << std::string(retCode) << std::endl;
        return;
    }
    const InuDev::CImageFrame* LFrame = iFrame.GetLeftFrame();
    const InuDev::CImageFrame* RFrame = iFrame.GetRightFrame();

    cv::Mat videoR,videoL;
    cv::Mat videoL_raw(LFrame->Height(), LFrame->Width(), CV_8UC4, (void *)LFrame->GetData(), LFrame->Width() * 4);
    cv::Mat videoR_raw(RFrame->Height(), RFrame->Width(), CV_8UC4, (void *)RFrame->GetData(), RFrame->Width() * 4);

    cv::cvtColor(videoL_raw, videoL, CV_BGRA2RGB);
    cv::cvtColor(videoR_raw, videoR, CV_BGRA2RGB);
	cv::imshow("videoL", videoL);
	cv::waitKey(1);
	cv::imshow("videoR", videoR);
	cv::waitKey(1);
#endif
}


void Inuitive::getOpticalParam()
{
    inuSensor->GetOpticalData(OpticalData);
    printf("OpticalData Version: %d\n",OpticalData.Version);
    printf("OpticalData Left IR: Center [%lf, %lf], Focal:%lf\n",OpticalData.CenterL[0],OpticalData.CenterL[1],OpticalData.FocalL);
    printf("OpticalData Right IR: Center [%lf, %lf], Focal:%lf\n",OpticalData.CenterR[0],OpticalData.CenterR[1],OpticalData.FocalR);
    printf("OpticalData Baseline: %lf\n",OpticalData.BaseLine);
    printf("OpticalData Sensor size (resolution) [%lf, %lf]\n",OpticalData.EffectiveSize[0],OpticalData.EffectiveSize[1]);
    printf("OpticalData Correction for squint between left and right sensors: coefficients of linear fit <K0,K1> K0:%lf, K1:%lf\n",OpticalData.Squint[0],OpticalData.Squint[1]);
    if(!OpticalData.WebcamDataValid)
    	printf("OpticalData Webcam invalid");
    printf("OpticalData Webcam: Center [%lf, %lf], Focal [%lf, %lf]\n",OpticalData.WebcamCenter[0],OpticalData.WebcamCenter[1],OpticalData.WebcamFocal[0],OpticalData.WebcamFocal[1]);
    printf("OpticalData WebcamTranslate: [%lf, %lf, %lf]\n",OpticalData.WebcamTranslate[0],OpticalData.WebcamTranslate[1],OpticalData.WebcamTranslate[2]);
    printf("OpticalData WebcamRotate: [%lf, %lf, %lf]\n",OpticalData.WebcamRotate[0],OpticalData.WebcamRotate[1],OpticalData.WebcamRotate[2]);
    printf("OpticalData WebcamDistortion: [%lf, %lf, %lf, %lf, %lf]\n",OpticalData.WebcamDistortion[0],OpticalData.WebcamDistortion[1],OpticalData.WebcamDistortion[2],OpticalData.WebcamDistortion[3],OpticalData.WebcamDistortion[4]);
    printf("OpticalData TranslationUV(Internal use for fine tuning ): [%lf, %lf]\n",OpticalData.TranslationUV[0],OpticalData.TranslationUV[1]);

}

bool Inuitive::StartVideo()
{
	videoStream = inuSensor->CreateVideoStream();
	if (videoStream == nullptr) {
        std::cout << "Unexpected error, failed to get videoStream" << std::endl;
        return false;
    }
	InuDev::CInuError retCode = videoStream->Init();
	if (retCode != InuDev::eOK) {
        std::cout << "Video initiation error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    }
	std::cout << "Video Stream is initialized" << std::endl;
	retCode = videoStream->Start();
	if (retCode != InuDev::eOK) {
        std::cout << "Video start error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    }
	std::cout << "Camera frames acquisition started" << std::endl;
	retCode = videoStream->Register(std::bind(&Inuitive::SaveVideoFrams, this, 
					std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	if (retCode != InuDev::eOK) {
		printf("Video register error: %d\n", (int)retCode);
		return false;
	}
	return true;
}

bool Inuitive::StartAux()
{
	auxStream = inuSensor->CreateAuxStream();
	if (auxStream == nullptr) {
        std::cout << "Unexpected error, failed to get auxStream" << std::endl;
        return false;
    }
	InuDev::CInuError retCode = auxStream->Init();
	if (retCode != InuDev::eOK) {
        std::cout << "Aux initiation error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    }
	std::cout << "Aux Stream is initialized" << std::endl;

	retCode = auxStream->Start();
	if (retCode != InuDev::eOK) {
        std::cout << "Aux start error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    }
	std::cout << "Aux frames acquisition started" << std::endl;

	retCode = auxStream->Register(
		          std::bind(&Inuitive::SaveAuxFrams, this, std::placeholders::_1,
		                    std::placeholders::_2, std::placeholders::_3));
	if (retCode != InuDev::eOK) {
		printf("Aux register error: %d\n", (int)retCode);
		return false;
	}
	return true;
}

bool Inuitive::StartHead()
{
	headStream = inuSensor->CreateHeadStream();
	if (headStream == nullptr) {
        std::cout << "Unexpected error, failed to get headStream" << std::endl;
        return false;
    }
	InuDev::CInuError retCode = headStream->Init();
	if (retCode != InuDev::eOK) {
        std::cout << "head initiation error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    }
	std::cout << "head Stream is initialized" << std::endl;

	retCode = headStream->Start();
	if (retCode != InuDev::eOK) {
        std::cout << "head start error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    }
	std::cout << "head frames acquisition started" << std::endl;

	retCode = headStream->Register(
                      std::bind(&Inuitive::SaveHeadFrams, this, std::placeholders::_1,
                                std::placeholders::_2, std::placeholders::_3));
    if (retCode != InuDev::eOK) {
        printf("hands register error: %d\n", (int)retCode);
        return false;
    }
	return true;
}

bool Inuitive::StartHands()
{
/*
	handsStream = inuSensor->CreateHandsStream();
	if (handsStream == nullptr) {
        std::cout << "Unexpected error, failed to get handsStream" << std::endl;
        return false;
    }
	InuDev::CInuError retCode = handsStream->Init();
	if (retCode != InuDev::eOK) {
        std::cout << "hands initiation error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    }
	std::cout << "hands Stream is initialized" << std::endl;

	retCode = handsStream->Start();
	if (retCode != InuDev::eOK) {
        std::cout << "hands start error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    }
	std::cout << "hands frames acquisition started" << std::endl;

	retCode = handsStream->Register(std::bind(&Inuitive::SaveHandsFrams, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	if (retCode != InuDev::eOK) {
		printf("Hands register error: %d\n", (int)retCode);
		return false;
	}
	return true;

*/
}

bool Inuitive::StartGaze()
{
	gazeStream = inuSensor->CreateGazeStream();
	if (gazeStream == nullptr) {
        std::cout << "Unexpected error, failed to get gazeStream" << std::endl;
        return false;
    }
	InuDev::CInuError retCode = gazeStream->Init();
	if (retCode != InuDev::eOK) {
        std::cout << "gaze initiation error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    }
	std::cout << "gaze Stream is initialized" << std::endl;

	retCode = gazeStream->Start();
	if (retCode != InuDev::eOK) {
        std::cout << "gaze start error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    }
	std::cout << "gaze frames acquisition started" << std::endl;

//  retCode = gazeStream->StartCalibration();
	retCode = gazeStream->Register(
		          std::bind(&Inuitive::SaveGazeFrams, this, std::placeholders::_1,
		                    std::placeholders::_2, std::placeholders::_3));
	if (retCode != InuDev::eOK) {
		printf("gaze register error: %d\n", (int)retCode);
		return false;
	}
	return true;
}

bool Inuitive::StartGPP()
{
	generalPurposeStream = inuSensor->CreateGeneralPurposeStream();
	if (generalPurposeStream == nullptr) {
        std::cout << "Unexpected error, failed to get generalPurposeStream" << std::endl;
        return false;
    }
	InuDev::CInuError retCode = generalPurposeStream->Init();
	if (retCode != InuDev::eOK) {
        std::cout << "generalPurposeStream initiation error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    }
	std::cout << "generalPurpose Stream is initialized" << std::endl;

	retCode = generalPurposeStream->Start();
	if (retCode != InuDev::eOK) {
        std::cout << "generalPurpose start error: " << std::hex << int(retCode) << " - "  << std::string(retCode) << std::endl;
        return false;
    }
	std::cout << "generalPurposeStream acquisition started" << std::endl;

	retCode = generalPurposeStream->Register(
		          std::bind(&Inuitive::SaveGeneralPurposeFrams, this, std::placeholders::_1,
		                    std::placeholders::_2, std::placeholders::_3));
	if (retCode != InuDev::eOK) {
		printf("generalPurposeStream register error: %d\n", (int)retCode);
		return false;
	}
	return true;
}



