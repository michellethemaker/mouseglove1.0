#include "logitech-mouse.h"

logiMouse myMouse;

float mouseSpeed = 10.0f;
float degreesToRadians = 2.0f*3.14f/360.0f;

//--------------MPU6050-----------------//
// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

#define LED_PIN 13
bool blinkState = false;
//---------------MPU6050----------------//

//---------------FastLED----------------//
#include <FastLED.h>
#define LED_PIN1     4
#define NUM_LEDS1    9
#define LED_PIN2     5
#define NUM_LEDS2    12
#define LED_PIN3     6
#define NUM_LEDS3    14
#define NUM_STRIPS 3
#define BRIGHTNESS  60
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds1[9];
CRGB leds2[12];
CRGB leds3[14];

CRGBPalette16 currentPalette;
TBlendType    currentBlending;


bool offFlag = false;
//---------------FastLED----------------//


//---------------FLEXSENSORS----------------//
int resistpin1 = A0;
int resistpin2 = A1;
int resistpin3 = A2;
int ohmvalue1,ohmvalue2,ohmvalue3;
//---------------FLEXSENSORS----------------//

void setup() {

//---------------FastLED----------------//
delay( 3000 ); // power-up safety delay
Serial.begin(115200);
    FastLED.addLeds<LED_TYPE, LED_PIN1, COLOR_ORDER>(leds1, NUM_LEDS1);
    FastLED.addLeds<LED_TYPE, LED_PIN2, COLOR_ORDER>(leds2, NUM_LEDS2);
    FastLED.addLeds<LED_TYPE, LED_PIN3, COLOR_ORDER>(leds3, NUM_LEDS3);
    FastLED.setBrightness(  BRIGHTNESS );
    
    pinMode(A0, INPUT);//resistpin
    pinMode(A1, INPUT);//resistpin
    pinMode(A2, INPUT);//resistpin
    Serial.println("LEDs Initialised");
//---------------FastLED----------------//
  

  Wire.begin();
  Serial.println("Starting");
  myMouse.begin();
  Serial.println("Trying to reconnect");

  if(myMouse.reconnect())
      Serial.println("Reconnected using previous configuration !");
  else
  {
      Serial.println("Unable to reconnect to dongle... Pairing with any dongle...");
      myMouse.pair();
  }
//---------MPU6050----------//
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();
    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
//---------MPU6050----------//

  Serial.println("Ready :)");
}

void loop() {

    int x, y = 0;
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    Serial.print("a/g: Motion\t");
    Serial.print(-(ax/1000)); Serial.print("\t");
    Serial.print(-(ay/ 800)); Serial.println("\t");
    myMouse.move( -((ax/1000)), -((ay/800)));
    delay(5);

  //---------------FastLED----------------//
  FastLED.clear();
  fill_solid( currentPalette, 16, CRGB::Black);
  FastLED.show(); //for separate strips use "controllers[0].show();"
  ohmvalue1 = analogRead(resistpin1);
  Serial.print(ohmvalue1);
  Serial.println(" ohm1"); 
  ohmvalue2 = analogRead(resistpin2);
  Serial.print(ohmvalue2);
  Serial.println(" ohm2");
  ohmvalue3 = analogRead(resistpin3);
  Serial.print(ohmvalue3);
  Serial.println(" ohm3");
  if (offFlag ==true)
          Serial.println("falsee toggled");   
        else
        Serial.println("true toggled");  
  CountToTen(); 
//---------------FastLED----------------//
}
  
void CountToTen()
{
  int flag1 = 10;  
  int flag2 = 10; 
  int flag3 = 10; 
  while (analogRead(resistpin2)>50) //while FLEXIN is happenin. otherwise return to loop()
  {
    while (analogRead(resistpin1)>100)//OFF MOUSE
    {
      Serial.print("flag1: ");
      Serial.println(flag1);
      flag1 -=1;
      if(flag1 ==0)
      {
        FillLEDsFromPaletteColors( 3,2);
          if (offFlag ==true)
          {
            offFlag = false;
            Serial.println("falsee toggled");
          }
          
          else
          {
          offFlag = true;
          Serial.println("true toggled");  
          }
        break;
      }
    }
    Serial.print("flag2: ");
    Serial.println(flag2);
    flag2 -=1;
    if(flag2 ==0 & offFlag == false)
    {
      Serial.println("LEFT CLICKK");//LEFT CLICKK
      Click(2);
      //=============//

      FillLEDsFromPaletteColors( 2,1);

     // delay(500); //1 sec to recover
      //============//
      break;
    }
  }
  flag1 = 10;  
  flag3 = 10; 
  
  while (analogRead(resistpin3)>50) //while FLEXIN3 is happenin. otherwise return to loop()
  {
    while (analogRead(resistpin1)>100)//OFF MOUSE
    {
      Serial.print("flag1: ");
      Serial.println(flag1);
      flag1 -=1;
      if(flag1 ==0)
      {
        if (offFlag ==true)
        {
          offFlag = false;
          Serial.println("falsee toggled");
        }
        
        else
        {
        offFlag = true;
        Serial.println("true toggled");  
        }
        
        FillLEDsFromPaletteColors( 3,2);
        break;//add code later for synthwave standby lights
      }
    }
    Serial.print("flag3: ");
    Serial.println(flag3);
    flag3 -=1;
    if(flag3 ==0& offFlag == false)
    {
      Serial.println("RIGHT CLICK"); //RIGHT CLICK
      Click(3);
      //=============//

      FillLEDsFromPaletteColors( 3,1);
 
     // delay(500); //1 sec to recover
      //============//
      break;
    }
  }

  flag1 = 10;  
  flag2 = 10; 
  
  while (analogRead(resistpin1)>100) //while FLEXIN3 is happenin. otherwise return to loop()
  {
    while (analogRead(resistpin2)>50)//OFF MOUSE
    {
      Serial.print("flag2: ");
      Serial.println(flag2);
      flag2 -=1;
      if(flag2 ==0)
      {
        if (offFlag ==true)
        {
          offFlag = false;
          Serial.println("falsee toggled");
        }
        
        else
        {
        offFlag = true;
        Serial.println("true toggled");  
        }
        
        break;
      }
    }
    Serial.print("flag1: ");
    Serial.println(flag1);
    flag1 -=1;
    if(flag1 ==0& offFlag == false)
    {
      Serial.println("MIDDLE CLICK"); //MIDDLE CLICK
      Click(1);
      //=============//
      FillLEDsFromPaletteColors( 1,1);

     // delay(500); //1 sec to recover
      //============//
      break;
    }
  }
  return;
}
void FillLEDsFromPaletteColors(int strip, int palette)
{
  static uint8_t colorIndex = 0;/* palette color change speed */
    currentBlending = LINEARBLEND;
    uint8_t brightness = 40;

    
  switch(palette) {
    case 1:// fingers light sequence
      SetupKJPalette();
      switch(strip)
      {
        case 1:
          for( int i = NUM_LEDS1-1; i >=0; i--) {
            leds1[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
            colorIndex += 1;
            FastLED.show();
            delay(10);
            colorIndex = colorIndex + 1; /* palette color change speed */
          }
           for (int i = NUM_LEDS1-1; i >=0; i--)
            {
              leds1[i] = CRGB::Black;
              FastLED.show();
              delay(15);
            }
          
          break;
        case 2:
          for( int i = NUM_LEDS2-1; i >=0; i--) {
              leds2[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
              colorIndex += 1;
              FastLED.show();
              delay(10);
              colorIndex = colorIndex + 1; /* palette color change speed */
              }
             for (int i = NUM_LEDS2-1; i >=0; i--)
              {
                leds2[i] = CRGB::Black;
                FastLED.show();
                delay(15);
              }
             
          break;
        case 3:
          for( int i = NUM_LEDS3-1; i >=0; i--) {
              leds3[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
              colorIndex += 1;
              FastLED.show();
              delay(10);
              colorIndex = colorIndex + 1; /* palette color change speed */
              }
             for (int i = NUM_LEDS3-1; i >=0; i--)
              {
                leds3[i] = CRGB::Black;
                FastLED.show();
                delay(15);
              }
            
          break;
      }
      break;
    case 2: // ON/OFF light sequence
      SetupTogglePalette();
        for( int i = NUM_LEDS3-1; i >=0; i--) {
                leds1[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
                leds2[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
                leds3[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
                colorIndex += 1;
                FastLED.show();
                delay(10);
                colorIndex = colorIndex + 1; /* palette color change speed */
                }
               for (int i = NUM_LEDS3-1; i >=0; i--)
                {
                  leds1[i] = CRGB::Black;
                  leds2[i] = CRGB::Black;
                  leds3[i] = CRGB::Black;
                  FastLED.show();
                  delay(15);
                }
      break;
  }
  

    
}



// This function sets up a palette of purple and green and aqua stripes.
void SetupKJPalette()
{
  CRGB deeppink  = CRGB::DeepPink;
  CRGB blue  = CRGB::Blue;
  CRGB black  = CRGB::Black;
  currentPalette = CRGBPalette16(
                                   deeppink,  blue,   deeppink );
}

void SetupTogglePalette()
{
  CRGB red  = CRGB::Red;
  CRGB darkgold  = CRGB::DarkGoldenRod;
  CRGB gold  = CRGB::Goldenrod ;
  currentPalette = CRGBPalette16(
                                   red,  gold,darkgold );
}



void Click(int i)
{
  switch(i) {
    case 1:
    myMouse.click(false,false,true);//only this function can control clicks. Mouse.click doesnt have the code but u can add it from this .move function in the header file if u want
    delay(10);
      break;
    case 2:
    myMouse.click(false,true,false);//only this function can control clicks. Mouse.click doesnt have the code but u can add it from this .move function in the header file if u want
    delay(10);
      break;
    case 3:
    myMouse.click(true,false,false);//only this function can control clicks. Mouse.click doesnt have the code but u can add it from this .move function in the header file if u want
    delay(10);
      break;
  }
  
}


  
