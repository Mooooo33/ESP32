//SCL -- D22, SDA -- D21
#include "Adafruit_SHT4x.h"
#include "Arduino.h"
#include "SSD1306Wire.h" 
#include "Ticker.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"

#define GPIO_PWM0A_OUT 19   //Set GPIO 19 as PWM0A
#define GPIO_PWM0B_OUT 18   //Set GPIO 18 as PWM0B

SSD1306Wire display(0x3c, SDA, SCL);       //实例化OLED显示对象
Adafruit_SHT4x sht4 = Adafruit_SHT4x();    //实例化SHT4对象
Ticker tim1;

void TIM1_CallBack();

void Pwm_Init()
{
	mcpwm_pin_config_t pin_config = {
		.mcpwm0a_out_num = GPIO_PWM0A_OUT,
		.mcpwm0b_out_num = GPIO_PWM0B_OUT
	};
	mcpwm_set_pin(MCPWM_UNIT_0, &pin_config);
	mcpwm_config_t pwm_config;
	pwm_config.frequency = 1000;    //frequency = 1000Hz
	pwm_config.cmpr_a = 60.0;       //duty cycle of PWMxA = 60.0%
	pwm_config.cmpr_b = 50.0;       //duty cycle of PWMxb = 50.0%
	pwm_config.counter_mode = MCPWM_UP_COUNTER;
	pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
	mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);   //Configure PWM0A & PWM0B with above settings
}

void setup() {
  Serial.begin(115200);                      //初始化串口1
  while (! sht4.begin())                     //初始化SHT4
  delay(1);
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  sht4.setHeater(SHT4X_NO_HEATER);            //设置SHT4
  display.init();                           //OLED初始化
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);            //设置显示字符的大小
  Pwm_Init();
  tim1.attach_ms(5, TIM1_CallBack);     //定时器1中断，5ms一个周期
}


void loop() 
{
  char dis_str[50];
  sensors_event_t humidity, temp;
  display.clear();                             //清屏，不清屏会保留上一次显示的内容
  sht4.getEvent(&humidity, &temp);             //读取SHT4的数据
  
  sprintf(dis_str,"temperature:%.2f C",temp.temperature);    
  display.drawString(0, 0, dis_str);
  sprintf(dis_str,"humidity:%.2f %%",humidity.relative_humidity);
  display.drawString(0, 16, dis_str);                   //OLED显示
  display.display();

  delay(100);
}

unsigned char cnt;
void TIM1_CallBack()                 //定时器1回调函数
{
  cnt++;
  if(cnt == 200)
  {
    cnt = 0;

  }
  
}