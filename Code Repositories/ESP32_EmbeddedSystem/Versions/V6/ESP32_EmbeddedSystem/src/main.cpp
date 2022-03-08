#include <Wire.h>
#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Motor.h"
#include "Key.h"
#include "Ticker.h"
#include "PID.h"

#define BLINKER_PRINT Serial
#define BLINKER_WIFI

#include <Blinker.h>

char auth[] = "167fa7cd1e27";
char ssid[] = "16";
char pswd[] = "13189580025";

typedef unsigned char u8;
typedef unsigned int u16;
char name[]="temp";
BlinkerNumber TEMP(name);

Ticker tim1;
button_state  Key1(12);    
button_state  Key2(13);
button_state  Key3(14);   //按键实例化

_PID Temp_Ctrl(60,0,10);    //PID实例化
void TIM1_CallBack();

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire, -1);
unsigned long delayTime;
//OLED: SCL -- D22, SDA -- D21
#define DHTTYPE DHT11 // DHT 11
uint8_t DHTPin = 23;
DHT dht(DHTPin, DHTTYPE);
int interruptCounter = 0;
uint8_t button = 0;
float Temperature;
float Humidity;
float Temp_Fahrenheit;
float Set_TempL = 20, Set_TempH = 28;
float Set_HumL = 60, Set_HumH = 70;

void Key_Scan()
{
    if(Key1.press== SHORT_PRESS)        //按键1短按
    {
        Set_TempH++;
        Key1.press = NO_PRESS;
    }else if(Key1.press== LONG_PRESS)  //按键1长按
    {

        Key1.press = NO_PRESS;
    }
 
    if(Key2.press== SHORT_PRESS)    //按键2短按
    {

        Set_TempH--;
        Key2.press = NO_PRESS;
    }else if(Key2.press== LONG_PRESS)  //按键2长按
    {

        Key2.press = NO_PRESS;
    }

    if(Key3.press== SHORT_PRESS)       //按键3短按
    {

        Key3.press = NO_PRESS;
    }else if(Key3.press== LONG_PRESS)   //按键3长按
    {

        Key3.press = NO_PRESS;
    }
}

unsigned char cnt;
void TIM1_CallBack()
{
    Key1.Read_button_state();   //读取按键状态
    Key2.Read_button_state();
    Key3.Read_button_state();
    Temp_Ctrl.Set_target(Set_TempH);
    Temp_Ctrl.Set_feedback(Temperature);
    Temp_Ctrl.PID_Count();      //PID计算
    if(Temp_Ctrl.get_output() > 0)
        Motor_Control(Temp_Ctrl.get_output(),0);
    else
    {
        Motor_Control(0,0);
    }
    
    Key_Scan();                 //按键响应
    cnt++;
    if(cnt == 230)
    {
        cnt = 0;
        TEMP.print(Temperature);
       
    }
}

// 新建组件对象


int counter = 0;

// 按下按键即会执行该函数
void button1_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
}

// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
    counter++;
  
}

void setup() {
    Serial.begin(115200);

     #if defined(BLINKER_PRINT)
        BLINKER_DEBUG.stream(BLINKER_PRINT);
    #endif
    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    pinMode(DHTPin, INPUT);//定义DHT11输入
    delay(1000);
    dht.begin();
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
    Motor_Init();
    
    // init done
    display.display();
    display.clearDisplay();
    display.display();
    display.setTextSize(1.75);
    display.setTextColor(WHITE);
    tim1.attach_ms(5, TIM1_CallBack);  
}
void loop() 
{
    Blinker.run();
    Humidity = dht.readHumidity();
    // Read temperature as Celsius (the default)
    Temperature = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    Temp_Fahrenheit= dht.readTemperature(true);
    display.setCursor(0,0);
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("Temperature: ");
    display.setTextSize(2);
    display.setCursor(0,10);
    display.print(Temperature);
    display.print(" ");
    display.setTextSize(1);
    display.cp437(true);
    display.write(167);
    display.setTextSize(2);
    display.print("C");
    
    // display humidity
    display.setTextSize(1);
    display.setCursor(0, 35);
    display.print("Humidity: ");
    display.setTextSize(2);
    display.setCursor(0, 45);
    display.print(Humidity);
    display.print(" %");
    display.setTextSize(1);
    display.setCursor(90, 45);
    display.print(Set_TempH);
    display.display();
    
    Blinker.delay(500);
}