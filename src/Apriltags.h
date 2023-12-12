#ifndef APRIL_TAGS
#define APRIL_TAGS

#include <Arduino.h>
#include <Wire.h>
#include <openmv.h>

class AprilTags{
    private:
        OpenMV camera;
    public:
        void Init(void);
        //returns the april tag id that it has found.
        int FindAPs(void);
};

#endif