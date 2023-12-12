#ifndef IR_SENSOR
#define IR_SENSOR

#include <Romi32U4.h>

class IRsensor{
    private:
    //front ir = 11
        int pin_IR = A0;
        //int pin_IR_alt = A11;
    public:
        void Init(int);
        float ReadData(void);
        float PrintData(void);
};

#endif