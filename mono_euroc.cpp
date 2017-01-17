/**
 * This file is part of ORB-SLAM2.
 *
 * Copyright (C) 2014-2016 Raúl Mur-Artal <raulmur at unizar dot es> (University of Zaragoza)
 * For more information see <https://github.com/raulmur/ORB_SLAM2>
 *
 * ORB-SLAM2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ORB-SLAM2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.
 */

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

int main(int argc, char **argv) {
	int camreaID =0;
	bool bUseViewer = true;
	string pathToVocabulary = "../ORBvoc.bin";
	string pathToSettings = "../fisheye.yaml";
	if (argc != 3) {
		cout << "use default ..." << "cameraID= " << camreaID
				<< "pathToVocabulary=  " << pathToVocabulary
				<< "pathToSettings=  " << pathToSettings << endl;
	} else {
		camreaID = atoi(argv[2]);
		pathToSettings = argv[1];
	}
	ORB_SLAM2::System SLAM(pathToVocabulary, pathToSettings,
			ORB_SLAM2::System::MONOCULAR, bUseViewer);

	cout << endl << "-------" << endl;
	cout << "Start processing sequence ..." << endl;

	// Main loop
	cv::Mat im;
	cv::VideoCapture cap(camreaID);
	double tframe = 0;
	while (1) {
		// Read image from file
		cap >> im;
		// im = cv::imread(vstrImageFilenames[ni],CV_LOAD_IMAGE_UNCHANGED);
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
		Mat resultMat = SLAM.TrackMonocular(im, tframe);
		if(resultMat.cols!=0){
		Mat R=resultMat(Range(0, 3), Range(0, 3)).clone();
		Mat Tmat=resultMat(Range(0,3),Range(3,4)).clone();
		//std::cout<<"R:"<<R<<std::endl;
		//std::cout<<"Tmat:"<<Tmat<<std::endl;
		Mat pos=R.t()*Tmat;
		double aa=acos(resultMat.at<float>(0,0))*180/3.1415929;
		double aa1=acos(resultMat.at<float>(0,1))*180/3.1415929;
		double aa2=acos(resultMat.at<float>(0,2))*180/3.1415929;
		cout <<"x:"<<pos.at<float>(0,0)<<" y:"<<pos.at<float>(0,1)<<" z:"<<pos.at<float>(0,2) <<" a= "<< aa<<"bb=  "<<aa1<<"cc=  "<<aa2<<"  dd= "<<resultMat.at<float>(0,0) <<endl;
		cout<<resultMat<<endl;
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

	}

	// Stop all threads
	SLAM.Shutdown();

	// Tracking time statistics
	/* sort(vTimesTrack.begin(),vTimesTrack.end());
	 float totaltime = 0;
	 for(int ni=0; ni<nImages; ni++)
	 {
	 totaltime+=vTimesTrack[ni];
	 }
	 cout << "-------" << endl << endl;
	 cout << "median tracking time: " << vTimesTrack[nImages/2] << endl;
	 cout << "mean tracking time: " << totaltime/nImages << endl;
	 */
	// Save camera trajectory
	SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");

	return 0;
}
