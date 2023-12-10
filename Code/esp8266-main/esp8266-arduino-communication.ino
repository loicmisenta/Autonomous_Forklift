void sendToArduino(Instruction toWrite) { sendToArduino((int) toWrite); }

void sendToArduino(int toWrite) {
  Serial.printf("Writing %d to the Arduino UNO\n", toWrite);
  s.write(toWrite);
}