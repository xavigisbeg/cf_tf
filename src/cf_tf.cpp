//  THIS IS RESTRUCTURE  //

#include "cf_tf/cf_tf_node.hpp"

int main(int argc, char* argv[])
{
  ros::init(argc, argv, "cf_tf"); //Name of the node
  Cf_Tf Node;
  Node.initializeWorldPose();

  //ros::Duration(2).sleep();

  while(Node.nh.ok())
  {
    ros::spinOnce();
    Node.broadcastWorld();
    //Node.broadcastCF();
  }
}
