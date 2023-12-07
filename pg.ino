#include "Servo.h"
Servo myservo; // create servo object to control a servo

#define ServoM 4 // Connected to the servo motor.
#define Keluar 3 // Pin connected to the EXIT button.
#define Masuk 5  // Pin connected to the IN button.

#define BarLow 180  // Low position of the barrier.
#define BarUp 93    // Up position of the barrier.
#define Kapasitas 0 // Capacity of the parking lot.
#define INTEN 80    // Display intensity %

// Pins conections to segments (cathodes).
#define segA 13
#define segB 12
#define segC 11
#define segD 10
#define segE 9
#define segF 8
#define segG 7
#define segDP 6

// Array with the segments to represent the decimal numbers (0-9).
byte segments[10] = {
    // pgfedcba  <--- segments
    B00111111, // number 0
    B00000110, // number 1
    B01011011, // number 2
    B01001111, // number 3
    B01100110, // number 4
    B01101101, // number 5
    B01111101, // number 6
    B00000111, // number 7
    B01111111, // number 8
    B01101111  // number 9
};

// Seven-segment display pins
const byte segPin[8] = {segA, segB, segC, segD, segE, segF, segG, segDP};
const byte digitPin[2] = {A0, A1}; // Common cathode pins for two 7-segment displays

const int buttonPinMasuk = 5;
const int buttonPinKeluar = 3; // Assuming Pin 2 for EXIT button

// Variables will change
int buttonStateMasuk = 0;
int lastButtonStateMasuk = 0;
int buttonStateKeluar = 0;
int lastButtonStateKeluar = 0;
int Tersedia = 0; // Number of places available.

void setup()
{
    myservo.attach(ServoM); // attaches the servo.

    pinMode(Keluar, INPUT_PULLUP); // set "EXIT" button pin to input
    pinMode(Masuk, INPUT_PULLUP);  // set "IN" button pin to input
    digitalWrite(Keluar, HIGH);    // Connect Pull-Up resistor.
    digitalWrite(Masuk, HIGH);     // Connect Pull-Up resistor.
    pinMode(segA, OUTPUT);
    pinMode(segB, OUTPUT);
    pinMode(segC, OUTPUT);
    pinMode(segD, OUTPUT);
    pinMode(segE, OUTPUT);
    pinMode(segF, OUTPUT);
    pinMode(segG, OUTPUT);
    pinMode(segDP, OUTPUT);
    analogWrite(segDP, 255 * INTEN / 100);
    myservo.write(BarLow); // Barrier in the low position

    for (int i = 0; i < 8; i++)
    {
        pinMode(segPin[i], OUTPUT);
    }
    for (int j = 0; j < 2; j++)
    {
        pinMode(digitPin[j], OUTPUT);
        digitalWrite(digitPin[j], HIGH);
    }

    pinMode(buttonPinMasuk, INPUT_PULLUP);
    pinMode(buttonPinKeluar, INPUT_PULLUP);
    displayDigit(Tersedia);
}

void loop()
{
    buttonStateMasuk = digitalRead(buttonPinMasuk);
    buttonStateKeluar = digitalRead(buttonPinKeluar);

    if (buttonStateMasuk != lastButtonStateMasuk)
    {
        if (buttonStateMasuk == LOW)
        {
            if (buttonStateMasuk == LOW && buttonStateKeluar == LOW)
            {
                myservo.write(BarLow);
            }
            else
            {
                if (Tersedia == Kapasitas)
                {
                    Tersedia = Kapasitas; // Reset counter after reaching 0
                }
                myservo.write(BarUp);
                // You can choose whether to wait for a specific duration or not here
            }
        }
        delay(50); // Debouncing delay
    }

    if (buttonStateKeluar != lastButtonStateKeluar)
    {
        if (buttonStateKeluar == LOW)
        {
            Tersedia++;
            if (Tersedia < 0)
            {
                Tersedia = Kapasitas; // Reset counter after reaching capacity
            }
            myservo.write(BarLow);
            // You can choose whether to wait for a specific duration or not here
        }
        else if (buttonStateKeluar == HIGH && buttonStateMasuk == LOW)
        {
            myservo.write(BarUp);
            // You can choose whether to wait for a specific duration or not here
        }
        delay(50); // Debouncing delay
    }

    lastButtonStateMasuk = buttonStateMasuk;
    lastButtonStateKeluar = buttonStateKeluar;

    turnOff();
    displayDigit(Tersedia);
}

void displayDigit(int number)
{
    int digit1 = number / 10;
    int digit2 = number % 10;

    // Display the tens digit
    digitalWrite(digitPin[1], LOW);
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(segPin[i], bitRead(segments[digit1], i));
    }
    delayMicroseconds(1000);
    digitalWrite(digitPin[1], HIGH);

    // Display the ones digit
    digitalWrite(digitPin[0], LOW);
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(segPin[i], bitRead(segments[digit2], i));
    }
    delayMicroseconds(1000);
    digitalWrite(digitPin[0], HIGH);
}

void turnOff()
{
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(segPin[i], LOW);
    }
}
