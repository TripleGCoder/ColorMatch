#include <Keypad.h>

/** 4x4 Keypad:
y1: x  x  x  x
y2  x  x  x  x
y3  x  x  x  x
y4  x  x  x  x
   x1  x2 x3 x4
**/

//Var for size of the array for ROW and COL
const byte ROW = 2; //Y#'s
const byte COL = 4; // X#'s

//define the corresponding pins that im using from the arduino to the buttons col and rows. 
const byte ROW_PINS[ROW] = { 5, 4 };
const byte COL_PINS[COL] = { 6, 7, 8, 9 };

//Define the active buttons with their corresponding event output
const char BUTTONS[ROW][COL] = {
  { 'R', 'G', 'Y', 'B' },
  { 'S', '0', '0', 'X' }
};

//Encode/map the Events, Arduino Pin Selection and Row/Col's onto the keypad with the instance name heroKeypad
Keypad heroKeypad = Keypad(makeKeymap(BUTTONS), ROW_PINS, COL_PINS, ROW, COL);
/**Current 4x4 Keypad being used (- = Inactive)
    R G Y B
    S 0 0 X
    - - - -
    - - - -
**/

/** ---------LED SETUP------------------*/

//Encode LED's to corresponding Arduino Pins
#define RED_LED 13
#define GREEN_LED 12
#define YELLOW_LED 11
#define BLUE_LED 10

char const LEDs[] = {'R', 'G', 'Y', 'B'}; //Make a "list" array for all possible LED colors (Used for when randomizing for nextLED)


//Flashing Function Variables
bool allSync = false; //Flag for enabling all LED's
int flashGoal = 1;    //total num of flashed on LED's. Default: 1
int StartFlashDelay = 200; //Delay for lightFlashing

//set all Led's flag to off 
bool enRedLED = false;
bool enGreenLED = false;
bool enYellowLED = false;
bool enBlueLED = false;


/**-------------- Game SetUp -----------**/

//GAME FLAGS
bool timeDone = false;    //If no response was given when awaiting user for certain time
bool corAnsw = false;     //If user gave a correct valid answer
bool lastLife = false;    //If user is on their last attempt 
bool lastRound = false;   //If user is at the last round of THAT Level
bool lastLevel = false;   //If user is at the Last level of the game

//Round variables
int roundNum = 0; //Keeps track of each round in a level
char nextLED;     //global var holder for next LED

//Level variables
int gamelvl = 1; //Keeps track of each level in a round


//User Settings
char pressedButton = NO_KEY; //Makes a global var for the what button is pressed on the keypad  

//GAME PATTERN SETUP
const byte max_PatternLen = 9;      //Creates the biggest possible length array to fit all
char currentPattern[max_PatternLen]; //Creates the array to hold the pattern

byte patternLen = 0;              //len of the current pattern
byte startLen = 0;                //initial starting length for each level



//Max Length/Rounds per level
const byte MaxL_lvl1 = 5;               //5 rounds in level 1
const byte MaxL_lvl2 = 7;               //7       ||        2
const byte MaxL_lvl3 = max_PatternLen;  //9       ||        3

byte currentlvl_maxLen = 0; //Variable holder for the current level Max Len. If lvl1, 5 is max. Used for dynamic looping sequence.

//Minimum starting Lenght per Level
//Level 2 will start wil 3 patterns already while level 3 will start with 4.
const byte MinL_lvl1 = 0;
const byte MinL_lvl2 = 3;
const byte MinL_lvl3 = 4;


//States all States for the game (FSM)
enum gameSTATE{
  START,
  FLASH_LEDS,
  UPDATE_PTRN,
  DISPLAY_PTRN,
  GET_RES,
  CHECK_RES,
  UPDATE_GAME,
  GAME_WON,
  GAME_LOST,
  DEBUG //A state where one can light each individual LED using first row 1,2,3,A
};
unsigned char currentState = START;

//flashing helper function. When ever I need to flash certain LED's, I call the helper function 
//and provide the amount of times somethign should flash and the msdelay interval between each Flash.
void flash_leds(int msDelay = 150)
{
  if (allSync)            //Quick Variable to turn on all LEDs
    {
      enRedLED = true;
      enGreenLED = true;
      enYellowLED = true;
      enBlueLED = true;
    }

  delay(msDelay*2);
  //Turns on the enabled LEDS
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
  //Resets all LEDs to off
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      delay(msDelay);
    }
    //reset LEDs flags
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
    pressedButton = heroKeypad.getKey();  // Wait until a button is pressed
    if (pressedButton)
      {
        // Serial.println(pressedButton);
      }
    if (pressedButton == BUTTONS[1][0])
      {
        allSync = true;
        flashGoal = 5;
        currentState = UPDATE_PTRN;
        Serial.print("Completed State Start");      //Debugging Flag

        flash_leds(StartFlashDelay);
      }
    pressedButton = NO_KEY;
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
            currentlvl_maxLen = MaxL_lvl1;
          }

        //GAME PATTERN INITIALIZATION FOR LEVEL 2
        if (gamelvl == 2)
          {
            startLen = MinL_lvl2;
            currentlvl_maxLen =  MaxL_lvl2;            
          }

        //GAME PATTERN INITIALIZATION FOR LEVEL 3
        if (gamelvl == 3)
          {
            startLen = MinL_lvl3;
            currentlvl_maxLen = MaxL_lvl3;
          }
    

        //Begin Prefilling the pattern for the level
        for (byte i = 0; i < startLen; i++)
          {
            byte randLED_index =  random(0,4);  //Using the random seeding choses a "random" number from 1-4
            //assign the color index to its color
            nextLED = LEDs[randLED_index];
            currentPattern[patternLen] = nextLED;
            patternLen++;
          }
      }





      //Adding the next light to the pattern
      if (patternLen <  currentlvl_maxLen)
        {
          byte randLED_index = random(0,4);
          nextLED = LEDs[randLED_index];
          currentPattern[patternLen] = nextLED;
          patternLen++;
        }






        
    // // Debug print pattern
    // Serial.print("Pattern: ");
    // roundNum++;
    // for (byte i = 0; i < patternLen; i++) 
    // {
    //   Serial.print(currentPattern[i]);
    //   Serial.print(' ');
    // }
    // Serial.println();

    currentState = DISPLAY_PTRN;
    Serial.print("Completed State: UPDATE_PTRN");      //Debugging Flag

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
    currentState = GET_RES;
    Serial.print("Completed State: DISPLAY_PTRN");      //Debugging Flag
    break;
  }


//--------------------------------------------------------------------|
//------------------------AWAIT RES----------------------------------|
  case GET_RES: {
    //Awaits for 1 response then moves to STATE:CHECK_RES,
    //Will return back if response is correct
    pressedButton = NO_KEY; //Resets pressedButton
    pressedButton = heroKeypad.getKey();  // Wait until a button is pressed

    if (pressedButton != NO_KEY)
    {
      currentState = CHECK_RES;
      Serial.print("Completed State: GET_RES");      //Debugging Flag
      break
    }
    else if (timeDone)
    {
      pressedButton = NO_KEY;
      currentState = CHECK_RES;
      break
    }

    //else it remain in Get Res 
    currentState = GET_RES;
    Serial.print("NO Response Yet");      //Debugging Flag

    break;
  }

//--------------------------------------------------------------------|
//------------------------CHECK RES----------------------------------|
  case CHECK_RES: {
    if (pressedButton == currentPattern[roundNum] && !timeDone)
      {
        corAnsw = true;
      }
      else
      {
        corAnsw = false;
      }
    currentState = UPDATE_GAME;
    Serial.print("Completed State: CHECK_RES");      //Debugging Flag
    break;
  }

//--------------------------------------------------------------------|
//------------------------UPDATE_GAME----------------------------------|
  case UPDATE_GAME: {
    if(corAnsw)
    {
      if (lastRound)
      {
        if (lastLevel)
        {
          currentState = GAME_WON;
        }
        else
        {
          roundNum = 0;
          lastLife = false;
          gamelvl++;
          currentState = UPDATE_PTRN;

        }
      }
      else
      {
      
      roundNum++;
      currentState = UPDATE_PTRN;
      }
    }
    else
    {
      if(lastLife)
      {
        currentState = GAME_LOST;
      }
      else
      {
        roundNum = 0;
        lastLife = true;
        currentState = UPDATE_PTRN;
      }
    }
    Serial.print("Completed State: UPDATE_GAME");      //Debugging Flag
    break;
  }

//--------------------------------------------------------------------|
//------------------------GAME WON----------------------------------|
  case GAME_WON: {
    for (int i=0; ;i++)
    {
      allSync = true;
      flash_leds();
      Serial.print("GAMEWON");      //Debugging Flag
    }
    break;
  }

//--------------------------------------------------------------------|
//------------------------GAME LOST----------------------------------|
  case GAME_LOST: {
      enRedLED = true;
      flash_leds();
      Serial.print("GAMELOST");      //Debugging Flag

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
  // if (roundNum < max_PatternLen) {
  //   currentPattern[roundNum] = newPattern
  //   roundNum++;
  // }










    //-------------------------------------------------------------------------------------------
    //---------------------------- LED SYSTEM ---------------------------------------------------------------
    //-------------------------------------------------------------------------------------------
 

                   




}
