#include "PID.h"

void _PID::PID_Count()
{
    _PID::Err = _PID::feedback - _PID::target;
    _PID::Int_Err+=_PID::Err;
    _PID::out = _PID::Kp*_PID::Err+_PID::Ki*_PID::Int_Err+_PID::Kd*(_PID::Err - _PID::Last_Err)*_PID::dt;
    _PID::Last_Err = _PID::Err;
}











