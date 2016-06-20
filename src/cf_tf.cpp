//  THIS IS RESTRUCTURE  //

#include "cf_tf/cf_tf_node.hpp"

int main(int argc, char* argv[])
{
  ros::init(argc, argv, "cf_tf"); //Name of the node
  Cf_Tf Node;
  Node.initializeWorldPose();

  //tf::TransformListener listenmarker;
  //ros::Time now = ros::Time::now();

  // Broadcast the world frame at any moment
  ros::Duration(2).sleep();
  while(Node.nh.ok())
  {
    //listenmarker.waitForTransform("camera1", "marker", now, ros::Duration(10.0));
    ros::spinOnce();
    Node.broadcastWorld();
    //Node.broadcastCF();
  }
}
