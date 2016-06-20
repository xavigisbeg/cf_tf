//  THIS IS RESTRUCTURE  //
#include <ros/ros.h>

#include "cf_tf/cf_tf_listener.hpp"

/*int main(int argc, char **argv)
{
  return 0;
}*/



// Cob_fiducials namespace should be added
bool flag_world = false;
geometry_msgs::Pose::_position_type wrldp;
geometry_msgs::Pose::_orientation_type wrldo;
geometry_msgs::Pose::_position_type CFposep;
geometry_msgs::Pose::_orientation_type CFposeo;
geometry_msgs::PoseStamped::_pose_type world;


//geometry_msgs::Pose::_position_type worldpos;
//geometry_msgs::Pose::_orientation_type worldori;

char getch();
void Callback(const cob_object_detection_msgs::DetectionArray &msg);        //Do we want reference or not? CppStyleGuide says no
void broadcastWorld();
void broadcastCF();

/*using cf_tf;

void listenerCallback(const cob_object_detection_msgs::DetectionArray msg15);

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("/fiducials/fiducial_detection_array", 10, listenerCallback);

  puts("Default world set, press key to set world where the marker is detected");

  while(nh.ok())
  {
    ros::spinOnce();
    broadcastWorld();
  }
  return 0;
}*/

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle nh;
  tf::TransformListener listenw;
  ros::Subscriber sub = nh.subscribe("/fiducials/fiducial_detection_array",10,Callback);   // Basic subscriber. Just has the topic it is subscribed to, the callback function and how many messages it caches
  puts("Press any key to set /world");
  wrldp.x = 0.0;
  wrldp.y = 0.0;
  wrldp.z = 3.0;
  wrldo.x = 0.0;
  wrldo.y = 0.0;
  wrldo.z = 0.0;
  wrldo.w = 1.0;

  CFposep.x = 0.0;
  CFposep.y = 0.0;
  CFposep.z = 3.0;
  CFposeo.x = 0.0;
  CFposeo.y = 0.0;
  CFposeo.z = 0.0;
  CFposeo.w = 1.0;

  //ros::Duration(0.5).sleep();

  while(nh.ok())
  {
    //ros::spinOnce();

    ros::Time now = ros::Time::now();

    //Broadcast the world frame at any moment

    listenw.waitForTransform("cam0", "marker", now, ros::Duration(10.0)); //"marker", "cam0"

    ros::getGlobalCallbackQueue()->callAvailable(ros::WallDuration(0));
    broadcastWorld();
    broadcastCF();
  }

  return 0;
}

void Callback(const cob_object_detection_msgs::DetectionArray &msg)
{                        // const because we are just reading it, & to pass by reference
  //geometry_msgs::Pose::_position_type CFposep;
  //geometry_msgs::Pose::_orientation_type CFposeo;
  //      WORLD FRAME CREATION      //
  if (flag_world == false)
  {                                                               // We have no /world yet
      if (msg.detections.empty() != true)
      {
      // Reads key input in terminal

      char c = getch();                                                               // Call non-blocking input function
      if (c == 'w')
      {           // BUG: Does not compare properly the char value, is true when any key is pressed
        flag_world = true;                                                              // Will not enter this condition again
        puts("/world set");
        ROS_INFO_STREAM(wrldp << " " << wrldo);

        // Overwrite the global array world with the detected marker's pose
        wrldp.x = msg.detections[0].pose.pose.position.x;
        wrldp.y = msg.detections[0].pose.pose.position.y;
        wrldp.z = msg.detections[0].pose.pose.position.z;
        wrldo.x = msg.detections[0].pose.pose.orientation.x;
        wrldo.y = msg.detections[0].pose.pose.orientation.y;
        wrldo.z = msg.detections[0].pose.pose.orientation.z;
        wrldo.w = msg.detections[0].pose.pose.orientation.w;
      }
    }
  }
  // We have a world
  else{
    if (msg.detections.empty() != true)
    {

    // We try to post the world marker frame.

    // Here we repeat some actions for each marker detected. Note that marker id and detection [i] are not the same.
    for (int i=0; i <= msg.detections.size()-1; i++)
      {                               // Checks if a marker has been detected
        try
        {
          int j = msg.detections[i].id;
          switch (j)
          {                                          // We do this so that we know that we can address the pose to the right marker. Will be replaced by the radio choice
            case 0: {ROS_INFO("CF 0:"); break;};     //ROS_INFO("CF 0:")
            case 1: {ROS_INFO("CF 1:"); break;};
            case 2: {ROS_INFO("CF 2:"); break;};
            case 3: {ROS_INFO("CF 3:"); break;};
            default:{ROS_INFO("ID Fail"); break;};                                  // If an error occurs and the detections[i] is accessed erronially
          }
          CFposep.x = msg.detections[i].pose.pose.position.x;
          CFposep.y = msg.detections[i].pose.pose.position.y;
          CFposep.z = msg.detections[i].pose.pose.position.z;
          CFposeo.x = msg.detections[i].pose.pose.orientation.x;
          CFposeo.y = msg.detections[i].pose.pose.orientation.y;
          CFposeo.z = msg.detections[i].pose.pose.orientation.z;
          CFposeo.w = msg.detections[i].pose.pose.orientation.w;

          ROS_INFO_STREAM(msg.detections[i].pose.pose);
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

void broadcastWorld()
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
      transformw.setOrigin(tf::Vector3(wrldp.x, wrldp.y, wrldp.z));
      transformw.setRotation(tf::Quaternion(wrldo.x, wrldo.y, wrldo.z, wrldo.w));
      brw.sendTransform(tf::StampedTransform(transformw.inverse(), ros::Time::now(), "world" , "cam0"));
      ROS_INFO_STREAM(wrldp << wrldo);
      //ROS_INFO("World broadcasted");
  }
  catch(...)
  {
      ROS_INFO("Failed to broadcast world");
  }
}

// We try to post the detected crazyflie's frame

void broadcastCF()
{
  //std::isnan();     //Check if the values to pass the transform are valid
  try
  {
    tf::TransformBroadcaster brcf;
    tf::Transform transformcf;
    //tf::TransformListener listenw;
    //ros::Time now = ros::Time::now();

    // Broadcast the world frame at any moment

    //listenw.waitForTransform("marker", "cam0", now, ros::Duration(10.0));
    transformcf.setOrigin(tf::Vector3(CFposep.x, CFposep.y, CFposep.z));
    transformcf.setRotation(tf::Quaternion(CFposeo.x, CFposeo.y, CFposeo.z, CFposeo.w));
    brcf.sendTransform(tf::StampedTransform(transformcf, ros::Time::now(), "cam0" , "CF1"));  //+ sprintf(i))
    //ROS_INFO_STREAM(CFposep << CFposo);
    //ROS_INFO("CF broadcasted");
  }
  catch(...)
  {
      ROS_INFO("Failed to broadcast world");
  }
}


// CODE THAT HAS BEEN TESTED AND IS KEPT FOR FUTURE USEFUL REASONS

/*transformw.setOrigin(tf::Vector3(wrld.detections[0].pose.pose.position.x, wrld.detections[0].pose.pose.position.y, wrld.detections[0].pose.pose.position.z));
transformw.setRotation(tf::Quaternion(wrld.detections[0].pose.pose.orientation.x, wrld.detections[0].pose.pose.orientation.y, wrld.detections[0].pose.pose.orientation.z, wrld.detections[0].pose.pose.orientation.w));
brw.sendTransform(tf::StampedTransform(transformw, ros::Time::now(), "world" , "cam0"));


  // Reads key input in terminal
*/char getch()
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

/*char c = getch();                                                               // Call non-blocking input function
if (c == 'w');
{           // BUG: Does not compare properly the char value, is true when any key is pressed

}*/

/*CFposep[msg.detections[i].id].x = msg.detections[i].pose.pose.position.x;
CFposep[msg.detections[i].id].y = msg.detections[i].pose.pose.position.y;
CFposep[msg.detections[i].id].z = msg.detections[i].pose.pose.position.z;
CFposeo[msg.detections[i].id].x = msg.detections[i].pose.pose.orientation.x;
CFposeo[msg.detections[i].id].y = msg.detections[i].pose.pose.orientation.y;
CFposeo[msg.detections[i].id].z = msg.detections[i].pose.pose.orientation.z;
CFposeo[msg.detections[i].id].w = msg.detections[i].pose.pose.orientation.w;

//ROS_INFO_STREAM(msg.detections[i].pose.pose);

// We try to post the detected crazyflie's frame

tf::TransformBroadcaster brcf;
tf::Transform transformcf[msg.detections.size()];
transformcf[i].setOrigin(tf::Vector3(CFposep[i].x, CFposep[i].y, CFposep[i].z));
transformcf[i].setRotation(tf::Quaternion(CFposeo[i].x, CFposeo[i].y, CFposeo[i].z, CFposeo[i].w));
brcf.sendTransform(tf::StampedTransform(transformcf[i], ros::Time::now(), "world" , "CF1"));  //+ sprintf(i))*/

// goal transform, wrong because it is a topic with PoseStamped /goal

//geometry_msgs::Pose::_position_type goalp;
//geometry_msgs::Pose::_orientation_type goalo;

/*goalp.x = wrldp.x;
goalp.y = wrldp.y;
goalp.z = wrldp.z + 1.0;
goalo.x = wrldo.x;
goalo.y = wrldo.y;
goalo.z = wrldo.z;
goalo.w = wrldo.w;

tf::TransformBroadcaster brgoal;
tf::Transform transformgoal;
transformgoal.setOrigin(tf::Vector3(goalp.x, goalp.y, goalp.z));
transformgoal.setRotation(tf::Quaternion(goalo.x, goalo.y, goalo.z, goalo.w));
brgoal.sendTransform(tf::StampedTransform(transformgoal, ros::Time::now(), "world" , "goal"));  //+ sprintf(i)) */

//Publish this: rostopic pub /goal geometry_msgs/PoseStamped '{header: {stamp: now, frame_id: "world"}, pose: {position: {x: 0.0, y: 0.0, z: 2.0}, orientation: {w: 1.0}}}'
