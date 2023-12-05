#include <Arduino.h>
#include "Odometry.h"
#include "Speed_controller.h"
#include "Encoders.h"

Encoder magneticEncoder;
SpeedController robot;
Odometry odometry;
Romi32U4ButtonB buttonB;

//this is a test can you see what I am typing

/*
The Industrial Revolution and its consequences have been a disaster for the human race. They have greatly increased the life-expectancy of those of us who live in “advanced” countries, but they have destabilized society, have made life unfulfilling, have subjected human beings to indignities, have led to widespread psychological suffering (in the Third World to physical suffering as well) and have inflicted severe damage on the natural world. The continued development of technology will worsen the situation. It will certainly subject human being to greater indignities and inflict greater damage on the natural world, it will probably lead to greater social disruption and psychological suffering, and it may lead to increased physical suffering even in “advanced” countries*/
void setup() {
    Serial.begin(9600);
    robot.Init();
    Serial.println("Hello World");

}

float yPrevious = 0;
float targetYposition;
enum ROBOT_STATE {IDLE, DRIVE};
    ROBOT_STATE robot_state = IDLE; //initial state: IDLE

void loop() {
    if(magneticEncoder.UpdateEncoderCounts()){
        odometry.UpdateOdometry();

        switch (robot_state) {
            case IDLE:
                if(buttonB.getSingleDebouncedRelease()){ 
                    robot_state = DRIVE; 
                    robot.Stop();             
                } 
                else { 
                    robot_state = IDLE;
                    robot.Stop(); 
                }   
                break;
            
            case DRIVE:
                
                //assignment
                Serial.println("drive");
                //robot.PositionController(500, 0, odometry.getX(), odometry.getY());
                robot.Straight(50,5);
                // float ySpeed = odometry.calculateFigureight(odometry.getX(), 20);
                // robot.Run(50, ySpeed);
                // Serial.println(ySpeed);


/*
                // //robot.Straight(25,10); //velocity, duration
                // // Serial.println("!!!!!!!!!!!!");
                // //robot.Turn(180,0); //degrees, direction
                // // Serial.println("!!!!!!!!!!!!");
                // //robot.Straight(25,10);
                // // robot.Curved(95.75,155.58,30); //velocity left, velocity right, duration
                // // robot.Curved(155.58,95.75,30);
                // robot.Curved(47.87 * a,77.79 * a, 30 / a); //velocity left, velocity right, duration
                // robot.Curved(77.79 * a,47.87 * a, 30 / a);
                // // Serial.println("!!!!!!!!!!!!");
                // //robot.Curved();
*/   
                if(buttonB.getSingleDebouncedPress()){
                    robot_state = IDLE;
                    robot.Stop(); 
                } 
                else {
                    robot_state = DRIVE;
                }
                break;
            }

            
            }

}