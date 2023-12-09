#include <Arduino.h>
//#include "Behaviors.h"
#include "Speed_controller.h"
#include <Rangefinder.h>
#include "Apriltags.h"


//Behaviors positionEstimation;
//this is a test can you see what I am typing


Romi32U4ButtonA buttonA;
SpeedController robot; 
enum ROBOT_STATE {IDLE, SEARCH};
ROBOT_STATE robot_state = IDLE; //initial state: IDLE

enum DRIVE_STATE {LINE_FOLLOW, TURN};
DRIVE_STATE drive_state = LINE_FOLLOW;
int LEFT_LINE_SENSOR_PIN = 21;
int RIGHT_LINE_SENSOR_PIN = 22;
const float kp_line = 0.15;             // line following kp
const float base_speed = 40;
const float REFLECTANCE_THRESHOLD = 50;
const float VTC_TO_CENTER = 100;//mm
Rangefinder rangefinder(17, 30);
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

void lineFollow() // line follow function
{
  long left = analogRead(LEFT_LINE_SENSOR_PIN);   // read the left line sensor
  long right = analogRead(RIGHT_LINE_SENSOR_PIN); // read the right line sensor

  int16_t reflectanceError = left - right;   // calculate the reflectance error
  float effort = kp_line * reflectanceError; // calculate the line effort

  robot.setEfforts(base_speed + effort, base_speed - effort); // set motor speed
}
bool reachedIntersection(){
  long left = analogRead(LEFT_LINE_SENSOR_PIN);   // read the left line sensor
  long right = analogRead(RIGHT_LINE_SENSOR_PIN); // read the right line sensor
  Serial.println(left + right);
  if (left + right < REFLECTANCE_THRESHOLD){
    return true;
  }else {
    return false;
  }

}
void restOdomytry(){
  //always orent the robot with x facing x+
//increment world waypoint counter
// if we are at 0, increment x by 1
// if we are at 90, increment y by 1
//if we are at 180, decrement x by 1
//if we are at 270, decrement y by 1 
//assume x and y unit are equivelent to 40cm -> 400mm
  if(robot.getTheta())
}

//***blocking****
void centerVTC(){
  //drive forward 100mm in x
  robot.Straight(25,4);
}


void loop() {
  //positionEstimation.Run();
  if(robot.UpdateEncoderCounts()){
    switch(drive_state){
      case LINE_FOLLOW:
        if(reachedIntersection()){
          robot.makeWaypoint();
          drive_state = TURN;
        }else{
          lineFollow();
        }
      break;
      case TURN:
        robot.Turn(90,1);//Turn 90 degrees CW
        if(rangefinder.distance() <= TOO_CLOSE){
          //We are in a deadend
          robot.Turn(90,1); //This is blocking code
          drive_state = LINE_FOLLOW;
        }else{
          drive_state = LINE_FOLLOW;
        }
      break;
    }

    switch(robot_state){
      case IDLE:
        if(buttonA.getSingleDebouncedRelease()){
          robot_state = SEARCH;
        }
        break;
      case SEARCH:
        robot.UpdatePose(robot.ReadVelocityLeft(), robot.ReadVelocityRight());
        //search for april tag denoting key or garrage

        //if on straight line, basic drive
        //if on intersection
          // drive until vtc is over intection and update odomytry
          // read ultrasonnic an decide where to go
          //make turn movement while still scanning 
          //when turn is over go back to basic drive
          //lineFollow();
          //robot.setEfforts(50,50);
          reachedIntersection();
          
          if(buttonA.getSingleDebouncedRelease()){
            robot.Stop();
            robot_state = IDLE;
          }
        break;
    }
  }
}