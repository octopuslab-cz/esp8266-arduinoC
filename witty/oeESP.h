//octopus engine - lib. for Witty board (ESP8266)
//2016/09
//-----------------------------------------------
const int RED = 15;
const int GREEN = 12;//13
const int BLUE = 13;//12
const int PHOTO = A0;
const int BUTTON = 4;

//-----------------------------------------------
void ledRGB(){
  analogWrite(RED, 1023);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);
  delay(300);

  analogWrite(RED, 0);
  analogWrite(GREEN, 1023);
  analogWrite(BLUE, 0);
  delay(300);

  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 1023);
  delay(300);
}  

//-----------------------------------------------
void ledRed()
{
   analogWrite(RED, 1023);
   analogWrite(GREEN,0);
   analogWrite(BLUE,0);  
  }

void ledGre()
{
   analogWrite(RED, 0);
   analogWrite(GREEN,1023);
   analogWrite(BLUE,0);  
  }

void ledBlu()
{
   analogWrite(RED, 0);
   analogWrite(GREEN,0);
   analogWrite(BLUE,1023);  
  }

//-----------------------------------------------
void blinkRed(){
   analogWrite(RED, 1023);
   analogWrite(GREEN, 0);
   analogWrite(BLUE, 0);
   delay(300);
   analogWrite(RED, 0);
   analogWrite(GREEN, 0);
   analogWrite(BLUE, 0);    
   delay(300);    
  }

void blinkGre(){
   analogWrite(RED, 0);
   analogWrite(GREEN,1023);
   analogWrite(BLUE, 0);
   delay(300);
   analogWrite(RED, 0);
   analogWrite(GREEN, 0);
   analogWrite(BLUE, 0);    
   delay(300);    
  }

void blinkBlu(){
   analogWrite(RED, 0);
   analogWrite(GREEN,0);
   analogWrite(BLUE,1023);
   delay(200);
   analogWrite(RED, 0);
   analogWrite(GREEN, 0);
   analogWrite(BLUE, 0);    
   delay(500);    
  }
