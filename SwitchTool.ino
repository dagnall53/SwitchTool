#define ver 001



#include <SSD1306Wire.h>  //https://github.com/ThingPulse/esp8266-oled-ssd1306

#include "images.h" 
#include "Terrier.h"
#include "MQTT.h"; //new place for mqtt stuff

#ifdef _Use_Wifi_Manager
       #include <WiFiManager.h>
#else
       #include "Secrets.h";
       String wifiSSID = SSID_RR;
       String wifiPassword = PASS_RR; 
#endif
#include <ESP8266WiFi.h>
uint8_t wifiaddr;
uint8_t ip0;
uint8_t ip1;
uint8_t    subIPH;
uint8_t    subIPL;
uint16_t HandControlID;
//LOCO throttle settings from mqtt
extern int SwitchNumbers; //set in parse to actual number of locos in lc list

extern String SW_id[MaxSwitches];
extern int SW_bus[MaxSwitches];
extern int SW_addr[MaxSwitches];

//tern String LOCO_V_cru[MaxSwitches];
//extern String LOCO_V_max[MaxSwitches];
//extern String LOCO_spcnt[MaxSwitches];

IPAddress ipBroad;
IPAddress mosquitto;
int connects;

// connect to display using pins D1, D2 for I2C on address 0x3c
SSD1306Wire  display(0x3c, D1, D2);
 #include "Menu.h"; //place for menu and display stuff

 
int y;
const int buttonPin3 = D3;     // the number of the pushbutton pin
const int buttonPin5 = D5;     // the number of the pushbutton pin
const int buttonPin6 = D6;     // the number of the pushbutton pin
const int buttonPin7 = D7;     // the number of the pushbutton pin
bool buttonState3 = 0;         // variable for reading the pushbutton status
bool buttonState4 = 0;         // variable for reading the pushbutton status
bool buttonState5 = 0;         // variable for reading the pushbutton status
bool buttonState6 = 0;         // variable for reading the pushbutton status
bool buttonState7 = 0;         // variable for reading the pushbutton status
bool buttonpressed;  

// display and selection stuff
int MenuLevel;
int switchindex;
int RightIndexPos;
int LeftIndexPos;
bool directionindex;
bool AllDataRead;
void OLED_Display(char* L1,char* L2,char* L3){
  display.clear();
 // Picture();
  display.drawString(64, 10, L1);
  display.drawString(64, 24, L2);
  display.drawString(64, 38, L3);
  display.display();
}

void ConnectionPrint() {
  
 char MsgTemp[127];
 int cx;
  Serial.println("");
  Serial.println(F("---------------------------Connected-----------------------"));
  Serial.print (F(" Connected to SSID:"));
  Serial.print(WiFi.SSID());
  Serial.print(F("  IP:"));
  Serial.println(WiFi.localIP());
 
 
 cx= sprintf (MsgTemp, " IP: %d:%d:%d:%d ", ipBroad[0],ipBroad[1],ipBroad[2],wifiaddr);
 OLED_Display("Connected",MsgTemp,"");
 delay(500); 

 
}


void Status(){
delay(10);
  Serial.println();Serial.println();
  Serial.println(F("-----------------------------------------------------------"));
  Serial.println(F("             ESP8266 Rocrail Client 'Switch Tool'    ")); 
  Serial.print(F("-------------------- limit "));
  Serial.print(MaxSwitches);
  Serial.println(F( " switches ------------------------"));
  Serial.print(F(  "                    revision:"));
  Serial.println(ver);
  Serial.println(F("-----------------------------------------------------------"));
 OLED_Display("Looking for:",SSID_RR,"");
  //display.drawString(64, 10, "Looking for:");
  //display.drawString(64, 24, SSID_RR);
  //display.display();
  delay(500);
  WiFi.setOutputPower(30); //  0 sets transmit power to 0dbm to lower power consumption, but reduces usable range.. try 30 for extra range

#ifdef _Use_Wifi_Manager
   WiFiManager wifiManager;  // this  stores ssid and password invisibly  !!
  //reset settings - for testing
  //wifiManager.resetSettings();
  wifiManager.autoConnect("ROCNODE ESP AP");  //ap name (with no password) to be used if last ssid password not found
#else    

  WiFi.mode(WIFI_STA);  //Alternate "normal" connection to wifi
  
  WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
  Serial.print(F("Trying to connect to {"));  Serial.print(wifiSSID.c_str());Serial.print(F("} "));
  while (WiFi.status() != WL_CONNECTED) {delay(500);Serial.print(".");}
 
#endif 

//if you get here you have connected to the WiFi
  Serial.println();Serial.println(F("--Connected--"));
 
  ipBroad = WiFi.localIP();
  subIPH = ipBroad[2];
  subIPL = ipBroad[3];
 wifiaddr = ipBroad[3];
  ConnectionPrint();
  delay(100);
  ipBroad[3] = 255; //Set broadcast to local broadcast ip e.g. 192.168.0.255 // used in udp version of this program
 
 //   ++++++++++ MQTT setup stuff   +++++++++++++++++++++
  mosquitto[0] = ipBroad[0]; mosquitto[1] = ipBroad[1]; mosquitto[2] = ipBroad[2];
  mosquitto[3] = BrokerAddr; //18;                //forced  mosquitto address, where the broker is! originally saved as RN[14], 
  Serial.print(F(" Mosquitto will first try to connect to:"));
  Serial.println(mosquitto);
  char Message[80];
  sprintf(Message, "SUBIP:%i", mosquitto[3]);
  OLED_Display("MQTT Setup",Message,"");
  delay(100);
  MQTT_Setup();  

    Serial.println(F("-----------MQTT NOW setup ----------------")); 
}




void PrintLocoSettings(){
        for (int loco=1; loco<= SwitchNumbers; loco++){
            Serial.print(F("Attributes stored for <"));Serial.print(loco);Serial.print(F("> <"));Serial.print(SW_id[loco]);
            Serial.print(F("> Vmin:"));Serial.print (SW_bus[loco]);
            Serial.print(F(" Vmid:"));Serial.print (SW_addr[loco]);
            //Serial.print(F(" Vcru:"));Serial.print (LOCO_V_cru[loco]);
           // Serial.print(F(" Vmax:"));Serial.print (LOCO_V_max[loco]);
           // Serial.print(F(" spdcnt:"));Serial.print (LOCO_spcnt[loco]);
      Serial.println("");}
}

void Picture(){
  display.clear(); 
  drawImageDemo(); 
  display.display();
}

void setup() {
  // init serial port
  Serial.begin(115200);
 
  // set the builtin LED pin to work as an output
 pinMode(LED_BUILTIN, OUTPUT);
  
 pinMode(buttonPin3, INPUT_PULLUP);
 pinMode(buttonPin5, INPUT_PULLUP);
 pinMode(buttonPin6, INPUT_PULLUP);
 pinMode(buttonPin7, INPUT_PULLUP);
  // init the display
  display.init();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
 
  y = 0;

 connects=0;

 Status();

// initial defaults
MenuLevel=0;
switchindex=1;
RightIndexPos=3;
LeftIndexPos=0;
directionindex=true;
SwitchNumbers=0; 
AllDataRead=false;
SW_id[0]="this is an unikely name";
}

 void MQTT_DO(void){
    // MQTT stuff, & check we are connected.. 
  if (!MQTT_Connected()) {reconnect();}           // was if (!client.connected()) {
      
  MQTT_Loop(); // for client.loop(); //gets wifi messages etc..
 }
 
void loop() {
  
 
  display.clear();   // clear the screen get loco list if not  
  
  MQTT_DO();
  buttonpressed=false; 
  DoDisplay(MenuLevel);

  if (y>=64){y=0;}
  // turn off the LED
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  
buttonState3 = digitalRead(buttonPin3);
buttonState5 = digitalRead(buttonPin5);
buttonState6 = digitalRead(buttonPin6);
buttonState7 = digitalRead(buttonPin7);

//Serial.print(" Buttons 3<");Serial.print(buttonState3);Serial.print("  4<");Serial.print(buttonState4);Serial.print("  5<");Serial.print(buttonState5);Serial.print(">  6<");Serial.print(buttonState6);Serial.print(">  7<");Serial.print(buttonState7);Serial.println(">");
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (!digitalRead(buttonPin3)) {
    buttonpressed=true;delay(100);ButtonRight(MenuLevel);
          MenuLevel=MenuLevel+1; // nb cannot change MenuLevel in a function that called with menulevel as a variable,
           if (MenuLevel>= 3){MenuLevel=0;}
      }
  if (!digitalRead(buttonPin5)) {
    buttonpressed=true;delay(100);
    ButtonSelect(MenuLevel);     
      } 
  if (!digitalRead(buttonPin6)) {
    buttonpressed=true;delay(100);
    ButtonUp(MenuLevel);
    } 
  if (!digitalRead(buttonPin7)) {
    buttonpressed=true;delay(100);
    ButtonDown(MenuLevel);
    } 

  y++;  // move text down 1 pixel next time and increment progress bar 


  // turn on the LED
  digitalWrite(LED_BUILTIN, LOW);
  delay(1);
}
