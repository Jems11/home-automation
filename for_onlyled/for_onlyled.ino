//Include the library files
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_AUTH_TOKEN "V3GVS6JdNvd2ZlsKRj8c1QKy4Q9UYQiI" //Enter your blynk auth token

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "DJ";//Enter your WIFI name
char pass[] = "Jaimin1112";//Enter your WIFI password

//Get the button value
BLYNK_WRITE(V0) {
  digitalWrite(D0, param.asInt());
  Serial.println(param.asInt());
}
  int ledPin = 2; // GPIO2 of ESP8266
  WiFiServer server(80);//Service Port

void setup() {
  Serial.begin(115200);
  delay(10);

  //Set the LED pin as an output pin
  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  
  //Initialize the Blynk library
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}

void loop() {

  //Run the Blynk library
  Blynk.run();
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
  return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
  delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

  int value = LOW;
  if (request.indexOf("/LED=ON") != -1) {
  digitalWrite(D0, HIGH);
  value = HIGH;
  } 
  if (request.indexOf("/LED=OFF") != -1){
  digitalWrite(D0, LOW);
  value = LOW;
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  
  client.print("Led pin is now: ");
  
  if(value == HIGH) {
  client.print("On");  
  } else {
  client.print("Off");
  }
  client.println("<br><br>");
  client.println("Click <a href=\"/LED=ON\">here</a> turn the LED on pin 2 ON<br>");
  client.println("Click <a href=\"/LED=OFF\">here</a> turn the LED on pin 2 OFF<br>");
  client.println("</html>");
  
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
  
}
