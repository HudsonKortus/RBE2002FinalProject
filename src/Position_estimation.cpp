#include  "Position_estimation.h"
#include "Encoders.h"

Encoder RomiEncoders;

unsigned long time_prev = 0;
unsigned long time_now = millis();
float timeIncrement = 0.050;

void Position::Init(void)
{
    time_now = millis();
    x = 0;
    y = 0;
    theta = 0;
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

void Position::PrintPose(void)
{
    Serial.print(x);
    Serial.print("  ");
    Serial.print(y);
    Serial.print("  ");
    Serial.print(theta);
    Serial.print(" ");
    Serial.print("|");
    Serial.print(" ");
    Serial.print(x_theoretical);
    Serial.print("  ");
    Serial.print(y_theoretical);
    Serial.print("  ");
    Serial.println(theta_theoretical);
}

void Position::UpdatePose(float measured_speed_left, float measured_speed_right, 
    float theoretical_speed_left, float theoretical_speed_right) //target speed in mm/s
{
    //Serial.println(millis() - time_prev);
        time_prev = millis();
        //assignment
   
        if(theoretical_speed_right != theoretical_speed_left 
            && abs(measured_speed_right - measured_speed_left) > .1 ) {
            //curved movement
            //Serial.println("Curved");

            //target calculations
            float R_theoretical = (l/2)*((theoretical_speed_right + theoretical_speed_left) 
                / (theoretical_speed_right - theoretical_speed_left));
            float w_theoretical = (theoretical_speed_right - theoretical_speed_left) / l;
            

            x_theoretical = x_theoretical + ((-R_theoretical * sin(theta_theoretical)) + 
                (R_theoretical * sin((theta_theoretical + (w_theoretical * timeIncrement)))));
            
            y_theoretical += (R_theoretical * cos(theta_theoretical)) - 
                (R_theoretical * cos(theta_theoretical + (w_theoretical * timeIncrement)));

            theta_theoretical += (w_theoretical * timeIncrement);

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
            //Serial.println("Straight");

            //target calculations
            float V_theoretical = (theoretical_speed_left + theoretical_speed_right)/2;
            x_theoretical += V_theoretical * cos(theta_theoretical) * timeIncrement; //50ms is the set time update
            y_theoretical += V_theoretical * sin(theta_theoretical) * timeIncrement;
            //theta stays the same
            
            //measured calculations
            float V_calculated = (measured_speed_left + measured_speed_right)/2;
            x_calculated += V_calculated * cos(theta_calculated) * timeIncrement; //50ms is the set time update
            y_calculated += V_calculated * sin(theta_calculated) * timeIncrement;
            
        }
        x = x_calculated;
        y = y_calculated;
        theta = theta_calculated;
        //PrintPose();
}
