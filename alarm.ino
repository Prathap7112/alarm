  #include <LiquidCrystal.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <Keypad.h>
#include <ds3231.h>

#define alarmOutput 13

union get_time_u
{
  struct{
   char h_b;
   char l_b;
  } time_byte;
   unsigned char b;
};

union get_time_u get_hour,get_min,get_sec;

struct bell 
{
  unsigned int hrs[18];
  unsigned int mins[18];
  unsigned int secs[18];
};

struct bell alarm_1={8,8,9,10,10,10,10,11,12,12,13,13,14,14,14,15,16,0,40,45,30,20,30,35,35,25,15,50,0,50,40,45,50,40,30,0,
                   5,10,5,5,5,10,5,5,5,5,10,5,5,5,10,5,10,0};
struct bell alarm_2={9,10,10,11,11,12,12,12,12,13,13,14,14,15,15,16,16,17,45,00,30,00,30,00,30,50,00,45,00,30,00,30,00,30,50,0,
                   5,5,5,5,5,5,5,5,10,5,5,5,5,5,5,5,5,5};
struct bell alarm_3={8,8,9,10,10,10,11,12,0,0,0,0,0,0,0,0,0,0,40,45,30,20,30,35,25,15,0,0,0,0,0,0,0,0,0,0,
                   5,10,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0};                   
                   
struct bell *p_alarm_1=&alarm_1;
struct bell *p_alarm_2=&alarm_2;
struct bell *p_alarm_3=&alarm_3;


unsigned int mode=0,alarm_count=0,alarm_on=0,mode_count=0,mode_pos=0,modeGetCount=0,i;
char mode_get[2]={0,0};
volatile unsigned int alarmTimCount=0,passPos=0,passEn=0;
const char pass[4]={'0','0','0','0'};
char pass_enter[4];
struct ts t; 
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'#','0','*','D'}
};
byte rowPins[ROWS] = {A0,A1,A2,A3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8,9,10,11}; //connect to the column pinouts of the keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

LiquidCrystal lcd(2,3,4,5,6,7);
char day[7][3]={"SUN","MON","TUE","WED","THU","FRI","SAT"};

void timer_1_Init(void)//initilize timer 1 for alarm on duration
{
  TCNT1=49910;//65535-one sec duration with 1024 preacalar(15625)
  TIMSK1=0x01;
  sei();
}

ISR(TIMER1_OVF_vect)
{
   Serial.print("In interrupt");

  if(alarmTimCount>0)
  {
    alarmTimCount--;
    TCNT1=48410;
    Serial.print(alarmTimCount);
  }
  else
  {
    alarmTimCount=0;
    digitalWrite(alarmOutput,LOW);
    TCCR1B=0x00; 
    cli();  
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(alarmOutput,OUTPUT);
  DS3231_init(DS3231_CONTROL_INTCN);
  lcd.begin(16, 2);
  lcd.clear();
 /* t.hour=8;
  t.min=39;
  t.sec=55;
  t.wday=0;
  DS3231_set(t);*/
  timer_1_Init();
  alarm_on=0;
}

void loop() {
  
  DS3231_get(&t);
  lcd.setCursor(0,0);
  lcd.print(day[t.wday][0]);
  lcd.print(day[t.wday][1]);
  lcd.print(day[t.wday][2]);
  lcd.print("   ");
  lcd.print(t.hour);
  lcd.print(":");
  lcd.print(t.min);
  lcd.print(":");
  lcd.print(t.sec);
  lcd.print("      ");
  lcd.setCursor(0,1);
  lcd.print("Mode : ");
  lcd.print(mode);
  
  //code to select alarm 
  
  if((t.wday==1 || t.wday==2 || t.wday==3 || t.wday==4 || t.wday==5) && mode==0)
  {
      if((p_alarm_1->hrs[0]==t.hour) && (p_alarm_1->mins[0]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=0;
      }
      
     
      if((p_alarm_1->hrs[1]==t.hour) && (p_alarm_1->mins[1]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=1;
      }
     
      
      if((p_alarm_1->hrs[2]==t.hour) && (p_alarm_1->mins[2]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=2;
      }
      
     
      if((p_alarm_1->hrs[3]==t.hour) && (p_alarm_1->mins[3]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=3;
      }
      
     
      if((p_alarm_1->hrs[4]==t.hour) && (p_alarm_1->mins[4]==t.min)&& (t.sec==1) )
      {
        alarm_on=1;
        alarm_count=4;
      }
      
     
      if((p_alarm_1->hrs[5]==t.hour) && (p_alarm_1->mins[5]==t.min)&& (t.sec==1))
      {
        alarm_on=1;
        alarm_count=5;
      }      
      
      if((p_alarm_1->hrs[6]==t.hour) && (p_alarm_1->mins[6]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=6;
      }
      
      if((p_alarm_1->hrs[7]==t.hour) && (p_alarm_1->mins[7]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=7;
      }  
     
      if((p_alarm_1->hrs[8]==t.hour) && (p_alarm_1->mins[8]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=8;
      }
      
     
      if((p_alarm_1->hrs[9]==t.hour) && (p_alarm_1->mins[9]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=9;
      }
     
      if((p_alarm_1->hrs[10]==t.hour) && (p_alarm_1->mins[10]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=10;
      }
      
      
      if((p_alarm_1->hrs[11]==t.hour) && (p_alarm_1->mins[11]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=11;
      }
      
     
      if((p_alarm_1->hrs[12]==t.hour) && (p_alarm_1->mins[12]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=12;
      }
      
     
      if((p_alarm_1->hrs[13]==t.hour) && (p_alarm_1->mins[13]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=13;
      }
      
     
      if((p_alarm_1->hrs[14]==t.hour) && (p_alarm_1->mins[14]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=14;
      }

      if((p_alarm_1->hrs[15]==t.hour) && (p_alarm_1->mins[15]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=15;
      }

      if((p_alarm_1->hrs[16]==t.hour) && (p_alarm_1->mins[16]==t.min) && (t.sec==1))
      {
        alarm_on=1;   
        alarm_count=16; 
      }
        
       if((p_alarm_1->hrs[17]==t.hour) && (p_alarm_1->mins[17]==t.min) && (t.sec==1))
       {
        alarm_on=1;
        alarm_count=17;
       }
  }
  if((t.wday==6) && mode==1)
  {
    if((p_alarm_2->hrs[0]==t.hour) && (p_alarm_2->mins[0]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=0;
      }
      
     
      if((p_alarm_2->hrs[1]==t.hour) && (p_alarm_2->mins[1]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=1;
      }
     
      
      if((p_alarm_2->hrs[2]==t.hour) && (p_alarm_2->mins[2]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=2;
      }
      
     
      if((p_alarm_2->hrs[3]==t.hour) && (p_alarm_2->mins[3]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=3;
      }
      
     
      if((p_alarm_2->hrs[4]==t.hour) && (p_alarm_2->mins[4]==t.min)&& (t.sec==1) )
      {
        alarm_on=1;
        alarm_count=4;
      }
      
     
      if((p_alarm_2->hrs[5]==t.hour) && (p_alarm_2->mins[5]==t.min)&& (t.sec==1))
      {
        alarm_on=1;
        alarm_count=5;
      }      
      
      if((p_alarm_2->hrs[6]==t.hour) && (p_alarm_2->mins[6]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=6;
      }
      
      if((p_alarm_2->hrs[7]==t.hour) && (p_alarm_2->mins[7]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=7;
      }  
     
      if((p_alarm_2->hrs[8]==t.hour) && (p_alarm_2->mins[8]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=8;
      }
      
     
      if((p_alarm_2->hrs[9]==t.hour) && (p_alarm_2->mins[9]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=9;
      }
     
      if((p_alarm_2->hrs[10]==t.hour) && (p_alarm_2->mins[10]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=10;
      }
      
      
      if((p_alarm_2->hrs[11]==t.hour) && (p_alarm_2->mins[11]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=11;
      }
      
     
      if((p_alarm_2->hrs[12]==t.hour) && (p_alarm_2->mins[12]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=12;
      }
      
     
      if((p_alarm_2->hrs[13]==t.hour) && (p_alarm_2->mins[13]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=13;
      }
      
     
      if((p_alarm_2->hrs[14]==t.hour) && (p_alarm_2->mins[14]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=14;
      }

      if((p_alarm_2->hrs[15]==t.hour) && (p_alarm_2->mins[15]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=15;
      }

      if((p_alarm_2->hrs[16]==t.hour) && (p_alarm_2->mins[16]==t.min) && (t.sec==1))
      {
        alarm_on=1;   
        alarm_count=16; 
      }
        
       if((p_alarm_2->hrs[17]==t.hour) && (p_alarm_2->mins[17]==t.min) && (t.sec==1))
       {
        alarm_on=1;
        alarm_count=17;
       }
  }
  
   if(mode==2)
  {
    if((p_alarm_3->hrs[0]==t.hour) && (p_alarm_3->mins[0]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=0;
      }
      
     
      if((p_alarm_3->hrs[1]==t.hour) && (p_alarm_3->mins[1]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=1;
      }
     
      
      if((p_alarm_3->hrs[2]==t.hour) && (p_alarm_3->mins[2]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=2;
      }
      
     
      if((p_alarm_3->hrs[3]==t.hour) && (p_alarm_3->mins[3]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=3;
      }
      
     
      if((p_alarm_3->hrs[4]==t.hour) && (p_alarm_3->mins[4]==t.min)&& (t.sec==1) )
      {
        alarm_on=1;
        alarm_count=4;
      }
      
     
      if((p_alarm_3->hrs[5]==t.hour) && (p_alarm_3->mins[5]==t.min)&& (t.sec==1))
      {
        alarm_on=1;
        alarm_count=5;
      }      
      
      if((p_alarm_3->hrs[6]==t.hour) && (p_alarm_3->mins[6]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=6;
      }
      
      if((p_alarm_3->hrs[7]==t.hour) && (p_alarm_3->mins[7]==t.min) && (t.sec==1))
      {
        alarm_on=1;
        alarm_count=7;
      }  
    }
  
  ///////////code to on alarm
  if(alarm_on==1)
  {
    if(mode==0)
    alarmTimCount=p_alarm_1->secs[alarm_count];
    if(mode==1)
    alarmTimCount=p_alarm_2->secs[alarm_count];
    if(mode==2)
    alarmTimCount=p_alarm_3->secs[alarm_count];
    //////////////////////////////
    TCNT1=49910;
    ////////////////////////////////
    TCCR1B=0x05;
    digitalWrite(alarmOutput,HIGH);
    alarm_on=0;
    if(alarm_count>=18)
     alarm_count=0;
    else
     alarm_count++; 
  }
  

  char key=kpd.getKey();// Read the key
  if (key){
    //Serial.print("Key Pressed : ");
   // Serial.println(key);
    
    if(key=='A')
     mode=0;
    if(key=='B')
     mode=1;
    if(key=='C')
     mode=2;
    if(key=='D')
     mode=3;
   }
   
   while(mode==3)
   {

    lcd.setCursor(0,1);
    lcd.print("Pass       ");
    char key=kpd.getKey();

     if(key)
      {
        passPos++;
         pass_enter[passPos-1]=key;
      }
      if(passPos==4)
       {
        passPos=0;
        for(i=0;i<4;i++)
        {
          if(pass[i]==pass_enter[i])
            passEn=1;
          else
          {
            mode=0;
            passEn=0;
           break; 
          }
        }
       }
       if(passEn==1)
       {
         lcd.setCursor(6,1);
         lcd.print("ok");
         lcd.clear();
         i=0;
         break;
       }
   }
   while(passEn==1)
   {
    
      char key=kpd.getKey();
      if(key=='#')
      {
        passEn=0;
        mode=0;
        break;
      }
      if(key=='*')
      {
        t.hour=((get_hour.time_byte.h_b*10)+ get_hour.time_byte.l_b);
        t.min=((get_min.time_byte.h_b*10)+ get_min.time_byte.l_b);
        t.sec=((get_sec.time_byte.h_b*10)+ get_sec.time_byte.l_b);
        if(t.hour>23)
         t.hour=23;
        if(t.min>60)
         t.min=60;
        if(t.sec>60)
         t.sec=60;  
        DS3231_set(t);
        passEn=0;
        mode=0;
       
      }
      
      if(key)
      {
        if(key!='#' || key!='#')
        {
         if(i==0)
          get_hour.time_byte.h_b=(int)(key-48);
         if(i==1)
          get_hour.time_byte.l_b=(int)(key-48);
         if(i==2)
          get_min.time_byte.h_b=(int)(key-48);
         if(i==3)
          get_min.time_byte.l_b=(int)(key-48);
         if(i==4)
          get_sec.time_byte.h_b=(int)(key-48);
         if(i==5)
          get_sec.time_byte.l_b=(int)(key-48);
         if(i==6)
          t.wday=(int)(key-48); 
          if(i<6)
           i++;
        }
       }
     
       // Serial.print(get_hour.h_b);
      //  Serial.print(get_hour.l_b);
     
      lcd.setCursor(0,0);
      lcd.print((get_hour.time_byte.h_b*10)+ get_hour.time_byte.l_b);
      lcd.print(":");
      lcd.setCursor(5,0);
      lcd.print((get_min.time_byte.h_b*10)+ get_min.time_byte.l_b);
      lcd.print(":");
      lcd.setCursor(9,0);
      lcd.print((get_sec.time_byte.h_b*10)+ get_sec.time_byte.l_b);
      lcd.setCursor(1,1);
      lcd.print(day[t.wday][0]);
      lcd.print(day[t.wday][1]);
      lcd.print(day[t.wday][2]);
      }
  
}
