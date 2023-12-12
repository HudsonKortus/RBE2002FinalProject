#include <Arduino.h>
//#include "Behaviors.h"
#include "Speed_controller.h"
#include "line_follow.h"
#include "IR_sensor.h"
#include "Sonar_sensor.h"

//Behaviors positionEstimation;


Romi32U4ButtonB buttonB;
LineFollow robot; 
enum ROBOT_STATE {IDLE, SEARCH, DRIVE_LINE, TURN, OPEN_LEFT, CLOSED_LEFT, CLOSED_FRONT};
ROBOT_STATE robot_state = IDLE; //initial state: IDLE
int count = 0;


IRsensor irSensor;
IRsensor irSensorFront;
SonarSensor sonarSensor;
const float TOO_CLOSE = 20, TOO_CLOSE_BUT_THIS_TIME_WITH_THE_IR_SENSOR = 15; //cm

void setup() {
  //positionEstimation.Init();
  robot.Init();
  irSensorFront.Init(A11);
  irSensor.Init(A0);
}



void loop() {
  //positionEstimation.Run();
  if(millis() % 10 == 0){
  Serial.print("Front: ");
  irSensorFront.PrintData();
  Serial.print("Side: ");
  irSensor.PrintData();
  }
  if(robot.UpdateEncoderCounts()){
    robot.UpdatePose(robot.ReadVelocityLeft(), robot.ReadVelocityRight());
    //Serial.println(robot.getThetaDeg());
    switch(robot_state){
      case IDLE:

        if(buttonB.getSingleDebouncedRelease()){
          robot_state = DRIVE_LINE;
          Serial.println("DRIVE_LINE");
        }
        break;

        case DRIVE_LINE:
          if(robot.reachedIntersection()){
            //robot.makeWaypoint();
            robot.centerVTC();
            robot.resetOdomytry();
            robot_state = TURN;
                      Serial.println("TURN");

          }else{
            robot.lineFollow(45);
          }
          if(buttonB.getSingleDebouncedRelease()){
            robot_state = DRIVE_LINE;
                      Serial.println("DRIVE_LINE");

          }
        break;

        case TURN:
        /*
          Check ultrasonic at intersections to see if it's "too close"
            If too close, check IR to see if we can turn left
              if we can, turn left
              if we cannot, turn right
                if the ultrasonic says there's a wall there, turn right again

        */
       //if nop wall on left
        if(irSensor.ReadData() >= TOO_CLOSE){
            //Open on left
            robot_state = OPEN_LEFT;
                      Serial.println("OPEN_LEFT");

        //if wall is to front of robot
        }else if(irSensorFront.ReadData() <= TOO_CLOSE){
          //Closed on left
          robot_state = CLOSED_LEFT;
                    Serial.println("CLOSED_LEFT");

        }else{
          //Closed on left open on front
          robot_state = DRIVE_LINE;
                    Serial.println("DRIVE_LINE");

        }
          // if(robot.turnToNextline(-100)){
          //   robot_state = DRIVE_LINE;
          // }

        break;
        case OPEN_LEFT:
            if(robot.turnToNextline(75)){
              robot_state = DRIVE_LINE;
                        Serial.println("DRIVE_LINE");

            }
        break;
        case CLOSED_LEFT:
          if(robot.turnToNextline(-75)){
            irSensorFront.PrintData();
            if(irSensorFront.ReadData() <= TOO_CLOSE){
              //closed on front
              robot_state = CLOSED_FRONT;
                        Serial.println("CLOSED_FRONT");

            }else{
              robot_state = DRIVE_LINE;
                        Serial.println("DRIVE_LINE");

            }
          }
        break;
        case CLOSED_FRONT:
          if(robot.turnToNextline(-75)){
             robot_state = DRIVE_LINE;
                       Serial.println("DRIVE_LINE");

          }
        break;
        if(buttonB.getSingleDebouncedRelease()){
          robot.Stop();
          robot_state = IDLE;
                    Serial.println("IDLE");

        }
    }

  }

}