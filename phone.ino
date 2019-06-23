int signalStrength = 0; // Is reciever lifted or not
int signalStrengthStat = 0;

int dialPin = 0;
int counter = 0;
int dialedNumber = 10;

unsigned long int timeOfSignal = 0;
unsigned long int timeSinceLastChange = 0;

unsigned long int finishedDialingTimeout = 200;
unsigned long int clatterTimeout = 120;

int signalThreshold = 400;

bool dialing = false;
bool hungUp = false;

String finalNumber;

void configurePhone(int pinDial) {
  dialPin = pinDial;

  pinMode(dialPin, INPUT);
}

void checkSignal() {
  if (analogRead(dialPin) > signalThreshold) {
    signalStrength = 1;
  } else {
    signalStrength = 0;
  }
}

bool phone_loop() {
  checkSignal();


  /*
  *  DIALING
  */
  if ( signalStrength != signalStrengthStat ) {
    signalStrengthStat = signalStrength;
    // on dial start one signal is send
    if (signalStrength == HIGH) {
      dialing = true;
      
      if (hungUp) {
        // RECIEVER LIFTED
        hungUp = false;
        timeOfSignal = millis();

        finalNumber = "";
      }
    } 

    else if (signalStrength == LOW)
    {
      // CLATTER
      timeOfSignal = millis();
      counter++;
    }
  }

  timeSinceLastChange = millis() - timeOfSignal;

  if ( dialing == true ) {
    // finished dialing?
    if ( timeSinceLastChange > clatterTimeout ) {
      if (counter == 10) {
        dialedNumber = 0;
      }  else if (counter > 0) {
        dialedNumber = counter;
      }
      
      if (dialedNumber < 10) {
        finalNumber += dialedNumber;
      }

      dialing = false;
      counter = 0;
      dialedNumber = 10;
    }
  }

  if ( signalStrength == LOW ) {
    if ( timeSinceLastChange > finishedDialingTimeout && hungUp == false && finalNumber != "") {
      // HUNG UP
      calledNumber(finalNumber);
//      Serial.println("finalNumber: " + finalNumber);
      
      hungUp = true;
      dialing = false;
      counter = 0;
      dialedNumber = 10;
    }
  } 
}
