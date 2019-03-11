//
// Created by skaegy on 11/03/19.
//
#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "std_msgs/Int64.h"
#include "std_msgs/String.h"


int main( int argc, char** argv )
{
    ros::init(argc, argv, "TestComm");
    ros::NodeHandle n;
    ros::Publisher server_pub = n.advertise<geometry_msgs::PoseStamped>("test_comm", 1);
    geometry_msgs::PoseStamped message;
    ros::Rate r(100);
    int counter = 0;
    while (ros::ok()) {
        ros::spinOnce();
        message.header.stamp = ros::Time::now();

        std::stringstream ss;
        ss << counter;
        std::string str = ss.str();
        message.header.frame_id = str;
        server_pub.publish(message);
        r.sleep() ;
        counter++;
    }
}