#include <LiquidCrystal.h>
#include <Keypad.h>

void Get_Password();
void Check_Password();
void send_message(char *data);
void send_LO_message();
void send_LC_message();

const byte ROWS = 3; //four rows
const byte COLS = 3; //three columns

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
};
byte rowPins[ROWS] = {10 , 9 , 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {13 , 12 , 11}; //connect to the column pinouts of the keypad

int randNumber;
char randNumber1[4]; 
int small = 1111;
int big = 9999;
int i,j,k,l,m,n,p,wrong,CD;
char Scratch[12];

int LOCKER = A0;
int LOCKER1 = A1;
char MyKey[4];
char card1[13] = "55000D5F282F";
int CLOSE = 16;    //A2

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(CLOSE , INPUT);
  digitalWrite(CLOSE , HIGH);
}

void loop()
{ 
   lcd.clear();
   lcd.setCursor(0,0);
   delay(5);
back:  
   lcd.clear();
   delay(3);
   lcd.print("SCRATCH THE CARD");
   delay(100);
   
   while(1)
   {
    if(Serial.available())
    {
      for(p = 0 ; p < 12 ; p++)
      {
        Scratch[p] = Serial.read();
        delay(2);        
      }

      if(p == 12)
      {
        break;
      }   
    }//if 
  }//while(1)
  
  for(p = 0 ; p < 12 ; p++)
  {
    if(Scratch[p] != card1[p])
    {     
          lcd.clear();
          lcd.setCursor(0 , 0);
          delay(1);
          lcd.print("SORRY ");
          lcd.setCursor(0 , 1);
          delay(1);
          lcd.print("Can't Verified");
          delay(2000);
          goto back;
    }//if
  }//for
  
  lcd.clear();
  lcd.setCursor(1,0);
  delay(5);
  lcd.print("WELCOME TO THE");
  lcd.setCursor(0,1);
  delay(5);
  lcd.print("BANK LOCKER SYS.");
  
again: 

  randNumber = small + (random() % (big - small));
     
  i = randNumber/1000;
  j = randNumber%1000;
  k = j/100;
  l = j%100;
  m = l/10;
  n = l%10;
  randNumber1[0] = char(i+0x30);
  randNumber1[1] = char(k+0x30);
  randNumber1[2] = char(m+0x30);
  randNumber1[3] = char(n+0x30);
  
  if(i == 0 || k == 0 || m == 0 || n == 0)
  {
     goto again;
  }
  delay(1000);        
  send_message(randNumber1);
  delay(200);
  lcd.clear();
      
  int count=0;
next: 
  wrong = 0;
  lcd.setCursor(0,0);
  delay(5);
  lcd.print("ENTER PASSWORD");
  Get_Password();
  Check_Password();
  
if(wrong == 1)
{
  if(count==0)
  { 
     lcd.clear();
     delay(5);
     lcd.print("1ST TRIAL LOST");
     delay(1000);
     lcd.clear();
     count++;
     goto next;
   }
    
   if(count==1)
   {
      lcd.clear();
      delay(5);
      lcd.print("2ND TRIAL LOST");
      delay(1000);
      lcd.clear();
      count++;
      goto next;
    }
    
    if(count==2)
    {
      lcd.clear();
      delay(5);
      lcd.print("SYSTEM IS BLOCK");
some:
      delay(5000);
      goto some;
    }
  }   
}

void Get_Password()
{
   for(;;)
       {
         MyKey[0] = keypad.getKey();
        
         if(MyKey[0])
         {
            lcd.setCursor(6,1);
            lcd.print('*');
    
            for(;;)
            {
               MyKey[1] = keypad.getKey();
               if(MyKey[1])
               {
                  lcd.setCursor(7,1);
                  lcd.print('*');
        
                  for(;;)
                  {
                     MyKey[2] = keypad.getKey();
                     if(MyKey[2])
                     {
                       lcd.setCursor(8,1);
                       lcd.print('*');
     
                       for(;;)
                       {
                         MyKey[3] = keypad.getKey();
                         if(MyKey[3])
                         {
                           lcd.setCursor(9,1);
                           lcd.print('*');
                           delay(500);
                           goto lable;
           
                         }
                       }
                     }
                  }
               }
           }
       }
     }
 lable:return;
}


void Check_Password()
{
    if(MyKey[0] == (i+48) && MyKey[1]== (k+48)  && MyKey[2] == (m+48)  &&  MyKey[3] == (n+48))
   {

           lcd.clear();
           delay(5);
           lcd.print("RIGHT PASSWORD");
           delay(1000);
           lcd.clear();
           lcd.print("LOCKER OPENED");
           delay(5);
           analogWrite(LOCKER , 255);
           analogWrite(LOCKER1 , 0);
           delay(1500);
           analogWrite(LOCKER , 0);
           analogWrite(LOCKER1 , 0);
           
           send_LO_message();
           delay(5000);
           while(1)
           {
               CD = digitalRead(CLOSE);
               {
                 if(CD ==  LOW)
                 {
                     analogWrite(LOCKER , 0);
                     analogWrite(LOCKER1 , 255);
                     delay(1500);
                     analogWrite(LOCKER , 0);
                     analogWrite(LOCKER1 , 0);
                     lcd.clear();
                     lcd.setCursor(0,0);
                     delay(3);
                     lcd.print("LOCKER CLOSED");
                     send_LC_message();
                     delay(5000);
                     digitalWrite(CLOSE , HIGH);
                     break;
                 }
               }
           }     
    }
    else
    {
          lcd.clear();
          delay(5);
          lcd.print("WRONG PASSWORD");
          delay(500);
          wrong =1;
       // goto back;
     }

}

 

void send_message(char *data)
{
   int num;  
   int length = strlen(data);
   Serial.print("AT+CMGF=1\r\n");    //set text mode
   delay(1000);  
   Serial.print("AT+CMGD=1\r\n");    //delet current contain
   delay(1000); 
   Serial.print("AT+CMGS=");
   Serial.print('"');
   Serial.print("9879686901");
   Serial.print('"');
   Serial.print('\r');
   delay(1000);
   for(num = 0 ; num < length-1 ; num++)    //change
   {
     Serial.write(data[num]);
   }
   delay(1000);
   Serial.print(char(0x1A));
   delay(5000);
} 


void send_LO_message()
{ 
   Serial.print("AT+CMGF=1\r\n");
   delay(1000);  
   Serial.print("AT+CMGD=1\r\n");
   delay(1000); 
   Serial.print("AT+CMGS=");
   Serial.print('"');
   Serial.print("9879686901");
   Serial.print('"');
   Serial.print('\r');
   delay(1000);
   Serial.println("LOCKER OPENED");
   delay(1000);
   Serial.print(char(0x1A));
   delay(5000);

}

void send_LC_message()
{
  Serial.print("AT+CMGF=1\r\n");
   delay(1000);  
   Serial.print("AT+CMGD=1\r\n");
   delay(1000); 
   Serial.print("AT+CMGS=");
   Serial.print('"');
   Serial.print("9879686901");
   Serial.print('"');
   Serial.print('\r');
   delay(1000);
   Serial.println("LOCKER CLOSED");
   delay(1000);
   Serial.print(char(0x1A));
   delay(5000);
}
