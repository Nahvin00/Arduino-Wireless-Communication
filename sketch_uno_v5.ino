// SlaveSwapRoles

#include <Wire.h>
#include <LiquidCrystal_I2C.h> //add I2C library


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define CE_PIN  9
#define CSN_PIN 8

#define joyX A0  
#define joyY A1  

LiquidCrystal_I2C lcd(0x27,16,2);     

const byte slaveAddress[5] = {'R','x','A','A','A'};
const byte masterAddress[5] = {'T','X','a','a','a'};

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

int dataReceived; // must match dataToSend in master
int replyData[2] = {0,0}; // the two values to be sent to the master
bool newData = false;

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1000; // send once per second


void setup() {

    Serial.begin(9600);

    Serial.println("SlaveSwapRoles Starting");

    lcd.init(); //Initialize the LCD
	  lcd.backlight();  //Set LCD background bright
    lcd.setCursor(0,0);  
    lcd.print("SlaveSwapRoles");


    radio.begin();
    radio.setDataRate( RF24_250KBPS );

    radio.openWritingPipe(masterAddress); // NB these are swapped compared to the master
    radio.openReadingPipe(1, slaveAddress);

    radio.setRetries(3,5); // delay, count
    radio.startListening();

}

//====================

void loop() {
    getData();
    showData();
    send();
}

//====================

void send() {
    if (newData == true) {
        replyData[0] = analogRead(joyX);  
        replyData[1] = analogRead(joyY);  

        lcd.setCursor(0,1);
        radio.stopListening();
            bool rslt;
            rslt = radio.write( &replyData, sizeof(replyData));
        radio.startListening();

        Serial.print("Reply Sent ");
        Serial.print(replyData[0]);
        Serial.print(", ");
        Serial.println(replyData[1]);

        if (rslt) {
            Serial.println("Acknowledge Received");
            lcd.print("S: " + String(replyData[0]) + ", " + String(replyData[1]));
        }
        else {
            Serial.println("Tx failed");
            lcd.print("Tx failed");
        }
        Serial.println();
        newData = false;
    }
}

//================

void getData() {

    if ( radio.available() ) {
        radio.read( &dataReceived, sizeof(dataReceived) );
        newData = true;
    }
}

//================

void showData() {
    if (newData == true) {
        Serial.print("Data received ");
        Serial.println(dataReceived);
        lcd.clear();  
        lcd.setCursor(0,0);
    	  lcd.print("R: " + String(dataReceived));  

    }
}