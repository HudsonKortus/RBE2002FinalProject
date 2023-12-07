#include <Romi32U4.h>
#include "Behaviors.h"
#include "Speed_controller.h"
#include "Position_estimation.h"

//sensors
Romi32U4ButtonA buttonA;

//motor-speed controller
SpeedController robot;

void Behaviors::Init(void)
{
    robot.Init();
}

void Behaviors::Stop(void)
{
    robot.Stop();
}

void Behaviors::Run(void)
{
    switch (robot_state)
    {
    case IDLE:
        if(buttonA.getSingleDebouncedRelease()){ 
            robot_state = DRIVE; 
            robot.Stop();             
        } 
        else { 
            robot_state = IDLE;
            robot.Stop(); 
        }   
        break;
    
    case DRIVE:
        robot_state = DRIVE;
        //assignment
        Serial.println("running");
        delay(1000);
        for(int i = 0; i < 4; i++){
            robot.Straight(70,(1000/70));//move forward 50 cm
            robot.Turn(90,1);
        }
        robot_state = IDLE;
        
        break;
    }
}