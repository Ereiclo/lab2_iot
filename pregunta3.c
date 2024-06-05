
#define READING 0
#define WATERING 1
#define IDLE 2

#define WATERING_TIME 3000


int STATE = READING;
int TIMEOUT_END = 0;
int WATERING_END = 0;

float humidity = 0;

#define BLUE_PIN 2
#define RED_PIN 3
#define PIN_BUZZER 9
#define PIN_HUMEDAD A0


void setup()
{
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_HUMEDAD, INPUT);
}


void wateringState() {
  //turn on blue light
  //change to IDLE after wait
  digitalWrite(BLUE_PIN, LOW);  
  if (millis() > WATERING_END) {
    STATE = IDLE;
    digitalWrite(BLUE_PIN, HIGH);  
  }
}

void idleState() {
  // wait until timeout
  if (millis() > TIMEOUT_END) {
    STATE = READING;
	}
}

void readingState(){
  if(humidity < 20){
    STATE = WATERING;
    WATERING_END = millis() + WATERING_TIME;
  } 
}


void handleState (){
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
  if(humidity > 80.) {
    digitalWrite(RED_PIN, LOW);
    tone(PIN_BUZZER, 1000); // Send 1KHz sound signal...
  }
  else {
    digitalWrite(RED_PIN, HIGH);
    noTone(PIN_BUZZER);
  }
}


void readHumidity() {
  humidity = (analogRead(PIN_HUMEDAD)*100.0/1023.0);
}

void loop()
{
  readHumidity();
  handleWarning();
  handleState();
}
