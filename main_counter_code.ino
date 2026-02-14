// Libraries
#include <Arduino_LED_Matrix.h>

// Activate LED-matrix
ArduinoLEDMatrix matrix;

// PINNEN EN VARIABELEN
int switchPin = 12;    // drukknop
int ledPin = 10;       // ingebouwde LED

int switchState = LOW;
int lastSwitchState = LOW;

int counter = 0;        // grote teller 0-9
int resetCounter = 0;   // kleine cijfers 0-9

unsigned long lastPressTime = 0;
const int debounceDelay = 200; // ms


// DISPLAY BUFFER

byte displayBuffer[8][12];

// GROTE CIJFERS 0-9 (8x12)

byte numbers[10][8][12] = {
  // <<< PLAK HIER JE GROTE 0-9 BITMAPS >>>
    // 0
  {
    {0,1,1,1,1,1,1,0,0,0,0,0},
    {1,1,0,0,0,0,1,1,0,0,0,0},
    {1,1,0,0,1,0,1,1,0,0,0,0},
    {1,1,0,1,0,0,1,1,0,0,0,0},
    {1,1,1,0,0,0,1,1,0,0,0,0},
    {1,1,0,0,0,0,1,1,0,0,0,0},
    {0,1,1,1,1,1,1,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0}
  },

  // 1
  {
    {0,0,1,1,0,0,0,0,0,0,0,0},
    {0,1,1,1,0,0,0,0,0,0,0,0},
    {1,0,1,1,0,0,0,0,0,0,0,0},
    {0,0,1,1,0,0,0,0,0,0,0,0},
    {0,0,1,1,0,0,0,0,0,0,0,0},
    {0,0,1,1,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0}
  },

  // 2
  {
    {0,1,1,1,1,1,0,0,0,0,0,0},
    {1,1,0,0,0,1,1,0,0,0,0,0},
    {0,0,0,0,1,1,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0,0,0},
    {0,0,1,1,0,0,0,0,0,0,0,0},
    {0,1,1,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0}
  },

  // 3
  {
    {1,1,1,1,1,1,0,0,0,0,0,0},
    {0,0,0,0,1,1,0,0,0,0,0,0},
    {0,0,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,1,1,0,0,0,0,0,0},
    {0,0,0,0,1,1,0,0,0,0,0,0},
    {1,1,0,0,1,1,0,0,0,0,0,0},
    {0,1,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0}
  },

  // 4
  {
    {0,0,0,1,1,0,0,0,0,0,0,0},
    {0,0,1,1,1,0,0,0,0,0,0,0},
    {0,1,1,1,1,0,0,0,0,0,0,0},
    {1,1,0,1,1,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0}
  },

  // 5
  {
    {1,1,1,1,1,1,1,0,0,0,0,0},
    {1,1,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,1,1,0,0,0,0,0,0},
    {0,0,0,0,1,1,0,0,0,0,0,0},
    {1,1,0,0,1,1,0,0,0,0,0,0},
    {0,1,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0}
  },

  // 6
  {
    {0,1,1,1,1,0,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,0,0,0,0,0,0,0},
    {1,1,0,0,1,1,0,0,0,0,0,0},
    {1,1,0,0,1,1,0,0,0,0,0,0},
    {1,1,0,0,1,1,0,0,0,0,0,0},
    {0,1,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0}
  },

  // 7
  {
    {1,1,1,1,1,1,1,0,0,0,0,0},
    {0,0,0,0,1,1,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0,0,0},
    {0,0,1,1,0,0,0,0,0,0,0,0},
    {0,1,1,0,0,0,0,0,0,0,0,0},
    {0,1,1,0,0,0,0,0,0,0,0,0},
    {0,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0}
  },

  // 8
  {
    {0,1,1,1,1,0,0,0,0,0,0,0},
    {1,1,0,0,1,1,0,0,0,0,0,0},
    {1,1,0,0,1,1,0,0,0,0,0,0},
    {0,1,1,1,1,0,0,0,0,0,0,0},
    {1,1,0,0,1,1,0,0,0,0,0,0},
    {1,1,0,0,1,1,0,0,0,0,0,0},
    {0,1,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0}
  },

  // 9
  {
    {0,1,1,1,1,0,0,0,0,0,0,0},
    {1,1,0,0,1,1,0,0,0,0,0,0},
    {1,1,0,0,1,1,0,0,0,0,0,0},
    {0,1,1,1,1,1,0,0,0,0,0,0},
    {0,0,0,0,1,1,0,0,0,0,0,0},
    {0,0,0,0,1,1,0,0,0,0,0,0},
    {0,1,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0}
  }
  
};

// KLEINE CIJFERS 0-9 (3x5) RECHTS-ONDER

byte smallNumbers[10][5][3] = {
  // 0
  {
    {1,1,1},
    {1,0,1},
    {1,0,1},
    {1,0,1},
    {1,1,1}
  },
  // 1
  {
    {0,1,0},
    {1,1,0},
    {0,1,0},
    {0,1,0},
    {1,1,1}
  },
  // 2
  {
    {1,1,1},
    {0,0,1},
    {1,1,1},
    {1,0,0},
    {1,1,1}
  },
  // 3
  {
    {1,1,1},
    {0,0,1},
    {0,1,1},
    {0,0,1},
    {1,1,1}
  },
  // 4
  {
    {1,0,1},
    {1,0,1},
    {1,1,1},
    {0,0,1},
    {0,0,1}
  },
  // 5
  {
    {1,1,1},
    {1,0,0},
    {1,1,1},
    {0,0,1},
    {1,1,1}
  },
  // 6
  {
    {1,1,1},
    {1,0,0},
    {1,1,1},
    {1,0,1},
    {1,1,1}
  },
  // 7
  {
    {1,1,1},
    {0,0,1},
    {0,1,0},
    {0,1,0},
    {0,1,0}
  },
  // 8
  {
    {1,1,1},
    {1,0,1},
    {1,1,1},
    {1,0,1},
    {1,1,1}
  },
  // 9
  {
    {1,1,1},
    {1,0,1},
    {1,1,1},
    {0,0,1},
    {1,1,1}
  }
};

// FUNCTIES

// updateMatrix: grote cijfers rechts, kleine links
void updateMatrix() {
  // buffer leegmaken
  for (int y = 0; y < 8; y++)
    for (int x = 0; x < 12; x++)
      displayBuffer[y][x] = 0;

  // ===== KLEINE CIJFERS LINKS-ONDER =====
  // kleine cijfers 3x5, x=0..2, y=3..7
  for (int y = 0; y < 5; y++)
    for (int x = 0; x < 3; x++)
      if (smallNumbers[resetCounter][y][x])
        displayBuffer[y + 3][x] = 1; // links onder

  // ===== GROTE CIJFERS RECHTS =====
  // grote cijfers 8x12, we verschuiven ze naar rechts (x offset = 4)
  for (int y = 0; y < 8; y++)
    for (int x = 0; x < 8; x++)   // gebruik 8 kolommen van grote cijfer
      displayBuffer[y][x + 4] = numbers[counter][y][x]; // rechts

  // render buffer
  matrix.renderBitmap(displayBuffer, 8, 12);
}

// SNEL VUURWERK (non-blocking) – 20 FRAMES

unsigned long lastFrameTime = 0;
const int fireworkFrameDelay = 50; // snelheid per frame
int fireworkFrame = 0;
bool fireworkActive = false;

void startFirework() {
  fireworkActive = true;
  fireworkFrame = 0;
  lastFrameTime = millis();
}

void handleFirework() {
  if (!fireworkActive) return;

  if (millis() - lastFrameTime >= fireworkFrameDelay) {
    lastFrameTime = millis();

    // buffer leegmaken
    for (int y = 0; y < 8; y++)
      for (int x = 0; x < 12; x++)
        displayBuffer[y][x] = 0;

    // willekeurige 3 pixels
    for (int i = 0; i < 3; i++) {
      int x = random(0, 12);
      int y = random(0, 8);
      displayBuffer[y][x] = 1;
    }

    // toon frame
    matrix.renderBitmap(displayBuffer, 8, 12);

    fireworkFrame++;

    // AANPASSING: MEER FRAMES
    if (fireworkFrame >= 50) { // eerder 20 → nu 50 frames
      fireworkActive = false;
      updateMatrix(); // terug naar cijfers
    }
  }
}

// SETUP
void setup() {
  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);
  matrix.begin();
  randomSeed(analogRead(A3)); // seed voor vuurwerk
  updateMatrix();
}

// LOOP

void loop() {
  // lees knop
  switchState = digitalRead(switchPin);

  // debounce + knop detectie
  if (switchState == HIGH && lastSwitchState == LOW) {
    if (millis() - lastPressTime > debounceDelay) {
      counter++;
      digitalWrite(ledPin, HIGH);

      if (counter > 9) {
        counter = 0;
        resetCounter++;

        if (resetCounter > 9) {
          startFirework(); // start vuurwerkshow
          resetCounter = 0;
        }
      }

      updateMatrix();
      lastPressTime = millis();
    }
  }

  if (switchState == LOW) digitalWrite(ledPin, LOW);
  lastSwitchState = switchState;

  // vuurwerk animatie afhandelen
  handleFirework();
}
