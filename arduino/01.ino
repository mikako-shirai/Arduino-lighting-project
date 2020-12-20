
// MEGA 1 - MEGA 2 

// 2-13, 44-45
// 1 2 3 4 / 5 6 7 8 9 / 10 11 12 13 14

// arrays
const int trigs[3] = {A0, A2, A4}; // sensor pin
const int echos[3] = {A1, A3, A5};  // sensor pin
const int LEDPins[14] = {2, 6, 11, 3, 7, 12, 4, 8, 13, 5, 9, 44, 10, 45}; // PWM

float distance[3] = {100, 100, 100};
float distance_new[3];
int moves[3] = {0, 0, 0};  // 0:stay high   1:stay low   2:down   3:up
int LEDMoves[14];


//int LED_pins[14] = {2,  3,  4,  5, 
//                    6,  7,  8,  9,  10, 
//                    11, 12, 13, 44, 45};   // PWM
//    int index1[] = {0,  3,  6,  9};        // LED index
//    int index2[] = {1,  4,  7,  10, 12};   // LED index
//    int index3[] = {2,  5,  8,  11, 13};   // LED index


void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 3; i++) {
    pinMode(trigs[i], OUTPUT);
    pinMode(echos[i], INPUT);
  }
  for (int i = 0; i < 14; i++) {
    pinMode(LEDPins[i], OUTPUT);
  }
}

void loop() {
  Serial.println(" ");

  for (int i = 0; i < 3; i++) {
    Serial.print(distance[i]);
    Serial.print("   ");
    Serial.print(moves[i]);
    Serial.print("   ");
  }
  Serial.println(" ");
  
  for (int i = 0; i < 3; i++) {
    durationToMove(i);
  }

  for (int i = 0; i < 3; i++) {
    Serial.print(distance[i]);
    Serial.print("   ");
    Serial.print(moves[i]);
    Serial.print("   ");
  }
  Serial.println(" ");
    
  for (int i = 0; i < 14; i++) {
    if (i == 0 || i == 3 || i == 6 || i == 9) {
      LEDMoves[i] = moves[0];
    } else if (i == 1 || i == 4 || i == 7 || i == 10 || i == 12) {
      LEDMoves[i] = moves[1];
    } else {
      LEDMoves[i] = moves[2];
    }
  }

  int brightness[2] = {255, 1};
  for (int j = 0; j <254; j++) {
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
