#include <Adafruit_LEDBackpack.h>

#define BUTTON_START 2
#define BUTTON_FINISH 3

#define A 11
#define B 10
#define C 9
#define D 8
#define E 7
#define F 6
#define G 5
#define H 4

unsigned long startTime = 0;
unsigned long lapTime = 0;
unsigned long prevLapTime = 0;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200;
unsigned int currentLap = 0;
bool raceStarted = false;

Adafruit_7segment currentLapDisplay = Adafruit_7segment();
Adafruit_7segment lastLapDisplay = Adafruit_7segment();

const int digitStates[10][8] = {
   {LOW, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW},  // 0
   {LOW, LOW, LOW, HIGH, LOW, LOW, HIGH, LOW},     // 1
   {HIGH, LOW, HIGH, HIGH, HIGH, HIGH, LOW, LOW},  // 2
   {HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH, LOW},  // 3
   {HIGH, HIGH, LOW, HIGH, LOW, LOW, HIGH, LOW},   // 4
   {HIGH, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW},  // 5
   {HIGH, HIGH, HIGH, LOW, HIGH, HIGH, HIGH, LOW}, // 6
   {LOW, LOW, HIGH, HIGH, LOW, LOW, HIGH, LOW},    // 7
   {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW},// 8
   {HIGH, HIGH, HIGH, HIGH, LOW, HIGH, HIGH, LOW}  // 9
};

void displayDigit(int digit) {
   if (digit < 0 || digit > 9) return;
   for (int i = 0; i < 8; ++i) {
       digitalWrite(A - i, digitStates[digit][i]);
   }
}

void setup() {
  pinMode(BUTTON_START, INPUT_PULLUP);
  pinMode(BUTTON_FINISH, INPUT_PULLUP);
  
  lastLapDisplay.begin(0x70);
  currentLapDisplay.begin(0x71);
  
  
  for (int i = A; i <= H; ++i) {
    pinMode(i, OUTPUT);
  }
    
  displayDigit(0);
}

void loop() {
  unsigned long currentTime = millis();

  if (digitalRead(BUTTON_START) == HIGH && !raceStarted) {
    startTime = currentTime;
    raceStarted = true;
  }

  if (digitalRead(BUTTON_FINISH) == HIGH && raceStarted) {
    if (currentTime - lastDebounceTime >= debounceDelay) {
      if(currentLap < 9) {
        prevLapTime = lapTime;
        lastLapDisplay.println(prevLapTime / 1000);
        lastLapDisplay.writeDisplay();

        currentLap++;
        
        startTime = millis();
        lastDebounceTime = currentTime;
      } else {
      	startTime = millis();
        lastDebounceTime = currentTime;
        currentLap = 0;
        prevLapTime = 0;
        lapTime = 0; 
        
        lastLapDisplay.println(prevLapTime / 1000);
        lastLapDisplay.writeDisplay();
	
        currentLapDisplay.println(currentLap / 1000);
        currentLapDisplay.writeDisplay();       
        
        raceStarted = false;
      }
      
      displayDigit(currentLap);
    }
  }
  
  if(raceStarted) {
	lapTime = currentTime - startTime;
  }

  currentLapDisplay.println(lapTime / 1000);
  currentLapDisplay.writeDisplay();
}