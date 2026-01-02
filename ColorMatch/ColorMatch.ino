#include <Keypad.h>

const byte ROW = 2;
const byte COL = 4;


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

//  GAME PATTERN SETUP
const byte MAX_PATTERN = 9;      //Defines the size of the array
char currentPattern[MAX_PATTERN]; //Declares the array
byte patternLen = 0;              //len of current pattern
byte startLen = 0;
char nextLED;

//Max Pattern Lenght per level
const byte MaxL_lvl1 = 5;
const byte MaxL_lvl2 = 7;
const byte MaxL_lvl3 = MAX_PATTERN;
byte levelLength = 0; //This will dictate the current Levels max length

//Minimum starting Lenght per Level
const byte MinL_lvl1 = 0;
const byte MinL_lvl2 = 3;
const byte MinL_lvl3 = 4;


char const LEDs[] = {'R\n', 'G\n', 'Y\n', 'B\n'};
byte currentIndex = 0; 


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

//Round variables
int roundNum = 0;
int gamelvl = 1;

bool enRedLED = false;
bool enGreenLED = false;
bool enYellowLED = false;
bool enBlueLED = false;







void flash_leds()
{
  if (allSync)
    {
      enRedLED = true;
      enGreenLED = true;
      enYellowLED = true;
      enBlueLED = true;
    }

  delay(msDelay*2);
  for (int i = 0; i < flashGoal; i++)
    {
      if (enRedLED)
        {
          digitalWrite(RED_LED, HIGH);
        }
      if (enGreenLED)
        {
          digitalWrite(GREEN_LED, HIGH);
        }
      if (enYellowLED)
        {
          digitalWrite(YELLOW_LED, HIGH);
        }
      if (enBlueLED)
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
    //reset LEDs
    allSync = false;
    enRedLED = false;
    enGreenLED = false;
    enYellowLED = false;
    enBlueLED = false;

  
}

void setup() {
  //LED PINMODE Setup
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  randomSeed(analogRead(A0));  // A0 should be floating (not connected)

  
  Serial.begin(9600);  // Begin monitoring via the serial monitor
  delay(200);          // Delay a brief period to let things settle before displaying prompt.
  Serial.println("Press any button");

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
        currentState = UPDATE_PTRN;
        flash_leds();
      }



    break;
  }

//--------------------------------------------------------------------|
//------------------------FLASH LEDs----------------------------------|
//redacted
//--------------------------------------------------------------------|
//------------------------UPDATE PTRN----------------------------------|

//Des: Will randomly add the next light to the current LED pattern 
//In LEVEL 1, Start with empty list and start the pattern from 1 -> 5 patterns 
//In LEVEL 2, Start with 3 light patern then 3->8
//In LEVEL 3, Start with 4 light pattern then from 4 -> 9 

  case UPDATE_PTRN: {
    
    if ((roundNum == 0))
      {
        patternLen = 0; //moves the cursor back to the beggining to start overwriting "clear" the pattern
        //GAME PATTERN INITIALIZATION FOR LEVEL 1
        if (gamelvl == 1)
          {
            startLen = MinL_lvl1;
            levelLength = MaxL_lvl1;
          }

        //GAME PATTERN INITIALIZATION FOR LEVEL 2
        if (gamelvl == 2)
          {
            startLen = MinL_lvl2;
            levelLength =  MaxL_lvl2;            
          }

        //GAME PATTERN INITIALIZATION FOR LEVEL 3
        if (gamelvl == 3)
          {
            startLen = MinL_lvl3;
            levelLength = MaxL_lvl3;
          }
    

        //Begin Prefilling the pattern for the level
        for (byte i = 0; i < startLen; i++)
          {
            byte randLED_index =  random(0,4);  //Using the random seeding choses a "random" number from 1-4
            //assign the color index to its color
            if (randLED_index == 0)
              {
                nextLED = 'R';
              }
            else if (randLED_index == 1)
              {
                nextLED = 'G';
              }
            else if (randLED_index == 2)
              {
                nextLED = 'Y';
              }
            else
              {
                nextLED = 'B';
              }

            currentPattern[patternLen] = nextLED;
            patternLen++;
          }
      }





      //Adding the next light to the pattern
      if (patternLen <  levelLength)
        {
          byte randLED_index = random(0,4);
          if (randLED_index == 0)
              {
                nextLED = 'R';
              }
            else if (randLED_index == 1)
              {
                nextLED = 'G';
              }
            else if (randLED_index == 2)
              {
                nextLED = 'Y';
              }
            else
              {
                nextLED = 'B';
              }

            currentPattern[patternLen] = nextLED;
            patternLen++;
        }






        
    // Debug print pattern
    Serial.print("Pattern: ");
    roundNum++;
    for (byte i = 0; i < patternLen; i++) 
    {
      Serial.print(currentPattern[i]);
      Serial.print(' ');
    }
    Serial.println();

    currentState = DISPLAY_PTRN;

    break;
  }
  

//--------------------------------------------------------------------|
//------------------------DISPLAY PTRN----------------------------------|
  case DISPLAY_PTRN: {
    delay(2000);
    for (byte i = 0; i < patternLen; i++)
    {
      switch(currentPattern[i]){

      case 'R':
      {
        enRedLED = true;
        flashGoal = 1;
        flash_leds();
        break;
      }
      
      case 'G':
      {
        enGreenLED = true;
        flashGoal = 1;
        flash_leds();
        break;
      }

      case 'Y':
      {
        enYellowLED = true;
        flashGoal = 1;
        flash_leds();
        break;
      }

      case 'B':
      {
        enBlueLED = true;
        flashGoal = 1;
        flash_leds();
        break;
      }
      default:
      {
        Serial.println("Ligth Pattern Error");
        break;
      }
      }
    }
    currentState = DEBUG;
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
