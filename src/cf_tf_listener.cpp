//  THIS IS RESTRUCTURE  //
#include <ros/ros.h>

#include "cf_tf/cf_tf_listener.hpp"

ControlNode::ControlNode()
{
  // Not sure what goes here
}

void ControlNode::listenerCallback(cob_object_detection_msgs::DetectionArray &msg)
{
  //      WRITING IN WORLD FRAME VARIABLE      //
  if (flag_world == false)
  {                                                               // We have no /world yet
    if (msg.detections.empty() != true)
    {
    // Reads key input in terminal

    char c = getch();                                                               // Call non-blocking input function
    if (c == 'w')
    {
      flag_world = true;                                                            // Will not enter this condition again
      setWorldPose(msg);
      puts("/world set");
      ROS_INFO_STREAM(world_pose);

      // Overwrite the global array world with the detected marker's pose

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
          int j = msg.detections[i].id; //msg.header.frame_id

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
void ControlNode::broadcastWorld()   // Does not work properly
{
  //std::isnan();     //Check if the values to pass the transform are valid
  try
  {
    tf::TransformBroadcaster brw;
    tf::Transform transformw;
    //tf::TransformListener listenw;
    //ros::Time now = ros::Time::now();

    // Broadcast the world frame at any moment

    //listenw.waitForTransform("marker", "cam0", now, ros::Duration(10.0));
    transformw.setOrigin(tf::Vector3(world_pose.position.x, world_pose.position.y, world_pose.position.z));
    transformw.setRotation(tf::Quaternion(world_pose.orientation.x, world_pose.orientation.y, world_pose.orientation.z, world_pose.orientation.w));
    brw.sendTransform(tf::StampedTransform(transformw.inverse(), ros::Time::now(), "world" , "cam0"));
    ROS_INFO_STREAM(world_pose);
    //ROS_INFO("World broadcasted");
  }
  catch(...)
  {
    ROS_INFO("Failed to broadcast world");
  }
}

void ControlNode::broadcastCF(int cf_id)
{
  //std::isnan();     //Check if the values to pass the transform are valid
  try
  {
    tf::TransformBroadcaster brcf[4];
    tf::Transform transformcf[4];
    //tf::TransformListener listenw;
    //ros::Time now = ros::Time::now();

    // Broadcast the world frame at any moment

    //listenw.waitForTransform("marker", "cam0", now, ros::Duration(10.0));
    transformcf[cf_id].setOrigin(tf::Vector3(cf_pose[cf_id].position.x, cf_pose[cf_id].position.y, cf_pose[cf_id].position.z));
    transformcf[cf_id].setRotation(tf::Quaternion(cf_pose[cf_id].orientation.x, cf_pose[cf_id].orientation.y, cf_pose[cf_id].orientation.z, cf_pose[cf_id].orientation.w));
    brcf[cf_id].sendTransform(tf::StampedTransform(transformcf[cf_id], ros::Time::now(), "cam0" , "CF1"));  //+ sprintf(i))
    //ROS_INFO_STREAM(cf_pose);
    //ROS_INFO("CF broadcasted");
  }
  catch(...)
  {
    ROS_INFO("Failed to broadcast world");
  }
}

  // Reads key input in terminal
char ControlNode::getch()
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

geometry_msgs::PoseStamped::_pose_type ControlNode::getWorldPose() const
{
  return world_pose;
}

geometry_msgs::PoseStamped::_pose_type ControlNode::getCfPose(int cf_id) const
{
  return cf_pose[cf_id];
}

  // Set private variables
void ControlNode::setWorldPose(cob_object_detection_msgs::DetectionArray &wdp) // geometry_msgs::PoseStamped::_pose_type
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

void ControlNode::setCfPose(cob_object_detection_msgs::DetectionArray &cfp, int jj, int ii)
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


/*switch (j)
              {                                          // We do this so that we know that we can address the pose to the right marker. Will be replaced by the radio choice
                case 0: {ROS_INFO("CF 0:"); break;};     //ROS_INFO("CF 0:")
                case 1: {ROS_INFO("CF 1:"); break;};
                case 2: {ROS_INFO("CF 2:"); break;};
                case 3: {ROS_INFO("CF 3:"); break;};
                default:{ROS_INFO("ID Fail"); break;};                                  // If an error occurs and the detections[i] is accessed erronially
              }*/
