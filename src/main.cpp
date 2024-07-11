#include <Arduino.h>
#include <AccelStepper.h>

#define dirPin 22
#define stepPin 52
#define motorInterfaceType 1
#define lead_size 1

using namespace std;

int stepsPerRev;
int currSpeed;

AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
void setup() {
  // put your setup code here, to run once:
  stepper.setMaxSpeed(10000);
  stepper.setAcceleration(10000);
  Serial.begin(9600);
  Serial.setTimeout(10);
  stepsPerRev = 6400;
}

void moveRevolutions(float revs) {
	long stepsToGo = static_cast<long>(revs * static_cast<float>(stepsPerRev));
  //Serial.println(stepsToGo);
	stepper.moveTo(stepsToGo);
  stepper.setSpeed(currSpeed);
  stepper.runToPosition();
}

void moveDistance(float distance) { // move a distance in MM
  long mmToRev = static_cast<long>(distance * static_cast<float>(lead_size));
  moveRevolutions(mmToRev);
}

double rpmToSps(float rpm) { // 
  float steps_per_second = (rpm / 60) * stepsPerRev;
  return steps_per_second;
}

double vToSps(float v) { //velocity in mm/s
  float rpm = v / lead_size;
  return rpmToSps(rpm);
}

void processLine(String str) { // process incoming lines, returns true when processed
/*
Format for incoming Strings
setting: [s, type, val]
  types: spr (steps per rev), mrpm (max rpm), mv (max velocity) sr (speed for rpm), sv (speed in velocity)

moving: [m, type, val]
  types: r (rotation), d (distance in mm)
*/
  String processed[20];
  int str_index = 0;

  while (str.length() > 0)
  {
    int index = str.indexOf(' ');
    if (index == -1) // No space found
    {
      processed[str_index++] = str;
      break;
    }
    else
    {
      processed[str_index++] = str.substring(0, index);
      str = str.substring(index+1);
    }
  }
  Serial.println(processed[0]);
  Serial.println(processed[1]);
  Serial.println(processed[2]);
  if (processed[0] == "s") {
    if (processed[1] == "spr") {
        stepsPerRev = processed[2].toInt();
    }
    else if (processed[1] == "mrpm") {
        stepper.setMaxSpeed(rpmToSps(processed[2].toFloat()));
    }
    else if (processed[1] == "mv") {
        stepper.setMaxSpeed(vToSps(processed[2].toFloat()));
    }
    else if (processed[1] == "sr") {
        currSpeed = (rpmToSps(processed[2].toFloat()));
    }
    else if (processed[1] == "sv") {
        currSpeed = (vToSps(processed[2].toFloat()));
    }
  }

  else if (processed[0] == "m") {
    if (processed[1] == "r") {
        moveRevolutions(processed[2].toFloat());
    }
    else if (processed[1] == "d") {
        moveRevolutions(processed[2].toFloat());
    }
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  // moveDistance(10); // move 10 mm
  // delay(1000);
  // moveDistance(-10);
  // delay(1000);
  // stepper.moveTo(4000);
  // stepper.runToPosition();

  // delay(1000);

  // stepper.moveTo(0);
  // stepper.runToPosition();
  if (Serial.available() > 0) {
    String line = Serial.readString();
    line.trim();
    processLine(line);
  }
  delay(10);
  

}
