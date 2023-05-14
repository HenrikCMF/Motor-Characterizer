#ifndef SPEEDDATA_H
#define SPEEDDATA_H
#include "data.hpp"
#define DATAID_SPEEDDATA 0
class speeddata : data
{
    private:
    float sample;
    public:
    
    inline speeddata(float speed) : data(DATAID_SPEEDDATA)
    {
        sample=speed;
    };
    inline speeddata(void): data()
    {

    }
    inline float get_speed()
    {
        return sample;
    }
};

#endif