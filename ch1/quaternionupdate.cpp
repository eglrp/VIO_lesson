//
// Created by $melodic on 19-6-13.
//
#include <iostream>
#include <ctime>
using namespace std;

#include <Eigen/Core>
#include <Eigen/Dense>
#include <math.h>
#include "sophus/so3.h"
#include "sophus/se3.h"

int main( int argc, char** argv )
{
    Eigen::AngleAxisd v(M_PI/4, Eigen::Vector3d(0,0,1));

    cout.precision(4);

    Eigen::Matrix3d R = v.toRotationMatrix();
    cout << "\n" << "R is :" << "\n" << R << endl;

    Eigen::Quaterniond q(R);
    Eigen::Vector3d w(0.01, 0.02, 0.03);

    R = R * Sophus::SO3::exp(w).matrix();
    cout << "\n" << "R_update is :" << "\n" << R << endl;

    Eigen::Quaterniond delta_q = Eigen::Quaterniond(1, 0.5*w(0), 0.5*w(1), 0.5*w(2));

    q = q * delta_q;

    Eigen::Quaterniond q_norm = q.normalized();

    Eigen::Matrix3d q2R = q_norm.toRotationMatrix();
    cout << "\n" << "The q_update is :" << "\n" << q2R << endl;

    return 0;
}
