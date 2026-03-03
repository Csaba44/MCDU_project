#include <Arduino.h>

#define BAUD_RATE 115200

const uint8_t rows[2] = {52, 50}; // Keep as is
const uint8_t rowCount = sizeof(rows) / sizeof(rows[0]);

const uint8_t cols[1] = {53}; // Keep as is
const uint8_t colCount = sizeof(cols) / sizeof(cols[0]);

uint8_t prevState[colCount][rowCount];

void setupSerialConnection()
{
  Serial.begin(BAUD_RATE);
  Serial.println("--2x2 matrix--");
}

void initPrevState()
{
  for (uint8_t c = 0; c < colCount; c++)
  {
    for (uint8_t r = 0; r < rowCount; r++)
    {
      prevState[c][r] = HIGH;
    }
  }
}

void setupMatrix()
{
  // Initialize rows as OUTPUT and set them HIGH initially
  for (int i = 0; i < rowCount; i++)
  {
    pinMode(rows[i], OUTPUT);
    digitalWrite(rows[i], HIGH);
  }

  // Initialize cols as INPUT_PULLUP
  for (int i = 0; i < colCount; i++)
  {
    pinMode(cols[i], INPUT_PULLUP);
  }
}

void keyPressedEvent(uint8_t row, uint8_t col)
{
  Serial.print("R");
  Serial.print(row + 1);
  Serial.print("C");
  Serial.print(col + 1);
  Serial.println(" pressed");
}

void keyReleasedEvent(uint8_t row, uint8_t col)
{
  Serial.print("R");
  Serial.print(row + 1);
  Serial.print("C");
  Serial.print(col + 1);
  Serial.println(" released");
}

void scanMatrix()
{
  for (int rowIndex = 0; rowIndex < rowCount; rowIndex++)
  {
    uint8_t currRowPin = rows[rowIndex];

    // Enable this row (drive it LOW)
    pinMode(currRowPin, OUTPUT);
    digitalWrite(currRowPin, LOW);

    // Small delay for signal to settle
    delayMicroseconds(10);

    // Read all columns for this row
    for (int colIndex = 0; colIndex < colCount; colIndex++)
    {
      uint8_t currColPin = cols[colIndex];

      // Read column state
      byte state = digitalRead(currColPin);

      if (prevState[colIndex][rowIndex] != state)
      {
        // LOW = press | HIGH = release
        if (state == LOW)
        {
          keyPressedEvent(rowIndex, colIndex);
        }
        else
        {
          keyReleasedEvent(rowIndex, colIndex);
        }
      }

      prevState[colIndex][rowIndex] = state;
    }

    // Disable this row (set it back to HIGH)
    digitalWrite(currRowPin, HIGH);
    pinMode(currRowPin, INPUT);
  }
}

void setup()
{
  setupSerialConnection();
  initPrevState();
  setupMatrix();
}

void loop()
{
  scanMatrix();
}