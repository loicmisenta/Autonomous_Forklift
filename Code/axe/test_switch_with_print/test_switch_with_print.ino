/*
  [!] [!] [!] THIS CODE WAS NOT ORIGINALY CREATED BY US, IT IS ONLY HERE TO HAVE A COMMON REFERENCE FOR ALL THE TEAM MEMBERS [!] [!] [!]

    ARDUINO LIMIT SWITCH TUTORIAL: NORMALLY CLOSED
    By: TheGeekPub.com
    More Arduino Tutorials: https://www.thegeekpub.com/arduino-tutorials/
*/
 
#define LIMIT_SWITCH_PIN D0
 
void setup() {
  Serial.begin(9600);
  pinMode(LIMIT_SWITCH_PIN, INPUT);
}
 
void loop() {
 
  if (digitalRead(LIMIT_SWITCH_PIN) == HIGH)
  {
    Serial.println("Activated!");
  }
 
  else
  {
    Serial.println("Not activated.");
  }
   
  delay(100);
}