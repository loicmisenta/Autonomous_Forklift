void dispatchInstruction(Instruction instruction, String arg) {
  switch(instruction) {
    case Stop:
      setVerticalDirection(Stop);
      sendToArduino(Stop);
      break;
    case Up: 
    case Down:
      setVerticalDirection(instruction);
      break;
    case Position:
      goToPosition(arg.toInt());
      break;
    case PickUp:
      startPickUp();
      break;
    default: 
      sendToArduino(instruction);
      break;      
  }
}