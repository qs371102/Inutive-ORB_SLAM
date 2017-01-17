
#include"inuitive.hpp"
#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<System.h>/////////////////////////

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char *argv[]) {
    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	int camreaID = 0;
	bool bUseViewer = true;
	string pathToVocabulary = "../ORBvoc.bin";
	string pathToSettings = "../rgb.yaml";
	if (argc != 3) {
		cout << "use default ..." << "cameraID= " << camreaID
				<< "pathToVocabulary=  " << pathToVocabulary
				<< "pathToSettings=  " << pathToSettings << endl;
	} else {
		camreaID = atoi(argv[2]);
		pathToSettings = argv[1];
	}
	//ORB_SLAM2::System SLAM(pathToVocabulary, pathToSettings,
	//		ORB_SLAM2::System::MONOCULAR, bUseViewer);
	 	ORB_SLAM2::System SLAM(pathToVocabulary, pathToSettings,
			ORB_SLAM2::System::MONOCULAR, bUseViewer);

	cout << endl << "-------" << endl;
	cout << "Start processing sequence ..." << endl;

	// Main loop
	cv::Mat im;
	double tframe = 0;
	
	///^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//	cv::namedWindow("rgb", cv::WINDOW_AUTOSIZE);
	//cv::namedWindow("depth", cv::WINDOW_AUTOSIZE);
	

	Inuitive dev;

	/**
		rgb depth => 640 x 480
	**/
	cv::Mat Rgb;
	cv::Mat Depth;

	/**
		imu data
	index 0 	=> Timestamp
	index 1-3	=> acce
	index 4-5	=> gyro
	index 7-9	=> mag
	**/
	std::vector<double> imu_data;


	if(!dev.Start())
		dev.Stop();
	while(1) {
		if (dev.getRGB_Depth(im, Depth) == 0){
			/**

			**/
		//	cv::imshow("rgb", im);
		//	cv::waitKey(1);			
		//	cv::imshow("depth", Depth);
		//	cv::waitKey(1);
			
			
			//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
			
					tframe = tframe + 7000;

		if (im.empty()) {
			return 1;
		}

#ifdef COMPILEDWITHC11
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
#else
		std::chrono::monotonic_clock::time_point t1 =
				std::chrono::monotonic_clock::now();
#endif

		// Pass the image to the SLAM system
	//	Mat resultMat = SLAM.TrackMonocular(im, tframe);
			Mat resultMat =  SLAM.TrackMonocular(im,tframe);
		if(resultMat.cols!=0){
		Mat R=resultMat(Range(0, 3), Range(0, 3)).clone();
		Mat Tmat=resultMat(Range(0,3),Range(3,4)).clone();
			Mat raw=resultMat.t();
		//std::cout<<"R:"<<R<<std::endl;
		//std::cout<<"Tmat:"<<Tmat<<std::endl;
		Mat pos=R.t()*Tmat;
		double aa=acos(resultMat.at<float>(0,0))*180/3.1415929;
		double aa1=acos(resultMat.at<float>(0,1))*180/3.1415929;
		double aa2=acos(resultMat.at<float>(0,2))*180/3.1415929;
		cout <<"x:"<<pos.at<float>(0,0)<<" y:"<<pos.at<float>(0,1)<<" z:"<<pos.at<float>(0,2) <<" a= "<< aa<<"bb=  "<<aa1<<"cc=  "<<aa2<<"  dd= "<<resultMat.at<float>(0,0) <<endl;
			Mat cameraMatrix  ,rotMatrix,  transVect,  rotMatrixX,  rotMatrixY,  rotMatrixZ,  eulerAngles;
			cout << "0b" << endl;
			 decomposeProjectionMatrix(raw,  cameraMatrix,  rotMatrix,  transVect,  rotMatrixX,  rotMatrixY,  rotMatrixZ,  eulerAngles);
	//cout<<"s:"<<s<<"message:"<<message<<endl;
			//cout<<atan(resultMat.at<float>(1,2)/resultMat.at<float>(2,2))<<"/"<<-asin(resultMat.at<float>(0,2))<<"/"<<atan(resultMat.at<float>(0,1)/resultMat.at<float>(0,0))<<endl;
			//cout<<a0<<" "<<a1<<" "<<a2<<endl;
			eulerAngles=eulerAngles;//
			
			cout<<eulerAngles.t()<<"T"<<transVect.t()<<endl;
			
		//cout<<resultMat<<endl;
		}

#ifdef COMPILEDWITHC11
		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
#else
		std::chrono::monotonic_clock::time_point t2 =
				std::chrono::monotonic_clock::now();
#endif

		double ttrack = std::chrono::duration_cast
				< std::chrono::duration<double> > (t2 - t1).count();

		// Wait to load the next frame
		double T = 0.01;
		//cout << "fps" << 1.0/ttrack << endl << endl;
		if (T > ttrack)
			usleep((T - ttrack) * 1e6);
		/*int charkey = (char)cv::waitKey(1);
		 if (charkey == 27)
		 break;*/
			
			
			
			
			//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		} else {
			//cout << "Get RGB Depth Error" << endl;
		}


		if (dev.getImuData(imu_data) == 0){ 
			std::cout << imu_data[0] << "\n" << imu_data[1] << "\n" << imu_data[2] << "\n" << imu_data[3] << "\n" << imu_data[4] << "\n" << imu_data[5] << "\n" << imu_data[6] << "\n" << imu_data[7] << "\n" << imu_data[8] << "\n" << imu_data[9] << std::endl << std::endl;
		} else {
			//std::cout << "IMU Error." << std::endl; 
		}

		usleep(100);
	}
	cv::destroyAllWindows();
    return 0;
}
