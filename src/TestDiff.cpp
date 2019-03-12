//
// Created by skaegy on 11/03/19.
//

#include <ros/ros.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "std_msgs/String.h"
#include <geometry_msgs/PoseStamped.h>

void callback(const geometry_msgs::PoseStamped::ConstPtr& msg){
    geometry_msgs::PoseStamped ps;
    ros::Time currentOS = ros::Time::now();
    double SecDiff = abs(currentOS.sec - msg->header.stamp.sec);
    double NsecDiff = abs(static_cast<int>(currentOS.nsec - msg->header.stamp.nsec));
    ROS_INFO("Time difference between server and client is: %f [second] + %f [nanosecond]:",
                     SecDiff,NsecDiff);

}

int main(int argc, char *argv[]) {
    ros::init(argc, argv, "test_diff");
    ros::NodeHandle nh;
    ros::Rate loop_rate(100); //

    ros::Subscriber client_sub = nh.subscribe("test_comm", 1, callback);
    while(ros::ok()) {
        ros::spinOnce();
    }

    return 0;
}
