#include <cppQueue.h>
#include <LedControl.h>

#define DIN 11
#define CS 7
#define CLK 13
#define JOYSTICK_X A1
#define JOYSTICK_Y A0
#define START_LENGTH 5
#define POS_UPDATE_TIME 500
#define APPLE_UPDATE_TIME 250

// Vector stuff
class Vector {

  int calcSwapping(int value) {
    if (value == 0) return 0;
    else if (value > 0) return value % 8;
    else return 8 + (value % 8);
  }
  
  public:
    int x;
    int y;

    Vector(int x, int y) {
      this->x = x;
      this->y = y;
    }

    Vector() {
      this->x = 0;
      this->y = 0;
    }

    Vector add(int x, int y) {
      return Vector(calcSwapping(this->x + x), calcSwapping(this->y + y));
    }

    Vector add(Vector vect) {
      return this->add(vect.x, vect.y);
    }

    bool sumInBounds(int x, int y) {
      int newX = this->x + x;
      int newY = this->y + y;

      return newX >=0 && newX < 8 && newY >=0 && newY < 8;
    }

    bool sumInBounds(Vector vect) {
      return sumInBounds(vect.x, vect.y);
    }

    Vector opposite() {
      return Vector(-this->x, -this->y);
    }

    bool isZero() {
      return this->x == 0 && this->y == 0;
    }

    bool equalTo(Vector vect) {
      return this->x == vect.x && this->y == vect.y;
    }

    void print() {
      Serial.print("[");
      Serial.print(this->x);
      Serial.print(",");
      Serial.print(this->y);
      Serial.print("]");
    }

    void println() {
      this->print();
      Serial.println();
    }
  
};

Vector randVect() {
  return Vector((int) random(0, 8), (int) random(0, 8));
}

Vector randDir() {
  int rand = random(0, 4);
  switch (rand) {
    case 0: return Vector(0, 1);
    case 1: return Vector(0, -1);
    case 2: return Vector(1, 0);
    default: return Vector(-1, 0);
  }
}

Vector velocity = Vector(0, 0);

cppQueue snake(sizeof(Vector), 64, FIFO);

bool snakePixels[8][8];

Vector freePixels[64];

bool stopped;

bool joystickLock = false;

Vector applePos = Vector(-1, -1);

LedControl lc = LedControl(DIN, CLK, CS,0);

long posUpdateMillis = 0;

long appleMillis = 0;

//AdvanceSevenSegment pointDisplay1 = sevenSegment(52, 50, 48, 46, 44, 42, 40, 38);
//AdvanceSevenSegment pointDisplay2 = sevenSegment(53, 51, 49, 47, 45, 43, 41, 39);

void setPix(Vector pos, bool state) {
  lc.setLed(0,pos.x,pos.y,state);
}

void spawnSnake() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      snakePixels[i][j] = false;
    }
  }
  
  Vector current = randVect();
  snake.push(&current);
  setPix(current, true);
  snakePixels[current.x][current.y] = true;

  Vector dir = randDir();

  for (int i = 1; i < START_LENGTH; i++) {
    current = current.add(dir);
    snake.push(&current);
    setPix(current, true); 
    snakePixels[current.x][current.y] = true;
  }

  velocity = dir;
  
}

void spawnApple() {
  int freeCount = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (!snakePixels[i][j]) {
        freePixels[freeCount] = Vector(i, j);
        freeCount++;
      }
    }
  }

  setPix(applePos, false);
  applePos = freePixels[random(0, freeCount)];
  setPix(applePos, true);
}

// Makes game start
void gameStart() {
  randomSeed(analogRead(A2));

  spawnSnake();
  spawnApple();
}

void setup() {
  
  Serial.begin(9600);

  pinMode(JOYSTICK_X, INPUT);
  pinMode(JOYSTICK_Y, INPUT);
  
  lc.shutdown(0,false);
  lc.setIntensity(0,0);
  lc.clearDisplay(0);

  gameStart();
  
}

// Updates snake position periodically
void updatePos() {
  if (millis() >= posUpdateMillis) {
    Vector newPos = Vector(0, 0);
    snake.peekPrevious(&newPos);
    newPos = newPos.add(velocity);
    bool turnOffRear = true;

    if (snakePixels[newPos.x][newPos.y]) {
      Vector rear = Vector(0, 0);
      snake.peek(&rear);
      if (!rear.equalTo(newPos)) {
        setPix(applePos, false);
        stopped = true;
        return;
      } else {
        turnOffRear = false;
      }
      
    }
    
    snake.push(&newPos);
    setPix(newPos, true);
    snakePixels[newPos.x][newPos.y] = true;

    if (applePos.equalTo(newPos)) {
      spawnApple();
      setPix(newPos, true);
    } else {
      Vector rear = Vector(0, 0);
      snake.pop(&rear);
      if (turnOffRear) setPix(rear, false);
      snakePixels[rear.x][rear.y] = false;
    }

    joystickLock = false;
    posUpdateMillis = millis() + POS_UPDATE_TIME;
  }
}

// Reads input from joystick and sets direction of snake movement
void updateVelocity() {
  if (joystickLock) return;
  
  int x = analogRead(JOYSTICK_X);
  int y = analogRead(JOYSTICK_Y);

// Snake moves horizontally
  if (velocity.x != 0) {

    if (x < 400 || x > 624) return;
    
    if (y > 624) {
      velocity = Vector(0, -1);
      joystickLock = true;
    }
    else if (y < 400) {
      velocity = Vector(0, 1);
      joystickLock = true;
    }

    // Snake moves vertically
  } else if (velocity.y != 0) {

    if (y < 400 || y > 624) return;
    
    if (x > 624) {
      velocity = Vector(-1, 0);
      joystickLock = true;
    }
    else if (x < 400) {
      velocity = Vector(1, 0);
      joystickLock = true;
    }

    // Snakes does not move (game start)
  }
  
}

Vector make4dir(Vector v) {
  if (v.x > 0 || v.x < 0) return Vector(v.x, 0);
  return v;
}

bool step = false;

void animateApple() {
  if (millis() >= appleMillis) {

    setPix(applePos, step = !step);
    
    appleMillis = millis() + APPLE_UPDATE_TIME;
  }
}

bool inversed = false;

// Loop
void loop() {
  if (stopped) {
    lc.setIntensity(0,inversed ? 15 : 0);
    inversed = !inversed;
    delay(250);
    return;
  }
  
  updateVelocity();
  updatePos();
  animateApple();

}
