
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <iomanip>

#include <cv.h>
#include <opencv2/opencv.hpp>
#include <highgui.h>
#include <eigen3/Eigen/Dense>
#include "System.h"

using namespace std;
using namespace cv;
using namespace Eigen;

const int nDelayTimes = 2;
string sData_path = "/home/stevencui/dataset/EuRoC/MH-05/mav0/";
string sConfig_path = "../config/";

std::shared_ptr<System> pSystem;

void PubImuData()
{
	string sImu_data_file = sConfig_path + "imu_pose.txt";
	cout << "1 PubImuData start sImu_data_filea: " << sImu_data_file << endl;
	ifstream fsImu;
	fsImu.open(sImu_data_file.c_str());
	if (!fsImu.is_open())
	{
		cerr << "Failed to open imu file! " << sImu_data_file << endl;
		return;
	}

	std::string sImu_line;
	double dStampNSec = 0.0;
	Vector3d vAcc;
	Vector3d vGyr;
	while (std::getline(fsImu, sImu_line) && !sImu_line.empty()) // read imu data
	{
		std::istringstream ssImuData(sImu_line);
		ssImuData >> dStampNSec;
//		cout<<"dStampNSec: "<<dStampNSec<<std::endl;
		double tmp;
        for(int i = 0 ; i < 7 ; i++)
        {
            ssImuData >> tmp;
        }
        ssImuData>> vGyr.x() >> vGyr.y() >> vGyr.z() >> vAcc.x() >> vAcc.y() >> vAcc.z();
//		 cout << "Imu t: " << fixed << dStampNSec << " gyr: " << vGyr.transpose() << " acc: " << vAcc.transpose() << endl;
		pSystem->PubImuData(dStampNSec , vGyr, vAcc);//yzp !!! 这里的时间戳要处理的
		usleep(5000*nDelayTimes);
	}
	fsImu.close();
}

void PubImageData()
{
	string sImage_file = sConfig_path + "cam_pose.txt";
	string keyframePath = sConfig_path + "keyframe/";

	cout << "1 PubImageData start sImage_file: " << sImage_file << endl;

	ifstream fsImage;
	fsImage.open(sImage_file.c_str());
	if (!fsImage.is_open())
	{
		cerr << "Failed to open image file! " << sImage_file << endl;
		return;
	}

	std::string sImage_line;
	double dStampNSec;
	string sImgFileName;
	
	// cv::namedWindow("SOURCE IMAGE", CV_WINDOW_AUTOSIZE);
	int imgNums = 0;
	char imgName[128] = "";
	ifstream fin;
	string strTmp;
	//yzp 读取cam_post.txt的每一行 , 这里只是读取每一行的时间戳数据，其他数据没有用到，只是模仿源程序的读取数据
	while (std::getline(fsImage, sImage_line) && !sImage_line.empty())
	{
	    //yzp 读取keyframe ：我们所需要的读取每一帧图像所采集到的点的数据,（点的世界坐标，id，归一化坐标）,这里每一帧数据都是通过txt来保存的,并将其放入vec_camObs容器中
        sprintf(imgName,"%s%s%d%s",keyframePath.c_str(),"all_points_",imgNums++,".txt");
        std::cout<<"image name : "<<imgName<<endl;
        fin.open(imgName);
        std::vector<camObs> vec_camObs;
//        string strTmp;
        int ptsCount = 0;
        while(std::getline(fin,strTmp))
        {
            std::istringstream sstrTmp(strTmp);
            camObs data_obs;
            sstrTmp>>data_obs.pw[0]>>data_obs.pw[1]>>data_obs.pw[2]>>data_obs.pw[3]>>data_obs.un_pc[0]>>data_obs.un_pc[1];
            vec_camObs.push_back(data_obs);
            ++ptsCount;
        }
        fin.close();
        cout<<"ptsCount: "<<ptsCount<<std::endl;
        if(imgNums == 599)
        {
            int i = 0;
            std::cout<<" is 599 now !"<<std::endl;
            std::cin >> i ;
        }

        //yzp:读取keyframe对应的时间戳
		std::istringstream ssImuData(sImage_line);
		ssImuData >> dStampNSec;

/*		// cout << "Image t : " << fixed << dStampNSec << " Name: " << sImgFileName << endl;
		string imagePath = sData_path + "cam0/data/" + sImgFileName;

		Mat img = imread(imagePath.c_str(), 0);
		if (img.empty())
		{
			cerr << "image is empty! path: " << imagePath << endl;
			return;
		}*/

		pSystem->PubImageData(dStampNSec , vec_camObs);
		// cv::imshow("SOURCE IMAGE", img);
		// cv::waitKey(0);
		usleep(50000*nDelayTimes);
	}
	fsImage.close();
}

int main(int argc, char **argv)
{
    char buf[80];
    getcwd(buf, sizeof(buf));
    printf("current working directory : %s\n", buf);
    cout<<"argc: "<<argc<<std::endl;
	if(argc != 3)
	{
		cerr << "./run_euroc PATH_TO_FOLDER/MH-05/mav0 PATH_TO_CONFIG/config \n" 
			<< "For example: ./run_euroc /home/stevencui/dataset/EuRoC/MH-05/mav0/ ../config/"<< endl;
		return -1;
	}
	sData_path = argv[1];
	sConfig_path = argv[2];

	pSystem.reset(new System(sConfig_path));
	
	std::thread thd_BackEnd(&System::ProcessBackEnd, pSystem);
		
	// sleep(5);
	std::thread thd_PubImuData(PubImuData);

	std::thread thd_PubImageData(PubImageData);
	
	std::thread thd_Draw(&System::Draw, pSystem);
	
	thd_PubImuData.join();
	thd_PubImageData.join();

	// thd_BackEnd.join();
	// thd_Draw.join();

	cout << "main end... see you ..." << endl;
	return 0;
}
