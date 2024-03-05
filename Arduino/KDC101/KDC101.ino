#include <SPI.h>
#include <SD.h>

const int chipSelect = 10; // Set the chip select pin of the SD card module

void setup() {
  // Open a serial connection
  Serial.begin(9600);

  // Initialize the SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("Initialization failed!");
    return;
  }

  Serial.println("Initialization done.");
}

void loop() {
  // Open or create a file named "data.txt" on the SD card
  File dataFile = SD.open("data.txt", FILE_WRITE);

  // Check if the file opened successfully
  if (dataFile) {
    // Write data to the file
    dataFile.println("Hello, world!");
    // Close the file
    dataFile.close();
    Serial.println("Data written to file.");
  } else {
    // If the file failed to open, print an error message
    Serial.println("Error opening data.txt");
  }

  // Wait for a second
  delay(1000);
}
