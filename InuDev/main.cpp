
#include"inuitive.hpp"


int main(int argc, char *argv[]) {
    
	cv::namedWindow("rgb", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("depth", cv::WINDOW_AUTOSIZE);
	

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
		if (dev.getRGB_Depth(Rgb, Depth) == 0){
			/**

			**/
			cv::imshow("rgb", Rgb);
			cv::waitKey(1);			
			cv::imshow("depth", Depth);
			cv::waitKey(1);
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
