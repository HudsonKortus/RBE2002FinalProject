#include <Arduino.h>
//#include "Behaviors.h"
#include "Speed_controller.h"
#include <Rangefinder.h>
#include "Apriltags.h"
#include "line_follow.h"


//Behaviors positionEstimation;
//this is a test can you see what I am typing


Romi32U4ButtonA buttonA;
LineFollow robot; 
enum ROBOT_STATE {IDLE, SEARCH, DRIVE_LINE, TURN};
ROBOT_STATE robot_state = IDLE; //initial state: IDLE



//Rangefinder rangefinder(17, 30);
const float TOO_CLOSE = 9; //cm

//For apriltags:
AprilTags ap_finder;

void setup() {
  //positionEstimation.Init();
  //robot.Init();

  rangefinder.init();
  ap_finder.Init();
}

//for verifying correct april tag:
//input the id you want it to be.
bool correct_tag(int id) {
  if(ap_finder.FindAPs() == id) {
    return true;
  }
  return false;

}


//test reset odomyttry




void loop() {
  //positionEstimation.Run();
  if(robot.UpdateEncoderCounts()){
    robot.UpdatePose(robot.ReadVelocityLeft(), robot.ReadVelocityRight());
    //Serial.println(robot.getThetaDeg());
    switch(robot_state){
      case IDLE:

        if(buttonA.getSingleDebouncedRelease()){
          robot_state = DRIVE_LINE;
        }
        break;

        case DRIVE_LINE:
          if(robot.reachedIntersection()){
            robot.makeWaypoint();
            robot.centerVTC();
            robot.resetOdomytry();
            robot_state = TURN;
          }else{
            robot.lineFollow(45);
          }
          if(buttonA.getSingleDebouncedRelease()){
            robot_state = DRIVE_LINE;
          }
        break;

        case TURN:
          if(robot.turnToNextline(-20)){
            robot_state = DRIVE_LINE;
          }
        break;
      
        if(buttonA.getSingleDebouncedRelease()){
          robot.Stop();
          robot_state = IDLE;
        }
    }

  }

}