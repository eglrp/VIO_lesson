//
// Created by hyj on 18-11-11.
//
#include <iostream>
#include <vector>
#include <random>  
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Eigenvalues>
#include <Eigen/SVD>
#include <Eigen/Dense>


struct Pose
{
    Pose(Eigen::Matrix3d R, Eigen::Vector3d t):Rwc(R),qwc(R),twc(t) {};
    Eigen::Matrix3d Rwc;
    Eigen::Quaterniond qwc;
    Eigen::Vector3d twc;

    Eigen::Vector2d uv;    // 这帧图像观测到的特征坐标
};

int main()
{

    int poseNums = 10;
    double radius = 8;
    double fx = 1.;
    double fy = 1.;
    std::vector<Pose> camera_pose;
    
    // 生成10个相机位姿
    for(int n = 0; n < poseNums; ++n ) {
        double theta = n * 2 * M_PI / ( poseNums * 4); // 1/4 圆弧
        // 绕 z轴 旋转
        Eigen::Matrix3d R;
        R = Eigen::AngleAxisd(theta, Eigen::Vector3d::UnitZ());
        Eigen::Vector3d t = Eigen::Vector3d(radius * cos(theta) - radius, radius * sin(theta), 1 * sin(2 * theta));
        camera_pose.push_back(Pose(R,t));
    }

    // 随机数生成 1个 三维特征点
    std::default_random_engine generator;
    std::uniform_real_distribution<double> xy_rand(-4, 4.0);    // 在(-4，4.0)之间产生
    std::uniform_real_distribution<double> z_rand(8., 10.);     // 在(8.0，10.0)之间产生
    double tx = xy_rand(generator);
    double ty = xy_rand(generator);
    double tz = z_rand(generator);

    Eigen::Vector3d Pw(tx, ty, tz);

    // 这个特征从第三帧相机开始被观测，i=3
    // 这里生成的是该点的7个观测
    int start_frame_id = 3;
    int end_frame_id = poseNums;
    for (int i = start_frame_id; i < end_frame_id; ++i) {
        Eigen::Matrix3d Rcw = camera_pose[i].Rwc.transpose();
        Eigen::Vector3d Pc = Rcw * (Pw - camera_pose[i].twc);

        double x = Pc.x();
        double y = Pc.y();
        double z = Pc.z();

        camera_pose[i].uv = Eigen::Vector2d(x/z,y/z);
    }
    
    // TODO::homework; 请完成三角化估计深度的代码
    // 遍历所有的观测数据，并三角化
    Eigen::Vector3d P_est;           // 结果保存到这个变量
    P_est.setZero();

    /* your code begin */
    Eigen::Matrix<double, 14, 4> D;

    for (int i = start_frame_id; i < end_frame_id; ++i)
    {
        Eigen::Matrix<double, 3, 4> P;
        P.setZero();

        P.block(0,0,3,3) = camera_pose[i].Rwc;
        P.block(0,3,3,1) = camera_pose[i].Rwc * camera_pose[i].twc;

        D.block(2*(i-start_frame_id),0,1,4) = camera_pose[i].uv[0] * P.row(2) - P.row(0);
        D.block(2*(i-start_frame_id)+1,0,1,4) = camera_pose[i].uv[1] * P.row(2) - P.row(1);
    }

    std::cout <<"D: \n"<< D << std::endl;

    Eigen::Matrix<double, 4, 4> DTD;
    DTD = D.transpose() * D;

    std::cout <<"DTD: \n"<< DTD << std::endl;

    Eigen::JacobiSVD<Eigen::MatrixXd> svd(DTD, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::MatrixXd U = svd.matrixU();
    Eigen::MatrixXd V = svd.matrixV();
    Eigen::MatrixXd A = svd.singularValues();
    Eigen::MatrixXd S = U.inverse() * DTD * V.transpose().inverse();
    Eigen::MatrixXd W = U * S * V.transpose();

    std::cout <<"U: \n"<< U <<std::endl;
    std::cout <<"V: \n"<< V <<std::endl;
    std::cout <<"A: \n"<< A <<std::endl;
    std::cout <<"W: \n"<< W <<std::endl;

    // P_est = ;
    
    /* your code end */
    
    std::cout <<"ground truth: \n"<< Pw.transpose() << std::endl;
    std::cout <<"my result: \n"<< P_est.transpose() << std::endl;
    return 0;
}
