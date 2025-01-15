const int s0 =8;
const int s1 =9;
const int s2 =10;
const int s3 =11;

const int analogPin = A0;

int latch = 2;
int clock = 3;
int data = 4;
int j; 
int sink = -1;
void setup() {

  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);

  pinMode(latch,OUTPUT);
  pinMode(clock,OUTPUT);
  pinMode(data,OUTPUT);

  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  for (j=0;j<8;j++)
  {
    digitalWrite(latch,LOW);
    shiftOut(data,clock,LSBFIRST,1<<j);  
    digitalWrite(latch,HIGH);
    for(int i = 0; i < 16; i ++){ 
  delay(1);
  Serial.print(readMux(i)); 
  Serial.print(" ");
  
  } 
  Serial.println(" ");
  }
  Serial.println(-1);
  delay(100);

}
  int readMux(int channel)  { 
  int controlPin[] = {s0, s1, s2, s3}; 
  int muxChannel[16][4]={ {0,0,0,0},  
  {1,0,0,0}, //channel 1 
  {0,1,0,0}, //channel 2 
  {1,1,0,0}, //channel 3 
  {0,0,1,0}, //channel 4 
  {1,0,1,0}, //channel 5 
  {0,1,1,0}, //channel 6 
  {1,1,1,0}, //channel 7 
  {0,0,0,1}, //channel 8 
  {1,0,0,1}, //channel 9 
  {0,1,0,1}, //channel 10 
  {1,1,0,1}, //channel 11 
  {0,0,1,1}, //channel 12 
  {1,0,1,1}, //channel 13 
  {0,1,1,1}, //channel 14 
  {1,1,1,1} //channel 15 
  }; 
  //loop through the 4 sig 
  for(int i = 0; i < 4; i ++){ 
    digitalWrite(controlPin[i], muxChannel[channel][i]); 
    } 
    //read the value at the SIG pin 
    int val = analogRead(analogPin); //return the value 
    return val; 

  }
  // put your main code here, to run repeatedly:


