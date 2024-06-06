
#define READING 0
#define WATERING 1
#define IDLE 2

#define WATERING_TIME 3000
#define IDLE_TIME 3000
#include <LiquidCrystal.h>

int STATE = READING;
long TIMEOUT_END = 0;
long WATERING_END = 0;

float humidity = 0;

#define BLUE_PIN 2
#define RED_PIN 3
#define PIN_MOTOR 44
#define PIN_BUZZER 9
#define PIN_HUMEDAD A0


const int rs = 33, en = 31, d4 = 29, d5 = 27, d6 = 25, d7 = 23;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_HUMEDAD, INPUT);
  pinMode(PIN_MOTOR, OUTPUT);

  digitalWrite(BLUE_PIN, HIGH);
  digitalWrite(RED_PIN, HIGH);
	noTone(PIN_BUZZER);
  digitalWrite(PIN_MOTOR, HIGH);
}


void wateringState() {
  //turn on blue light
  //change to IDLE after wait
  digitalWrite(BLUE_PIN, LOW);  
  digitalWrite(PIN_MOTOR, LOW);  
  Serial.println(millis());
  Serial.println(WATERING_END);
  if (millis() > WATERING_END) {
    STATE = IDLE;
    TIMEOUT_END =millis()+IDLE_TIME;
    digitalWrite(BLUE_PIN, HIGH);
    digitalWrite(PIN_MOTOR, HIGH);  
  }
  lcd.clear(); 
  lcd.print(humidity);
}

void idleState() {
  // wait until timeout
  if (millis() > TIMEOUT_END) {
    STATE = READING;
    
	}
  lcd.clear();
  lcd.print("IDLE");
}

void readingState(){
  if(humidity < 10.f){
    STATE = WATERING;
    WATERING_END = millis() + WATERING_TIME;
  } 
  lcd.clear(); 
  lcd.print(humidity);
}


void handleState (){
  Serial.println(STATE);

  switch(STATE){
    case READING:
      readingState();
  
      break;
    case WATERING:
      wateringState();

      break;
  	case(IDLE):
      idleState();

      break;
  }
}




void handleWarning() {
  if(humidity > 50.) {
    digitalWrite(RED_PIN, LOW);
    tone(PIN_BUZZER, 1000); // Send 1KHz sound signal...
  }
  else {
    digitalWrite(RED_PIN, HIGH);
    noTone(PIN_BUZZER);
  }
}


void readHumidity() {
  float rawHumidity= analogRead(PIN_HUMEDAD);
  humidity = 100.0- (rawHumidity*100.0/1023.0);

  //Serial.println(humidity);

  
  
  
}

void loop()
{
  readHumidity();
  handleWarning();
  handleState();
}
