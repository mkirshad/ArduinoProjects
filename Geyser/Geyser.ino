#define GAS A0      // Gas Sensor Analog Pin
#define FLAME A1    // Flame Sensor Analog Pin
#define RELAY1 2    // Ignitor Relay Pin

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial.println("program started");
pinMode(GAS, INPUT);
pinMode(FLAME, INPUT);
pinMode(RELAY1, OUTPUT);
digitalWrite(RELAY1, HIGH);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  
  int i_gas =  analogRead(GAS);
  Serial.println("Gas Sensor Output");
  Serial.println(i_gas);

  int i_flame =  analogRead(FLAME);
  Serial.println("Flame Sensor Output");
  Serial.println(i_flame);

  //Turn on Relay / Igninter
  if(i_gas > 600 && i_flame > 1000){
    digitalWrite(RELAY1, LOW);
    delay(3000);
    digitalWrite(RELAY1, HIGH);
    delay(10000);
  }
  
  delay(1000);
}
