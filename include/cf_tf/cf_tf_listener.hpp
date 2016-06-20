//#ifndef LISTENERNODE_HPP
#define LISTENERNODE_HPP


#include <std_msgs/Float64.h>
#include <vector>
#include <stdlib.h>
#include <termios.h>
#include <stdio.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include "std_msgs/String.h"
#include "geometry_msgs/PoseStamped.h"
#include <cob_object_detection_msgs/DetectionArray.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>

/*class cf_tf
{
public:
  cf_tf();



  ros::NodeHandle nh;

    // Broadcasters
  void broadcastworld();
  void broadcastCF();

    // Special (maybe to erase) functions
  char getch();

  // Flags
  bool flag_world = false;

  // FUNCTIONS

    // Access private variables
  geometry_msgs::Pose getWorldPose() const
  {
    return world_pose;
  }

  geometry_msgs::Pose getCfPose() const
  {
    return cf_pose;
  }

    // Set private variables
   void setWorldPose(geometry_msgs::Pose wdp)
  {
    world_pose = wdp;
  }

  void getCfPose(geometry_msgs::Pose cfp)
  {
    cf_pose = cfp;
  }

    // StampedPose of world and CF


    // ROS message callbacks
  void listenerCallback(const cob_object_detection_msgs::DetectionArray &msg);

private:
  ros::Subscriber m_marker_pose_sub;
  ros::Publisher m_world_pose_pub;
  ros::Publisher m_cf_pose_pub;
  //ros::Publisher m_debug_pub; //! For debugging variables in rqt_plot
  //dynamic reconfigure server
  dynamic_reconfigure::Server<ardrone_velocity::dynamic_param_configConfig> m_server;
  nav_msgs::Odometry m_odo_msg;
  ros::Time t;
  ros::Time old_t;

  geometry_msgs::PoseStamped::_pose_type world_pose;
  geometry_msgs::PoseStamped::_pose_type cf_pose;
};*/

