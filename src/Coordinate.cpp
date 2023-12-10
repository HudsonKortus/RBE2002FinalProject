#include "Coordinate.h"
class Coordinate{
    private:
        float x, y; //measured in mm
    public:
        void Coordinate::Init(float x, float y)
        {
            this->x = x;
            this->y = y;
        }
        float Coordinate::getX(){
            return x;
        }
        float Coordinate::getY(){
            return y;
        }

};
