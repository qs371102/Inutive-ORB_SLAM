#include <System.h>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include "Camera.h"
#include "Converter.h"
#include "inuitive.hpp"
#include "common.h"
#include <PangolinViewer.h>
#include <opencv2/opencv.hpp>
//-----------------
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
//-----------------
#include <errno.h>
#include <mqueue.h>

using namespace cv;
using namespace std;

int PI = 3.141592654;

float* Matrix_Eulerian_Angle(Mat Rmat) {
  float T[9];
  float E[3];
  for (int i = 0; i < 9; i++) {
    T[i] = Rmat.at<float>(i / 3, i % 3);
  }
  E[0] = atan2(T[1], T[0]) * 180 / PI;  // Ny/Nx
  // E[1] = atan2(-1*T[2],(T[0]*cos(E[0])+T[1]*sin(E[0])));//Nz/(Nx+Ny)
  // E[2] =
  // atan2((T[6]*sin(E[0])-T[7]*cos(E[0])),(-1*T[3]*sin(E[0])+T[4]*cos(E[0])));//Ax-Ay/-Ox+Oy
  E[1] = atan2(T[7], T[8]) * 180 / PI;
  float m = sqrt(T[5] * T[5] + T[8] * T[8]);
  if (T[5] > T[8])
    m = -m;
  E[2] = atan2(T[2], m) * 180 / PI;
  for (int j = 1; j < 3; j++)
    if (E[j] < 0)
      E[j] = E[j] + 360;
  // cout<<E[0]<<" "<<E[1]<<" "<<E[2]<<endl;
  return E;
}

int main(int argc, char* argv[]) {
  //--------------message queue------------
  mqd_t mq;
  struct mq_attr attr;
  char buffer[MAX_SIZE + 1];

  /* initialize the queue attributes */
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = MAX_SIZE;
  attr.mq_curmsgs = 0;

  /* create the message queue */
  mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
  CHECK((mqd_t)-1 != mq);
  //----------------------------------------
  bool use_imu = false;
  int load_map = 0;

  int pid = -1;
  if (argc == 3) {
    pid = *argv[1];
    load_map = *argv[2] - '0';
  } else {
    std::cout << "argc != 3" << std::endl;
    return 0;
  }

  string pathToVocabulary = "ORBvoc.bin";
  string pathToSettings = "TUM1.yaml";

  string map_file = "KeyFrameTrajectory.bin";
  ORB_SLAM2::ORBVocabulary voc;

  ORB_SLAM2::Camera::Load(pathToSettings);

  // ORB_SLAM2::PangolinViewer viewer(pathToSettings);

  ORB_SLAM2::Map map;

  struct stat info;
  int ret = -1;
  voc.loadFromBinaryFile(pathToVocabulary);

  ret = stat(map_file.c_str(), &info);

  cout << "ret:" << ret << " load_map:" << load_map << std::endl;
  if (ret == 0) {
    if (load_map) {
      cout << "map loading...." << endl;
      map.Load(map_file, voc);
    } else {
      cout << "map droping...." << endl;
      map.clear();
      unlink(map_file.c_str());
    }
  } else {
    cout << "map clear !" << endl;
    map.clear();
  }

  //	ORB_SLAM2::System SLAM(pathToVocabulary, pathToSettings,
  //			ORB_SLAM2::System::RGBD, &viewer, &map, &voc);

  ORB_SLAM2::System SLAM(pathToVocabulary, pathToSettings,
                         ORB_SLAM2::System::RGBD, NULL, &map, &voc);
  if (ret == 0 && load_map)
    SLAM.ActivateLocalizationMode();
  cv::Mat im;
  double tframe = 0;
  Inuitive dev;

  /**
   rgb depth => 640 x 480
   **/
  cv::Mat Rgb;
  cv::Mat Depth;
  std::vector<double> imu_data;

  if (!dev.Start())
    dev.Stop();
  std::cout << "inutive open sucess!" << std::endl;
  while (true) {
    if (dev.getRGB_Depth(im, Depth) == 0) {
      if (im.empty() || Depth.empty()) {
        cout << "empty" << endl;
        return 1;
      };
#ifdef COMPILEDWITHC11
      std::chrono::steady_clock::time_point t1 =
          std::chrono::steady_clock::now();
#else
      std::chrono::monotonic_clock::time_point t1 =
          std::chrono::monotonic_clock::now();
#endif

      // Pass the image to the SLAM system
      // Mat resultMat = SLAM.TrackMonocular(im, tframe);
      Mat resultMat = SLAM.TrackRGBD(im, Depth, tframe);
      if (resultMat.cols != 0) {
        Mat raw = resultMat(Range(0, 3), Range(0, 4)).clone();
        Mat R = resultMat(Range(0, 3), Range(0, 3)).clone();
        Mat Tmat = resultMat(Range(0, 3), Range(3, 4)).clone();
        Mat Rvec;
        Rodrigues(R, Rvec);
        // std::cout<<"R:"<<R<<std::endl;
        // std::cout<<"Tmat:"<<Tmat<<std::endl;
        Mat pos = R.t() * Tmat;
        float x = pos.at<float>(0, 0);
        float y = pos.at<float>(0, 1);
        float z = pos.at<float>(0, 2);
        Mat cameraMatrix, rotMatrix, transVect, rotMatrixX, rotMatrixY,
            rotMatrixZ, eulerAngles;

        decomposeProjectionMatrix(raw, cameraMatrix, rotMatrix, transVect,
                                  rotMatrixX, rotMatrixY, rotMatrixZ,
                                  eulerAngles);
        if (abs(eulerAngles.at<double>(0, 2)) > 90)
          eulerAngles.at<double>(0, 1) = 180 - eulerAngles.at<double>(0, 1);
        for (int m = 0; m < 3; m++) {
          if (eulerAngles.at<double>(0, m) < 0)
            eulerAngles.at<double>(0, m) = 360 + eulerAngles.at<double>(0, m);
        }

        float angles[3];
        for (int i = 0; i < 3; i++) {
          angles[i] = (float)eulerAngles.at<double>(0, i);
        }
        float message[6] = {x, y, z, angles[0], angles[1], angles[2]};
        if (pid != -1) {
          ssize_t s = write(pid, message, sizeof(message));
          if (s > 0)
            cout << "send float data length:" << s << endl;

          for (int i = 0; i < 6; i++) {
            cout << "	message[" << i << "]=" << message[i] << "	";
          }
          cout << endl;
        }
      }
#ifdef COMPILEDWITHC11
      std::chrono::steady_clock::time_point t2 =
          std::chrono::steady_clock::now();
#else
      std::chrono::monotonic_clock::time_point t2 =
          std::chrono::monotonic_clock::now();
#endif
      double ttrack =
          std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1)
              .count();

      // Wait to load the next frame
      // cout << "fps:" << 1.0 / ttrack << endl << endl;
      //------------------------------------
      // cout << "message queue!" << endl;
      ssize_t bytes_read;

      /* receive the message */

      struct timespec tm;
      clock_gettime(CLOCK_REALTIME, &tm);
      tm.tv_nsec += 10;
      bytes_read = mq_timedreceive(mq, buffer, MAX_SIZE, NULL, &tm);
      // CHECK(bytes_read >= 0);
      buffer[bytes_read] = '\0';
      if (bytes_read >= 0) {
        if (!strncmp(buffer, MSG_STOP, strlen(MSG_STOP))) {
          std::cout << "save map" << std::endl;
          SLAM.SaveMap(map_file);
          SLAM.ActivateLocalizationMode();
        } else {
          std::cout << "bytes read:" << bytes_read << std::endl;
        }
      } else {
        // std::cout << "no data" << std::endl;
      }
      //------------------------------------
    } else {
      // cout << "Get RGB Depth Error" << endl;
    }

    if (use_imu) {
      /**
       imu data
       index 0 	=> Timestamp
       index 1-3	=> acce
       index 4-5	=> gyro
       index 7-9	=> mag
       **/
      if (dev.getImuData(imu_data) == 0) {
        std::cout << imu_data[0] << "\n"
                  << imu_data[1] << "\n"
                  << imu_data[2] << "\n"
                  << imu_data[3] << "\n"
                  << imu_data[4] << "\n"
                  << imu_data[5] << "\n"
                  << imu_data[6] << "\n"
                  << imu_data[7] << "\n"
                  << imu_data[8] << "\n"
                  << imu_data[9] << std::endl
                  << std::endl;
      } else {
        std::cout << "IMU Error." << std::endl;
      }
    }
    usleep(1000);
  }
  SLAM.Shutdown();
  // SLAM.SaveMap(map_file);
  // SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");
  cv::destroyAllWindows();
  return 0;
}
