int maxL; //max light value from photoresistor
int minL; //min light value
int midL; //mid point between highest and lowest light values to determin wether a bit is a 1 or 0
int dropThreshhold = 50; //minumin drop in light level to activate data reception
const int laser = 10; //pin connected to laser diode
const int photoR = A0; //pin connected to photoresistor
const int delayT = 5; //delay for sending and reciving bits. increasing this increases bandwidth but decreases signal quality. must be the same on both receiver and transmitter. max i found to be stable is 5
char recChar; //last char recieved
byte recVal; //last byte recived
int nullCount; //counts how many empty bit have been received in a row for ending reception
char transmitData[1000]; //Data to transmit. limit 1000 bytes by default. altering array size alters amount of memory used
byte transmitByte; //next byte to transmit
byte startCode = byte(204); //byte transmitted to tell receiver to get ready to receive data

void setup() {
  Serial.begin(9600); //baud rate 9600
  while(!Serial){} //wait for serial

  pinMode(laser, OUTPUT); //initialize pins
  pinMode(photoR, INPUT);
}

void transmit(){
  
  Serial.println("\n\n\nEnter text to transmit\n");

  while(Serial.available() == 0){ //wait for user to enter text
    
    }
  Serial.readBytes(transmitData, 1000); //read serial data for transmittion
  
  digitalWrite(laser, HIGH); //turn laser on and off again to let receiver know strength of the laser
  delay(delayT);
  digitalWrite(laser, LOW);

  delay(300);               //pulse to sync reciver and transmiter
  digitalWrite(laser, HIGH);
  delay(800);
  digitalWrite(laser, LOW);

  for(int i=0; i<8; i++){ //transmit start code to initilise transmittion
    
    if (bitRead(startCode, i) == 1){
      digitalWrite(laser, HIGH);
    }
    
    else{
      digitalWrite(laser, LOW);
    }
    delay(delayT);
  }
  
  for(int a=0; a<1000; a++){ //loop through bytes in transmitData[]
    
    if(!transmitData[a]){ //if end of data stop sending
      Serial.println("Done");
      break;
    }

    transmitByte = byte(transmitData[a]); //set next byte to be sent

    digitalWrite(laser, HIGH); //sync after every byte
    delay(delayT);
    digitalWrite(laser, LOW);
    delay(delayT);
    
    for(int i=0; i<8; i++){ //transmit data

      if (bitRead(transmitByte, i) == 1){ //laser on (bit = 1)
        digitalWrite(laser, HIGH);
      }
      
      else{                              //laser off (bit = 0)
        digitalWrite(laser, LOW);
      }

      delay(delayT);

      
    }

  }

  for(int i=0; i<8; i++){     //keep in sync without transmitting any data to end message
    digitalWrite(laser, HIGH);
    delay(delayT);
    digitalWrite(laser, LOW);
    delay(delayT*9);
  }
  
  for(int i=0; i<1000; i++){ //empty data variable

    transmitData[i] = NULL;
  }
}

void receive(){

  delay(500);
  Serial.println("Ready");
  minL = 0; //set previous light value to zero
  
  while(true){
    maxL = analogRead(photoR);
    Serial.println(maxL);
    if(maxL < minL && (minL - maxL) > dropThreshhold){ //if light intesity drop exceeds threshold
      midL = (minL + ((maxL - minL) / 2));
      Serial.println("Lighting change detected");
      break;
    }
    else{
      minL = maxL;
    }
    
  }
  delay(500);
  while(analogRead(photoR) > midL){} //wait for laser to turn off for sync
  
  for (int i=0; i<8; i++){ //find start code

    if (analogRead(photoR) > midL){
      bitWrite(recVal, i, 1);
    }
    
    else{
      bitWrite(recVal, i, 0);
    }
    
    delay(delayT);
  }

  if(int(recVal) != 204){ //verify start code recived
    return;
  }
  Serial.println("Downlink verified, decoding");

  while(nullCount != 8){ //keep decoding untill 8 empty bytes are received

      while(analogRead(photoR) < midL){} //wait for sync pulse
      delay(delayT / 2);
      while(analogRead(photoR) > midL){}
      delay(delayT);
    
    for (int i=0; i<8; i++){ //decode data

      if (analogRead(photoR) > midL){ 
        bitWrite(recVal, i, 1); //write a 1
      }
      
      else{
        bitWrite(recVal, i, 0); //write a 0
      }
      
      delay(delayT); //delay
    }
    
    char recChar = char(recVal);
    Serial.print(recChar);

    if(recVal == byte(0)){ //increment counter if a empty byte is received
      nullCount++;
    }
  }

  Serial.println("\n\nEND OF MESSAGE\n");
  nullCount = 0; //reset null counter
  
  delay(6000);//short delay before restarting
  
}


void loop() {
  while(true){
    
    Serial.println("(1) Transmit\n(2) Receive\n");
    while(Serial.available() == 0){
    
    }
    char usrin = char(Serial.read()); //read user input
    if(usrin == '1'){
      while(true){
        transmit();
      }
    }
    else if(usrin == '2'){
      while(true){
        receive();
      }
    }
    else{
      Serial.println("invalid input");
    }
    
  }

  

}
