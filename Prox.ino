// HC-SR04 echo and trigger pin definitions
#define TRIGGER 13
#define ECHO 12
// Units divisor - 58.0 is millimeters, 1473.2 is inches
#define UNITS_DIVISOR 1473.2
// Ping setup for distance measurements
#define DELAY_BEFORE_NEW_PING 5
#define NUMBER_OF_PINGS 25
#define DELAY_BETWEEN_GROUPS 250
#define TRIGGER_LENGTH 11
// LED output pin definitions
#define RED_LED 11
#define GREEN_LED 10
#define BLUE_LED 9
// Inch distances for indicators, red is on for anything less than BLUE_ON.
// BLUE_ON is the closest edge of the sweet spot, GREEN_ON is the farthest edge.
#define ALL_OFF 96
#define GREEN_ON 36
#define BLUE_ON 32

void setup() {
   // Set up to send output to console
   Serial.begin(9600);

   // Initialiaze the I/O
   // LEDs
   pinMode(RED_LED, OUTPUT);
   pinMode(GREEN_LED, OUTPUT);
   pinMode(BLUE_LED, OUTPUT);
   // The HC-SR04 sensor
   pinMode(TRIGGER, OUTPUT);
   pinMode(ECHO, INPUT);

   // Reset the trigger pin and wait a half a second
   digitalWrite(TRIGGER, LOW);
   delayMicroseconds(500);
}

void loop() {
  delay(DELAY_BETWEEN_GROUPS);
  long measurement = groupPing();
  // Print out distance in inches
  Serial.print("Distance: ");
  Serial.print(measurement);
  Serial.println(" inches");
  // Set the indicator
  if(measurement > ALL_OFF) {
    // Greater than 96 inches, no indicator
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(RED_LED, LOW);
  } else if(measurement > GREEN_ON) {
    // Greater than 36 and less than 96, green indicator
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(RED_LED, LOW);
  } else if(measurement > BLUE_ON) {
    // Greater that 32 and less than 36 - sweet spot, blue indicator
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  } else {
    // Less than 32 inches, red indicator
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  }
}

long groupPing() {
  long sum = 0;
  for (int i = 0; i < NUMBER_OF_PINGS; i++)
  {
    delay(DELAY_BEFORE_NEW_PING);
    sum += ping();
  }
  return sum / NUMBER_OF_PINGS;
}

long ping() {
  // Trigger - raise the trigger for TRIGGER_LENGTH, then set it back low
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(TRIGGER_LENGTH);
  digitalWrite(TRIGGER, LOW);
  // Get the echo length - wonder if this can actually start before lowering
  // the trigger line...
  int pulseWidth = pulseIn(ECHO, HIGH);
  return (long) (((float) pulseWidth / UNITS_DIVISOR) * 10.0);
}
