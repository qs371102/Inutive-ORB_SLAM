
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
	//-------------------------------------------
	char message[60];
	int pid;
	 if (argc == 2) {
      pid = *argv[1];
	 }
	
	
	
	
	
	
	
	
	
    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	bool bUseViewer = true;
	string pathToVocabulary = "../ORBvoc.bin";
	string pathToSettings = "../TUM1.yaml";
	//ORB_SLAM2::System SLAM(pathToVocabulary, pathToSettings,
	//		ORB_SLAM2::System::MONOCULAR, bUseViewer);
	 ORB_SLAM2::System SLAM(pathToVocabulary,pathToSettings,ORB_SLAM2::System::RGBD,bUseViewer);

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
			double delay=30000;
		tframe = tframe + delay;

		if (im.empty()||Depth.empty()) {
			cout<<"empty"<<endl;
			return 1;
		};		
#ifdef COMPILEDWITHC11
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
#else
		std::chrono::monotonic_clock::time_point t1 =
				std::chrono::monotonic_clock::now();
#endif

		// Pass the image to the SLAM system
	//	Mat resultMat = SLAM.TrackMonocular(im, tframe);
	//		cout << "BEFORE" << endl;
			Mat resultMat =  SLAM.TrackRGBD(im,Depth,tframe);
	//		cout << "AFTER" << endl;
		if(resultMat.cols!=0){
		Mat R=resultMat(Range(0, 3), Range(0, 3)).clone();
		Mat Tmat=resultMat(Range(0,3),Range(3,4)).clone();
			Mat raw=resultMat(Range(0, 3), Range(0, 4)).clone();
			Mat Rvec;
			Rodrigues(R,Rvec);
		//std::cout<<"R:"<<R<<std::endl;
		//std::cout<<"Tmat:"<<Tmat<<std::endl;
		Mat pos=R.t()*Tmat;
		float	x=pos.at<float>(0,0);
		float	y=pos.at<float>(0,1);
		float	z=pos.at<float>(0,2);
		int xx=x*1000000;
		int yy=y*1000000;
		int zz=z*1000000;
		int a0=Rvec.at<float>(0,1)*10000*1.25874+360000;
		int a1=Rvec.at<float>(0,0)*10000*1.25874+360000;
		int a2=Rvec.at<float>(0,2)*10000*1.25874+360000;
			cout << "?" << endl;
			//string s;
		sprintf(message,"%+09d%+09d%+09d/%05d/%05d/%05d",xx,yy,zz,a0%36000,a1%36000,a2%36000);
		//message = "aaaaaaa"; 	
		ssize_t s = write(pid, message, (strlen(message) + 1)); 
			Mat cameraMatrix  ,rotMatrix,  transVect,  rotMatrixX,  rotMatrixY,  rotMatrixZ,  eulerAngles;
			cout << "0b" << endl;
			 decomposeProjectionMatrix(raw,  cameraMatrix,  rotMatrix,  transVect,  rotMatrixX,  rotMatrixY,  rotMatrixZ,  eulerAngles);
	//cout<<"s:"<<s<<"message:"<<message<<endl;
			//cout<<atan(resultMat.at<float>(1,2)/resultMat.at<float>(2,2))<<"/"<<-asin(resultMat.at<float>(0,2))<<"/"<<atan(resultMat.at<float>(0,1)/resultMat.at<float>(0,0))<<endl;
			//cout<<a0<<" "<<a1<<" "<<a2<<endl;
			eulerAngles=eulerAngles;//
			
			cout<<eulerAngles.t()<<"T"<<transVect.t()<<endl;
		//cout<<resultMat<<endl;
			//cout<<Rvec.t()<<endl;
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
		double T = delay/1000000;
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
