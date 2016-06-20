//  THIS IS RESTRUCTURE  //
#include <ros/ros.h>

#include "cf_tf/cf_tf_node.hpp"

Cf_Tf::Cf_Tf()
{
  ros::NodeHandle params("~");
  std::string s;

  params.param<std::string>("detection_array_topic", s, "/fiducials/fiducial_detection_array");
  m_marker_pose_sub = nh.subscribe(s, 10, &Cf_Tf::listenerCallback, this);
  //m_marker_pose_sub = nh.subscribe("/fiducials/fiducial_detection_array", 10, &Cf_Tf::listenerCallback, this);
}

void Cf_Tf::listenerCallback(const cob_object_detection_msgs::DetectionArray &msg)
{
  //      WRITING IN WORLD FRAME VARIABLE      //
  if (flag_world == false)
  {                                                               // We have no /world yet
    if (msg.detections.empty() != true)
    {
      // Reads key input in terminal
      ROS_INFO("Press 'w' to set world in the detected marker");
      char c = getch();                                                               // Call non-blocking input function
      if (c == 'w')
      {
        flag_world = true;                                                            // Will not enter this condition again
        setWorldPose(msg);
        puts("/world set");
        ROS_INFO_STREAM(world_pose);
      }
    }
  }
  // We have a world
  else{
    if (msg.detections.empty() != true)
    {
      // Note that marker id [j] and detection [i] are not the same.
      for (int i=0; i <= msg.detections.size()-1; i++)
      {                               // Checks if a marker has been detected
        try
        {
          int j = msg.detections[i].id; //msg.header.frame_id   msg.detections[i].id
          switch (j)
                        {                                          // We do this so that we know that we can address the pose to the right marker. Will be replaced by the radio choice
                          case 0: {ROS_INFO("CF 0:"); break;};     //ROS_INFO("CF 0:")
                          case 1: {ROS_INFO("CF 1:"); break;};
                          case 2: {ROS_INFO("CF 2:"); break;};
                          case 3: {ROS_INFO("CF 3:"); break;};
                          default:{ROS_INFO("ID Fail"); break;};                                  // If an error occurs and the detections[i] is accessed erronially
                        }
          setCfPose(msg, j, i);
          ROS_INFO_STREAM(msg.detections[i].id << msg.detections[i].pose.pose << j << cf_pose[j]);   //We want to check that the variable has been correctly written
        }
        catch(...)
        {
          ROS_INFO("Failed to write the CF pose "); //sprintf(buffer, i));              // Need to find how to efficiently convert int to string without too much pain
        }
      }
    }
    //else  ROS_INFO("No Marker");
  }
}

// TF Broadcasters
void Cf_Tf::broadcastWorld()   // Does not work properly
{
  //std::isnan();     //Check if the values to pass the transform are valid
  try
  {
    tf::TransformBroadcaster brw;
    tf::Transform transformw;
    //tf::TransformListener listenw;
    //ros::Time now = ros::Time::now();

    // Broadcast the world frame at any moment

    //listenw.waitForTransform("marker", "camera1", now, ros::Duration(10.0));
    transformw.setOrigin(tf::Vector3(world_pose.position.x, world_pose.position.y, world_pose.position.z));
    transformw.setRotation(tf::Quaternion(world_pose.orientation.x, world_pose.orientation.y, world_pose.orientation.z, world_pose.orientation.w));
    brw.sendTransform(tf::StampedTransform(transformw.inverse(), ros::Time::now(), "world" , "camera1"));
    ROS_INFO_STREAM(world_pose);
    //ROS_INFO("World broadcasted");
  }
  catch(...)
  {
    ROS_INFO("Failed to broadcast world");
  }
}

void Cf_Tf::broadcastCF(int cf_id)
{
  //std::isnan();     //Check if the values to pass the transform are valid
  try
  {
    tf::TransformBroadcaster brcf[4];
    tf::Transform transformcf[4];
    //tf::TransformListener listenw;
    //ros::Time now = ros::Time::now();

    // Broadcast the world frame at any moment

    //listenw.waitForTransform("marker", "camera1", now, ros::Duration(10.0));
    transformcf[cf_id].setOrigin(tf::Vector3(cf_pose[cf_id].position.x, cf_pose[cf_id].position.y, cf_pose[cf_id].position.z));
    transformcf[cf_id].setRotation(tf::Quaternion(cf_pose[cf_id].orientation.x, cf_pose[cf_id].orientation.y, cf_pose[cf_id].orientation.z, cf_pose[cf_id].orientation.w));
    brcf[cf_id].sendTransform(tf::StampedTransform(transformcf[cf_id], ros::Time::now(), "camera1" , "CF1"));  //+ sprintf(i))
    //ROS_INFO_STREAM(cf_pose);
    //ROS_INFO("CF broadcasted");
  }
  catch(...)
  {
    ROS_INFO("Failed to broadcast world");
  }
}

  // Reads key input in terminal
char Cf_Tf::getch()
{
  static struct termios oldt, newt;             // Defines the objects new and old terminals
  tcgetattr( STDIN_FILENO, &oldt);              // save old settings
  newt = oldt;
  newt.c_lflag &= ~(ICANON);                    // disable buffering
  tcsetattr( STDIN_FILENO, TCSANOW, &newt);     // apply new settings

  char c = getchar();                           // read character (non-blocking)

  tcsetattr( STDIN_FILENO, TCSANOW, &oldt);     // restore old settings
  return c;
}

geometry_msgs::PoseStamped::_pose_type Cf_Tf::getWorldPose() const
{
  return world_pose;
}

geometry_msgs::PoseStamped::_pose_type Cf_Tf::getCfPose(int cf_id) const
{
  return cf_pose[cf_id];
}

  // Set private variables
void Cf_Tf::setWorldPose(const cob_object_detection_msgs::DetectionArray &wdp) // geometry_msgs::PoseStamped::_pose_type
{
  //world_pose = wdp;  // Would not work as geometry_msgs::Pose would not be recognized the same as cob_object_detection_msgs::DetectionArray::Pose::Pose
  world_pose.position.x = wdp.detections[0].pose.pose.position.x;
  world_pose.position.y = wdp.detections[0].pose.pose.position.y;
  world_pose.position.z = wdp.detections[0].pose.pose.position.z;
  world_pose.orientation.x = wdp.detections[0].pose.pose.orientation.x;
  world_pose.orientation.y = wdp.detections[0].pose.pose.orientation.y;
  world_pose.orientation.z = wdp.detections[0].pose.pose.orientation.z;
  world_pose.orientation.w = wdp.detections[0].pose.pose.orientation.w;
}

void Cf_Tf::setCfPose(const cob_object_detection_msgs::DetectionArray &cfp, int jj, int ii)
{
  //cf_pose = cfp;  // Would not work as geometry_msgs::Pose would not be recognized the same as cob_object_detection_msgs::DetectionArray::Pose::Pose
  cf_pose[jj].position.x = cfp.detections[ii].pose.pose.position.x;
  cf_pose[jj].position.y = cfp.detections[ii].pose.pose.position.y;
  cf_pose[jj].position.z = cfp.detections[ii].pose.pose.position.z;
  cf_pose[jj].orientation.x = cfp.detections[ii].pose.pose.orientation.x;
  cf_pose[jj].orientation.y = cfp.detections[ii].pose.pose.orientation.y;
  cf_pose[jj].orientation.z = cfp.detections[ii].pose.pose.orientation.z;
  cf_pose[jj].orientation.w = cfp.detections[ii].pose.pose.orientation.w;
}

void Cf_Tf::initializeWorldPose()
{
  world_pose.position.x = 0;
  world_pose.position.y = 0;
  world_pose.position.z = 3;
  world_pose.orientation.x = 0;
  world_pose.orientation.y = 0;
  world_pose.orientation.z = 0;
  world_pose.orientation.w = 1;

  flag_world = false;
}

/*switch (j)
              {                                          // We do this so that we know that we can address the pose to the right marker. Will be replaced by the radio choice
                case 0: {ROS_INFO("CF 0:"); break;};     //ROS_INFO("CF 0:")
                case 1: {ROS_INFO("CF 1:"); break;};
                case 2: {ROS_INFO("CF 2:"); break;};
                case 3: {ROS_INFO("CF 3:"); break;};
                default:{ROS_INFO("ID Fail"); break;};                                  // If an error occurs and the detections[i] is accessed erronially
              }*/
