#include <avr/io.h>
#include <avr/interrupt.h> 
int sensorpin = 0;
int alarpin = 8;
int val = 0;
int sensitivity = 20;
int pastval;
int data;
int reading_count;
int diff;
void setup() {
 pinMode(alarpin,OUTPUT);
 Serial.begin(9600);
  data = 0;
  diff = 0;
  cli();
  
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A = 781;
  TCCR1B |= (1 << WGM12); 
  TCCR1B|= (1<< CS10); 
  TCCR1B|= (1<< CS12);
  TIMSK1 = (1<< OCIE1A);
  
  sei();
}

void loop() {
  if((diff <= -sensitivity) || (diff>= sensitivity)){
    ringTheAlarm();
  }
  Serial.println(diff);
}

void ringTheAlarm(){
  tone(alarpin,4978,1000);
  delay(300);
  noTone(alarpin);
  delay(1000);
}

ISR(TIMER1_COMPA_vect)
{
    data += analogRead(sensorpin);  
    ++reading_count;

    if(reading_count >= 10){
      reading_count = 0;
      pastval = val;
      val = data /10 ; 
      data = 0;
      diff = val - pastval ;
    }
} 
