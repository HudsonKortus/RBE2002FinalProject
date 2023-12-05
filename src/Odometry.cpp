#include <Romi32U4.h>
#include "Odometry.h"
#include "Speed_controller.h"
#include "Encoders.h"

//sensors
Romi32U4ButtonA buttonA;

//motor-speed controller
//SpeedController robot;

Encoder RomiEncoders;


void Odometry::UpdateOdometry() //target speed in mm/s
{

    float measured_speed_right = RomiEncoders.ReadVelocityRight();
    float measured_speed_left = RomiEncoders.ReadVelocityLeft();

    //Serial.println(millis() - time_prev);
        time_prev = millis();
        //assignment
   
        if(abs(measured_speed_right - measured_speed_left) > .1 ) {
            //curved movement
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
            /*
            // Serial.print(R_theoretical);
            // Serial.print(" ");
            // Serial.print(w_theoretical);
            // Serial.print(" ");
            // Serial.println(theta_calculated);

            // Serial.print(measured_speed_right);
            // Serial.print(" ");
            // Serial.println(measured_speed_left);
*/

        } else {
            //straight movement
            //Serial.println("Straight");

            //measured calculations
            float V_calculated = (measured_speed_left + measured_speed_right)/2;
            x_calculated += V_calculated * cos(theta_calculated) * timeIncrement; //50ms is the set time update
            y_calculated += V_calculated * sin(theta_calculated) * timeIncrement;
            Serial.print("V: ");
            Serial.print(V_calculated);
        }


        Serial.print("x: ");
        Serial.print(x_calculated);
        Serial.print("y: ");
        Serial.println(y_calculated);
        
}



float Odometry::calculateFigureight(float Xpostion, float xSpeed) {
    
    float targetYposition;
    targetYposition = sqrt(-(pow(x_calculated,4) / pow(a,2)) + pow(x_calculated,2) ) - (h*yPrevious);
    

    //move in y to compensate for error
    float yError = targetYposition - y_calculated;
    yPrevious = targetYposition;
    float ySpeed = xSpeed + yError;
    return ySpeed;
    //if y error is 0, it shoudl be tqual to x speed, if it is twice as large as x speed it 
    //
}

float Odometry::getX(void) {
    return x_calculated;
}

float Odometry::getY(void) {
    return y_calculated;
}

float Odometry::getTheta(void) {
    return theta_calculated;
}

