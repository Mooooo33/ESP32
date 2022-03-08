#include <Wire.h>
#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Motor.h"

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire, -1);//实例化OLED
//OLED: SCL -- D22, SDA -- D21
#define DHTTYPE DHT11 // DHT 11
uint8_t DHTPin = 23;
DHT dht(DHTPin, DHTTYPE);

float Temperature;
float Humidity;
float Set_TempL = 20, Set_TempH = 28;
float Set_HumL = 60, Set_HumH = 70;
uint8_t mode;//选择模式
uint8_t button = 0;
uint8_t key = 0;

#define K1_MODE 1
#define K2_UP 2
#define K3_DOWN 3


void Pin_Mode()//按键触发
{
    key = K1_MODE;
    Serial.printf("Pin_Mode Event.\r\n");
}
void Pin_Up()//按键触发
{
    key = K2_UP;
    Serial.printf("Pin_Up Event.\r\n");
}
void Pin_Down()//按键触发
{
    key = K3_DOWN;
    Serial.printf("Pin_Down Event.\r\n");
}
void Display()
{
    //串口发送数据
    Serial.print(F("Humidity: "));
    Serial.print(Humidity);
    Serial.print(F("%  Temperature: "));
    Serial.print(Temperature);
    Serial.print(F(" C \n\r"));

    //display temperature
    display.setTextSize(1);
    display.setCursor(0,16);
    display.print("Temperature: ");
    display.setTextSize(1);
    display.setCursor(80,16);
    display.print(Temperature);
    display.print(" ");
    display.setTextSize(1);
    display.cp437(true);
    display.write(167);//显示符号
    display.setTextSize(1);
    display.print("C");

    // display humidity
    display.setTextSize(1);
    display.setCursor(0, 26);
    display.print("Humidity: ");
    display.setTextSize(1);
    display.setCursor(80, 26);
    display.print(Humidity);
    display.print(" %");

    //display temp setting
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.print("TEM_L: ");
    display.setTextSize(1);
    display.setCursor(75, 40);
    display.print("H: ");
    display.setTextSize(1);
    display.setCursor(36,40);
    display.print(Set_TempL);
    display.setTextSize(1);
    display.setCursor(90,40);
    display.print(Set_TempH);

    //display hum setting
    display.setTextSize(1);
    display.setCursor(0, 50);
    display.print("Hum_L: ");
    display.setTextSize(1);
    display.setCursor(75, 50);
    display.print("H: ");
    display.setTextSize(1);
    display.setCursor(36,50);
    display.print(Set_HumL);
    display.setTextSize(1);
    display.setCursor(90,50);
    display.print(Set_HumH);

    display.display();

}

void setup() {

    Serial.begin(115200);

    Motor_Init();

    pinMode(DHTPin, INPUT);//定义DHT11输入
    dht.begin();

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
    display.clearDisplay();
    display.display();
    display.setTextSize(1.75);
    display.setTextColor(SSD1306_WHITE);
    // init done

    pinMode(12, INPUT_PULLUP);
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(12), Pin_Mode, FALLING);
    attachInterrupt(digitalPinToInterrupt(13), Pin_Up, FALLING);
    attachInterrupt(digitalPinToInterrupt(14), Pin_Down, FALLING);
    //利用中断触发按键

}
void loop() {
    
    Humidity = dht.readHumidity();//取值
    Temperature = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(Humidity) || isnan(Temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
    }

    if (Humidity > Set_HumH || Humidity < Set_HumL || Temperature > Set_TempH || Temperature < Set_TempL){//控制电机运转
        Motor_Control(50, 0);//通过调节占空比调整风扇速度，目前是灯亮度
        Serial.print("Motor running!\n");

        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0,0);
        display.print("Motor running!");
    }
    else{
        Motor_Control(0, 0);
        Serial.print("Motor pause.\n");

        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0,0);
        display.print("Motor pause.");
    }
    Serial.print(mode);


    if (key == K1_MODE){
        mode ++;
        if (mode > 4){ 
            mode = 0;
        }
        key = 0;
    }
	if(mode==1)	 //温度上限设置
	{
		switch(key)	
		{
			case K2_UP: 	   //加
						Set_TempH++;
						if(Set_TempH>=80)Set_TempH=80;
						break;
			case K3_DOWN: 	 //减
						Set_TempH--;
						if(Set_TempH<=0)Set_TempH=0;
						break;
		}

	}

	else if(mode==2)  //温度下限设置
	{
		switch(key)	
		{
			case K2_UP: 	   //加
						Set_TempL++;
						if(Set_TempL>=80)Set_TempL=80;
						break;
			case K3_DOWN: 	 //减
						Set_TempL--;
						if(Set_TempL<=0)Set_TempL=0;
						break;
		}

    }
	if(mode==3)	 //湿度上限设置
	{
		switch(key)	
		{
			case K2_UP: 	   //加
						Set_HumH++;
						if(Set_HumH>=80)Set_HumH=80;
						break;
			case K3_DOWN: 	 //减
						Set_HumH--;
						if(Set_HumH<=0)Set_HumH=0;
						break;
		}

	}

	else if(mode==4)  //湿度下限设置
	{
		switch(key)	
		{
			case K2_UP: 	   //加
						Set_HumL++;
						if(Set_HumL>=80)Set_HumL=80;
						break;
			case K3_DOWN: 	 //减
						Set_HumL--;
						if(Set_HumL<=0)Set_HumL=0;
						break;
		}

    }

    Display();
    delay(500);
    
}