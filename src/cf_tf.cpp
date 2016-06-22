//  THIS IS RESTRUCTURE  //

#include "cf_tf/cf_tf_node.hpp"

int main(int argc, char* argv[])
{
  ros::init(argc, argv, "cf_tf"); //Name of the node
  Cf_Tf Node;
  Node.initializeWorldPose();
  Node.initializeCfPose();

  //ros::Duration(2).sleep();

  while(Node.nh.ok())
  {
    ros::spinOnce();
    //ROS_INFO("iSpin");
    Node.broadcastWorld();
    for (int cf_id=0; cf_id<=3; cf_id++)
    {
      Node.broadcastCF(cf_id);
      ROS_INFO_STREAM(Node.getCfPose(cf_id));
    }
  }
  return 0;
}
