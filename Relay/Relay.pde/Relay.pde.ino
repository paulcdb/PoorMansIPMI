// Poor Mans IPMI!
// Code modified from:
// https://diyprojects.io/esp8266-web-server-tutorial-create-html-interface-connected-object/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


#define ssid      "Your Wifi SSD"
#define password  "Your Wifi Password"
#define relayPin  D1
String  RelayState = "OFF";

// create Objects
ESP8266WebServer server ( 80 );

String getPage(){
  String page = "<html><head>";
  page += "<title>Poor Man's IPMI</title>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  page += "</head><body><h1>Poor Man's IPMI Server</h1>";
  page += "<h3>Power Control</h3>";
  page += "<form action='/' method='POST'>";
  page += "<ul>";
  page += "<INPUT type='radio' name='Relay' value='1'>Turn Off</li><br>";
  page += "<INPUT type='radio' name='Relay' value='2'>Reboot</li>";
  page += "<ul><br>";
  page += "<INPUT type='submit' value='GO!'>";
  page += "</body></html>";
  return page;
}
void handleRoot(){ 
  if ( server.hasArg("Relay") ) {
    handleSubmit();
  } else {
    server.send ( 200, "text/html", getPage() );
  }  
}

void handleSubmit() {
  String relay;
  relay = server.arg("Relay");
  if ( relay == "1" ) {
    // If form Turn On/Off sent,
    // Simulate pressing the power button briefly.
    digitalWrite(relayPin, 1);
    digitalWrite(BUILTIN_LED, LOW);
    delay(500);
    digitalWrite(relayPin, 0);
    digitalWrite(BUILTIN_LED, HIGH);
    Serial.println ( "Turning Off Server" );
    server.send ( 200, "text/html", getPage() );
  } else if ( relay == "2" ) {
    // If form reset sent, 
    // simulate holding the power button to turn off
    // Then, simulate pressing the power button again to turn on.
    digitalWrite(relayPin, 1);
    digitalWrite(BUILTIN_LED, LOW);
    delay(5000);
    digitalWrite(relayPin, 0);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(1000);
    digitalWrite(relayPin, 1);
    digitalWrite(BUILTIN_LED, LOW);
    delay(500);
    digitalWrite(relayPin, 0);
    digitalWrite(BUILTIN_LED, HIGH);
    Serial.println ( "Hard Reset Server " );
    server.send ( 200, "text/html", getPage() );
  } else {
    Serial.println("Err Relay Value");
  }
}

void setup() {
  Serial.begin ( 115200 );
  digitalWrite(BUILTIN_LED, HIGH);
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(relayPin, OUTPUT);
  
  WiFi.begin ( ssid, password );
  // Attente de la connexion au réseau WiFi / Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 1000 ); Serial.print ( "." );
  }
  // Connexion WiFi établie / WiFi connexion is OK
  Serial.println ( "" ); 
  Serial.print ( "Connected to " ); Serial.println ( ssid );
  Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );
  Serial.print ( "Relay " ); Serial.println ( RelayState );
  // link to the function that manage launch page 
  server.on ( "/", handleRoot );

  server.begin();
  Serial.println ( "HTTP server started" );

}

void loop() {
  server.handleClient();
}
