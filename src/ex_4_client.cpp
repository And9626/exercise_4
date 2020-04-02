#include "ros/ros.h"
#include <iostream>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include "exercise_4/controllerAction.h"

int main (int argc, char **argv) {

	ros::init(argc, argv, "controller_action_client");
  if(argc != 5){
		ROS_INFO("%d",argc);
		ROS_WARN("Usage: demo_action_client <init_pos> <goal> <max_velocity> <time_to_preempt_in_sec>");
		return 1;
	}


  actionlib::SimpleActionClient<exercise_4::controllerAction> ac("c_action", true);

  ROS_INFO("Waiting for action server to start.");


  ac.waitForServer(); 

  ROS_INFO("Action server started, sending goal.");


  exercise_4::controllerGoal goal;
   
  goal.init_pos = atof(argv[1]);
  goal.position = atof(argv[2]);
  goal.max_velocity = atof(argv[3]);
 

	
 
  ROS_INFO("Sending Goal [%f] and Preempt time of [%f]",goal.position, atof(argv[4]));
  ac.sendGoal(goal);


  bool finished_before_timeout = ac.waitForResult(ros::Duration(atof(argv[4])));
  ac.cancelGoal();

  if (finished_before_timeout) {
    actionlib::SimpleClientGoalState state = ac.getState();
    ROS_INFO("Action finished: %s",state.toString().c_str());

		ac.cancelGoal();

  }
  else
    ROS_INFO("Action did not finish before the time out.");

  return 0;
}
