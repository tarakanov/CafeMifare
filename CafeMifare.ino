/*
 * ----------------------------------------------------------------------------
 * This is a MFRC522 library example; see https://github.com/miguelbalboa/rfid
 * for further details and other examples.
 * 
 * NOTE: The library file MFRC522.h has a lot of useful info. Please read it.
 * 
 * Released into the public domain.
 * ----------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC (that is: a RFID
 * Tag or Card) using a MFRC522 based RFID Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout
 * below), load this sketch into Arduino IDE then verify/compile and upload it.
 * To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M).
 * When you present a PICC (that is: a RFID Tag or Card) at reading distance
 * of the MFRC522 Reader/PCD, the serial output will show the ID/UID, type and
 * any data blocks it can read. Note: you may see "Timeout in communication"
 * messages when removing the PICC from reading distance too early.
 * 
 * If your reader supports it, this sketch/program will read all the PICCs
 * presented (that is: multiple tag reading). So if you stack two or more
 * PICCs on top of each other and present them to the reader, it will first
 * output all details of the first and then the next PICC. Note that this
 * may take some time as all data blocks are dumped, so keep the PICCs at
 * reading distance until complete.
 * 
 * Typical pin layout used:
 * ------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino
 *             Reader/PCD   Uno           Mega      Nano v3
 * Signal      Pin          Pin           Pin       Pin
 * ------------------------------------------------------------
 * RST/Reset   RST          9             5         D9
 * SPI SS      SDA(SS)      10            53        D10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11
 * SPI MISO    MISO         12 / ICSP-1   50        D12
 * SPI SCK     SCK          13 / ICSP-3   52        D13
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN		9		// 
#define SS_PIN		10		//
#define LED              8  //status led
MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance

void setup() {
        pinMode(LED, OUTPUT);
        digitalWrite(LED, LOW);
       	Serial.begin(115200);		// Initialize serial communications with the PC
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	ShowReaderDetails();	// Show details of PCD - MFRC522 Card Reader details

}

void loop() {
      digitalWrite(LED, HIGH);

  if (getID()) {
    digitalWrite(LED, HIGH); // Light the LED for success reading
    delay(100);
    digitalWrite(LED, LOW);
        delay(100);
    digitalWrite(LED, HIGH);
        delay(100);
    digitalWrite(LED, LOW);
    delay(100);
    digitalWrite(LED, HIGH);
  }
}

int getID() {
  byte readCard[4];           // Stores scanned ID read from RFID Module

  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
    return 0;
  }
  // There are Mifare PICCs which have 4 byte or 7 byte UID care if you use 7 byte PICC
  // I think we should assume every PICC as they have 4 byte UID
  // Until we support 7 byte PICCs
  Serial.println("Scanned PICC's UID:");
  for (int i = 0; i < 4; i++) {  // 
    readCard[i] = mfrc522.uid.uidByte[i];
    Serial.print(readCard[i], HEX);
  }
  Serial.println("");
  mfrc522.PICC_HaltA(); // Stop reading
  return 1;
}

void ShowReaderDetails() {
	// Get the MFRC522 software version
	byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
        Serial.println("CafeMifare Reader");
  
        while (!Serial.available()) {delay(100);}

        if (Serial.read() == 2)  digitalWrite(LED, HIGH);

        Serial.println("More info - github.com/");
	Serial.print("MFRC522 Software Version: 0x");
	Serial.print(v, HEX);
	if (v == 0x91)
		Serial.print(" = v1.0");
	else if (v == 0x92)
		Serial.print(" = v2.0");
	else
		Serial.print(" (unknown)");
	Serial.println("");
	// When 0x00 or 0xFF is returned, communication probably failed
	if ((v == 0x00) || (v == 0xFF)) {
		Serial.println("WARNING: Communication failure, is the MFRC522 properly connected?");
	}
}
