#include <Arduino.h>
//#include "Behaviors.h"
#include "Speed_controller.h"
#include "line_follow.h"
//#include <Rangefinder.h>

//Behaviors positionEstimation;
//this is a test can you see what I am typing


Romi32U4ButtonA buttonA;
LineFollow robot; 
enum ROBOT_STATE {IDLE, SEARCH, DRIVE_LINE, TURN};
ROBOT_STATE robot_state = IDLE; //initial state: IDLE



//Rangefinder rangefinder(17, 30);
const float TOO_CLOSE = 9; //cm

void setup() {
  //positionEstimation.Init();
  //robot.Init();
  //rangefinder.init();
}


//test reset odomyttry




void loop() {
  //positionEstimation.Run();


  if(robot.UpdateEncoderCounts()){
      robot.UpdatePose(robot.ReadVelocityLeft(), robot.ReadVelocityRight());
    switch(robot_state){
      case IDLE:

        if(buttonA.getSingleDebouncedRelease()){
          robot_state = DRIVE_LINE;
        }
        break;

        case DRIVE_LINE:
          if(robot.reachedIntersection()){
            //robot.makeWaypoint();
            robot.centerVTC();
            robot_state = TURN;
          }else{
            robot.lineFollow(45);
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