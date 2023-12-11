#include  "Position_estimation.h"
#include "Encoders.h"
/*
    Use arrays for x and y, then repeat the last real coordinate when we delete dead ends. Double the size of the array when it 
    gets too big
*/
unsigned long time_prev = 0;
unsigned long time_now = millis();
float timeIncrement = 0.050;

void Position::Init(void)
{
    time_now = millis();
    x = 0;
    y = 0;
    theta = 0;
    waypoint[0][0] = 0;
    waypoiontCounter = 1;
}

void Position::Stop(void)
{
    time_now = millis();
    x = 0; 
    y = 0;
    theta = 0;
}

float Position::getX()
{
    return x;
}

float Position::getY()
{
    return y;
}

float Position::getTheta()
{
    return theta;
}

float Position::getThetaDeg()
{
    return theta*(180/PI);
}


void Position::PrintPose(void)
{
    Serial.print(x);
    Serial.print("  ");
    Serial.print(y);
    Serial.print("  ");
    Serial.print(theta);
    Serial.print(" ");
//     Serial.print("|");
//     Serial.print(" ");
//     Serial.print(x_theoretical);
//     Serial.print("  ");
//     Serial.print(y_theoretical);
//     Serial.print("  ");
//     Serial.println(theta_theoretical);
}

void Position::UpdatePose(float measured_speed_left, float measured_speed_right) //target speed in mm/s
{
        time_prev = millis();
         if(abs(measured_speed_right - measured_speed_left) > .25 ) {
            Serial.println("Curved");
           //measured calculations
            float R_measured = (l/2)*((measured_speed_right + measured_speed_left) 
                / (measured_speed_right - measured_speed_left));
            float w_measured = (measured_speed_right - measured_speed_left) / l;

            x_calculated = x_calculated - (R_measured * sin(theta_calculated)) + 
                (R_measured * sin((theta_calculated + (w_measured * timeIncrement))));

            y_calculated += (R_measured * cos(theta_calculated)) - 
                (R_measured * cos(theta_calculated + (w_measured * timeIncrement)));
            
            theta_calculated += w_measured * timeIncrement;
            

        } else {
            //straight movement
            Serial.println("Straight");

            //measured calculations
            float V_calculated = (measured_speed_left + measured_speed_right)/2;
            x_calculated += V_calculated * cos(theta_calculated) * timeIncrement; //50ms is the set time update
            y_calculated += V_calculated * sin(theta_calculated) * timeIncrement;
            
        }

        if(theta_calculated > 2*PI){
            theta_calculated -= 2*PI;
        }
        x = x_calculated;
        y = y_calculated;
        theta = theta_calculated;
        //PrintPose();
}

//untested
void Position::restOdomytry(){
    float heading = getThetaDeg();
    
    Serial.print("heading");

    Serial.println(heading);
    //0 degree case
    if (heading > 345 || heading < 15){
            Serial.print("0 degree case");
            waypoint[waypoiontCounter][0] ++;
            waypoiontCounter++;
    }
    //90 degree case
    else if (heading > 75 && heading < 105){
        waypoint[waypoiontCounter][1] ++;
            waypoiontCounter++;    
    }
    // 180 degree case
    else if (heading > 165 && heading < 195){
        waypoint[waypoiontCounter][0] --;
        waypoiontCounter++;  
        Serial.print("180 degree case");
    }
    //270 degree case
    else if (heading > 255 && heading < 285){
        waypoint[waypoiontCounter][1] --;
        waypoiontCounter++;  
        Serial.print("270 degree case");
    }
    else{
        Serial.print("error");
    }


  //always orent the robot with x facing x+
//increment world waypoint counter
// if we are at 0, increment x by 1
// if we are at 90, increment y by 1
//if we are at 180, decrement x by 1
//if we are at 270, decrement y by 1 
//assume x and y unit are equivelent to 40cm -> 400mm
  //if(robot.getTheta())
}