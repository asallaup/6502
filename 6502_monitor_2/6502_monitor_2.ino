#include "hello-world-final.h"

// const char ADDR[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};

const char ADDR[] = { 52, 50, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30, 28, 26, 24, 22 };
//const char DATA[] = {39, 41, 43, 45, 47, 49, 51, 53};

const char DATA[] = { 53, 51, 49, 47, 45, 43, 41, 39 };

char STACK[256];
char ZERO_PAGE[256];

#define CLOCK 2
#define READ_WRITE 3
#define ROM_CS 4
#define RAM_CS 5
#define VIA_CS 6

#define PROBE 7

#define START_PROG 0x200
void setup() {
  for (int n = 0; n < 16; n += 1) {
    pinMode(ADDR[n], INPUT);
  }
  for (int n = 0; n < 8; n += 1) {
    pinMode(DATA[n], INPUT);
  }
  pinMode(CLOCK, OUTPUT);
  pinMode(READ_WRITE, INPUT);
  pinMode(ROM_CS, INPUT);
  pinMode(RAM_CS, INPUT);
  pinMode(VIA_CS, INPUT);
  pinMode(PROBE, INPUT);
  for (int i = 0; i < 0x100; i++){
    STACK[i] = 0;
  }
  digitalWrite(CLOCK, 0);

 // attachInterrupt(digitalPinToInterrupt(CLOCK), onClock, RISING);
  Serial.begin(9600);
}

void setDataPinMode(uint8_t mode) {
  for (int n = 0; n < 8; n += 1) {
    pinMode(DATA[n], mode);
  }
}

void writeData(unsigned int data) {
  setDataPinMode(OUTPUT);
  int n = 0;
  for (unsigned int test = 0x80; test; test >>= 1) {
    digitalWrite(DATA[n++], data & test);
  }
}

unsigned int readData() {
  setDataPinMode(INPUT);
  unsigned int data = 0;
  for (int n = 0; n < 8; n += 1) {
    int bit = digitalRead(DATA[n]) ? 1 : 0;
    data = (data << 1) + bit;
  }
  return data;
}

unsigned int databus(unsigned int address, char readWrite) {
  unsigned int data = 0;

  if (readWrite == 'r') {
    if (address == 0xfffc) {
      data = 0x00;
      writeData(data);
    } if (address == 0xfffd) {
      data = 0x02;
      writeData(data);
    } if (address >= START_PROG && address < RAMLEN + START_PROG) {
      data = RAM[address - START_PROG];
      writeData(data);
    } if ( address >= 0x100 && address < 0x200 ){
      data = STACK[address - 0x100];
      writeData(data);
    }
  }
  
  else {
    data = readData();
    if ( address >= 0x100 && address < 0x200 ){
      STACK[address - 0x100] = data;
      writeData(data);
    }
  }
  return data;
}

void onClock() {
  char output[15];

  unsigned int address = 0;

  for (int n = 0; n < 16; n += 1) {
    int bit = digitalRead(ADDR[n]) ? 1 : 0;
    address = (address << 1) + bit;
  }

  unsigned int data = 0;
  char readWrite = digitalRead(READ_WRITE) ? 'r' : 'W';
  char *select = readSelect();

  data = databus(address, readWrite);
  printToSerial(address, data, select, readWrite);
}

void printToSerial(unsigned int address, unsigned int data, char *select, char *readWrite) {
  return;
  char output[15];

  for (unsigned int test = 0x8000; test; test >>= 1) {
    Serial.write(address & test ? '1' : '0');
  }
  Serial.print(" ");
  for (unsigned int test = 0x80; test; test >>= 1) {
    Serial.write(data & test ? '1' : '0');
  }

  sprintf(output, "  %s %04x %c %02x", select, address, readWrite, data);
  Serial.println(output);
}

char *readSelect() {
  if (!digitalRead(ROM_CS)) {
    return "ROM";
  }
  if (!digitalRead(VIA_CS)) {
    return "VIA";
  }
   if (!digitalRead(RAM_CS)) {
    return "RAM";
  }
  return "XXX";
}

void loop() {
    delay(1);
    digitalWrite(CLOCK, 1);
    delay(1);
    onClock();
    digitalWrite(CLOCK, 0);



}
