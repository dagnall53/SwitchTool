      #define SSID_RR "Router"
      #define PASS_RR "password"
       #define BrokerAddr 21 // sub ip of the broker BrokerAddr; 
       #define MaxSwitches 100  // the number we can store (+1 as we use 0) (limited by length of mqtt undelivered message buffers!
/* PIN References... Also defined somewhere else in the esp included code so do not unhide this section!!!...
  static const uint8_t D0   = 16;  and Red Led on NodeMcu V2 (not present on NodeMCU v3)
  static const uint8_t D1   = 5;
  static const uint8_t D2   = 4;
  static const uint8_t D3   = 0;
  static const uint8_t D4   = 2;  and Blue Led on ESP8266 also is i2c clk?
  static const uint8_t D5   = 14;
  static const uint8_t D6   = 12;
  static const uint8_t D7   = 13;
  static const uint8_t D8   = 15;
  static const uint8_t D9   = 3;
  static const uint8_t D10  = 1;


  #define NodeMCUPinD[SignalLed] 2 // same as PIN D4!

*/
