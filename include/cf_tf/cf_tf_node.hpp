//  THIS IS RESTRUCTURE  //

#ifndef CF_TF_HPP
#define CF_TF_HPP


#include <std_msgs/Float64.h>
#include <std_srvs/Empty.h>
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

class Cf_Tf
{
public:
  Cf_Tf();
  ros::NodeHandle nh;

  // FLAGS


  // FUNCTIONS
    //Subscribers and Publishers
  void listenerCallback(const cob_object_detection_msgs::DetectionArray &msg);

    // Broadcasters
  void broadcastWorld();
  void broadcastCF(int cf_id);

    // Special (maybe to erase) functions
  char getch();

    // Access private variables
  geometry_msgs::PoseStamped::_pose_type getWorldPose() const;
  geometry_msgs::PoseStamped::_pose_type getCfPose(int cf_id) const;

    // Set private variables
  void setWorldPose(const cob_object_detection_msgs::DetectionArray &wdp);
  void setCfPose(const cob_object_detection_msgs::DetectionArray &cfp, int jj, int ii);  // We will pass the cf ID [jj = j] to store and the messaga detection ID [ii = i]
  void initializeWorldPose();
  void initializeCfPose();

    // StampedPose of world and CF
  bool serviceSetWorld(std_srvs::Empty::Request& req, std_srvs::Empty::Response& res);

private:
  ros::Subscriber m_marker_pose_sub;
  ros::ServiceServer m_serviceSetWorld;

  tf::TransformBroadcaster brcf[4];
  tf::Transform transformcf[4];
  tf::TransformBroadcaster brw;
  tf::Transform transformw;

  ros::Publisher m_world_pose_pub;
  ros::Publisher m_cf_pose_pub[4];
  //ros::Publisher m_debug_pub; //! For debugging variables in rqt_plot
  //dynamic reconfigure server
  //dynamic_reconfigure::Server<ardrone_velocity::dynamic_param_configConfig> m_server;
  //ros::Time t;
  //ros::Time old_t;

/*  enum State
  {
      Default = 0,
      WorldSet = 1,
      CrazyfliesSet = 2,
      Operating = 3,
  };*/
  //ros::ServiceServer m_serviceSetWorld;

  bool flag_world; // = false;  //To become a private parameter

  geometry_msgs::PoseStamped::_pose_type world_pose;
  geometry_msgs::PoseStamped::_pose_type cf_pose[4];
};

#endif  // Cf_Tf_HPP
