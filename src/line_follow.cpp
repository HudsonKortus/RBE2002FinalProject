
#include <Romi32U4.h>
#include "Encoders.h"
#include "Speed_controller.h"
#include "line_follow.h"
bool first = true;
// line follow function - tested - works
void LineFollow::lineFollow(float baseSpeed) 
{
  long left = analogRead(LEFT_LINE_SENSOR_PIN);   // read the left line sensor
  long right = analogRead(RIGHT_LINE_SENSOR_PIN); // read the right line sensor
//   Serial.println(left);
//   Serial.println(right);
//   Serial.println("");
  int16_t reflectanceError = left - right;   // calculate the reflectance error
  float effort = kp_line * reflectanceError; // calculate the line effort

  setEfforts(baseSpeed - effort, baseSpeed + effort); // set motor speed
}

// tested - works
bool LineFollow::reachedIntersection(){
  long left = analogRead(LEFT_LINE_SENSOR_PIN);   // read the left line sensor
  long right = analogRead(RIGHT_LINE_SENSOR_PIN); // read the right line sensor

  if (left + right < REFLECTANCE_THRESHOLD){
    return true;
  }else {
    return false;
  }

}

//tested works
bool LineFollow::turnToNextline(float baseSpeed){
  if(first){
    Turn(30,(baseSpeed/abs(baseSpeed)));
    first = false;
  }

  setEfforts(-baseSpeed, baseSpeed); // set motor speed
  long left = analogRead(LEFT_LINE_SENSOR_PIN);   // read the left line sensor
  long right = analogRead(RIGHT_LINE_SENSOR_PIN);

  //Serial.println(left);
//   if((baseSpeed < 0 && right < REFLECTANCE_THRESHOLD) || 
//       (baseSpeed > 0 && left < REFLECTANCE_THRESHOLD)){
    if(right < REFLECTANCE_THRESHOLD || left < REFLECTANCE_THRESHOLD){
        Stop();
        first = true;
        return true;
    } else {
        return false;
    }
}

//***blocking****
//tested - works
void LineFollow::centerVTC(){
  //drive forward 100mm in x
  Straight(33,3);
}
