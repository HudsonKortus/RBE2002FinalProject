#include <Romi32U4.h>
#include "Encoders.h"
#include "Speed_controller.h"

Romi32U4Motors motors;
Encoder MagneticEncoder; 


void SpeedController::Init(void)
{
    MagneticEncoder.Init();
}

void SpeedController::Run(float target_velocity_left, float target_velocity_right)
{
    if(MagneticEncoder.UpdateEncoderCounts()){
        float e_left = target_velocity_left - MagneticEncoder.ReadVelocityLeft();
        float e_right = target_velocity_right - MagneticEncoder.ReadVelocityRight();

        E_left += e_left;
        E_right += e_right;

        float u_left = Kp*e_left + Ki*E_left;
        float u_right = Kp*e_right + Ki*E_right;

        motors.setEfforts(u_left,u_right);
        
    }
}

void SpeedController::PositionController(float targetXPosition, float targetYPosition, 
    float x_current, float y_current){
    float xPositionError = targetXPosition - x_current;
    float yPositionError = targetYPosition - x_current;
    float xEffort = xPositionError * P_Kp + (xPositionError - xPositionError_previous) * P_Ki;
    float yEffort = yPositionError * P_Kp + (yPositionError - yPositionError_previous) * P_Ki;
    motors.setEfforts(xEffort,yEffort);
    Serial.print(xEffort);
    Serial.print(" ");
    Serial.println(yEffort);
    xPositionError_previous = xPositionError;
    yPositionError_previous = yPositionError;

}