// MasterSwapRoles
#include <Servo.h>  
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define CE_PIN  9
#define CSN_PIN 8
#define PIN_AO 2

const byte slaveAddress[5] = {'R','x','A','A','A'};
const byte masterAddress[5] = {'T','X','a','a','a'};


RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

Servo myservo;
int pos = 0;

int dataToSend;
char txNum = '0';
int dataReceived[2]; // to hold the data from the slave - must match replyData[] in the slave
bool newData = false;

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1000; // send once per second

//============

void setup() {

    pinMode(PIN_AO, INPUT);

    Serial.begin(9600);

    Serial.println("MasterSwapRoles Starting");

    myservo.attach(10);

    radio.begin();
    radio.setDataRate( RF24_250KBPS );

    radio.openWritingPipe(slaveAddress);
    radio.openReadingPipe(1, masterAddress);

    radio.setRetries(3,5); // delay, count
    send(); // to get things started
    prevMillis = millis(); // set clock
}

//=============

void loop() {
    currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis) {
        send();
        prevMillis = millis();
    }
    getData();
    showData();
}

//====================

void send() {

        radio.stopListening();
        dataToSend = int(analogRead(PIN_AO));
        bool rslt;
        rslt = radio.write(&dataToSend, sizeof(dataToSend));
        radio.startListening();
        Serial.print("Data Sent ");
        Serial.print(dataToSend);
        if (rslt) {
            Serial.println("  Acknowledge received");
        }
        else {
            Serial.println("  Tx failed");
        }
}

//================

void getData() {

    if ( radio.available() ) {
        radio.read( &dataReceived, sizeof(dataReceived) );
        pos = map(dataReceived[0], 0, 1023, 0, 180);
        myservo.write(pos);
        newData = true;
    }
}

//================

void showData() {
    if (newData == true) {
        Serial.print("Data received ");
        Serial.println(dataReceived[0]);
        // Serial.print(", ");
        // Serial.println(dataReceived[1]);
        Serial.println();
        newData = false;
    }
}
