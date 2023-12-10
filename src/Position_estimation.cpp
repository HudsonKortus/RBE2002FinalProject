#include  "Position_estimation.h"
#include "Encoders.h"

/**
    Arrays of x and y, when there are coordinates smooshed between others we will set those in between ones to the outside ones
    {1,2},{1,3},{1,2} means that {1,3} is a dead end, so we will change this to {1,2},{1,2},{1,2}
*/
Encoder RomiEncoders;

/*
    Use arrays for x and y, then repeat the last real coordinate when we delete dead ends. Double the size of the array when it 
    gets too big
*/
unsigned long time_prev = 0;
unsigned long time_now = millis();
float timeIncrement = 0.050;
int coordinateListSize = 27, currentCoordinate = 0;
bool second = false;

//Based on how things are run right now, we can go up to 54 coordinates
struct coordinates{
    float xCoords[coordinateListSize];
    float yCoords[coordinateListSize];
}

void Position::Init(void)
{
    time_now = millis();
    x = 0;
    y = 0;
    theta = 0;
    coordinates firstSet;
}
void Position::makeWaypoint(void){
    if(currentCoordinate < 27){
        firstSet.xCoords[currentCoordinate] = x;
        firstSet.yCoords[currentCoordinate] = y;
        currentCoordinate++;
    }else if(coordinateListSize == 27){
        coordinateListSize *= 2;
        coordinates secondSet;
        for(int i = 0; i <= 26; i++){
            secondSet.xCoords[i] = firstSet.xCoords[i];
            secondSet.yCoords[i] = firstSet.yCoords[i];
        }
        second = true;
    }else{
        secondSet.xCoords[currentCoordinate] = x;
        secondSet.yCoords[currentCoordinate] = y;
        currentCoordinate++;
    }
}
void Position::cleanMapFirst(void){
    //Find where coordinates are sandwiched between identical coordinates
        //find identical coordinates that are not next to eachother
        //replace the coordinates in between them with that coordinate (this is easier than deleting and redoing the map)
    for(int i = 0; i < --currentCoordinate; i++){
        for(int j = ++i; j < currentCoordinate; j++){
            //Check for bread of sandwich
            if(firstSet.xCoords[i] == firstSet.xCoords[j] && firstSet.yCoords[i] == firstSet.yCoords[j]){
                for(int a = i; a < j; a++){
                    firstSet.xCoords[a] = firstSet.xCoords[i];
                    firstSet.yCoords[a] = firstSet.yCoords[i];
                }
            }
        }
    }
}
void Position::cleanMapSecond(void){
    //Find where coordinates are sandwiched between identical coordinates
        //find identical coordinates that are not next to eachother
        //replace the coordinates in between them with that coordinate (this is easier than deleting and redoing the map)
    for(int i = 0; i < --currentCoordinate; i++){
        for(int j = ++i; j < currentCoordinate; j++){
            //Check for bread of sandwich
            if(secondSet.xCoords[i] == secondSet.xCoords[j] && secondSet.yCoords[i] == secondSet.yCoords[j]){
                for(int a = i; a < j; a++){
                    secondSet.xCoords[a] = secondSet.xCoords[i];
                    secondSet.yCoords[a] = secondSet.yCoords[i];
                }
            }
        }
    }
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
    //Serial.println(millis() - time_prev);
        time_prev = millis();
        //assignment
   
        // if(theoretical_speed_right != theoretical_speed_left 
        //     && abs(measured_speed_right - measured_speed_left) > .1 ) {
            //curved movement
         if(abs(measured_speed_right - measured_speed_left) > .25 ) {
            Serial.println("Curved");

            // //target calculations
            // float R_theoretical = (l/2)*((theoretical_speed_right + theoretical_speed_left) 
            //     / (theoretical_speed_right - theoretical_speed_left));
            // float w_theoretical = (theoretical_speed_right - theoretical_speed_left) / l;
            

            // x_theoretical = x_theoretical + ((-R_theoretical * sin(theta_theoretical)) + 
            //     (R_theoretical * sin((theta_theoretical + (w_theoretical * timeIncrement)))));
            
            // y_theoretical += (R_theoretical * cos(theta_theoretical)) - 
            //     (R_theoretical * cos(theta_theoretical + (w_theoretical * timeIncrement)));

            // theta_theoretical += (w_theoretical * timeIncrement);

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

            // //target calculations
            // float V_theoretical = (theoretical_speed_left + theoretical_speed_right)/2;
            // x_theoretical += V_theoretical * cos(theta_theoretical) * timeIncrement; //50ms is the set time update
            // y_theoretical += V_theoretical * sin(theta_theoretical) * timeIncrement;
            // //theta stays the same
            
            //measured calculations
            float V_calculated = (measured_speed_left + measured_speed_right)/2;
            x_calculated += V_calculated * cos(theta_calculated) * timeIncrement; //50ms is the set time update
            y_calculated += V_calculated * sin(theta_calculated) * timeIncrement;
            
        }
        x = x_calculated;
        y = y_calculated;
        theta = theta_calculated;
        PrintPose();
}
