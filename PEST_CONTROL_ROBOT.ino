//-----------------------------------------------------------------------------------------------------------------------------------------------------------------//
//                                                     AGV ROBOT For Desinfaction Software Created On 8/15/2023 from scratch by Sam Kodo                           //                          
//                                                                                                                                                                 //
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------//
#include <LcdBarGraphRobojax.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Note to be played at startUp of the Robot

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

// change this to make the song slower or faster
int tempo = 105;

// change this to whichever pin you want to use
int buzzer = 12;

//Water level Detector for the sprayer
int waterSensor = A3 ; 
int valueSensor ; 
int calibrationValue = 500 ;  


// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  // Pacman
  // Score available at https://musescore.com/user/85429/scores/107109
  NOTE_B4, 16, NOTE_B5, 16, NOTE_FS5, 16, NOTE_DS5, 16, //1
  NOTE_B5, 32, NOTE_FS5, -16, NOTE_DS5, 8, NOTE_C5, 16,
  NOTE_C6, 16, NOTE_G6, 16, NOTE_E6, 16, NOTE_C6, 32, NOTE_G6, -16, NOTE_E6, 8,

  NOTE_B4, 16,  NOTE_B5, 16,  NOTE_FS5, 16,   NOTE_DS5, 16,  NOTE_B5, 32,  //2
  NOTE_FS5, -16, NOTE_DS5, 8,  NOTE_DS5, 32, NOTE_E5, 32,  NOTE_F5, 32,
  NOTE_F5, 32,  NOTE_FS5, 32,  NOTE_G5, 32,  NOTE_G5, 32, NOTE_GS5, 32,  NOTE_A5, 16, NOTE_B5, 8
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
   
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Control of Motor A
int motora_1 = 4;
int motora_2 = 5;

//Control of Motor B 
int motorb_1 = 2;
int motorb_2 = 3;

//Activating Speed Port on ENA and ENB
int speeda = 10 ;
int speedb = 9 ; 
char data ; 

// Activating WaterPump Motor for Spray
int motorc_1 = 6 ; 
int motorc_2 = 7 ; 
// Reading Voltage from power divider
int voltageV  = A0;   // it is the input pin

// Storing Value to change speed of the robot
int speedValuea = 230;  
int speedValueb = 230; 

boolean dataStatus = false ; // Check data from blueooth Module
boolean batteryStatus = false ; // Check state of the battery

void setup()

 {
 pinMode (voltageV, INPUT);
  lcd.init();
  lcd.backlight();

// iterate over the notes of the melody.
 for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }

// -- initializing the relay
 
 /*pinMode(relaypower , OUTPUT) ; 
 digitalWrite(relaypower , HIGH) ; */

  // Ports setup as OutPut on the board for the motion motors control 
  pinMode(motora_1, OUTPUT);
  pinMode(motora_2, OUTPUT);
  pinMode(motorb_1,   OUTPUT);
  pinMode(motorb_2, OUTPUT);
  pinMode(waterSensor , INPUT) ; 

  // Ports setup as OutPut for spray motor
  pinMode(motorc_1, OUTPUT) ;
  pinMode(motorc_2, OUTPUT) ;

  //Enable Speed Control of Motors on ENA and ENB on Pin 9 and Pin 10
  pinMode(speeda,   OUTPUT); 
  pinMode(speedb, OUTPUT);
  Serial.begin (9600) ;
  delay(100) ;
 

}

void loop() {

valueSensor = analogRead(waterSensor) ; 

Serial.print("Sensor Value") ; 
Serial.println(valueSensor) ;


int VoltageMsmt = analogRead(voltageV);
  float Vcalc = (5.0/1023)*VoltageMsmt;
  Vcalc = Vcalc*7.14;
  lcd.setCursor(0,0);
  lcd.print("BATTERIE:");
  lcd.print(Vcalc);

// function to protect internal voltage of batteries

if (Vcalc < 12.0 )
{

lcd.setCursor(0,1) ; 
lcd.print("BATTERIE: ALERTE") ;
tone (buzzer , 500 , 1050) ; 
delay(500) ;  

}

else {
lcd.setCursor(0,1) ; 
lcd.print("BATTERIE: BONNE") ;

}




while (Serial.available())   { 
      { 
        data = Serial.read();
        Serial.println(data);
  
        }

      switch (data)
       {
          case 'F':                               
            //Serial.println("Forward");
            forward();
            break;
          case 'B':                
           //Serial.println("Reverse");
            reverse();
            break;
          case 'L':        
           //Serial.println("Left");
            left();
            break;   
          case 'R':                    
            //Serial.println("Right");
            right();
            break;           
          case 'S':                                           
            //Serial.println("Stop");
            stoprobot();
            break;   
            //Serial.println("Spray") ;   
            case 'K':
            dataStatus = true ; 
            break; 
            //Serial.println("StopSpraying") ;
            case 'k':
            dataStatus = false ; 
            break;

          }
      }                                                             
       if (dataStatus && valueSensor>calibrationValue )                         
        
          {
              digitalWrite(motorc_1, LOW) ; 
              digitalWrite(motorc_2, HIGH) ; 
              delay(5) ;
          }
            else {
                    digitalWrite(motorc_1, LOW) ; 
                    digitalWrite(motorc_2, LOW) ; 
                    delay(5) ;

                }

          }

// function to move the Robot Forward
void forward() {       
 
   
      analogWrite(speeda , speedValuea);
      analogWrite(speedb,  speedValueb) ; 
           
     digitalWrite(motora_1, HIGH);
    digitalWrite(motora_2, LOW);
   digitalWrite(motorb_1, HIGH);
  digitalWrite(motorb_2, LOW);
  delay(5) ;

}

// function to move the Robot Backward
void reverse() {
      analogWrite(speeda , speedValuea);
      analogWrite(speedb,  speedValueb) ; 
           
     digitalWrite(motora_1, LOW);
    digitalWrite(motora_2, HIGH);
   digitalWrite(motorb_1, LOW);
  digitalWrite(motorb_2, HIGH);
  delay(5) ;

}
//function to turn on the Left
void left() {
 analogWrite(speeda , speedValuea);
  analogWrite(speedb,  speedValueb) ; 
           
     digitalWrite(motora_1, HIGH);
    digitalWrite(motora_2, LOW);
   digitalWrite(motorb_1, LOW);
  digitalWrite(motorb_2, HIGH);
 delay(5) ;
}
//function to turn on the right 
void right() {
  analogWrite(speeda , speedValuea);
      analogWrite(speedb,  speedValueb) ; 
           
     digitalWrite(motora_1, LOW);
    digitalWrite(motora_2, HIGH);
   digitalWrite(motorb_1, HIGH);
  digitalWrite(motorb_2, LOW);
delay(5) ;
}
// Function to Stop all the motor 

void stoprobot() {

  
     digitalWrite(motora_1, LOW);
    digitalWrite(motora_2, LOW);
   digitalWrite(motorb_1, LOW);
  digitalWrite(motorb_2, LOW);
  delay(5) ;
 
}



