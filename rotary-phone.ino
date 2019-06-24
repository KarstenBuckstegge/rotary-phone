

#define RX 10
#define TX 11

void setup() {
  Serial.begin(115200);

  configurePhone(0);
  configureNetwork("sbstjn.phone", "fussball");
}

void calledNumber(String number) {
  Serial.println("called Number: " + number);

  sendRequest(number);
}

void loop() {
  phone_loop();
}
