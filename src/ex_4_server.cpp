#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include <actionlib/server/simple_action_server.h>
#include "exercise_4/controllerAction.h"
#include <iostream>
#include <sstream>

class action_class {

	private:
  	ros::NodeHandle nh_;
  	actionlib::SimpleActionServer<exercise_4::controllerAction> as; 
  	ros_action::controllerFeedback feedback;
	ros_action::controllerResult result;

  	std::string action_controller;
  	int goal;
  	int progress = atoi(argv[1]);

	public:
		action_class(std::string name) :
  	  as(nh_, controller, boost::bind(&action_class::executeCB, this, _1), false),
  	  action_controller(controller) {
				as.registerPreemptCallback(boost::bind(&action_class::preemptCB, this));
				as.start();
  		}
	
		void preemptCB(){
			ROS_WARN("%s got preempted!", action_controller.c_str());
			result.final_position = progress;
			as.setPreempted(result,"I got Preempted"); 
  	}
  
		void executeCB(const ros_action::controllerGoalConstPtr &goal) {
			if(!as.isActive() || as.isPreemptRequested()) return;
			ros::Rate rate(10);
			ROS_INFO("%s is processing the goal %f", action_controller.c_str(), goal->position);
			float control_action;
				do{
				if (!ros::ok()) {
					result.final_position = progress;
					as.setAborted(result,"I failed !");
					ROS_INFO("%s Shutting down",action_controller.c_str());
					break;
				}
		
				if(!as.isActive() || as.isPreemptRequested()){
					return;
				}	

				if(goal->position == progress) {
					ROS_INFO("%s Succeeded at getting to goal %d", action_controller.c_str(), goal->position);
					result.final_position = progress;
					as.setSucceeded(result);
				}
				else {
					ROS_INFO("Setting to goal %d / %d",feedback.current_position,goal->position);
					feedback.current_position = progress;
					as.publishFeedback(feedback);
			}
		control_action = atoi(argv[2])/10;
		if(progress>goal->position)
			progress = progress - control_action;
		if(progress<goal->position)
			progress = progress + control_action;
		rate.sleep();
		}while( progress != goal->position)	
  }
};

int main(int argc, char** argv)
{

  ros::init(argc, argv, "controller_action");
  ROS_INFO("Starting Controller");
  action_class controller_action_obj(ros::this_node::getName());
  ros::spin();
  return 0;
}
