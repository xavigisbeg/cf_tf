//  THIS IS RESTRUCTURE  //

#define glue(a,b) a ## b

#include <ros/ros.h>

#include "cf_tf/cf_tf_node.hpp"

Cf_Tf::Cf_Tf()
{
  ros::NodeHandle params("~");
  std::string s;

  params.param<std::string>("detection_array_topic", s, "/fiducials/fiducial_detection_array");
  m_marker_pose_sub = nh.subscribe(s, 10, &Cf_Tf::listenerCallback, this);
  //for (int i=0; i<=3; i++)
  //{
    m_cf_pose_pub[0] = nh.advertise<geometry_msgs::PoseStamped::_pose_type>("cf_pose0", 1);
    m_cf_pose_pub[1] = nh.advertise<geometry_msgs::PoseStamped::_pose_type>("cf_pose1", 1);
    m_cf_pose_pub[2] = nh.advertise<geometry_msgs::PoseStamped::_pose_type>("cf_pose2", 1);
    m_cf_pose_pub[3] = nh.advertise<geometry_msgs::PoseStamped::_pose_type>("cf_pose3", 1);
  //}
  m_world_pose_pub = nh.advertise<geometry_msgs::PoseStamped::_pose_type>("world_pose", 1);
  m_serviceSetWorld = nh.advertiseService("setworld", &Cf_Tf::serviceSetWorld, this);
  //m_marker_pose_sub = nh.subscribe("/fiducials/fiducial_detection_array", 10, &Cf_Tf::listenerCallback, this);
}

void Cf_Tf::listenerCallback(const cob_object_detection_msgs::DetectionArray &msg)
{
  //      WRITING IN WORLD FRAME VARIABLE      //
  if (flag_world == true)
  {                                                               // We have no /world yet
    if (msg.detections.empty() != true)
    {
      setWorldPose(msg);                                        // Will not enter this condition again
      m_world_pose_pub.publish(world_pose);
      puts("World set");
      ROS_INFO_STREAM("World Position:" << world_pose);
    }
  }
  // We have a world
  else{   
    if (msg.detections.empty() != true)
    {
      broadcastWorld();
      // Note that marker id [j] and detection [i] are not the same.
      for (int i=0; i <= msg.detections.size()-1; i++)
      {                               // Checks if a marker has been detected
        try
        {
          int j = msg.detections[i].id; //msg.header.frame_id   msg.detections[i].id
          /*switch (j)
          {                                          // We do this so that we know that we can address the pose to the right marker. Will be replaced by the radio choice
            case 0: {ROS_INFO("CF 0:"); break;};     // ROS_INFO("CF 0:")
            case 1: {ROS_INFO("CF 1:"); break;};
            case 2: {ROS_INFO("CF 2:"); break;};
            case 3: {ROS_INFO("CF 3:"); break;};
            default:{ROS_INFO("ID Fail"); break;};                                  // If an error occurs and the detections[i] is accessed erronially
          }*/
          setCfPose(msg, j, i);
          m_cf_pose_pub[j].publish(cf_pose[j]);
          broadcastCF(j);
          //nav_msgs::Path m_cf_path_pub[j].publish(cf_pose[j])
          //ROS_INFO_STREAM(msg.detections[i].id << msg.detections[i].pose.pose << j << cf_pose[j]);   //We want to check that the variable has been correctly written
          //ROS_INFO_STREAM(msg.detections.size());
        }
        catch(...)
        {
          ROS_ERROR("Failed to write the CF pose"); //sprintf(buffer, i));              // Need to find how to efficiently convert int to string without too much pain
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
    // Broadcast the world frame at any moment

    Cf_Tf::transform.setOrigin(tf::Vector3(world_pose.position.x, world_pose.position.y, world_pose.position.z));
    Cf_Tf::transform.setRotation(tf::Quaternion(world_pose.orientation.x, world_pose.orientation.y, world_pose.orientation.z, world_pose.orientation.w));
    Cf_Tf::br.sendTransform(tf::StampedTransform(transform.inverse(), ros::Time::now(), "world" , "camera"));
    //ROS_INFO_STREAM(world_pose);
    //ROS_INFO("World broadcasted");
  }
  catch(...)
  {
    ROS_ERROR("Failed to broadcast world");
  }
}

void Cf_Tf::broadcastCF(int cf_id)
{
  //std::isnan();     //Check if the values to pass the transform are valid
  try
  {   
    // Broadcast the crazyflies' frames at any moment

    Cf_Tf::transform.setOrigin(tf::Vector3(cf_pose[cf_id].position.x, cf_pose[cf_id].position.y, cf_pose[cf_id].position.z));
    Cf_Tf::transform.setRotation(tf::Quaternion(cf_pose[cf_id].orientation.x, cf_pose[cf_id].orientation.y, cf_pose[cf_id].orientation.z, cf_pose[cf_id].orientation.w));
    std::string str_ch_fr = "cf" + boost::lexical_cast<std::string>(cf_id);
    Cf_Tf::br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "camera", str_ch_fr));  //+ sprintf(i))
    //ROS_INFO_STREAM(cf_pose);
    //ROS_INFO("CF broadcasted");
  }
  catch(...)
  {
    ROS_ERROR("Failed to broadcast a Crazyflie");
  }
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

  flag_world = false;
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

  m_world_pose_pub.publish(world_pose);

  flag_world = false;
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

  ROS_WARN("The world has been initialized to default");

  flag_world = false;
}

void Cf_Tf::initializeCfPose()
{
  try
  {
    for (int cf_id=0; cf_id<=3; cf_id++)
    {
      cf_pose[cf_id].position.x = 0;
      cf_pose[cf_id].position.y = 0;
      cf_pose[cf_id].position.z = 3;
      cf_pose[cf_id].orientation.x = 0;
      cf_pose[cf_id].orientation.y = 0;
      cf_pose[cf_id].orientation.z = 0;
      cf_pose[cf_id].orientation.w = 1;
    }
  }
  catch(...)
  {
    ROS_ERROR("Failed to initialize the crazyflies' poses");
  }
}

bool Cf_Tf::serviceSetWorld(std_srvs::Empty::Request& req, std_srvs::Empty::Response& res)
{
  ROS_INFO("World setup requested!");
  // If states are used, change state here

  flag_world = true;

  return true;
}

//Publish this: rostopic pub /goal geometry_msgs/PoseStamped '{header: {stamp: now, frame_id: "world"}, pose: {position: {x: 0.0, y: 0.0, z: 1.0}, orientation: {w: 1.0}}}'
