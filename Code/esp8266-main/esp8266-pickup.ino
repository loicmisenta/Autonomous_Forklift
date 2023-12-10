#define PICKUP_FORWARD_TIMEOUT 8000
#define PICKUP_UP_TIME 1000

unsigned long pickUpForwardStartTime = 0;
unsigned long pickUpForwardEndTime = 0;

void startPickUp() {
  pickupDown();
}


void pickupDown() {
  if(!verticalSwitchReleased) {
    pickupForward();
    return;
  }
  setVerticalDirection(Down);
  pickupState = PickUpDown;
}

void pickupForward() {
  pickUpForwardStartTime = millis();
  if(!horizontalSwitchReleased) {
    pickupUp();
    return;
  }
  pickupState = PickUpForward;
  sendToArduino(Forward);
  pickupForwardTimeout = millis() + PICKUP_FORWARD_TIMEOUT;
}

void pickupUp() {
  pickUpForwardEndTime = millis();
  sendToArduino(Stop);
  pickupState = PickUpUp;
  setVerticalDirection(Up);
  pickupUpStop = millis() + PICKUP_UP_TIME;
}

void pickupBackward() {
  pickupState = PickUpBackward;
  sendToArduino(Backward);
  pickupBackwardTimeout = millis() + (pickUpForwardEndTime - pickUpForwardStartTime);
}


void startDeliver() {
  isDelivering = true;
  setVerticalDirection(Down);
}