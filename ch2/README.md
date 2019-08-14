###第2节

#2.1 设置IMU仿真代码中的不同参数，生成Allen方差标定曲线。

	Allen方差工具：

		（https://github.com/gaowenliang/imu_utils）

		（https://github.com/kalibr_allen）

#2.2 将IMU仿真代码中的欧拉积分换成中值积分

**ceres**




###build

**先编译 code_utils 再编译 imu_utils 再编译 vio_data_simulation-ros 包要一个一个添加进去。**

**imu_utils是一个catkin_make包，需要code_utils支持**

##1. code_utils

（https://github.com/gaowenliang/code_utils）

**依赖项**

**libdw-dew**
```
sudo apt-get install libdw-dev
```

**code_utils-master编译之前需要在CMakeLists.txt 中需要包含：**

```
include_directories("include/code_utils")

```


这一节作业的编译条件限制比较多，需要在根目录下新建文件夹运行。

```
mkdir ch2_ros_version/src
cd ch2_ros_version/src
git clone https://github.com/gaowenliang/code_utils
cd ..
catkin_make
```

##2. imu_utils
```
cd ~/ch2_ros_version/src
git clone https://github.com/gaowenliang/imu_utils.git
cd ..
catkin_make
```



##3. vio_data_simultation-ros_version

```
cd ~/ch2_ros_version/src
git clone https://github.com/HeYijia/vio_data_simulation-ros_version // 手动下载
git checkout ros_version
cd ..
catkin_make
```




###run


修改gener_alldata下面的imu.bag路径为相对路径

```
./imu.bag
```

播放IMU.bag

```
rosbag play -r 200 imu.bag
-r 200 是设置播放速度
```

关于roslaunch

如果用Github提供的bag就不用改，如果是自己的IMU，就根据IMU的name和topic改。

把roslaunch imu_utils A3.launch中的A3.launch替换为16448.launch就行了，name和topic去16448.launch里面看。



## 错误：
```
The specified base path "/home/melodic/桌面/catkin_ch2" contains a CMakeLists.txt 
but "catkin_make" must be invoked in the root of workspace
```

解决办法：
```
unlink CMakeLists.txt
```




(https://blog.csdn.net/learning_tortosie/article/details/89878769)

(https://www.cnblogs.com/wongyi/p/11057900.html)
