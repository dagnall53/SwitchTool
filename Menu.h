// extern variables for display and selection stuff
extern int MenuLevel;
extern int switchindex;
extern int RightIndexPos;
extern int LeftIndexPos;
extern bool directionindex;
extern int SwitchNumbers;
byte SW_all[9];
extern int y;
bool switchstate;
extern byte ParseIndex;

extern void SetWordIn_msg_loc_value(uint8_t* msg, uint8_t firstbyte, int value);

char* Str2Chr(String stringin){
  char* Converted;
  int cx;
  Converted="";
 // Serial.print(" Converting :");Serial.print(stringin);
   for (int i=0; i<=(stringin.length()+1);i++){ Converted[i]=stringin.charAt(i); }
 //  Serial.print(" to:");Serial.println(Converted);
   return Converted;
}

void drawImageDemo() {
    // Use GIMP to convert files..

   display.drawXbm(0,0, Terrier_Logo_width, Terrier_Logo_height, Terrier_Logo); 
   // display.drawXbm(34,14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits); 

}
void UpdateSetPositions(){
  uint8_t RocSendAddrCommand[15];
  RocSendAddrCommand[0]=0;
  RocSendAddrCommand[1]=0;
  RocSendAddrCommand[2]=SW_bus[switchindex];
  RocSendAddrCommand[3]=0;
  RocSendAddrCommand[4]=0;
  RocSendAddrCommand[5]=0x05; //group
  RocSendAddrCommand[6]=0x10; //code
  RocSendAddrCommand[7]=0x08; //length  
    for (byte x = 1; x <= 8; x++) { RocSendAddrCommand[7+x]=SW_all[x];} // paste other data as recieved..
  SetWordIn_msg_loc_value(RocSendAddrCommand, 7+2, LeftIndexPos);//re write bits we want to change..&+n because I should have done it on sw_all!
  SetWordIn_msg_loc_value(RocSendAddrCommand, 7+4, RightIndexPos);
  MQTTRocnetSend("rocnet/ps",RocSendAddrCommand);
}
extern char DebugMsg[127];
void SetSwitch(int switchindex,bool Throw){
   char MsgTemp[200];
  int cx;
  if (Throw)
  {cx=sprintf(MsgTemp,"<sw id=\"%s\" cmd=\"turnout\"  />",Str2Chr(SW_id[switchindex]),RightIndexPos);}
  else {cx=sprintf(MsgTemp,"<sw id=\"%s\" cmd=\"straight\"  />",Str2Chr(SW_id[switchindex]),RightIndexPos);}
  MQTTSend("rocrail/service/client",MsgTemp);
  DebugSprintfMsgSend( sprintf ( DebugMsg, " sending {%s}",MsgTemp));
  
  }
  
  extern void Picture();
void DoDisplay(int MenuLevel){
String TopMessage = "Available Switches:";
TopMessage += (SwitchNumbers);
display.setFont(ArialMT_Plain_10);
//display.drawProgressBar(0, 58,127,6, ((y*100)/64)); 
display.setTextAlignment(TEXT_ALIGN_CENTER);
if (SwitchNumbers>=1){
display.drawString(64,54,TopMessage);}

switch (MenuLevel){
  
 case 0:  // top level
    display.setFont(ArialMT_Plain_10);
    display.drawString(64,1,"--- Select Switch ---");
if (SwitchNumbers<=0){
  Picture();
   display.setFont(ArialMT_Plain_10);
    display.drawString(64,25," Press to ");
    display.drawString(64,38,"List of Turnouts"); 
    display.setFont(ArialMT_Plain_16);
}
  else{  
    if (switchindex>=1){
     display.setFont(ArialMT_Plain_10);
     display.drawString(64,20,SW_id[switchindex-1]);
        }
     if (switchindex<=SwitchNumbers-1){
     display.setFont(ArialMT_Plain_10);
     display.drawString(64,44,SW_id[switchindex+1]);
        }    
    
     display.setFont(ArialMT_Plain_16);
    display.drawString(64,30,SW_id[switchindex]);
   
  }
      break;
 case 1: // selected Switch show Left pos

 if (SwitchNumbers<=0){
   display.setFont(ArialMT_Plain_16);
   display.drawString(64,1,"No Switch");
  display.drawString(64,16,"selected");}
   else
    {
     display.setFont(ArialMT_Plain_16);
     display.drawString(64,1,SW_id[switchindex]); 
     display.setFont(ArialMT_Plain_10);
     display.drawString(10,16,"Bus:");display.drawString(42,16,String(SW_bus[switchindex]));
     display.drawString(75,16,"Addr:");display.drawString(100,16,String(SW_addr[switchindex]));
    }
 //show setting  
    display.setFont(ArialMT_Plain_16);
    display.drawString(25,32,"Left:");
    display.drawString(60,32,String(LeftIndexPos));
   
      break;
      
case 2: // selected switch, set its right position

 if (SwitchNumbers<=0){
   display.setFont(ArialMT_Plain_16);
   display.drawString(64,1,"No Switch");
  display.drawString(64,16,"selected");}
   else
    {
     display.setFont(ArialMT_Plain_16);
     display.drawString(64,1,SW_id[switchindex]); 
     display.setFont(ArialMT_Plain_10);
     display.drawString(10,16,"Bus:");display.drawString(42,16,String(SW_bus[switchindex]));
     display.drawString(75,16,"Addr:");display.drawString(100,16,String(SW_addr[switchindex]));
    }
 //show setting  
    display.setFont(ArialMT_Plain_16);
    display.drawString(25,32,"Right:");
    display.drawString(60,32,String(RightIndexPos));
   


      break;
case 3: 
     display.setFont(ArialMT_Plain_16);
     display.drawString(64,1,"--Level 3--");





      break;

 
 default:
 
      break; 
 
  
}//end  
display.display(); 
}

extern bool buttonState5;
void ButtonDown(int MenuLevel){
  Serial.print("DOWN");
switch (MenuLevel){
  
 case 0:  // top level
 switchindex=switchindex-1;
 break;
 case 1:  // top level
 if(buttonState5){ LeftIndexPos=LeftIndexPos+2;}
 else  {LeftIndexPos=LeftIndexPos+15;}
 if (LeftIndexPos>=600){LeftIndexPos=600;}
  UpdateSetPositions();
  SetSwitch(switchindex,0);
 break;
 case 2:
 if(buttonState5){RightIndexPos=RightIndexPos+2;}
 else{RightIndexPos=RightIndexPos+15;}
 if (RightIndexPos>=600){RightIndexPos=600;}
  UpdateSetPositions();
  SetSwitch(switchindex,1);
 break;
 default:
 break;
  

}


if (switchindex <=0) {switchindex=0;}
if (switchindex>=SwitchNumbers){switchindex=SwitchNumbers;}
}

void ButtonUp(int MenuLevel){
  Serial.print("UP");
switch (MenuLevel){
  
 case 0:  // top level
 switchindex=switchindex+1;
 break;
 case 1:  // level 1 (left)
 if(buttonState5){LeftIndexPos=LeftIndexPos-2;}
 else{LeftIndexPos=LeftIndexPos-15;}
 if (LeftIndexPos<=160){LeftIndexPos=160;}
 UpdateSetPositions();
  SetSwitch(switchindex,0);

 break;
 case 2:
 if(buttonState5){RightIndexPos=RightIndexPos-2;}
  else {RightIndexPos=RightIndexPos-15;}
 if (RightIndexPos<=160){RightIndexPos=160;}
 UpdateSetPositions();
  SetSwitch(switchindex,1);
 break;
 default:
 break;
  
}

if (switchindex <=0) {switchindex=0;}
if (switchindex>=SwitchNumbers){switchindex=SwitchNumbers;}
}



void ButtonRight(int MenuLevel){
  uint8_t RocCommand[9];
  
  // nb Looks like you cannot change MenuLevel in a function that called with MenuLevel as a variable, presumably it sets internal variable only?
switch (MenuLevel){
  
 case 0:  // top level
  // get left and right pos from rocrail 
//  MQTT rx topic:[rocnet/ps]  00 00 1C 00 00 05 0F 02 01 08
// MSG  is < NetID:0 Rec:28 Sdr:0 Grp:5 Code[Req]:15 D1=1 D2=8>
RocCommand[0]=0;
RocCommand[1]=0;
RocCommand[2]=SW_bus[switchindex];
RocCommand[3]=0;
RocCommand[4]=0;
RocCommand[5]=0x05; //group
RocCommand[6]=0x0F; //code
RocCommand[7]=0x02; //length
RocCommand[8]=SW_addr[switchindex]; //was 1
RocCommand[9]=SW_addr[switchindex]; //was 8

  MQTTRocnetSend("rocnet/ps",RocCommand);
  delay(50);
  switchstate=0;
  SetSwitch(switchindex,switchstate);// set to left position 
 break;
 case 1:
  UpdateSetPositions();
 switchstate=1;
  SetSwitch(switchindex,switchstate);// set to right position 
 break;
 case2:
  UpdateSetPositions();

 
 break;

 default:
  
 break;
}


}

void ButtonSelect(int MenuLevel){ 
  char MsgTemp[200];
  int cx;

switch (MenuLevel){
  
 case 0:  // top level
  ParseIndex=0;
  MQTTSend("rocrail/service/client","<model cmd=\"swprops\" />");
 
 break;

 case 1:
 /// alternate switch position
 //UpdateSetPositions();
 SetSwitch(switchindex,switchstate);// like MQTTSend("rocrail/service/client","<sw id=\"wood7\" cmd=\"straight\"   />");
 switchstate=!switchstate;
  
 break;
 case 2:
  // alternate switch position 
 // UpdateSetPositions();
  SetSwitch(switchindex,switchstate);// like MQTTSend("rocrail/service/client","<sw id=\"wood7\" cmd=\"turnout\"   />");
  switchstate=!switchstate; 

 break;
 default:
 break;
  
}
}

