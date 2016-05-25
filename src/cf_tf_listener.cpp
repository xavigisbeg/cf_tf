#include <ros/ros.h>

#include <tf/transform_listener.h>
#include "std_msgs/String.h"
//#include <cob_object_detection_msg/Detection.msg>
//#include <cob_object_detection_msgs/DetectObjects.h>
#include "geometry_msgs/PoseStamped.h"
#include <cob_object_detection_msgs/DetectionArray.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
/*void Callback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}*/
//void Callback(const geometry_msgs::PoseStamped msg)
void Callback(const cob_object_detection_msgs::DetectionArray msg)
{
  //ROS_INFO("I heard: [%s]", msg.pose.orientation.x pose->data.c_str());
  ROS_INFO_STREAM("I heard " << msg); //"<<
}

int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "listener");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle nh;

  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
  //ros::Subscriber sub = n.subscribe("fiducial_detection_array", 1000); //, chatterCallback);
  //std::string s;
  //ros::Subscriber sub = nh.subscribe("listener",s,1,const geometry_msgs::PoseStamped,this ,1000);//, CallBack);
  ros::Subscriber sub = nh.subscribe("/fiducials/fiducial_detection_array",1,Callback); //Basic subscriber. Just has the topic it is subscribed to, the callback function and how many messages it caches
    //void Callback (const geometry_msgs::Twist)
  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  while(nh.ok()){
    ros::spinOnce();
  }

  return 0;
}
