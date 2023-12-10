#define ONE_UNIT_LENGTH 3500
#define NINETY_DEGREE_STEP_CLK 3650
#define NINETY_DEGREE_STEP_CNTCLK 3650

unsigned long length = 0;
bool leftSide = false;
int position = 0;

void move(unsigned long step)
{
  delay(step);
  sendToArduino(Stop);
}

void goToPosition(int pos)
{ 
  position = pos;
  onPickupFinished = handlePosition;
  startPickUp();
}

void handlePosition()
{ 
  switch (position)
  {
    case 1:      
      moveToSidePosition(true, 1);
      break;
    case 2:
      moveToSidePosition(false, 1);
      break;
    case 3:
      moveToSidePosition(true, 2);
      break;
    case 4:
      moveToSidePosition(false, 2);
      break;
    case 5:
      moveToSidePosition(true, 3);
      break;
    case 6:
      moveToSidePosition(false, 3);
      break;
    case 7:
      moveToForwardPosition1();
      break;
    case 8:
      moveToForwardPosition2();
      break;
    default:
      break;
  }
}

void setRow(int row)
{
  switch (row)
  {
    case 1:
      length = ONE_UNIT_LENGTH;
      break;
    case 2:
      length = ONE_UNIT_LENGTH * 1.8;
      break;
    case 3:
      length = ONE_UNIT_LENGTH * 2.5;
      break;
  }
}

void moveToSidePosition(bool lSide, int row)
{
  leftSide = lSide;
  setRow(row);
  sendToArduino(Forward);
  move(length);
  delay(1000);

  sendToArduino(leftSide ? TurnLeft: TurnRight);
  move(leftSide ? NINETY_DEGREE_STEP_CNTCLK: NINETY_DEGREE_STEP_CLK);
  delay(1000);
  sendToArduino(Forward);
  move(ONE_UNIT_LENGTH * 0.4);

  onDeliverFinished = returnFromSidePosition;
  if(verticalSwitchReleased) {
    startDeliver();
  } else {
    onDeliverFinished();
  }
}

void returnFromSidePosition() {
  sendToArduino(Backward);
  move(ONE_UNIT_LENGTH * 0.4);
  delay(1000);
  sendToArduino(leftSide ? TurnRight : TurnLeft);
  move(leftSide ? NINETY_DEGREE_STEP_CLK : NINETY_DEGREE_STEP_CNTCLK);
  delay(1000);

  sendToArduino(Backward);
  move(length * 0.85);
  delay(3000);
  startScan();
}

void moveToForwardPosition1()
{
  sendToArduino(Forward);
  move(ONE_UNIT_LENGTH * 0.5);
  delay(500);
  sendToArduino(TurnLeft);
  move(NINETY_DEGREE_STEP_CNTCLK);
  delay(500);
  sendToArduino(Forward);
  move(ONE_UNIT_LENGTH * 0.4);
  delay(500);
  sendToArduino(TurnRight);
  move(NINETY_DEGREE_STEP_CLK);
  delay(500);
  sendToArduino(Forward);
  move(ONE_UNIT_LENGTH * 2.1);

  onDeliverFinished = returnFromForwardPosition1;
  if(verticalSwitchReleased) {
    startDeliver();
  } else {
    onDeliverFinished();
  }
}

void returnFromForwardPosition1() {
  sendToArduino(Backward);
  move(ONE_UNIT_LENGTH * 2.1);
  delay(500);
  sendToArduino(TurnLeft);
  move(NINETY_DEGREE_STEP_CNTCLK);
  delay(500);
  sendToArduino(Backward);
  move(ONE_UNIT_LENGTH * 0.4);
  delay(500);
  sendToArduino(TurnRight);
  move(NINETY_DEGREE_STEP_CLK);
  delay(500);
  sendToArduino(Backward);
  move(ONE_UNIT_LENGTH * 0.4);
  startScan();
}

void moveToForwardPosition2()
{
  sendToArduino(Forward);
  move(ONE_UNIT_LENGTH * 0.4);
  delay(500);
  sendToArduino(TurnRight);
  move(NINETY_DEGREE_STEP_CLK);
  delay(500);
  sendToArduino(Forward);
  move(ONE_UNIT_LENGTH * 0.4);
  delay(500);
  sendToArduino(TurnLeft);
  move(NINETY_DEGREE_STEP_CNTCLK);
  delay(500);
  sendToArduino(Forward);
  move(ONE_UNIT_LENGTH * 2);

  onDeliverFinished = returnFromForwardPosition2;
  if(verticalSwitchReleased) {
    startDeliver();
  } else {
    onDeliverFinished();
  }
}

void returnFromForwardPosition2() {
  sendToArduino(Backward);
  move(ONE_UNIT_LENGTH * 2);
  delay(500);
  sendToArduino(TurnRight);
  move(NINETY_DEGREE_STEP_CLK);
  delay(500);
  sendToArduino(Backward);
  move(ONE_UNIT_LENGTH * 0.4);
  delay(500);
  sendToArduino(TurnLeft);
  move(NINETY_DEGREE_STEP_CNTCLK);
  delay(500);
  sendToArduino(Backward);
  move(ONE_UNIT_LENGTH * 0.4);
  startScan();
}