

#define PIN_Trigger1 2 
#define PIN_Trigger2 3 
#define TEMPO 15 // secondes

#define ETAT_FC_TRAPPEOUVERTE LOW
#define ETAT_FC_TRAPPEFERMEE LOW

#define PIN_FC_HAUT 4
#define PIN_FC_BAS 5

#define PIN_DIRECTION 12
#define PIN_PWM 3
#define PIN_BRAKE 9

#define SPEED_MONTE 255
#define SPEED_DESCEND 255
#define DELAY_CHECK 10  // Interval de vérification de l'état des Fin de Course
#define DELAY_OUVERTURE 5000
#define DELAY_REARMEMENT 2000

volatile byte cnt = 0;
volatile byte COUNTDOWN_ENABLED = 0;


void setup()
{
  // initialisation du timer1
  cli();
  TCCR1A=0;
  TCCR1B=0;
  TCNT1=0;
  
  OCR1A = 15624;  //compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 1024 prescaler
  TCCR1B |= (1 << CS10);    // 1024 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  sei();  
  //////////////////////////////
  pinMode(PIN_Trigger1, INPUT_PULLUP);
  pinMode(PIN_Trigger2, INPUT_PULLUP);
  attachInterrupt(0, startCountDown, FALLING);
  attachInterrupt(1, startCountDown, FALLING);

  Serial.begin(9600);
//  while(1){} 
  pinMode(PIN_DIRECTION, OUTPUT);
  pinMode(PIN_PWM, OUTPUT);
 
  pinMode(PIN_BRAKE, OUTPUT);

  pinMode(PIN_FC_HAUT, INPUT_PULLUP);
  pinMode(PIN_FC_BAS, INPUT_PULLUP);
  analogWrite(PIN_PWM, SPEED_MONTE);
  
}

void loop()
{
  int B = trapeFermee();
  int H = trapeOuverte();
  int T = COUNTDOWN_ENABLED;
  
  int dir = (T && (!H));
  int mot = ((!(T || B)) || dir);
/*  Serial.print("B : ");
  Serial.println(B);
  Serial.print("H : ");
  Serial.println(H);
  Serial.print("T : ");
  Serial.println(T);
  Serial.print("dir : ");
  Serial.println(dir);
  Serial.print("mot : ");
  Serial.println(mot);
// Detection Presence manquante...
  delay(1000);

*/
  digitalWrite(PIN_DIRECTION, dir);
  digitalWrite(PIN_BRAKE, !mot);


//  ouvreTrappe();
//  fermeTrappe();

}

void stop()
{
  digitalWrite(PIN_BRAKE, HIGH);
}

void ouvreTrappe()
{
  if (!trapeOuverte())
  {
    monte();
  }
  while (!trapeOuverte())
  {
    delay(DELAY_CHECK);  
  }
  stop();
}

void fermeTrappe()
{
  if (!trapeFermee())
  {
    descend();
  }
  while (!trapeFermee())
  {
    delay(DELAY_CHECK);  
  }
  stop();
}


int trapeOuverte()
{
  //Serial.println("trappe ouverte ? ");
  return digitalRead(PIN_FC_HAUT) == ETAT_FC_TRAPPEOUVERTE ; 
}

int trapeFermee()
{
  //Serial.println("trappe fermee ? ");
  return digitalRead(PIN_FC_BAS) == ETAT_FC_TRAPPEFERMEE ; 
}
void monte()
{
  Serial.println("MONTE !");
  analogWrite(PIN_PWM, SPEED_MONTE);
  digitalWrite(PIN_DIRECTION, HIGH);
  digitalWrite(PIN_BRAKE, LOW);
}
void descend()
{
  Serial.println("DESCEND !");
  analogWrite(PIN_PWM, SPEED_DESCEND);
  digitalWrite(PIN_DIRECTION, LOW);
  digitalWrite(PIN_BRAKE, LOW);
}
void bloque()
{
  Serial.println("BLOQUE !");

  digitalWrite(PIN_BRAKE, HIGH);
}

void startCountDown()
{
   COUNTDOWN_ENABLED = 1;
   cnt = TEMPO;
}

ISR(TIMER1_COMPA_vect)
{
  cnt--;
  if (cnt == 0)
  {
    COUNTDOWN_ENABLED = 0;
  }
}
