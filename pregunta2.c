#include <Keypad.h>
#include <LiquidCrystal.h>

#define TRIGGER 25
#define ECHO 23
#define CONTRA "3756"
#define RED 5
#define GREEN 6
#define LED_A 31
#define LED_R 35
#define LED_V 33
#define CONTRAST_PIN 7
// estados
#define DEFAULT_STATE 10
#define INSERT_PASS 0
#define CORRECT_PASS 1
#define WRONG_PASS 2
#define INVALID_INPUT 3
#define CONTRAST 25

const int ROW_NUM = 4;
const int COLUMN_NUM = 4;
//
//
//
//

int state = 0;
int writeRequired = 1;
int globalOn = 0;

char keys[ROW_NUM][COLUMN_NUM] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte pin_rows[ROW_NUM] = {39, 41, 43, 45};
byte pin_column[COLUMN_NUM] = {47, 49, 51, 53};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

const int LIGHTS[] = {0, 1, 2, 3, 4};

// Time tracking
unsigned long startTime;
unsigned long nextTime = 2000;

// Keypad
// char inputString[10];
String inputString = "";
char inputStringIter = 0;

const int rs = 12, en = 11, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
    Serial.begin(9600);
    startTime = millis();
    pinMode(TRIGGER,
            OUTPUT);      // Sets the trigPin as an OUTPUT
    pinMode(ECHO, INPUT); // Sets the echoPin as an INPUT

    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    lcd.clear();
    // Print a message to the LCD.

    pinMode(LED_A, OUTPUT);
    pinMode(LED_R, OUTPUT);
    pinMode(LED_V, OUTPUT);
    pinMode(CONTRAST_PIN, OUTPUT);
    analogWrite(CONTRAST_PIN,  CONTRAST);
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    digitalWrite(LED_A, HIGH);
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_V, HIGH);
}

void readKeypadInput()
{
    // GetKey
    char key = keypad.getKey();
    if (key)
    {
        Serial.println(key);
    }
    else
    {

        return;
    }
    // NumberInput
    char currentInputChar = key;
    if (key - '0' >= 0 && key - '0' <= 9)
    {
        inputString += (char)key;
        Serial.println(inputString);
        // inputStringIter++;
    }
    // RegisterInput
    else if (key == '#')
    {
        Serial.println(inputString);
        int len = inputString.length();

        if (len == 0 || len > 4 || inputString != CONTRA)
        {
            state = WRONG_PASS;
            writeRequired = 1;
        }
        else
        {
            state = CORRECT_PASS;
            writeRequired = 1;
        }

        inputString = "";
    }
    // ResetInput
    else
    {
        inputString = "";
        state = INVALID_INPUT;
        writeRequired = 1;
    }
}

bool isClose()
{
    int distance;
    int duration;
    digitalWrite(TRIGGER, LOW);
    delayMicroseconds(2);

    digitalWrite(
        TRIGGER,
        HIGH);
    delayMicroseconds(
        10);

    digitalWrite(TRIGGER,
                 LOW);

    duration = pulseIn(ECHO, HIGH);
    distance = duration * 0.0344 / 2;

    Serial.print("Distance: ");
    Serial.print(
        distance);
    Serial.println(" cm");

    return distance <= 30.f;
}

void display_smth()
{
  if (writeRequired == 1){
    switch (state)
    {
    case CORRECT_PASS:
      lcd.clear();
      lcd.print("PASA");
      break;
    case INSERT_PASS:
      lcd.clear();
      lcd.print("DIGITA:");
      Serial.print("Distance: ");
      break;
    default:
      lcd.clear();
      lcd.print("ERROR");
      break;
    }
    writeRequired= 0;
  }
}

void display_off()
{
    lcd.clear();
}

void handleLeds()
{

    digitalWrite(LED_A, HIGH);
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_V, HIGH);

    if (globalOn)
    {
        digitalWrite(LED_A, LOW);
        switch (state)
        {
        case CORRECT_PASS:
            digitalWrite(LED_V, LOW);
            break;
        case INVALID_INPUT:
        case WRONG_PASS:
            digitalWrite(LED_R, LOW);
        }
    }
}

void loop()
{
   lcd.setCursor(0, 1);
    

    globalOn = isClose();

    if (globalOn)
    {
        readKeypadInput();
        display_smth();
    }
    else
    {
        state = INSERT_PASS;
        writeRequired = 1;
        display_off();
    }

    handleLeds();
}
