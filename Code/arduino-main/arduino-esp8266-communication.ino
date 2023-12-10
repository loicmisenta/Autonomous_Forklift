void handleInstructions() 
{
  int intInstruction = Serial.read();
  if(intInstruction == -1)
    return;

  Instruction instruction = (Instruction)intInstruction;
  if(instruction != Stop && !enabled)
    enable();

  switch(instruction) {
    case Stop:
      stop();
      break;
    case Forward:
      setDirection(Forward);
      break;
    case Backward:
      setDirection(Backward);
      break;
    case TurnRight:
      setDirection(TurnRight);
      break;
    case TurnLeft:
      setDirection(TurnLeft);
      break;  
    case Right:
      setDirection(Right);
      break;
    case Left:
      setDirection(Left);
      break;
    default:
      break;
  }
}