#include <Arduino.h>

#define interruptPin 3 // For Button Input

#define runDelay 50

int incomingMode = 0;                       // for Mode input by da user
int ledPins[] = {4, 5, 6, 7, 8, 9, 10, 11}; //An array for all your favourite led pins!
int pressCounter = 0;                       // To count button presses
int debouncer = 100;                        // To wait in milliseconds
bool pushDown = false;                      // for the button problem
int state;
int timeCounter = 0; //To count time at the third mode
int userInput;       // To hold the number sent by the user
int modeChosen = 0;

const byte numPins = 8; // num of leds

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);

    //Led pins set to be output
    for (int i = 0; i < numPins; i++)
    {
        pinMode(ledPins[i], OUTPUT);
    }
    pinMode(interruptPin, INPUT);

    //waiting for input
    Serial.println("Enter:");
    Serial.println("1 for running lights");
    Serial.println("2 for running lights with pause and resume");
    Serial.println("3 for binary count up - on timer");
    Serial.println("4 for binary count up - on button");
    Serial.println("5 for binary display of numbers [1-255]");

    //Try moving the modselect here

    if (Serial.available() > 0)
        modeChosen = Serial.read(); //Read input and save it to a variable

    Serial.println("You chose wisely!");
    Serial.println("Now running gamemode ");
    Serial.print(modeChosen);
}

void loop()
{
    if (modeChosen) // If the user has selected a gamemode, run the switch and methods
    {
        switch (modeChosen)
        {
        case 1:
            /* code */
            RunningLights();
            break;
        case 2:
            //Vanilla Code will be run by attaching an interrupt first
            attachInterrupt(digitalPinToInterrupt(interruptPin), StopTheLights, CHANGE);
            //TODO: Make method stop till further change
            //TODO: Use *CHANGE* with a loop and debouncing
            RunningLights();
            break;
        case 3:
            TimerCountUp();
            break;
        case 4:
            //TODO: Decide whether to attach interrupt here or to Setup
            //attachInterrupt(digitalPinToInterrupt(interruptPin), AddCount, RISING); //Use the button to call the increment method
            break;
        case 5:
            //code
            break;

        default:
            Serial.println("Weird Mod Chosen, please reset machine");
            break;
        }
    }
}

void RunningLights()
{ //Vanilla, Mode 1

    while (true)
    { //Runs forever
        for (int i = 0; i < numPins; i++)
        {
            digitalWrite(ledPins[i], HIGH); //take the pin and light it up
            delay(runDelay);                //Wait For It..
            digitalWrite(ledPins[i], LOW);  // lights out!
        }                                   //First loop to go right to left

        for (int i = numPins; i > -1; i--)
        {
            digitalWrite(ledPins[i], HIGH);
            delay(runDelay); //Wait For It..
            digitalWrite(ledPins[i], LOW);
        } // Second loop to go left to right
    }
}


void TimerCountUp() //Mode 3 Count in Binary with timer
{

    while (true)
    {

        timeCounter++;               //Increase 1 second each cycle
        Serial.println(timeCounter); //Show the current second we are in

        //I took the following binary convertion code from the internet
        /* convert presses to binary and store it as a string */
        String binNumber = String(timeCounter, BIN);
        /* get the length of the string */
        int binLength = binNumber.length();
        if (timeCounter <= 255)
        { // if we have less or equal to 255 presses
            // here is the scary code
            for (int i = 0, x = 1; i < binLength; i++, x += 2)
            {
                if (binNumber[i] == '0')
                    state = LOW;
                if (binNumber[i] == '1')
                    state = HIGH;
                digitalWrite(ledPins[i] + binLength - x, state);
            }
        }
        else
        {
            Serial.println("Congratulations, you pressed the button too much!");
        }
        delay(995); //Not 1000 ms to be more accurate
    }
}

/* Unneccessary now\ due to including in Switch, will be removed in next push
TODO:Include, debounce and delete method
void ButtonCountUp() //Mode 4, Count in binary with the button
{
    attachInterrupt(digitalPinToInterrupt(interruptPin), AddCount, RISING); //Use the button to call the increment method
}
/*/

void BinaryDisplay() //Mode 5, Display a number entered by the user
{
    userInput = 0;
    int oldUserInput;

    while (true)
    {
        if (Serial.available() > 0)
        {
            userInput = Serial.parseInt();

            Serial.println("User input is ");
            Serial.println(userInput);

            /* convert presses to binary and store it as a string */
            String binNumber = String(userInput, BIN);
            /* get the length of the string */
            int binLength = binNumber.length();
            if (userInput <= 255)
            { // if we have less or equal to 255 presses
                // here is the scary code
                for (int i = 0, x = 1; i < binLength; i++, x += 2)
                {
                    if (binNumber[i] == '0')
                        state = LOW;
                    if (binNumber[i] == '1')
                        state = HIGH;
                    digitalWrite(ledPins[i] + binLength - x, state);
                }
            }
            else
            {
                Serial.println("We can not display that number");
            }
        }
    }
}

void StopTheLights()
{ //TODO: FIX THIS
    Serial.println("OMG STOP TOUCHING ME!");
    Serial.println(digitalRead(interruptPin));
    pushDown = !pushDown;

    while (pushDown)
    { // I can not get out of this loop...
        delay(10);
    }
}

void AddCount()
{ // Function to increase count by one and update the lights
    pressCounter++;
    Serial.println(pressCounter);
    delay(debouncer); //TODO: Test for this

    /* convert presses to binary and store it as a string */
    String binNumber = String(pressCounter, BIN);
    /* get the length of the string */
    int binLength = binNumber.length();
    if (pressCounter <= 255)
    { // if we have less or equal to 255 presses
        // here is the scary code
        for (int i = 0, x = 1; i < binLength; i++, x += 2)
        {
            if (binNumber[i] == '0')
                state = LOW;
            if (binNumber[i] == '1')
                state = HIGH;
            digitalWrite(ledPins[i] + binLength - x, state);
        }
    }
    else
    {
        Serial.println("Congratz you pressed the button too much!");
    }
}