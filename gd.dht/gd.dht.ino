#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <FS.h>   //Include File System Headers
#include <EMailSender.h> //include Gmail Sender Header
#include <DHT.h> //For Humidity/Temp Controller

#define GDPIN 14 //define the relay pin
#define MONPIN 12 //define the reedswitch monitor pin
#define DHTPIN 4     // what digital pin the DHT22 is conected to
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors, specify proper one here. 

#define PASSLENGTH 16

DHT dht(DHTPIN, DHTTYPE);
const char* htmlfile = "/index.htm";

//WiFi Connection configuration
const char *ssid = "YOUR_2.4GHZ_SSID";
const char *password = "YOUR_SSID_PW";

//Random 16char String Generator for WebCam Session
char *alphabeth = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const long alphabethLength = 62;//sizeof(alphabeth);
char pass[PASSLENGTH + 1]; //we add 1 extra for the NULL

//Auth Config Block for remote web access:
const char* www_username = "YOUR_USERNAME";
const char* www_password = "YOUR_ACCESS_PW";

String buf;
int timeSinceLastRead = 0;

ESP8266WebServer server(80);

//Sendmail function to alert you of trigger. 
void sendEmail() {
  //Username to authenticate to SMTP
  EMailSender emailSend("youremail@gmail.com", "yourgmailpw");
  EMailSender::EMailMessage message;
  message.subject = "[gDoor] Triggered";
  message.message = "Your Garage Door was Triggered.";
  //Who are you sending to? 
  EMailSender::Response resp = emailSend.send("recipientEmail@gmail.com", message);
  Serial.println("Sending status: ");
  Serial.println(resp.code);
  Serial.println(resp.desc);
  Serial.println(resp.status);
}

void triggerDoor() {
  if (server.authenticate(www_username, www_password)) {
    Serial.print("State of GPIO MONPIN: ");
    Serial.println(digitalRead(MONPIN));
    digitalWrite(GDPIN, LOW);
    delay(500);
    digitalWrite(GDPIN, HIGH);
    sendEmail();
    server.send(200, "text/plain", "Login OK");
  } else {
    return server.requestAuthentication();
  }
}

void handleRoot() {
  if (server.authenticate(www_username, www_password)) {
    //server.send(200, "text/plain", "Login OK");
    server.sendHeader("Location", "/index.htm", true);  //Redirect to our html web page
    server.send(302, "text/plain", "");
  } else {
    return server.requestAuthentication();
  }
}

void handleENV() {
  Serial.println(buf);
  server.send(200, "application/json", buf);
}

void handleWebRequests() {
  if (server.authenticate(www_username, www_password)) {
    if (loadFromSpiffs(server.uri())) return;
    String message = "File Not Detected\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
      message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
    Serial.println(message);
  }
}


void setup() {
  randomSeed(analogRead(0));    // Seed RNG

  //Setup Serial Reading for debugging:
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  //Config GPIO
  digitalWrite(GDPIN, HIGH);
  digitalWrite(MONPIN, HIGH);
  pinMode(GDPIN, OUTPUT);
  pinMode(MONPIN, OUTPUT);
  dht.begin();
  //Initialize File System
  SPIFFS.begin();
  //SPIFFS.format();
  Serial.println("File System Initialized");

  //Connect to wifi Network
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  ArduinoOTA.begin();

  //Initialize Webserver
  server.on("/", handleRoot);
  server.on("/getENV", handleENV); //Reads ENV function is called from out index.htm
  server.on("/triggerGD", triggerDoor); //Reads ADC function is called from out index.htm
  server.onNotFound(handleWebRequests); //Set setver all paths are not found so we can handle as per URI
  server.begin();
}


void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  getData();
  delay(250);
  timeSinceLastRead += 100;
}

bool loadFromSpiffs(String path) {
  String dataType = "text/plain";
  if (path.endsWith("/")) path += "index.htm";

  if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".html")) dataType = "text/html";
  else if (path.endsWith(".htm")) dataType = "text/html";
  else if (path.endsWith(".css")) dataType = "text/css";
  else if (path.endsWith(".js")) dataType = "application/javascript";
  else if (path.endsWith(".png")) dataType = "image/png";
  else if (path.endsWith(".svg")) dataType = "image/svg+xml";
  else if (path.endsWith(".gif")) dataType = "image/gif";
  else if (path.endsWith(".jpg")) dataType = "image/jpeg";
  else if (path.endsWith(".ico")) dataType = "image/x-icon";
  else if (path.endsWith(".xml")) dataType = "text/xml";
  else if (path.endsWith(".pdf")) dataType = "application/pdf";
  else if (path.endsWith(".zip")) dataType = "application/zip";
  else if (path.endsWith(".ttf")) dataType = "application/octet-stream";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (server.hasArg("download")) dataType = "application/octet-stream";
  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
  }

  dataFile.close();
  return true;
}

void getData() {
  String d00rstate;
  int d00r = digitalRead(MONPIN);
  if (d00r == 1) {
    d00rstate = "Open";
  } else {
    d00rstate = "Closed";
  }

  if (timeSinceLastRead > 2000) {
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      timeSinceLastRead = 0;
      return;
    }

    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    for (int n = 0; n < PASSLENGTH; n++) {
      pass[n] = alphabeth[random(0, alphabethLength)];
    }
    pass[PASSLENGTH] = '\0';



    buf = "\{\n";
    buf += F("\"temperatureF\":\"");
    buf += String(f, 2);
    buf += F("\",\n");

    buf += F("\"humidity\":\"");
    buf += String(h, 2);
    buf += F("\",\n");

    buf += F("\"heat_index\":\"");
    buf += String(hif, 2);
    buf += F("\",\n");

    buf += F("\"door_state\":\"");
    buf += d00rstate;
    buf += F("\",\n");

    buf += F("\"session\":\"");
    buf += pass;

    buf += F("\"\n\}\n");
    timeSinceLastRead = 0;
  }
}
