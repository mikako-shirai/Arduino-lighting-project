
// MEGA 3

// 2-8, 10-13, 44-46
// 1 2 3 4 5 6 7 / 8 9 10 11 12 13 14

// arrays
const int trigs[2] = {A0, A2}; // sensor pin
const int echos[2] = {A1, A3};  // sensor pin
const int LEDPins[14] = {2, 10, 3, 11, 4, 12, 5, 13, 6, 44, 7, 45, 8, 46}; // PWM

float distance[2] = {100, 100};
float distance_new[2];
int moves[2] = {0, 0};  // 0:stay high   1:stay low   2:down   3:up
int LEDMoves[14];


//int LED_pins[14] = {2,  3,  4,  5,  6,  7,  8,
//                    10, 11, 12, 13, 44, 45, 46};   // PWM
//    int index1[] = {0,  2,  4,  6,  8, 10, 12};    // LED index
//    int index2[] = {1,  3,  5,  7,  9, 11, 13};    // LED index


void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 2; i++) {
    pinMode(trigs[i], OUTPUT);
    pinMode(echos[i], INPUT);
  }
  for (int i = 0; i < 14; i++) {
    pinMode(LEDPins[i], OUTPUT);
  }
}

void loop() {
  Serial.println(" ");

  for (int i = 0; i < 2; i++) {
    Serial.print(distance[i]);
    Serial.print("   ");
    Serial.print(moves[i]);
    Serial.print("   ");
  }
  Serial.println(" ");
  
  for (int i = 0; i < 2; i++) {
    durationToMove(i);
  }

  for (int i = 0; i < 2; i++) {
    Serial.print(distance[i]);
    Serial.print("   ");
    Serial.print(moves[i]);
    Serial.print("   ");
  }
  Serial.println(" ");
    
  for (int i = 0; i < 14; i++) {
    if (i == 0 || i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12) {
      LEDMoves[i] = moves[0];
    } else {
      LEDMoves[i] = moves[1];
    }
  }

  int brightness[2] = {255, 1};
  for (int j = 0; j < 254; j++) {
    for (int i = 0; i < 14; i++) {
      if (LEDMoves[i] == 0) {  // stay high
        analogWrite(LEDPins[i], 100);
      } else if (LEDMoves[i] == 1) {  // stay low
        analogWrite(LEDPins[i], 1);
      } else if (LEDMoves[i] == 2) {  // down
        analogWrite(LEDPins[i], brightness[0]);
      } else if (LEDMoves[i] == 3) {  // up
        analogWrite(LEDPins[i], brightness[1]);
      }
    }
    brightness[0] = brightness[0] - 1;
    brightness[1] = brightness[1] + 1;
    delay(2);
  }

  Serial.println(" ");
}


// functions
void durationToMove(int x) {
  int duration;
  float result;
  
  digitalWrite(trigs[x],LOW);
  delayMicroseconds(2);
  digitalWrite(trigs[x],HIGH);
  delayMicroseconds(10);
  digitalWrite(trigs[x],LOW);
  duration = pulseIn(echos[x],HIGH);
  result = (duration / 2) / 29.1;
  distance_new[x] = result;
  Serial.println(result);

  if (distance[x] > 50 && distance_new[x] > 50) {  // stay high
      moves[x] = 0;
      distance[x] = distance_new[x];
  }else if (distance[x] > 50 && distance_new[x] <= 50) {  // down
      moves[x] = 2;
      distance[x] = distance_new[x];
  }else if (distance[x] <= 50 && distance_new[x] <= 50) {  // stay low
      moves[x] = 1;
      distance[x] = distance_new[x];
  }else if (distance[x] <= 50 && distance_new[x] > 50) {  // up
      moves[x] = 3;
      distance[x] = distance_new[x];
  }
}
