#include <SoftwareSerial.h>

int countTrueCommand;
int countTimeCommand;

int loopCounter = 0;

boolean found = false; 
int valSensor = 1;

SoftwareSerial esp8266(RX, TX);

void configureNetwork(String wifiName, String wifiPassword) {
  esp8266.begin(115200);
  
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ wifiName +"\",\""+ wifiPassword +"\"",20,"OK");
}

void sendRequest(String number) {
  String HOST = "enr3irentj4a.x.pipedream.net";
  String PORT = "80";
  String url = "dialed-" + number;
  String req = "GET /" + url + " HTTP/1.1\r\nHost: " + HOST + "\r\nAccept: */*";

  sendCommand("AT+CIPMUX=1",5,"OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
  sendCommand("AT+CIPSEND=0," +String(req.length()+4),4,">");

  Serial.println("Request: " + req);
  
  esp8266.println(req);
  delay(1500);
  countTrueCommand++;
  
  sendCommand("AT+CIPCLOSE=0",5,"OK");
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(command);
  Serial.print(" ");
  
  while(countTimeCommand < (maxTime*1)) {
    esp8266.println(command);//at+cipsend
    
    if (esp8266.find(readReplay)) {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if (found == true) {
    Serial.println("OK");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if (found == false) {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
