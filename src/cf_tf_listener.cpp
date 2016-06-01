#include <ros/ros.h>

#include <stdio.h>
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

//void Callback(const cob_object_detection_msgs::Detection::Detection_::_pose_type msg) //You cannot subscribe just to the pose, you subscribe to the whole message. Therefore, the types must match
void Callback(const cob_object_detection_msgs::DetectionArray &msg)                      //const because we are just reading it, & to pass by reference
{
    //int nvmark=0;                                 //Visible markers
    //ROS_INFO_STREAM("I heard " << msg); //"<<
    //ROS_INFO_STREAM("Message 1: " << msg.detections[0].label);
    //if (msg.detections.empty() != true) ROS_INFO_STREAM(msg.detections.size());
    //char buffer [50];
    if (msg.detections.empty() != true){
        for (int i=0; i <= msg.detections.size()-1; i++){  //msg.detections.size()
            try{
                //ROS_INFO_STREAM("Message 1: " << msg.detections[i]);                  //Displays the whole detection information
                switch (msg.detections[i].id){              //We do this so that we know that we can address the pose to the right marker. Will be replaced
                    case 0: {ROS_INFO("CF 0:"); break;};
                    case 1: {ROS_INFO("CF 1:"); break;};
                    case 2: {ROS_INFO("CF 2:"); break;};
                    case 3: {ROS_INFO("CF 3:"); break;};
                }

                ROS_INFO_STREAM(msg.detections[i].pose.pose);
            }
            catch(...){
                //ROS_INFO("Failed to send Marker [%s]", i); //sprintf(buffer, i)
            }
        }
    }
    //else  ROS_INFO("No Marker");

    /* ****  **** */

    /* **** Function to chop the Detection Array into several Detection, so that they can be treated and sent separately**** */

    /* **** Function to chop the Detection into the different datatypes and publish it **** */
    /*for (int i=1; i<=nvmark+1; i++)           //For all visible markers:
    {
        try{
            ROS_INFO("Marker [s%]", i);     //We could chop this marker
            //Here we should chop the data that interests us, taking just the geometry_msgs::PoseStamped and send it to the
        }
        catch(){
            ROS_INFO("Failed to send Marker [s%]", i);
        }
    }*/
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

  ros::Subscriber sub = nh.subscribe("/fiducials/fiducial_detection_array",1,Callback); //Basic subscriber. Just has the topic it is subscribed to, the callback function and how many messages it caches

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
