//
// Created by skaegy on 06/03/19.
//

#include <ros/ros.h>
#include <ros/transport/transport_tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "std_msgs/String.h"
#include <geometry_msgs/PoseStamped.h>

#define MESSAGE_FREQ 100

void showerror(const char *msg) {
    perror(msg);
    exit(0);
}

class Listener {
private:
    char topic_message[256] = { 0 };

public:
    void callback(const geometry_msgs::PoseStamped::ConstPtr& msg);
    char* getMessageValue();
};

void Listener::callback(const geometry_msgs::PoseStamped::ConstPtr& msg) {
    ros::NodeHandle nh;
    ros::Publisher receivePose_pub = nh.advertise<geometry_msgs::PoseStamped>("/omega_received/", 1000);

    geometry_msgs::Point point3D;
    point3D.x = msg->pose.position.x;
    point3D.y = msg->pose.position.y;
    point3D.z = msg->pose.position.z;

    geometry_msgs::Quaternion quat;
    quat.w = msg->pose.orientation.w;
    quat.x = msg->pose.orientation.x;
    quat.y = msg->pose.orientation.y;
    quat.z = msg->pose.orientation.z;

    receivePose_pub.publish(msg);
    ROS_INFO("[Position]   x: %f y: %f z: %f", point3D.x,point3D.y,point3D.z);
    ROS_INFO("[Quaternion] w: %f x: %f y: %f z: %f", quat.w, quat.x,quat.y,quat.z);
}

char* Listener::getMessageValue() {
    return topic_message;
}


int main(int argc, char *argv[]) {
    /*
     * argv[0]: Host IP address
     * argv[1]: Port no
     * */

    // ------- Initialisation ------- //
    ros::init(argc, argv, "client_node");
    ros::NodeHandle nh;
    ros::Rate loop_rate(MESSAGE_FREQ); // set the rate as defined in the macro MESSAGE_FREQ
    Listener listener;
//    ros::SubscribeOptions ops;
//    std::string topic = "/server_messages/";
//    ops.init(&topic, 1, &Listener::callback, &listener);
//    ops.md5sum = "d3812c3cbc69362b77dc0b19b345f8f5";


    ros::Subscriber client_sub = nh.subscribe("/server_messages/", 1, &Listener::callback, &listener);
    ros::Publisher receivePose_pub = nh.advertise<geometry_msgs::PoseStamped>("/omega_received/", 1000);

    int sockfd, portno = 1;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    std_msgs::String message;
    std::stringstream ss_comm;

    // ------- Input ------- //
    if (argc < 2) {
        fprintf(stderr,"Usage: $ rosrun omega_client omega7_client <hostname> <port> \n");
        exit(0);
    }

    // ------- TCP ------- //
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        showerror("ERROR opening socket");
    ROS_INFO("Breakpoint: %s","1");
    server = gethostbyname(argv[1]);
    ROS_INFO("Breakpoint: %s","2");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    ROS_INFO("Breakpoint: %s","3");
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        showerror("ERROR connecting");
    std::cout << "Subscribe from the server and Publish the /omega_received";


    while(ros::ok()) {

        char buffer[8] = {'H','i','-','S','t','a','r','t'};
        int idx = write(sockfd,buffer,strlen(buffer));
        if (idx < 0) showerror("ERROR writing to socket");
        std::cout << buffer[0] << buffer[1] << std::endl;
        ros::spinOnce();
    }

    return 0;
}