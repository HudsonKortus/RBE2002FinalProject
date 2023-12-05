#include <Arduino.h>
#include "Odometry.h"
#include "Speed_controller.h"
#include "Encoders.h"

Encoder magneticEncoder;
SpeedController robot;
Odometry odometry;

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
        robot.PositionController(100, 70, odometry.getX(), odometry.getY());
        // float ySpeed = odometry.calculateFigureight(odometry.getX(), 20);
        // robot.Run(50, ySpeed);
        // Serial.println(ySpeed);
        delay(100);

    
    
    }

}