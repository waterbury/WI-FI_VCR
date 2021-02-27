/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/


TaskHandle_t Task1;
TaskHandle_t Task2;


#define VFD_SA 23
#define VFD_SB 25
#define VFD_SC 26 // G8-G1 A-Element Seven-Segment Portion
#define VFD_SD 27 // G8-G1 B-Element Seven-Segment Portion 
#define VFD_SE 32 // G8-G1 C-Element Seven-Segment Portion
#define VFD_SF 33 // G8-G1 D-Element Seven-Segment Portion
#define VFD_SG 34 // G8-G1 E-Element Seven-Segment Portion
#define VFD_SH 35 // G8-G1 F-Element Seven-Segment Portion
#define VFD_SI 36 // G8-G1 G-Element Seven-Segment Portion
#define VFD_SJ 37 // 
#define VFD_SK 5  // 
#define VFD_SL 14 // 


#define VFD_G1 4
#define VFD_G2 13
#define VFD_G3 16
#define VFD_G4 17
#define VFD_G5 18
#define VFD_G6 19
#define VFD_G7 21
#define VFD_G8 22

#define VCR_IR 2

#define ZERO_PADDING 40

uint8_t foundPins = 0;
//uint8_t  GRIDPINS = 0;
//uint16_t SEGPINS = 0;
uint64_t ALLPINS = 0;

char binstr[]="00000000";

void SPrintZeroPadBin(uint64_t number) {
char binstr[]="0000000000000000000000000000000000000000000000000000000000000000";
uint8_t i=0;
uint16_t n=number;

   while(n>0 && i<ZERO_PADDING){
      binstr[ZERO_PADDING-1-i]=n%2+'0';
      ++i;
      n/=2;
   }
  
   Serial.println(binstr);
}



uint16_t vfdChar[8] = {0,0,0,0,0,0,0,0};

uint8_t vfdFlag = 0;
uint8_t beginFrameFlag = 0;

void IRAM_ATTR screenCapture(){

  uint8_t  gridPins = 0;
  uint16_t segPins = 0;
  uint64_t allPins = 0;

if (!vfdFlag){
  uint32_t GPIO_IN_REG_VAL  = REG_READ(GPIO_IN_REG) ;
  uint32_t GPIO_IN1_REG_VAL = REG_READ(GPIO_IN1_REG) ;


if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_G1) ) gridPins |= 0b0000000000001 ;
if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_G2) ) gridPins |= 0b0000000000010 ;
if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_G3) ) gridPins |= 0b0000000000100 ;
if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_G4) ) gridPins |= 0b0000000001000 ;
if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_G5) ) gridPins |= 0b0000000010000 ;
if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_G6) ) gridPins |= 0b0000000100000 ;
if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_G7) ) gridPins |= 0b0000001000000 ;
if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_G8) ) gridPins |= 0b0000010000000 ;  

if ( pinStateCheck( GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SA) ) segPins |= 0b0000000000001 ;
if ( pinStateCheck( GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SB) ) segPins |= 0b0000000000010 ;
if ( pinStateCheck( GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SC) ) segPins |= 0b0000000000100 ;
if ( pinStateCheck( GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SD) ) segPins |= 0b0000000001000 ;
if ( pinStateCheck( GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SE) ) segPins |= 0b0000000010000 ;
if ( pinStateCheck( GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SF) ) segPins |= 0b0000000100000 ;
if ( pinStateCheck( GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SG) ) segPins |= 0b0000001000000 ;
if ( pinStateCheck( GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SH) ) segPins |= 0b0000010000000 ;
if ( pinStateCheck( GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SI) ) segPins |= 0b0000100000000 ;
if ( pinStateCheck( GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SJ) ) segPins |= 0b0001000000000 ;
if ( pinStateCheck( GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SK) )  segPins |= 0b0010000000000 ;
if ( pinStateCheck( GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SL) )  segPins |= 0b0100000000000 ;


     if (gridPins & 0b01)       { vfdChar[0] = segPins; beginFrameFlag = 0;}
else if (beginFrameFlag); // If beginFrameFlag stop assessing pins
else if (gridPins & 0b010)       { vfdChar[1] = segPins; }
else if (gridPins & 0b0100)      { vfdChar[2] = segPins; }
else if (gridPins & 0b01000)     { vfdChar[3] = segPins; }
else if (gridPins & 0b010000)    { vfdChar[4] = segPins; }
else if (gridPins & 0b0100000)   { vfdChar[5] = segPins; }
else if (gridPins & 0b01000000)  { vfdChar[6] = segPins; }
else if (gridPins & 0b010000000) {vfdChar[7] = segPins; vfdFlag = 1; beginFrameFlag = 1; }

}

//segPins |= 0x8000;
/*    
      if (gridPins & 0b01)       { Serial.print( segPins,HEX ); Serial.print( " "); }
else if (gridPins & 0b010)       { Serial.print( segPins,HEX ); Serial.print( " "); }
else if (gridPins & 0b0100)      { Serial.print( segPins,HEX ); Serial.print( " "); }
else if (gridPins & 0b01000)     { Serial.print( segPins,HEX ); Serial.print( " "); }
else if (gridPins & 0b010000)    { Serial.print( segPins,HEX ); Serial.print( " "); }
else if (gridPins & 0b0100000)   { Serial.print( segPins,HEX ); Serial.print( " "); }
else if (gridPins & 0b01000000)  { Serial.print( segPins,HEX ); Serial.print( " "); }
else if (gridPins & 0b010000000) Serial.println( segPins,HEX );
*/



//Serial.println(segPins,BIN);
//Serial.print(" ");
   
}

char returnSegChar(uint16_t sevenSegmentElements){

  uint16_t sevenSegmentChars   =  sevenSegmentElements & 0b01111111;
  uint16_t sevenSegmentChars2   =  sevenSegmentElements >> 8;

  sevenSegmentChars = (sevenSegmentElements >> 8) & 0b01111111;

  //sevenSegmentChars ^= 0x7F;
  
  uint16_t sevenSegmentSpecial =  sevenSegmentElements & 0b10000000;

  if ( sevenSegmentChars == 0b0111111) return '0';
  if ( sevenSegmentChars == 0b0000110) return '1';
  if ( sevenSegmentChars == 0b1011011) return '2';
  if ( sevenSegmentChars == 0b1001111) return '3';
  if ( sevenSegmentChars == 0b1100110) return '4';
  if ( sevenSegmentChars == 0b1101101) return '5';
  if ( sevenSegmentChars == 0b1111101) return '6';
  if ( sevenSegmentChars == 0b0000111) return '7';  
  if ( sevenSegmentChars == 0b1111111) return '8';
  if ( sevenSegmentChars == 0b1100111) return '9';
  if ( sevenSegmentChars == 0b1110111) return 'A';  
  if ( sevenSegmentChars == 0b1111100) return 'B';
  if ( sevenSegmentChars == 0b0111001) return 'C';  
  if ( sevenSegmentChars == 0b1011110) return 'D';
  if ( sevenSegmentChars == 0b1111001) return 'E';
  if ( sevenSegmentChars == 0b1110001) return 'F';
  if ( sevenSegmentChars == 0b1000000) return '-';
  if ( sevenSegmentChars == 0b0001000) return '-';
  if ( sevenSegmentChars == 0b0001101) return '1';
  if ( sevenSegmentChars == 0b0000000) return 'X';
/*
  if ( sevenSegmentChars2 == 0b0111111) return '0';
  if ( sevenSegmentChars2 == 0b0000110) return '1';
  if ( sevenSegmentChars2 == 0b1011011) return '2';
  if ( sevenSegmentChars2 == 0b1001111) return '3';
  if ( sevenSegmentChars2 == 0b1100110) return '4';
  if ( sevenSegmentChars2 == 0b1101101) return '5';
  if ( sevenSegmentChars2 == 0b1111101) return '6';
  if ( sevenSegmentChars2 == 0b0000111) return '7';  
  if ( sevenSegmentChars2 == 0b1111111) return '8';
  if ( sevenSegmentChars2 == 0b1100111) return '9';
  if ( sevenSegmentChars2 == 0b1110111) return 'A';  
  if ( sevenSegmentChars2 == 0b1111100) return 'B';
  if ( sevenSegmentChars2 == 0b0111001) return 'C';  
  if ( sevenSegmentChars2 == 0b1011110) return 'D';
  if ( sevenSegmentChars2 == 0b1111001) return 'E';
  if ( sevenSegmentChars2 == 0b1110001) return 'F';
  if ( sevenSegmentChars2 == 0b1000000) return '-';
  if ( sevenSegmentChars2 == 0b0001000) return '-';
  if ( sevenSegmentChars2 == 0b0001101) return '1';
  

  if ( sevenSegmentSpecial == 0b10000000) return '.';

 // if ( sevenSegmentElements >= 33 && sevenSegmentElements <= 122) return (char)sevenSegmentElements;
 // if ( sevenSegmentElements >= 123 && sevenSegmentElements <= 212) return (char)sevenSegmentElements - 90;
  */
  return '#';  
}


uint8_t getGrid()
{


 
 return 0;}


uint16_t getSeg(){

uint16_t segPins = 0;

if ( digitalRead(VFD_SA) ) segPins += 1;
if ( digitalRead(VFD_SB) ) segPins += 2;
if ( digitalRead(VFD_SC) ) segPins += 4; 
if ( digitalRead(VFD_SD) ) segPins += 8; 
if ( digitalRead(VFD_SE) ) segPins += 16; 
if ( digitalRead(VFD_SF) ) segPins += 32; 
if ( digitalRead(VFD_SG) ) segPins += 64; 
if ( digitalRead(VFD_SH) ) segPins += 128;
if ( digitalRead(VFD_SI) ) segPins += 256;
if ( digitalRead(VFD_SJ) ) segPins += 512;
if ( digitalRead(VFD_SK) ) segPins += 1024; 
if ( digitalRead(VFD_SL) ) segPins += 2048;

 return segPins;

}


//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  uint8_t foundPins = 0;
  

  while (1 == 1){
/*
    ALLPINS = REG_READ(GPIO_IN_REG) << 6;
    ALLPINS |= REG_READ(GPIO_IN1_REG);
    SPrintZeroPadBin(ALLPINS);


  if (foundPins == 255){
   //Serial.println("all pins!");
   foundPins = 0;
    
  }
  else {
   //Serial.println(foundPins);
  */
  
  vTaskDelay(10);
  } 
}




//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  while (1 == 1){

     vTaskDelay(10);
  }
}



// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.

  Serial.begin(115200);

  gpio_pad_select_gpio(VFD_SA);
  gpio_pad_select_gpio(VFD_SB);
  gpio_pad_select_gpio(VFD_SC);
  gpio_pad_select_gpio(VFD_SD);
  gpio_pad_select_gpio(VFD_SE);
  gpio_pad_select_gpio(VFD_SF);
  gpio_pad_select_gpio(VFD_SG);
  gpio_pad_select_gpio(VFD_SH);
  gpio_pad_select_gpio(VFD_SI);
  gpio_pad_select_gpio(VFD_SJ);
  gpio_pad_select_gpio(VFD_SK);
  gpio_pad_select_gpio(VFD_SL);  

  pinMode(VFD_SA, INPUT);
  pinMode(VFD_SB, INPUT);
  pinMode(VFD_SC, INPUT);
  pinMode(VFD_SD, INPUT);
  pinMode(VFD_SE, INPUT);
  pinMode(VFD_SF, INPUT);
  pinMode(VFD_SG, INPUT);
  pinMode(VFD_SH, INPUT);
  pinMode(VFD_SI, INPUT);
  pinMode(VFD_SJ, INPUT);
  pinMode(VFD_SK, INPUT);
  pinMode(VFD_SL, INPUT);
  
  pinMode(VFD_G1, INPUT);
  pinMode(VFD_G2, INPUT);
  pinMode(VFD_G3, INPUT);
  pinMode(VFD_G4, INPUT);
  pinMode(VFD_G5, INPUT);
  pinMode(VFD_G6, INPUT);
  pinMode(VFD_G7, INPUT);
  pinMode(VFD_G8, INPUT);



  //REG_WRITE(GPIO_FUNC39_IN_SEL_CFG_REG,0);

  attachInterrupt(VFD_G1, screenCapture, RISING);
  attachInterrupt(VFD_G2, screenCapture, RISING);
  attachInterrupt(VFD_G3, screenCapture, RISING);
  attachInterrupt(VFD_G4, screenCapture, RISING);
  attachInterrupt(VFD_G5, screenCapture, RISING);
  attachInterrupt(VFD_G6, screenCapture, RISING);
  attachInterrupt(VFD_G7, screenCapture, RISING);
  attachInterrupt(VFD_G8, screenCapture, RISING);


  pinMode(VCR_IR, OUTPUT);
}
/*
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
             //       "Task1",     /* name of task. */
           //         10000,       /* Stack size of task */
         //           NULL,        /* parameter of the task */
       //             1,           /* priority of the task */
     //               &Task1,      /* Task handle to keep track of created task */
   //                 0);          /* pin task to core 0 */                  
 // delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
/*
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
//                    "Task2",     /* name of task. */
//                    10000,       /* Stack size of task */
//                    NULL,        /* parameter of the task */
 //                   1,           /* priority of the task */
  //                  &Task2,      /* Task handle to keep track of created task */
   //                 1);          /* pin task to core 1 */
   // delay(500); 
//}

  




bool pinStateCheck(uint32_t GPIO_IN_REG_VAL, uint32_t GPIO_IN1_REG_VAL, uint8_t pinCheck){

if (pinCheck < 32){
 if ( (GPIO_IN_REG_VAL >> pinCheck) & (uint32_t)1  != 0) 
 
  return true; }
else{ 
 if ( ((GPIO_IN1_REG_VAL >> (pinCheck - 32)) & (uint32_t)1 ) != 0) 
   
  return true; }
 
return false;
  
}


void sevenSegmentPrint(int currentLine){

int i = 0;
int j = 0;

bool segSA;
bool seg_A;
bool seg_B;
bool seg_C;
bool seg_D;
bool seg_E;
bool seg_F;
bool seg_G;
bool seg_H;

bool seg_J;
bool segSK;
bool seg_L;

  //bool seg_A = 

if(currentLine == 0){  
      Serial.print(vfdChar[7]); Serial.print(" "); Serial.print(vfdChar[6]); Serial.print(" "); 
      Serial.print(vfdChar[5]); Serial.print(" "); Serial.print(vfdChar[4]); Serial.print(" ");
      Serial.print(vfdChar[3]); Serial.print(" "); Serial.print(vfdChar[2]); Serial.print(" ");
      Serial.print(vfdChar[1]); Serial.print(" "); Serial.println(vfdChar[0]); }
	  
//render from top left to bottom right; ie currenLine 0 is top
  for (i=7;i>=0;i--){
   // Serial.println(vfdChar[i]);
   
    segSA = (vfdChar[i] &    0b1);
    //seg_J = (vfdChar[i] &   0b1000000000);
    //seg_K = (vfdChar[i] &  0b10000000000);
    //seg_L = (vfdChar[i] & 0b100000000000);    

    seg_A = (vfdChar[i] &             0b100);
    seg_B = (vfdChar[i] &            0b1000);
    seg_C = (vfdChar[i] &           0b10000);
    seg_D = (vfdChar[i] &          0b100000);
    seg_E = (vfdChar[i] &         0b1000000);
    seg_F = (vfdChar[i] &        0b10000000);
    seg_G = (vfdChar[i] &       0b100000000);
    seg_H = (vfdChar[i] &      0b1000000000);
	
	segSK = (vfdChar[i] &     0b10000000000);

	Serial.print(" ");

	if(i == 7){          
		if ( (segSK) && ( currentLine == 1 || currentLine == 2 || currentLine == 4 || currentLine == 5) )
			Serial.print("I");
		else
			Serial.print(" ");
	}
	Serial.print(" ");
    

    for(j=0;j<5;j++){
     if (currentLine == 0){
       if( (j == 1 || j == 2 | j == 3) && seg_A  ) Serial.print('I'); 
       else Serial.print(" "); }
     else if (currentLine == 1 || currentLine == 2){
       if( (j == 0) && seg_F  ) Serial.print('I'); 
       else if( (j == 4) && seg_B  ) Serial.print('I');
       else Serial.print(" "); }
     else if (currentLine == 3){
       if( (j == 1 || j == 2 | j == 3) && seg_G  ) Serial.print('I'); 
       else Serial.print(" "); }  
     else if (currentLine == 4 || currentLine == 5){
       if( (j == 0) && seg_E  ) Serial.print('I'); 
       else if( (j == 4) && seg_C  ) Serial.print('I');
       else Serial.print(" "); }
     else if (currentLine == 6){
       if( (j == 1 || j == 2 | j == 3) && seg_D  ) Serial.print('I'); 
       else Serial.print(" "); }
     else
      Serial.print(' ');      
    }

    Serial.print(' ');    
  }

Serial.println("");
  
}

// the loop function runs over and over again forever
void loop() {



int i = 0;
int j = 0;

if (vfdFlag){
   Serial.write(27);
   Serial.println("");

  for(i=0;i<7;i++){
   sevenSegmentPrint(i);
  }
  vfdFlag = 0;
  Serial.println("");
}

delay(300);

}


  
  //digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
  //delay(2000);                       // wait for a second
  //digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
  //delay(2000);// wait for a second
/*
  uint32_t segPins = 0; 
 uint16_t *GPIO_IN_REG_ADDR = (uint16_t *)0x3FF4403C;

  uint32_t GPIO_IN_REG_VAL =  0;
  GPIO_IN_REG_VAL += 0x80000000;
  uint32_t GPIO_IN1_REG_VAL = REG_READ(GPIO_IN1_REG) ;
  
 // allPins |= ( REG_READ(GPIO_IN1_REG) << 32);
 // allPins |= 0b10000000000000000000000000000000;
while(1){

  GPIO_IN_REG_VAL =  REG_READ(GPIO_IN_REG);//*GPIO_IN_REG_ADDR ;
 //GPIO_IN_REG_VAL |=  *(GPIO_IN_REG_ADDR + 0xFFFF) << 16;
   // GPIO_IN_REG_VAL += 0x80000000;
  GPIO_IN1_REG_VAL = REG_READ(GPIO_IN1_REG) ;
    
segPins = 0;
if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SA) ) segPins +=             0b1 ;
if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SB) ) segPins +=            0b10 ;
if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SC) ) segPins +=           0b100 ;
if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SD) ) segPins +=          0b1000 ;
if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SE) ) segPins +=         0b10000 ;
if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SF) ) segPins +=        0b100000 ;
if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SG) ) segPins +=       0b1000000 ;
if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SH) ) segPins +=      0b10000000 ;
if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SI) ) segPins +=     0b100000000 ;
if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SJ) ) segPins +=    0b1000000000 ;
if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SK) ) segPins +=   0b10000000000 ;
if ( pinStateCheck(GPIO_IN_REG_VAL,GPIO_IN1_REG_VAL, VFD_SL) ) segPins +=  0b100000000000 ;

//segPins +=8000;
//Serial.print("allPins: ");
//print(allPins);
Serial.print("GPIO_IN_REG: ");
Serial.print(GPIO_IN_REG_VAL,BIN); 
Serial.print(" ");
Serial.print("GPIO_IN1_REG: ");
Serial.print(GPIO_IN1_REG_VAL,BIN); 
Serial.print(" ");
Serial.print("segPins: ");
Serial.print(segPins,BIN);
Serial.print("segPins DEC: ");  
Serial.print(segPins);
Serial.print(" ");
Serial.print(digitalRead(VFD_SL),BIN);   
Serial.print(digitalRead(VFD_SK),BIN);   
Serial.print(digitalRead(VFD_SJ),BIN);   
Serial.print(digitalRead(VFD_SI),BIN);   
Serial.print(digitalRead(VFD_SH),BIN);   
Serial.print(digitalRead(VFD_SG),BIN);   
Serial.print(digitalRead(VFD_SF),BIN);   
Serial.print(digitalRead(VFD_SE),BIN);   
Serial.print(digitalRead(VFD_SD),BIN);   
Serial.print(digitalRead(VFD_SC),BIN);   
Serial.print(digitalRead(VFD_SB),BIN);   
Serial.println(digitalRead(VFD_SA),BIN); 
SET_PERI_REG_BITS(IO_MUX_GPIO5_REG, MCU_SEL, 1, 3);  
Serial.print("RTCIO_PAD_DAC1_REG: ");
Serial.println(REG_READ(0x3FF4906C));
delay(3000);

}
*/
