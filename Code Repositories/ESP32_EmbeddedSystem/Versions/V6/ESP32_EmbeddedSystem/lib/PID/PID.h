#ifndef __PID_H
#define __PID_H

#include <Arduino.h>
#include <stdint.h>
#include <stddef.h>

class  _PID
{
public:
   float Kp,Ki,Kd,Err,Last_Err,Int_Err,dt;
   
    _PID(float P,float I,float D,float _dt=0.005f,int Lim = 100)    //参数初始化
    {
        Kp = P;
        Ki = I;
        Kd = D;
        dt = _dt;
        SetOut_Limit(Lim);
    }

    void PID_Count();

    void SetOut_Limit(int input)
    {
        outLimit = input;
    }

    void Set_target(int input)
    {
        target = input;
    }

    void Set_feedback(int input)
    {
        feedback = input;
    }

    float get_output()
    {
        return out;
    }

private:
     int outLimit;
     float out;
     float feedback;
     float target;
};

#endif
