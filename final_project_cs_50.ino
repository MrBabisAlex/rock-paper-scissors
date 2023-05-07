#include <SPI.h>  //include libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "bitmaps.h"  //include bitmaps

long test1 = A0;  // set pins
const int buttonPinUp = 2;  
const int buttonPinDown = 7;
const int buttonPinLeft = 3;
const int buttonPinRight = 6;
const int buttonPinEnter = 5;
const int buttonPinBack = 4;
const int chrono = 200; // set delay
int counter  = 0; // initialize counter

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)                  
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3c ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  
  pinMode (buttonPinUp  , INPUT_PULLUP); // initialize pins with internal pull up
  pinMode (buttonPinDown  , INPUT_PULLUP);
  pinMode (buttonPinEnter , INPUT_PULLUP);
  pinMode (buttonPinLeft , INPUT_PULLUP);
  pinMode (buttonPinRight , INPUT_PULLUP);
  pinMode (buttonPinBack , INPUT_PULLUP);
  randomSeed(analogRead(0)); //reset seed to random value from floating analog pin

  intro();  //play intro
}

void loop()
{
  if ((counter == 0) && (EnterButton())) { //when enter is pushed display main menu
    mainMenu();
    counter = 1;  //initialize counter to 1
    arrows();
  }

  if ((counter != 0) && (UpButton())) {  //use up down button yo navigate
    counter ++;
    arrows();
    if (counter > 3) {
      counter = 3;
    }
  }

  if ((counter != 0) && (DownButton())) {
    counter --;
    arrows();
    if (counter < 1) {
      counter = 1;
    }
  }

  if ((counter == 1) && (EnterButton())) {  //use counter and enter button to use each function
    game();
  }

  if ((counter == 2) && (EnterButton())) {
    rules();
  }

  if ((counter == 3) && (EnterButton())) {
    credits();
  }
}

void intro() {  //intro function
  display.clearDisplay();                                              //clear display
  display.drawBitmap(0, 0, cs50, bitmap_height, bitmap_width, WHITE);  //use bitmap library for the logo
  display.display();                                                   //print image
  delay(chrono);
  display.setTextSize(1);                                              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);                                 // Draw white text
  display.setCursor(30, 45);                                           // set doordinates
  display.println(F("PRESS ENTER"));                                   //print string
  display.setCursor(30, 55);
  display.println(F("TO CONTINUE!"));
  display.display();
  delay(50);
}

void mainMenu() {  //main menu function
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("MAIN MENU"));
  display.setTextSize(1);
  display.setCursor(40, 20);
  display.println(F("Play Game"));
  display.setCursor(40, 35);
  display.println(F("Rules"));
  display.setCursor(40, 50);
  display.println(F("Credit"));
  display.display();
}

void arrows() {  //arrow function
  switch (counter)
  {
    case 1: //every case changes the coordinates for the arrow
      mainMenu();
      display.drawBitmap(0, 20, arrow, height, width, WHITE);
      display.display();
      delay(chrono);
      break;

    case 2:
      mainMenu();
      display.drawBitmap(0, 35, arrow, height, width, WHITE);
      display.display();
      delay(chrono);
      break;

    case 3:
      mainMenu();
      display.drawBitmap(0, 50, arrow, height, width, WHITE);
      display.display();
      delay(chrono);
      break;

    default:
      break;
  }
}

void rules() {  //rules function
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Rock wins scissors"));
  display.println(F("paper wins rock"));
  display.println(F("and scissors wins"));
  display.println(F("paper."));
  display.display();
  delay(5000);
  
  while (!BackButton()) {  //exit only when back button is pressed
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 20);
    display.println(F("   press back "));
    display.println(F("    to continue"));
    display.display();
    delay(50);
    Serial.print(BackButton());
    if (BackButton()) {
      Serial.print(BackButton());
      break;
    }
  }
  arrows();
}

void credits() { //credits function using the center text function
  int t = 3000;
  int n = 2000;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  textDisplayCenter("Created by:");
  delay(n);
  textDisplayCenter("C. Alexiadis");
  delay(n);
  textDisplayCenter("Special thanks to:");
  delay(n);
  textDisplayCenter("David J. Malan");
  delay(n);
  textDisplayCenter("Doug Loyd");
  delay(n);
  textDisplayCenter("Brian Yu");
  delay(n);

  while (!BackButton()) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 20);
    display.println(F("   press back "));
    display.println(F("    to continue"));
    display.display();
    delay(50);
    Serial.print(BackButton());
    if (BackButton()) {
      Serial.print(BackButton());
      break;
    }
  }
  arrows();
}

void game() {  //game function
  int player = 0;
  int CPU = 0;
  int selector = 0;
  int number = 0;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Use arrows to select!"));
  display.display();
  delay(chrono);
  
  while (player < 3 && CPU < 3) {  //check the score if none reached 3 wins continue
    if (RightButton()) {           //use right and left buttons for selection
      delay(chrono);
      selector ++;
      if (selector >= 3) {
        selector = 3;
      }
      display.clearDisplay();
      selection(selector, 10, 10);
    }

    if (LeftButton()) {
      delay(chrono);
      selector --;
      if (selector <= 1) {
        selector = 1;
      }
      display.clearDisplay();
      selection(selector, 10 , 10);
    }

    if (EnterButton() && selector != 0) {
      delay (chrono);
      display.clearDisplay();
      number = selector;
      selection(selector, 10 , 10);
      int randNumber = random(1, 4);
      selection(randNumber, 80, 10);
      display.clearDisplay();
      delay(600);

      if ((number > randNumber) && (!(number == 3 && randNumber == 1))) {
        Serial.println("player wins!!");
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 10);
        display.println(F("  Player"));
        display.println(F("  wins!!"));
        display.display();
        player++;
      }
      else if (number == randNumber) {
        Serial.println("It's a Draw!!");
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 10);
        display.println(F("  It's a"));
        display.println(F("  Draw!!"));
        display.display();
      }
      else if (number == 3 && randNumber == 1) {
        Serial.println("CPU wins!!");
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 10);
        display.println(F("   CPU "));
        display.println(F("  wins!!"));
        display.display();
        CPU++;
      }
      else if (number == 1 && randNumber == 3) {
        Serial.println("Player wins!");
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 10);
        display.println(F("  Player"));
        display.println(F("  wins!!"));
        display.display();
        player++;
      }
      else {
        Serial.println("CPU wins!!");
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 10);
        display.println(F("   CPU "));
        display.println(F("  wins!!"));
        display.display();
        CPU++;
      }
      if (player == 3) {  //check for winner
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 10);
        display.println(F("  Player"));
        display.println(F("   wins"));
        display.println(F(" the game!"));
        display.display();
        delay(2000);
      }
      else if (CPU == 3) {  //check for winner
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 10);
        display.println(F(" CPU wins "));
        display.println(F("the game!!"));
        display.display();
        delay(2000);
      }
    }
  }
  while (!BackButton()) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 20);
    display.println(F("   press back "));
    display.println(F("    to continue"));
    display.display();
    Serial.print(BackButton());
    if (BackButton()) {
      Serial.print(BackButton());
      break;
    }
  }
  arrows();
}

void selection(int draw, int x , int y) {  //print the animations
  switch (draw)
  {
    case 1:
      display.drawBitmap(x, y, paper, paper_height, paper_width, WHITE);
      display.display();
      delay(chrono);
      break;

    case 2:
      display.drawBitmap(x, y, scissors, scissors_height, scissors_width, WHITE);
      display.display();
      delay(chrono);
      break;

    case 3:
      display.drawBitmap(x, y, rock, rock_height, rock_width, WHITE);
      display.display();
      delay(chrono);
      break;

    default:
      break;
  }
}

boolean EnterButton() {  //boolean button functions
  int buttonEnterState = 0;
  int buttonEnterLastState = 1;
  buttonEnterState = digitalRead(buttonPinEnter);  //read the value from pin

  if (buttonEnterState != buttonEnterLastState) {  //if value has changed return true
    buttonEnterLastState = buttonEnterState;       //remember new value
    return true;
  }
  return false;
}

boolean UpButton() {
  int buttonUpState = 0;
  int buttonUpLastState = 1;
  buttonUpState = digitalRead(buttonPinUp);

  if (buttonUpState != buttonUpLastState) {
    buttonUpLastState = buttonUpState;
    return true;
  }
  return false;
}

boolean DownButton() {
  int buttonDownState = 0;
  int buttonDownLastState = 1;
  buttonDownState = digitalRead(buttonPinDown);

  if (buttonDownState != buttonDownLastState) {
    buttonDownLastState = buttonDownState;
    return true;
  }
  return false;
}
boolean LeftButton() {
  int buttonLeftState = 0;
  int buttonLeftLastState = 1;
  buttonLeftState = digitalRead(buttonPinLeft);

  if (buttonLeftState != buttonLeftLastState) {
    buttonLeftLastState = buttonLeftState;
    return true;
  }
  return false;
}

boolean RightButton() {
  int buttonRightState = 0;
  int buttonRightLastState = 1;
  buttonRightState = digitalRead(buttonPinRight);

  if (buttonRightState != buttonRightLastState) {
    buttonRightLastState = buttonRightState;
    return true;
  }
  return false;
}

boolean BackButton() {
  int buttonBackState = 0;
  int buttonBackLastState = 1;
  buttonBackState = digitalRead(buttonPinBack);

  if (buttonBackState != buttonBackLastState) {
    buttonBackLastState = buttonBackState;
    return true;
  }
  return false;
}

void textDisplayCenter(String text) {
  int16_t x1;
  int16_t y1;
  uint16_t wwidth;
  uint16_t hheight;

  display.getTextBounds(text, 0, 0, &x1, &y1, &wwidth, &hheight);
  display.clearDisplay(); 
  display.setCursor((SCREEN_WIDTH - wwidth) / 2, (SCREEN_HEIGHT - hheight) / 2);
  display.println(text); 
  display.display();
}
