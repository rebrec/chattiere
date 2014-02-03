
int nb_erreur = 0;	
 
void setup()
{
  Serial.begin(9600);
  pinMode(10,INPUT);
  pinMode(13,OUTPUT);

}

void loop()
{
  static int a = 0; 
  a=digitalRead(10);
  
  if (a != HIGH) 
  {
  	nb_erreur++;
  }
  digitalWrite(13,a);
  Serial.println(nb_erreur);
}

