#ifndef __Key_H__
#define __Key_H__

#include <Arduino.h>
#include <stdint.h>
#include <stddef.h>

typedef enum
{
    NO_PRESS=0,
    SHORT_PRESS,
    LONG_PRESS,
    IN_PRESS,	
}press_state;

class  button_state
{
public:
	press_state press;
    uint8_t value;
	uint8_t last_value;	
	uint16_t pin;
	uint32_t press_time;
    uint32_t release_time; 
    uint32_t in_time; 
	uint32_t in_press_cnt;
	uint32_t state_lock_time;

    button_state(uint16_t pin,uint8_t value=1,uint8_t last_value=1);
    void Read_button_state();

    uint32_t Read_time_cnt(const uint32_t cnt)
    {
        return cnt;
    }
};

#define LONG_PRESS_LIMIT  1000       //1000ms
#define IN_PRESS_LIMIT     250        //250ms






#endif
