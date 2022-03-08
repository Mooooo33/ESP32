#include <Wire.h>
#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Motor.h"

typedef unsigned char u8;
typedef unsigned int u16;

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire, -1);
unsigned long delayTime;
//OLED: SCL -- D22, SDA -- D21
// Uncomment one of the lines below for whatever DHT sensor type you're using!
#define DHTTYPE DHT11 // DHT 11
//#define DHTTYPE DHT21 // DHT 21 (AM2301)
//#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
//DHT Sensor;
uint8_t DHTPin = 23;
DHT dht(DHTPin, DHTTYPE);
// hw_timer_t *timer = NULL; 
int interruptCounter = 0;
uint8_t button = 0;
float Temperature;
float Humidity;
float Temp_Fahrenheit;
float Set_TempL = 20, Set_TempH = 28;
float Set_HumL = 60, Set_HumH = 70;

// void IRAM_ATTR TimerEvent()
// {
//     Serial.println(interruptCounter++);
//     if (interruptCounter > 5)
//     {
//         interruptCounter = 1;
//     }
// }

void Pin_Mode()//打印事件
{
    Serial.printf("PinInt Event.\r\n");
}

// u8 KEY_Scan()
// {
//     // static uint8_t key = 1;

//     // if (key&&(digitalRead(13) == 0)){
//     //     delay(100);
//     //     key = 0;
//     //     if (digitalRead(13) == 0){
//     //         Serial.printf("mode被按下\n\r");
//     //         return 1;
//     //     }
//     // }
//     if ((digitalRead(13) == 0)){
//         delay(10);
//         if ((digitalRead(13) == 0)){
//             Serial.println(F("beian\r\n"));
//             return 1;
//         }
//     }

// }


void setup() {
    Serial.begin(115200);

    pinMode(DHTPin, INPUT);//定义DHT11输入
    dht.begin();
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
    Motor_Init();
    // init done
    display.display();
    delay(100);
    display.clearDisplay();
    display.display();
    display.setTextSize(1.75);
    display.setTextColor(WHITE);

    pinMode(12, INPUT_PULLUP);
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(12), Pin_Mode, FALLING);
    attachInterrupt(digitalPinToInterrupt(13), Pin_Mode, FALLING);
    attachInterrupt(digitalPinToInterrupt(14), Pin_Mode, FALLING);
//利用中断触发,没有消抖



        //	函数名称：timerBegin()
    //	函数功能：Timer初始化，分别有三个参数
    //	函数输入：1. 定时器编号（0到3，对应全部4个硬件定时器）
    //			 2. 预分频器数值（ESP32计数器基频为80M，80分频单位是微秒）
    //			 3. 计数器向上（true）或向下（false）计数的标志
    //	函数返回：一个指向 hw_timer_t 结构类型的指针
    // timer = timerBegin(0, 80, true);

    //	函数名称：timerAttachInterrupt()
    //	函数功能：绑定定时器的中断处理函数，分别有三个参数
    //	函数输入：1. 指向已初始化定时器的指针（本例子：timer）
    //			 2. 中断服务函数的函数指针
    //			 3. 表示中断触发类型是边沿（true）还是电平（false）的标志
    //	函数返回：无
 //   timerAttachInterrupt(timer, &TimerEvent, true);

    //	函数名称：timerAlarmWrite()
    //	函数功能：指定触发定时器中断的计数器值，分别有三个参数
    //	函数输入：1. 指向已初始化定时器的指针（本例子：timer）
    //			 2. 第二个参数是触发中断的计数器值（1000000 us -> 1s）
    //			 3. 定时器在产生中断时是否重新加载的标志
    //	函数返回：无
    // timerAlarmWrite(timer, 1000000, true);
    // timerAlarmEnable(timer); //	使能定时器

}
void loop() {
    
//    PWM_SetDuty(200 * interruptCounter, 200 * interruptCounter);
    // button = KEY_Scan();
    // Serial.print(button);
    
    Humidity = dht.readHumidity();
    // Read temperature as Celsius (the default)
    Temperature = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    Temp_Fahrenheit= dht.readTemperature(true);

    if (Humidity > Set_HumH || Humidity < Set_HumL || Temperature > Set_TempH || Temperature < Set_TempL){
        Motor_Control(50, 0);//通过调节占空比调整风扇速度，目前是灯亮度
        Serial.print("Motor running!\n");
    }
    else{
        Motor_Control(0, 0);
        Serial.print("Motor pause.\n");
    }

    // Check if any reads failed and exit early (to try again).
    if (isnan(Humidity) || isnan(Temperature) || isnan(Temp_Fahrenheit)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
    }

    Serial.print(F("Humidity: "));
    Serial.print(Humidity);
    Serial.print(F("%  Temperature: "));
    Serial.print(Temperature);
    Serial.print(F("°C "));
    Serial.print(Temp_Fahrenheit);
    Serial.println(F("°F "));

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

    display.display();
    delay(500);

}