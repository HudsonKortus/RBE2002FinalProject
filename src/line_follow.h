#ifndef LINE_FOLLOW
#define LINE_FOLLOW


#include <Romi32U4.h>
#include <Speed_controller.h>

class LineFollow: public SpeedController
{
    private:
        int LEFT_LINE_SENSOR_PIN = 21;
        int RIGHT_LINE_SENSOR_PIN = 22;
        const float kp_line = 0.015;             // line following kp
        const float REFLECTANCE_THRESHOLD = 200;
        const float VTC_TO_CENTER = 100;//mm

    public:

    void lineFollow(float);
    bool reachedIntersection(void);
    bool turnToNextline(float);
    void centerVTC(void);
};


#endif