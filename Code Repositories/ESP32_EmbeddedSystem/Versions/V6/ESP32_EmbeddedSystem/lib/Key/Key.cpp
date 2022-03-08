#include "Key.h"

button_state::button_state(uint16_t pin,uint8_t value,uint8_t last_value)
{
	button_state::pin = pin;
	button_state::value = value;
	button_state::last_value = last_value;
	pinMode(button_state::pin, INPUT_PULLUP);
}

void button_state::Read_button_state()
{
	button_state::value = digitalRead(button_state::pin);
	if(button_state::value == 0)
	{
		if(button_state::last_value!=0)//上一次的值不等于0就是首次按下
		{
			button_state::press_time=millis();  //记录按下时的时间点
			button_state::in_time=millis();     //记录按下时的时间点
			button_state::in_press_cnt=0;
		}
		else
		{
			if(millis()-button_state::in_time>IN_PRESS_LIMIT)   //持续按下，按键还未松开
			{
				button_state::in_time=millis();
				button_state::press=IN_PRESS;
				if(button_state::press==IN_PRESS)  button_state::in_press_cnt++;
			}
		}
	}
	else
	{
		if(button_state::last_value==0)//按下后释放
		{
			button_state::release_time=millis();//记录释放时的时间
			
			if(button_state::release_time-button_state::press_time>LONG_PRESS_LIMIT)   //大于长按的时间时就是触发长按
			{
			   button_state::press=LONG_PRESS;      //大于长按的时间时就是触发长按
			   button_state::state_lock_time=0;    
			}
			else
			{
			   button_state::press=SHORT_PRESS;    //小于长按的时间时就是触发短按
			   button_state::state_lock_time=0;
			}
		}
	}
	button_state::last_value=button_state::value;
	
	if(button_state::press==LONG_PRESS
	 ||button_state::press==SHORT_PRESS)   //按下释放后，程序后台1.5S内无响应，复位按键状态
	{
	  button_state::state_lock_time++;
		if(button_state::state_lock_time>=150)   //150*10ms=1.5s
		{			
			button_state::press=NO_PRESS;       //复位按键状态，正常响应按键时，都会复位按键状态
			button_state::state_lock_time=0;
		}
	}

}






