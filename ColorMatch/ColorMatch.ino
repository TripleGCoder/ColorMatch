#include <Keypad.h>

const byte ROW = 2;
const byte COL = 4;

const byte Round1_Length = 5;
const byte Round2_Length = 7;
const byte Round3_Length = 9;


char current_round = 1;

// Define what event will be returned by each button
const char BUTTONS[ROW][COL] = {
  { 'R', 'G', 'Y', 'B' },
  { 'S', '0', '0', 'X' }
};

// Define row and column pins connected to the keypad
const byte ROW_PINS[ROW] = { 5, 4 };
const byte COL_PINS[COL] = { 6, 7, 8, 9 };

Keypad heroKeypad = Keypad(makeKeymap(BUTTONS), ROW_PINS, COL_PINS, ROW, COL);

//LED SETUP
#define RED_LED 13
#define GREEN_LED 12
#define YELLOW_LED 11
#define BLUE_LED 10

//  GAME SETUP
// const byte MAX_PATTERNS = 9;      //Defines the size of the array
// int currentPattern[MAX_PATTERNS]; //Declares the array
// char const LEDs[] = {'R\n', 'G\n', 'Y\n', 'B\n'};
// byte currentIndex = 0; 


enum gameSTATE{
  START,
  FLASH_LEDS,
  UPDATE_PTRN,
  DISPLAY_PTRN,
  AWAIT_RES,
  CHECK_RES,
  UPDATE_GAME,
  GAME_WON,
  GAME_LOST,
  DEBUG
};
unsigned char currentState = START;

// FSM shared control variables
bool allSync = false;
int flashGoal = 0;
int msDelay = 150;

bool enRedLED = false;
bool enGreenLED = false;
bool enYellowLED = false;
bool enBlueLED = false;


void setup() {
  //LED PINMODE Setup
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  
  Serial.begin(9600);  // Begin monitoring via the serial monitor
  delay(200);          // Delay a brief period to let things settle before displaying prompt.
  Serial.println("Press any button");

}

void flash_leds(bool Red_LED, bool Green_LED, bool Blue_LED, bool Yellow_LED, int flashGoal)
{
  delay(msDelay*2);
  for (int i = 0; i < flashGoal; i++)
    {
      if (Red_LED)
        {
          digitalWrite(RED_LED, HIGH);
        }
      if (Green_LED)
        {
          digitalWrite(GREEN_LED, HIGH);
        }
      if (Yellow_LED)
        {
          digitalWrite(YELLOW_LED, HIGH);
        }
      if (Blue_LED)
        {
          digitalWrite(BLUE_LED, HIGH);
        }

      delay(msDelay);

      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      delay(msDelay);
    }
  
}

void loop() {

//Switch will read what currentState is, then it will go to its designated case situation
  switch (currentState) {
//---------------------------START------------------------------------|

  case START: {
    //NOTHING WORKS UNTIL START IS PRESSED
    char pressedButton = heroKeypad.getKey();  // Wait until a button is pressed
    if (pressedButton)
      {
        Serial.println(pressedButton);
      }
    if (pressedButton == BUTTONS[1][0])
      {
        allSync = true;
        flashGoal = 7;
        currentState = FLASH_LEDS;
      }

    break;
  }

//--------------------------------------------------------------------|
//------------------------FLASH LEDs----------------------------------|
  case FLASH_LEDS: {
    //This state takes in the input of Decide which LED should flash (allSync || R,G,Y,B), the number of flashes it should do (flashGoal), and the speed of the flsahing (flashSPeed)
    
    if (allSync)
      {
        enRedLED = true;
        enGreenLED = true;
        enYellowLED = true;
        enBlueLED = true;
      }
    
    
    flash_leds(enRedLED, enGreenLED, enYellowLED, enBlueLED, flashGoal);
    
    currentState = DEBUG;

    break;
  }

//--------------------------------------------------------------------|
//------------------------UPDATE PTRN----------------------------------|
  case UPDATE_PTRN: {

    break;
  }

//--------------------------------------------------------------------|
//------------------------DISPLAY PTRN----------------------------------|
  case DISPLAY_PTRN: {

    break;
  }

//--------------------------------------------------------------------|
//------------------------AWAIT RES----------------------------------|
  case AWAIT_RES: {

    break;
  }

//--------------------------------------------------------------------|
//------------------------CHECK RES----------------------------------|
  case CHECK_RES: {

    break;
  }

//--------------------------------------------------------------------|
//------------------------UPDATE_GAME----------------------------------|
  case UPDATE_GAME: {

    break;
  }

//--------------------------------------------------------------------|
//------------------------GAME WON----------------------------------|
  case GAME_WON: {
    
    break;
  }

//--------------------------------------------------------------------|
//------------------------GAME LOST----------------------------------|
  case GAME_LOST: {

      break;
    }

//--------------------------------------------------------------------|
//-----------------------Debugger Mode--------------------------------|
  case DEBUG: {
    char pressedButton = heroKeypad.getKey();  // Wait until a button is pressed
    //RED LED
    if (heroKeypad.getState() == PRESSED || heroKeypad.getState() == HOLD) {
        if (pressedButton == BUTTONS[0][0]) {
            digitalWrite(RED_LED, HIGH);
        }
    } 
    else if (heroKeypad.getState() == RELEASED) {
        digitalWrite(RED_LED, LOW);
    }

    //GREEN LED
    if (heroKeypad.getState() == PRESSED || heroKeypad.getState() == HOLD) {
        if (pressedButton == BUTTONS[0][1]) {
            digitalWrite(GREEN_LED, HIGH);
        }
    } 
    else if (heroKeypad.getState() == RELEASED) {
        digitalWrite(GREEN_LED, LOW);
    }

    //YELLOw LED
    if (heroKeypad.getState() == PRESSED || heroKeypad.getState() == HOLD) {
        if (pressedButton == BUTTONS[0][2]) {
            digitalWrite(YELLOW_LED, HIGH);
        }
    } 
    else if (heroKeypad.getState() == RELEASED) {
        digitalWrite(YELLOW_LED, LOW);
    }

    //YELLO LED
    if (heroKeypad.getState() == PRESSED || heroKeypad.getState() == HOLD) {
        if (pressedButton == BUTTONS[0][3]) {
            digitalWrite(BLUE_LED, HIGH);
        }
    } 
    else if (heroKeypad.getState() == RELEASED) {
        digitalWrite(BLUE_LED, LOW);
    }








    //RESETS THE GAME
    if (pressedButton == BUTTONS[1][3]){
      currentState = START;
    }


    
      break;
    }
}







  // put your main code here, to run repeatedly:

  // char newPattern = random.random(LEDs);


//This will appened a newPattern into currentPattern that the player must repeat back
  // if (currentIndex < MAX_PATTERNS) {
  //   currentPattern[currentIndex] = newPattern
  //   currentIndex++;
  // }










    //-------------------------------------------------------------------------------------------
    //---------------------------- LED SYSTEM ---------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
 

                   




}
