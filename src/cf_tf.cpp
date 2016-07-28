//  THIS IS RESTRUCTURE  //

#include "cf_tf/cf_tf_node.hpp"

int main(int argc, char* argv[])
{
  ros::init(argc, argv, "cf_tf"); //Name of the node
  Cf_Tf Node;
  Node.initializeWorldPose();
  Node.initializeCfPose();

  while(Node.nh.ok())
  {
    ros::spin();
  }
  return 0;
}
