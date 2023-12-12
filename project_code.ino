#include <Servo.h>
#include <LCD_I2C.h>

LCD_I2C lcd(0x27, 16, 2);
int IR_PIN = 2; // Define the digital pin for the IR sensor
const int inductiveSensorPin = 3; //But actual sensor attached to Pin4
Servo metalservo;  // create servo object to control a metal
Servo nonservo;
const int trigPin = 9;  // Trigger pin of the ultrasonic sensor
const int echoPin = 10; // Echo pin of the ultrasonic sensor

void setup() {
  lcd.begin();
  lcd.backlight();
  Serial.begin(9600); // Initialize serial communication for debugging
  pinMode(IR_PIN, INPUT); // Set the IR sensor pin as INPUT
  pinMode(inductiveSensorPin, INPUT);
  metalservo.attach(5);
  nonservo.attach(6);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  int sensorValue = digitalRead(IR_PIN); // Read the digital value from the IR sensor
  int metalValue = digitalRead(inductiveSensorPin);
  lcd.setCursor(1,0);
  lcd.print("Insert Trash");
  lcd.setCursor(5,1);
  lcd.print("Here!");


  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pulse duration in microseconds
  long duration = pulseIn(echoPin, HIGH);

  // Calculate distance in centimeters
  float distance_cm = duration * 0.034 / 2;
  
  if (metalValue == HIGH) {
    Serial.println("Metal detected!"); // Print a message when an obstacle is detected
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Metal Detected!");
    metalservo.write(120);  
    delay(3000);
    lcd.clear();
    metalservo.write(0);

    if(distance_cm < 16){
      float diff_dist = 16 - distance_cm;
      if (diff_dist > 0.5){
          float full = (diff_dist/16)*100;
          Serial.println(full);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(full);
          lcd.setCursor(8,0);
          lcd.print("%");
          lcd.setCursor(7, 1);
          lcd.print("full!");
          delay(3000);
          lcd.clear();



      }

    } 
    
  } else if((metalValue == LOW) &&(sensorValue == LOW)) {
    Serial.println("Non metal detected!"); // Print a message when no obstacle is detected
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Non-metal Found!");
    nonservo.write(120);
    delay(3000);
    lcd.clear();
    nonservo.write(0);

    if(distance_cm < 16){
      float diff_dist = 16 - distance_cm;
      if (diff_dist > 0.5){
          float full = (diff_dist/16)*100;
          Serial.println(full);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(full);
          lcd.setCursor(8,0);
          lcd.print("%");
          lcd.setCursor(7, 1);
          lcd.print("full!");
          delay(3000);
          lcd.clear();



      }

    } 

  } else{
    Serial.println("Nothing there");
  }

  
  
  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
  
  delay(1000); // Add a short delay to prevent rapid serial prints
}

