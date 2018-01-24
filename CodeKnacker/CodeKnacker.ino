// Necessary Libraries
#include <Adafruit_NeoPixel.h>        // For the 4 colorful neopixels
#include <Adafruit_LEDBackpack.h>     // For the 2-digit LED display
#include <LiquidCrystal.h>            // For the LCD text display
#include <Bounce2.h>                  // For several pushbuttons

//Digital Pins
#define PIN_NEOPIXEL_DI 0
#define PIN_BUTTON_NEXT 1
#define PIN_BUTTON_0 2
#define PIN_BUTTON_1 3
#define PIN_BUTTON_2 4
#define PIN_BUTTON_3 5
#define PIN_BUTTON_YES 6
#define PIN_BUTTON_NO 7
#define PIN_LCD_DISPLAY_RS 8
#define PIN_LCD_DISPLAY_E 9
#define PIN_LCD_DISPLAY_D4 10
#define PIN_LCD_DISPLAY_D5 11
#define PIN_LCD_DISPLAY_D6 12
#define PIN_LCD_DISPLAY_D7 13

// Analog Pins
#define ANALOG 14
#define PIN_BUTTON_NEXT_LED ANALOG+3
#define PIN_LED_DISPLAY_SDA ANALOG+4
#define PIN_LED_DISPLAY_SCK ANALOG+5

// Basic Infrastructure - Definitions
#define NEOPIXEL_COUNT 4
#define BUTTON_COUNT 7
#define LCD_DISPLAY_ROW_COUNT 2
#define LCD_DISPLAY_COLUMN_COUNT 16
#define LED_DISPLAY_DEVICE_ADDRESS 0x70
#define LED_DISPLAY_DIGIT_COUNT 2

// Basic Infrastructure - Objects
Adafruit_NeoPixel neopixel(NEOPIXEL_COUNT, PIN_NEOPIXEL_DI, NEO_RGB + NEO_KHZ800);
LiquidCrystal lcdDisplay(PIN_LCD_DISPLAY_RS, PIN_LCD_DISPLAY_E, PIN_LCD_DISPLAY_D4, PIN_LCD_DISPLAY_D5, PIN_LCD_DISPLAY_D6, PIN_LCD_DISPLAY_D7);
Adafruit_AlphaNum4 ledDisplay;
Bounce button0, button1, button2, button3, buttonYes, buttonNo, buttonNext;
Bounce bounce[BUTTON_COUNT] = {button0, button1, button2, button3, buttonYes, buttonNo, buttonNext};
const int button_pin[BUTTON_COUNT] = {PIN_BUTTON_0, PIN_BUTTON_1, PIN_BUTTON_2, PIN_BUTTON_3, PIN_BUTTON_YES, PIN_BUTTON_NO, PIN_BUTTON_NEXT};
#define BUTTON_0 0
#define BUTTON_1 1
#define BUTTON_2 2
#define BUTTON_3 3
#define BUTTON_YES 4
#define BUTTON_NO  5
#define BUTTON_NEXT 6

// Basic Infrastructure - Colors
#define COLOR_COUNT 7
#define COLOR_BLACK neopixel.Color(0, 0 , 0)
#define COLOR_RED neopixel.Color(180,0 , 0)
#define COLOR_GREEN neopixel.Color(0, 180 , 0)
#define COLOR_BLUE neopixel.Color(0, 0 , 180)
#define COLOR_YELLOW neopixel.Color(90, 90 , 0)
#define COLOR_PINK neopixel.Color(90, 0 , 90)
#define COLOR_ORANGE neopixel.Color(180, 40 , 00)
const uint32_t color[COLOR_COUNT] = {COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_YELLOW, COLOR_PINK, COLOR_ORANGE};

// Several Delay Values
#define MILLISECOND 1
#define DELAY_CELEBRATE_NEO_BLINK 100*MILLISECOND
#define DELAY_CELEBRATE_LED_BLINK 500*MILLISECOND
#define DELAY_YESNO_BLINK 1000*MILLISECOND
#define DELAY_LCD_TEXT_SCROLL 2000*MILLISECOND
#define DELAY_CONFUSED_SHIFT_PIXEL 20*MILLISECOND
#define DELAY_CONFUSED 6000*MILLISECOND
#define DELAY_GUESS_START DELAY_LCD_TEXT_SCROLL*3*MILLISECOND
#define DELAY_CODE_SELECT_BLINK 500*MILLISECOND
#define DELAY_HIDE_SCORE DELAY_LCD_TEXT_SCROLL*5*MILLISECOND
#define DELAY_HISTORY_SCROLL_BLINK 500*MILLISECOND

// German Umlauts as special characters
const byte OUMLAUT_SMALL[8] = {
 0b01010,
 0b00000,
 0b01110,
 0b10001,
 0b10001,
 0b10001,
 0b01110,
 0b00000
};
 
const byte UUMLAUT_SMALL[8] = {
 0b01010,
 0b00000,
 0b10001,
 0b10001,
 0b10001,
 0b10011,
 0b01101,
 0b00000
};
 
// All texts for the LCD Display in prog memory
const char TEXT_DATA_EMPTY_LINE[] PROGMEM = "                ";
const char TEXT_DATA_WELCOME[] PROGMEM = "Willkommen!|Möchtest Du|mit mir spielen?|Dann drücke|irgendeinen|Knopf!";
const char TEXT_DATA_GUESS_MODE_QUESTION[] PROGMEM = "Soll ich Deinen|Code raten?";
const char TEXT_DATA_GUESS_MODE_START[] PROGMEM = "Okay, dann denk'|Dir einen Code!|Fertig?";
const char TEXT_DATA_GUESS_MODE_START_REPEAT[] PROGMEM = "Mach' schon!|Denk' Dir einen|Code!|Fertig?";
const char TEXT_DATA_GUESS_MODE_THINK[] PROGMEM = "Hmmmm....";
const char TEXT_DATA_GUESS_MODE_WAIT_FOR_ANSWER[] PROGMEM = "Mein X. Versuch:|Wieviele habe|ich richtig?";
const char TEXT_DATA_GUESS_MODE_CELEBRATE[] PROGMEM = "Yippiieh!|Geschafft in|nur X Versuchen!";
const char TEXT_DATA_GUESS_MODE_ERROR[] PROGMEM = "??? Das kann gar|nicht sein ???|Bist Du sicher?";
const char TEXT_DATA_HIDE_MODE_QUESTION[] PROGMEM = "Willst Du meinen|Code raten?";
const char TEXT_DATA_HIDE_MODE_THINK[] PROGMEM = "Einen Moment!|Hmmmm....|Ich habe einen!";
const char TEXT_DATA_HIDE_MODE_GUESS[] PROGMEM = "Dein X. Versuch:|Na los! Rate den|C O D E ..(Hihi)";
const char TEXT_DATA_HIDE_MODE_SCORE[] PROGMEM = "Du hast X ganz|richtig und bei|Y stimmt nur die|Farbe, nicht die|Position!";
const char TEXT_DATA_HIDE_MODE_HISTORY[] PROGMEM = "Dein X. Versuch";
const char TEXT_DATA_HIDE_MODE_CELEBRATE[] PROGMEM = "Super!|Du hast es|geschafft in|nur X Versuchen!";
const char TEXT_DATA_CONFUSED[] PROGMEM = "Was machen wir|dann hier?";

// Adresses of all texts declared above, also in prog memory
const char* const textTable[] PROGMEM = {
  TEXT_DATA_EMPTY_LINE,
  TEXT_DATA_WELCOME,
  TEXT_DATA_GUESS_MODE_QUESTION,
  TEXT_DATA_GUESS_MODE_START,
  TEXT_DATA_GUESS_MODE_START_REPEAT,
  TEXT_DATA_GUESS_MODE_THINK,
  TEXT_DATA_GUESS_MODE_WAIT_FOR_ANSWER,
  TEXT_DATA_GUESS_MODE_CELEBRATE,
  TEXT_DATA_GUESS_MODE_ERROR,
  TEXT_DATA_HIDE_MODE_QUESTION,
  TEXT_DATA_HIDE_MODE_THINK,
  TEXT_DATA_HIDE_MODE_GUESS,
  TEXT_DATA_HIDE_MODE_SCORE,
  TEXT_DATA_HIDE_MODE_HISTORY,
  TEXT_DATA_HIDE_MODE_CELEBRATE,
  TEXT_DATA_CONFUSED
};

// Index of texts belonging to a topic
#define TEXT_EMPTY_LINE 0
#define TEXT_WELCOME 1
#define TEXT_GUESS_MODE_QUESTION 2
#define TEXT_GUESS_MODE_START 3
#define TEXT_GUESS_MODE_START_REPEAT 4
#define TEXT_GUESS_MODE_THINK 5
#define TEXT_GUESS_MODE_WAIT_FOR_ANSWER 6
#define TEXT_GUESS_MODE_CELEBRATE 7
#define TEXT_GUESS_MODE_ERROR 8
#define TEXT_HIDE_MODE_QUESTION 9
#define TEXT_HIDE_MODE_THINK 10
#define TEXT_HIDE_MODE_GUESS 11
#define TEXT_HIDE_MODE_SCORE 12
#define TEXT_HIDE_MODE_HISTORY 13
#define TEXT_HIDE_MODE_CELEBRATE 14
#define TEXT_CONFUSED 15

// Textbuffer for reading a text line from prog memory
char textBuffer[LCD_DISPLAY_COLUMN_COUNT+1];

// Count how many parts a text has (separated by "|").
byte countParts(byte textNr) {
  char* textSource = (char*) pgm_read_word(&(textTable[textNr]));
  byte parts=1;
  for (char c=pgm_read_byte(textSource++); c!=0; c=pgm_read_byte(textSource++)) {
    if (c=='|') {
      parts++;
    }
  }
  return parts;
}

// Read a text line from prog memory, skip parts.
char* getText(byte textNr, byte partNr) {
  char* textSource = (char*) pgm_read_word(&(textTable[textNr]));
  for (byte part=0; part<partNr; part++) {
    while (pgm_read_byte(textSource++)!='|');
  }
  int i=0;
  for (char c=pgm_read_byte(textSource++); (i<LCD_DISPLAY_COLUMN_COUNT)&&(c!='|')&&(c!=0); c=pgm_read_byte(textSource++)) {
    textBuffer[i++] = c;
  }
  while (i<LCD_DISPLAY_COLUMN_COUNT) {
    textBuffer[i++] = ' ';
  }
  textBuffer[i] = 0;
  return textBuffer;
}

// Print a text to the LCD, one part at a time, alternating LCD rows, optionally replacing placeholders with other text. 
void printLcd(byte textNr, long timeInState, String replace0="", String replace1="") {
  byte partNr = (timeInState / DELAY_LCD_TEXT_SCROLL) % countParts(textNr);
  byte row = partNr%2;
  String text = String(getText(textNr, partNr));
  text.replace("X", replace0);
  text.replace("Y", replace1);
  text.replace("ö", "\1");
  text.replace("ü", "\2");
  lcdDisplay.setCursor(0, row);
  lcdDisplay.print(text);
  if (row==0) {
    lcdDisplay.setCursor(0, 1);
    lcdDisplay.print("                ");
  }
}

// States of the state machine
#define STATE_WELCOME 0
#define STATE_GUESS_MODE_QUESTION 1
#define STATE_HIDE_MODE_QUESTION 2
#define STATE_CONFUSED 3
#define STATE_GUESS_MODE_START 4
#define STATE_GUESS_MODE_START_REPEAT 5
#define STATE_GUESS_MODE_THINK 6
#define STATE_GUESS_MODE_GUESS 7
#define STATE_GUESS_MODE_CELEBRATE 8
#define STATE_GUESS_MODE_ERROR 9
#define STATE_HIDE_MODE_THINK 10
#define STATE_HIDE_MODE_GUESS 11
#define STATE_HIDE_MODE_SCORE 12
#define STATE_HIDE_MODE_HISTORY 13
#define STATE_HIDE_MODE_CELEBRATE 14
#define STATE_BLACK 999
byte currentState;    // The current state of the state machine
long stateStartTime;  // Timestamp in milliseconds, when the current State started

// Additional variables vor the game.
byte buttonJustPressed;     // When user presses a butten, the button nr is stored here
int secretCode;             // The secret code the user has to guess
int currentGuess;           // Current guess of the user or the of the machine, depending on game mode (guess or hide)
byte currentAnswer;         // Answer to the current guess. The upper 4 bits are the blacks (color and position correct), the lower bits the whites (only color correct) 
byte historyCount;          // Size of current guess/answer-history
byte historyPosition;       // Position while scrolling through the history
#define HISTORY_MAX_SIZE 30
int guessHistory[HISTORY_MAX_SIZE];    // History of guesses
byte answerHistory[HISTORY_MAX_SIZE];  // History of answers

// Additional variables for the guessing algorithm.
#define POSSIBLE_CODES_MAX 200
int possibleCodes[POSSIBLE_CODES_MAX];

// Runs only once in the beginning
void setup() {
  neopixel.begin();
  ledDisplay.begin(LED_DISPLAY_DEVICE_ADDRESS);
  lcdDisplay.begin(LCD_DISPLAY_COLUMN_COUNT, LCD_DISPLAY_ROW_COUNT);
  lcdDisplay.createChar(1, OUMLAUT_SMALL);
  lcdDisplay.createChar(2, UUMLAUT_SMALL);
  for (int i=0; i<BUTTON_COUNT; i++) {
    pinMode(button_pin[i], INPUT_PULLUP);
    bounce[i].attach(button_pin[i]);
    bounce[i].interval(10);
  }
  pinMode(PIN_BUTTON_NEXT_LED, OUTPUT);
  clearAll();
  randomSeed(analogRead(0));
  changeStateTo(STATE_WELCOME);
}

// Clear all NeoPixels, LCD and LED displays and the LED at the next button-
void clearAll() {
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {
    neopixel.setPixelColor(i, COLOR_BLACK);
  }
  neopixel.show();
  lcdDisplay.clear();
  ledDisplay.clear();
  ledDisplay.writeDisplay();
  digitalWrite(PIN_BUTTON_NEXT_LED, LOW);
}

// Changes the state of the state machine
void changeStateTo(byte newState) {
  clearAll();
  currentState = newState;
  stateStartTime = millis();
}

// Runs again and again, endlessly. Mein hub of the state machine.
void loop() {
  long timeInState = millis() - stateStartTime;
  switch (currentState) {
    case STATE_WELCOME:
      doStateWelcome(timeInState);
      break;
    case STATE_GUESS_MODE_QUESTION:
      doStateGuessModeQuestion(timeInState);
      break;
    case STATE_GUESS_MODE_START:
      doStateGuessModeStart(timeInState);
      break;
    case STATE_GUESS_MODE_START_REPEAT:
      doStateGuessModeStartRepeat(timeInState);
      break;
    case STATE_GUESS_MODE_THINK:
      doStateGuessModeThink(timeInState);
      break;
    case STATE_GUESS_MODE_GUESS:
      doStateGuessModeGuess(timeInState);
      break;
    case STATE_GUESS_MODE_CELEBRATE:
      doStateGuessModeCelebrate(timeInState);
      break;
    case STATE_GUESS_MODE_ERROR:
      doStateGuessModeError(timeInState);
      break;
    case STATE_HIDE_MODE_QUESTION:
      doStateHideModeQuestion(timeInState);
      break;
    case STATE_HIDE_MODE_THINK:
      doStateHideModeThink(timeInState);
      break;
    case STATE_HIDE_MODE_GUESS:
      doStateHideModeGuess(timeInState);
      break;
    case STATE_HIDE_MODE_SCORE:
      doStateHideModeScore(timeInState);
      break;
    case STATE_HIDE_MODE_HISTORY:
      doStateHideModeHistory(timeInState);
      break;
    case STATE_HIDE_MODE_CELEBRATE:
      doStateHideModeCelebrate(timeInState);
      break;
    case STATE_CONFUSED:
      doStateConfused(timeInState);
      break;
    default:
      doStateBlack(timeInState);
      break;
  }
}

// Welcome the user, wait for a button pressed.
void doStateWelcome(long timeInState) {
  printLcd(TEXT_WELCOME, timeInState);
  celebrate(timeInState);
  if (buttonPressed()) {
    changeStateTo(STATE_GUESS_MODE_QUESTION);
  }
}

// Ask if the user wants the machine to guess (guess mode)
void doStateGuessModeQuestion(long timeInState) {
  printLcd(TEXT_GUESS_MODE_QUESTION, timeInState);
  yesNoQuestion(timeInState);
  if (buttonPressed()) {
    switch (buttonJustPressed) {
      case BUTTON_YES:
        changeStateTo(STATE_GUESS_MODE_START);
        break;
      case BUTTON_NO:
        changeStateTo(STATE_HIDE_MODE_QUESTION);
        break;
      default:
        changeStateTo(STATE_GUESS_MODE_QUESTION);
        break;
    }
  }
}

// Ask the user to think of a secret code and confirm
void doStateGuessModeStart(long timeInState) {
  printLcd(TEXT_GUESS_MODE_START, timeInState);
  yesNoQuestion(timeInState);
  if (buttonPressed()) {
    switch (buttonJustPressed) {
      case BUTTON_YES:
        historyCount = 0;
        changeStateTo(STATE_GUESS_MODE_THINK);
        break;
      case BUTTON_NO:
        changeStateTo(STATE_GUESS_MODE_START_REPEAT);
        break;
      default:
        changeStateTo(STATE_GUESS_MODE_START);
        break;
    }
  }
}

// If the user does not confirm, insist and ask again.
void doStateGuessModeStartRepeat(long timeInState) {
  printLcd(TEXT_GUESS_MODE_START_REPEAT, timeInState);
  yesNoQuestion(timeInState);
  if (buttonPressed()) {
    switch (buttonJustPressed) {
      case BUTTON_YES:
        historyCount = 0;
        changeStateTo(STATE_GUESS_MODE_THINK);
        break;
      case BUTTON_NO:
        changeStateTo(STATE_GUESS_MODE_START_REPEAT);
        break;
      default:
        changeStateTo(STATE_GUESS_MODE_START_REPEAT);
        break;
    }
  }
}

// Try to think of a good guess. Complain if there is no possibility left
void doStateGuessModeThink(long timeInState) {
  printLcd(TEXT_GUESS_MODE_THINK, timeInState);
  if (timeInState > DELAY_GUESS_START) {
    currentGuess = getNextGuess();
    if (currentGuess == -1) {
      changeStateTo(STATE_GUESS_MODE_ERROR);
    } else {
      currentAnswer = 0;
      guessHistory[historyCount] = currentGuess;
      answerHistory[historyCount] = currentAnswer;
      historyCount++;
      changeStateTo(STATE_GUESS_MODE_GUESS);
    }
  }
}

// Display the current guess of the machine. Ask the user to answer it and press the next button.
void doStateGuessModeGuess(long timeInState) {
  printLcd(TEXT_GUESS_MODE_WAIT_FOR_ANSWER, timeInState, String(historyCount));
  setNeopixelsTo(currentGuess);
  setLedDisplayTo(currentAnswer);
  offerNextButton();
  if (buttonPressed()) {
    byte blacks = currentAnswer >> 4;
    byte whites = currentAnswer & 15;
    switch (buttonJustPressed) {
      case BUTTON_YES:
        blacks = (blacks + 1) % (NEOPIXEL_COUNT + 1);
        whites = min(whites, NEOPIXEL_COUNT - blacks);
        break;
      case BUTTON_NO:
        whites = (whites + 1) % (NEOPIXEL_COUNT + 1);
        blacks = min(blacks, NEOPIXEL_COUNT - whites);
        break;
      case BUTTON_NEXT:
        answerHistory[historyCount - 1] = currentAnswer;
        if (blacks == NEOPIXEL_COUNT) {
          changeStateTo(STATE_GUESS_MODE_CELEBRATE);
        } else {
          changeStateTo(STATE_GUESS_MODE_THINK);
        }
        break;
      default:
        changeStateTo(STATE_GUESS_MODE_GUESS);
        break;
    }
    currentAnswer = (blacks << 4) + whites;
  }
}

// Celebrate if the answer was correct. Print the score and restart after a short black.
void doStateGuessModeCelebrate(long timeInState) {
  printLcd(TEXT_GUESS_MODE_CELEBRATE, timeInState, String(historyCount));
  celebrate(timeInState);
  offerNextButton();
  if (buttonPressed()) {
    changeStateTo(STATE_BLACK);
  }
}

// Complain if there is not possibility left to guess.
void doStateGuessModeError(long timeInState) {
  printLcd(TEXT_GUESS_MODE_ERROR, timeInState);
  beingConfused(timeInState);
  if (timeInState > DELAY_CONFUSED) {
    changeStateTo(STATE_BLACK);
  }
}

// The other way around: Ask if the user wants the machine to create a secret code (hide mode)
void doStateHideModeQuestion(long timeInState) {
  printLcd(TEXT_HIDE_MODE_QUESTION, timeInState);
  yesNoQuestion(timeInState);
  if (buttonPressed()) {
    switch (buttonJustPressed) {
      case BUTTON_YES:
        changeStateTo(STATE_HIDE_MODE_THINK);
        break;
      case BUTTON_NO:
        changeStateTo(STATE_CONFUSED);
        break;
      default:
        changeStateTo(STATE_HIDE_MODE_QUESTION);
        break;
    }
  }
}

// Think a while and create the secret code by random
void doStateHideModeThink(long timeInState) {
  printLcd(TEXT_HIDE_MODE_THINK, timeInState);
  if (timeInState > DELAY_GUESS_START) {
    secretCode = createRandomCode();
    currentGuess = 0;
    historyCount = 0;
    changeStateTo(STATE_HIDE_MODE_GUESS);
  }
}

// Let the user guess, blink the pixels still unset, display the guess number, light the next button when guess is complete. Allow for history scroll.
void doStateHideModeGuess(long timeInState) {
  printLcd(TEXT_HIDE_MODE_GUESS, timeInState, String(historyCount+1)) ;
  int unset = getFirstUnsetNeopixel(currentGuess);
  blinkNeopixel(currentGuess, unset, timeInState);
  historyPosition=historyCount;
  if (unset<0) {
    offerNextButton();
  }
  if (buttonPressed()) {
    switch (buttonJustPressed) {
      case BUTTON_0:
      case BUTTON_1:
      case BUTTON_2:
      case BUTTON_3:
        currentGuess = incrementColor(currentGuess, buttonJustPressed-BUTTON_0);
        break;
      case BUTTON_YES:
        if (historyPosition>0) {
          historyPosition--;
          changeStateTo(STATE_HIDE_MODE_HISTORY);
        }
        break;
      case BUTTON_NEXT:
        if (unset<0) {
           currentAnswer = getAnswer(secretCode, currentGuess);
           guessHistory[historyCount] = currentGuess;
           answerHistory[historyCount] = currentAnswer;
           historyCount++;
           if (currentAnswer >> 4 == NEOPIXEL_COUNT) {
             changeStateTo(STATE_HIDE_MODE_CELEBRATE);
           } else {
             changeStateTo(STATE_HIDE_MODE_SCORE);
           }
        }
        break;
    }
  }
}

// Creates and displays the answer to a user guess. Go celebrating if the answer is maximal. Put both guess and answer into the history
void doStateHideModeScore(long timeInState) {
  setLedDisplayTo(currentAnswer);
  byte blacks = currentAnswer >> 4;
  byte whites = currentAnswer & 15;
  printLcd(TEXT_HIDE_MODE_SCORE, timeInState, String(blacks), String(whites));
  if (timeInState > DELAY_HIDE_SCORE) {
    currentGuess=0;
    changeStateTo(STATE_HIDE_MODE_GUESS);
  }
}

// Allow scrolling backward and forward in the guess/answer-history. 
void doStateHideModeHistory(long timeInState) {
  printLcd(TEXT_HIDE_MODE_HISTORY, timeInState, String(historyPosition+1)) ;
  setNeopixelsTo(guessHistory[historyPosition]);
  setLedDisplayTo(answerHistory[historyPosition]);
  if (buttonPressed()) {
    switch (buttonJustPressed) {
      case BUTTON_YES:
        if (historyPosition>0) {
          historyPosition--;
          changeStateTo(STATE_HIDE_MODE_HISTORY);
        }
        break;
      case BUTTON_NO:
        historyPosition++;
        if (historyPosition<historyCount) {
          changeStateTo(STATE_HIDE_MODE_HISTORY);
        } else {
          changeStateTo(STATE_HIDE_MODE_GUESS);
        }
        break;
    }
  }
}

// Celebrate by diverse blinkings and corresponding text.
void doStateHideModeCelebrate(long timeInState) {
  printLcd(TEXT_HIDE_MODE_CELEBRATE, timeInState, String(historyCount));
  celebrate(timeInState);
  offerNextButton();
  if (buttonPressed()) {
    changeStateTo(STATE_BLACK);
  }
}

// Express that the machine is confused. Start over after a shot black.
void doStateConfused(long timeInState) {
  printLcd(TEXT_CONFUSED, timeInState);
  beingConfused(timeInState);
  if (timeInState > DELAY_CONFUSED) {
    changeStateTo(STATE_BLACK);
  }
}

// Set all to black. Wait. Start over.
void doStateBlack(long timeInState) {
  if (timeInState > DELAY_CONFUSED) {
    changeStateTo(STATE_WELCOME);
  }
}

// Increment the color of a NeoPixel when the user presses the button below it.
int incrementColor(int current, byte pos) {
  return (((((current>>(4*pos))&15)%(COLOR_COUNT-1))+1)<<(4*pos))+(current&(~(15<<(4*pos))));
}

// Ofer the next button by lighting its LED.
void offerNextButton() {
  digitalWrite(PIN_BUTTON_NEXT_LED, HIGH);
}

// Ask a yes/no question by blinking green/red NeoPixels and displaying J/N on the LED display.
void yesNoQuestion(long timeInState) {
  if ((timeInState / DELAY_YESNO_BLINK) % 2 == 0) {
    ledDisplay.writeDigitAscii(0, 'J');
    ledDisplay.writeDigitAscii(1, ' ');
    neopixel.setPixelColor(0, COLOR_GREEN);
    neopixel.setPixelColor(1, COLOR_GREEN);
    neopixel.setPixelColor(2, COLOR_BLACK);
    neopixel.setPixelColor(3, COLOR_BLACK);
  } else {
    ledDisplay.writeDigitAscii(0, ' ');
    ledDisplay.writeDigitAscii(1, 'N');
    neopixel.setPixelColor(0, COLOR_BLACK);
    neopixel.setPixelColor(1, COLOR_BLACK);
    neopixel.setPixelColor(2, COLOR_RED);
    neopixel.setPixelColor(3, COLOR_RED);
  }
  neopixel.show();
  ledDisplay.writeDisplay();
}

// Celebrate by diverse blinkings and corresponding text.
void celebrate(long timeInState) {
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {
    neopixel.setPixelColor(i, color[(timeInState / DELAY_CELEBRATE_NEO_BLINK + i) % (COLOR_COUNT - 1) + 1]);
  }
  neopixel.show();
  for (int i = 0; i < LED_DISPLAY_DIGIT_COUNT; i++) {
    if ((timeInState / DELAY_CELEBRATE_LED_BLINK) % 2 == 0) {
      ledDisplay.writeDigitAscii(i, '*');
      digitalWrite(PIN_BUTTON_NEXT_LED, HIGH);
    } else {
      ledDisplay.writeDigitAscii(i, ' ');
      digitalWrite(PIN_BUTTON_NEXT_LED, LOW);
    }
  }
  ledDisplay.writeDisplay();
}

// Express that the machine is confused by blinkings and question marks on the LED display.
void beingConfused(long timeInState){
  if ((timeInState / DELAY_YESNO_BLINK) % 2 == 0) {
    ledDisplay.writeDigitAscii(0, '?');
    ledDisplay.writeDigitAscii(1, '?');
  } else {
    ledDisplay.writeDigitAscii(0, ' ');
    ledDisplay.writeDigitAscii(1, ' ');
  }
  ledDisplay.writeDisplay();
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {
    if (i == abs((timeInState / DELAY_CONFUSED_SHIFT_PIXEL) % (NEOPIXEL_COUNT * 2) - NEOPIXEL_COUNT)) {
      neopixel.setPixelColor(i, COLOR_YELLOW);
    } else {
      neopixel.setPixelColor(i, COLOR_BLACK);
    }
    neopixel.show();
  }
}

// Set the NeoPixels to show a code
void setNeopixelsTo(int code) {
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {
    neopixel.setPixelColor(i, color[code & 15]);
    code >>= 4;
  }
  neopixel.show();
}

// Blink one NeoPixel to show parts of a guess still uneditet.
void blinkNeopixel(int code, int nr, long timeInState) {
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {
    if ((i == nr) && ((timeInState / DELAY_CODE_SELECT_BLINK) % 2 == 0)) {
      neopixel.setPixelColor(i, COLOR_YELLOW);
    } else {
      neopixel.setPixelColor(i, color[(code>>(4*i)) & 15]);
    }
  }
  neopixel.show();
}

// Look for a part of a guess still uneditet.
int  getFirstUnsetNeopixel(int code) {
  for (int i = 0; i <= NEOPIXEL_COUNT - 1; i++) {
    if ((code & (15 << (4 * i))) == 0) {
      return i;
    }
  }
  return -1;
}

// Set the LED display to show an answer.
void setLedDisplayTo(byte answer) {
  ledDisplay.writeDigitAscii(0, '0' + ((answer >> 4) & 15));
  ledDisplay.writeDigitAscii(1, '0' + (answer & 15));
  ledDisplay.writeDisplay();
}

// Refresh the button states and determine if a button was pressed.
boolean buttonPressed() {
  for (byte i = 0; i < BUTTON_COUNT; i++) {
    if (bounce[i].update() && bounce[i].fell()) {
      buttonJustPressed = i;
      return true;
    }
  }
  return false;
}

// Calculate the next guess of the machine. This is the brain code :-)
int getNextGuess() {
  int possibleCodeCount = 0;
  for (byte i = 1; i < COLOR_COUNT; i++)  // Generate all possible color combinations (no Black).
    for (byte j = 1; j < COLOR_COUNT; j++)
      for (byte k = 1; k < COLOR_COUNT; k++)
        for (byte l = 1; l < COLOR_COUNT; l++) {
          int possibleCode = (((((i << 4) + j) << 4) + k) << 4) + l; // Combine them to one integer code (4 bits for each color).
          boolean possible = true;
          for (int a = 0; a < historyCount; a++) {  // For each code look at all historic guesses.
            if (getAnswer(possibleCode, guessHistory[a]) != answerHistory[a]) { // And see if the answer would match the historic answer
              possible = false;
              break;
            }
          }
          if (possible && (possibleCodeCount < POSSIBLE_CODES_MAX)) {
            possibleCodes[possibleCodeCount++] = possibleCode; // If all answers match, this is a possible code and gets stored. Continue until we have enough of them.
          }
        }
  int bestValue = 2000;
  int bestCount = 0;
  for (int i = 0; i < possibleCodeCount; i++) { // Now iterate all possible codes as guesses.
    int answerCount[25];  // Prepare counting answers.
    for (int i = 0; i < 25; i++) {
      answerCount[i] = 0;
    }
    int maxAnswerCount = 0;
    for (int j = 0; j < possibleCodeCount; j++) { // Also, iterate all possible codes as potential secret codes.
      byte answer = getAnswer(possibleCodes[i], possibleCodes[j]); // Get the answer this would give.
      int pos = (answer >> 4) * 5 + (answer & 15);  // And count the number of occurrencies of each answer, the more balanced the better
      answerCount[pos]++;
      if (answerCount[pos] > maxAnswerCount) {
        maxAnswerCount++;  // Keep track, how often we get the most common answer.
      }
    }
    if (maxAnswerCount < bestValue) { // And try to keep it low (means all is better balanced)
      bestValue = maxAnswerCount;
      bestCount = 0;
    }
    if (maxAnswerCount == bestValue) { // Count the number of guesses with the same low (best) most common answer.
      bestCount++;
    }
  }
  int countDownToChosenOne = (int)random(bestCount); // Choose one of the best possible guesses by random
  int chosenOne = -1;
  for (int i = 0; i < possibleCodeCount; i++) { // Do the same as above (we didn't store it), but counting down guess number to retrieve the corresponding guess.
    int answerCount[25];
    for (int i = 0; i < 25; i++) {
      answerCount[i] = 0;
    }
    int maxAnswerCount = 0;
    for (int j = 0; j < possibleCodeCount; j++) {
      byte answer = getAnswer(possibleCodes[i], possibleCodes[j]);
      int pos = (answer >> 4) * 5 + (answer & 15);
      answerCount[pos]++;
      if (answerCount[pos] > maxAnswerCount) {
        maxAnswerCount++;
      }
    }
    if (maxAnswerCount == bestValue) {
      countDownToChosenOne--;
      if (countDownToChosenOne < 0) {
        chosenOne = possibleCodes[i];
        break;
      }
    }
  }
  return chosenOne; // And this is it (finally!)
}


// Calculates an answer to a guess (given the secrect code).
byte getAnswer(int secretCode, int guess) {
  int colorCount1[COLOR_COUNT];
  int colorCount2[COLOR_COUNT];
  for (int i = 0; i < COLOR_COUNT; i++) {
    colorCount1[i] = 0;
    colorCount2[i] = 0;
  }
  byte answer = 0;
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {  // Look at each light position.
    if ((secretCode & 15) == (guess & 15)) {  // If the color matches exactly the color of the secret code...
      answer += 16;  // ...increase the number of exact matches by one (in the upper 4 bits of the answer).
    } else {
      colorCount1[secretCode & 15]++; // Otherwise count the color for the non-exact matches.
      colorCount2[guess & 15]++;
    }
    secretCode >>= 4;  // Shift color bits to next position and repeat.
    guess >>= 4;
  }
  for (int i = 0; i < COLOR_COUNT; i++) { // Look at each existing color.
    answer += min(colorCount1[i], colorCount2[i]); // See if both the guess and the secret code contained it and how often.
  }
  return answer; // Upper 4 bits: Number of exact matches. Lower 4 bits: Number of non-exact matches.
}

// Create a random secret code for the user to guess.
int createRandomCode() {
  int code = 0;
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {
    code <<= 4;
    code += random(COLOR_COUNT - 1) + 1;
  }
  return code;
}


