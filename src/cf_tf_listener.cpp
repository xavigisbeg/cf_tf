#include <ros/ros.h>

#include <stdio.h>
#include <tf/transform_listener.h>
#include "std_msgs/String.h"
#include "geometry_msgs/PoseStamped.h"
#include <cob_object_detection_msgs/DetectionArray.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>

void Callback(const cob_object_detection_msgs::DetectionArray &msg)                     //const because we are just reading it, & to pass by reference
{
    if (msg.detections.empty() != true){
        for (int i=0; i <= msg.detections.size()-1; i++){  //msg.detections.size()
            try{
                //ROS_INFO_STREAM("Message 1: " << msg.detections[i]);                  //Displays the whole detection information
                switch (msg.detections[i].id){                                          //We do this so that we know that we can address the pose to the right marker. Will be replaced
                    case 0: {ROS_INFO("CF 0:"); break;};
                    case 1: {ROS_INFO("CF 1:"); break;};
                    case 2: {ROS_INFO("CF 2:"); break;};
                    case 3: {ROS_INFO("CF 3:"); break;};
                default:{ROS_INFO("ID Fail"); break;};                                  //If an error occurs and the detections[i] is accessed erronially
                }

                ROS_INFO_STREAM(msg.detections[i].pose.pose);
            }
            catch(...){
                ROS_INFO("Failed to send Marker "); //sprintf(buffer, i));              //Need to find how to efficiently convert int to string without too much pain
            }
        }
    }
    //else  ROS_INFO("No Marker");
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("/fiducials/fiducial_detection_array",1,Callback); //Basic subscriber. Just has the topic it is subscribed to, the callback function and how many messages it caches

  while(nh.ok()){
    ros::spinOnce();
  }

  return 0;
}
