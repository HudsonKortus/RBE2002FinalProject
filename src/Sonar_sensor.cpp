#include <Romi32U4.h>
#include "Sonar_sensor.h"

void SonarSensor::Init(void)
{
    pinMode(pin_TRIG,OUTPUT);
    pinMode(pin_ECHO, INPUT);   
}

float SonarSensor::PrintData(void)
{
    Serial.println(ReadData());
}

float SonarSensor::ReadData(void)
{
    //assignment 1.2
    digitalWrite(pin_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin_TRIG, LOW);
    float duration = pulseIn(pin_ECHO, HIGH);
    //distance = duration * 34,000 cm/s * 1/2 smallest distance based on 64 us /58.0
    return duration;
}