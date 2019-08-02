https://blog.csdn.net/learning_tortosie/article/details/89878769
https://www.cnblogs.com/wongyi/p/11057900.html

1. 编译时先编译 code_utils 再编译 imu_utils 再编译 vio_data_simulation-ros 包要一个一个添加进去

1. 编译 code_utils
错误：
/code_utils-master/src/sumpixel_test.cpp:2:10: fatal error: backward.hpp: 没有那个文件或目录
 #include "backward.hpp"
解决办法：
在 code_utils/src/mat_io_test.cpp 中第2行（或者其它报错的文件）添加 backward.hpp 的路径
	#include "code_utils/backward.hpp"

错误：
The specified base path "/home/melodic/桌面/catkin_ch3" contains a CMakeLists.txt but "catkin_make" must be invoked in the root of workspace
解决办法：
unlink CMakeLists.txt

关于roslaunch
如果你用Github提供的bag就不用改，如果是自己的IMU，就根据IMU的name和topic改。
把roslaunch imu_utils A3.launch中的A3.launch替换为16448.launch就行了，name和topic去16448.launch里面看。

3. 在编译 vio_data_simultation 的时候会出现 error：
	error: ‘random_device’ is not a member of ‘std’    
	std::random_device rd;
解决办法：包含万能头文件 #include<bits/stdc++.h>

修改gener_alldata下面的imu.bag路径为相对路径 ./imu.bag

rosbag play -r 200 imu.bag
-r 200 是设置播放速度
