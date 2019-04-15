/*********
 1151
*********/

// Load Wi-Fi library
#include <WiFi.h>

// Replace with your network credentials
const char* ssid     = "va_tmg";
const char* password = "password";

//Code for ultrasonic sound sensor
//Sensor should get raw data and program computes the distance of listener
//and relay the distance to the server and change the volume of the music player


#define TriggerPin 4
#define EchoPin 2
#define led1 15
#define led2 18
#define led3 19
#define MaxDistance 400 //maximum distance of sensor is 450, there we have set the distance in our case to be 400cm

long duration;
long distance;

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// SENSING SYSTEM FOR MUSIC PLAYER USING INFRARED SENSOR
//SENSOR GETS RAW DATA AND THE CODE TURNS THE ROOM MUSIC ON
int inputSignal = 17/*Or 7*/;//SET INPUT SIGNAL PIN NUMBER HERE
int ledPin = 13;//set pin number here this one can also be removed. not neccessary
int infra_SensorStart = LOW;// when there is no motion detected
int infra_sensorState = 0;//read the pin status

// Auxiliar variables to store the current output state
String power = "off";
String delta = "on";
int setvolume = 0;


void setup() {
  Serial.begin(115200);
    pinMode (ledPin, OUTPUT);//Setting LED as out put...CHANGE THIS TO DISPLAY ON SEVER
  pinMode (inputSignal, INPUT);//Setting sensor as input
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
//SETTING THE INPUT AND OUTPUT TERMINALS OF THE SENSOR
  pinMode(TriggerPin, OUTPUT);
  pinMode (EchoPin , INPUT);
  pinMode(led1, OUTPUT);
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

   if(infra_sensorState == LOW){//check if there is no input
    //TURN THE MUSIC Off
    digitalWrite(ledPin, LOW);
    if(infra_SensorStart == HIGH && power == "off"){
      Serial.println("NO MOTION DETECTED");
      infra_SensorStart =  LOW;
      power = "off";
    }
  }
  else{      
  //TURN THE LED ON-PLAY THE MUSIC
    digitalWrite(ledPin, HIGH);
    if(infra_SensorStart == LOW){
      Serial.println("MOTION DETECTED");
      infra_SensorStart = HIGH;
      power = "on";
    }
  } 

  
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,

      if(power == "on"){
  digitalWrite(TriggerPin , LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);
  
  duration = pulseIn(EchoPin, HIGH);
  distance = (duration/2)*.034;//29.1 is the ultrasonic sound speed

    if(distance <=40){
      setvolume = 1;

    Serial.println("Your range is ");
   // Serial.println(" ");
    Serial.println(distance);
    Serial.println("VOLUME = 1"); 
    Serial.println("======================");
  }
  if(distance > 40 && distance <= 80){
    setvolume = 2;
    Serial.println("Your range is");
    Serial.println(distance);
    Serial.println("VOLUME = 2");
   
    Serial.println("======================");
  }
  if (distance >80 && distance <=120){
    
setvolume = 3;
    Serial.println("Your range is");
    Serial.println(distance);
    Serial.println("VOLUME = 3");
    Serial.println("======================");
  }
  if(distance > 120){
  setvolume = 4;
    Serial.println("You are at maximum distance");
    Serial.println(distance);
    Serial.println("MAX VOLUME = 4");
    Serial.println("======================");
  }
  delay(2000);

}


        
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /power/on") >= 0) {
              Serial.println("power on");
              power = "on";
              
            } else if (header.indexOf("GET /power/off") >= 0) {
              Serial.println("power off");
              power = "off";
            } else if (header.indexOf("GET /distance/on") >= 0) {
              Serial.println("distance on");
              delta = "on";
             } else if (header.indexOf("GET /distance/off") >= 0) {
              Serial.println("distance off");
              delta = "off";
            }
            Serial.println("--> Beging HTML ");
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
 client.println("<style>  .button {background-color: #4CAF50;border: none; color: white;padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 16px;margin: 4px 2px;cursor: pointer;}");
client.println(".off {background-color: #f44336;}");
client.println(".active{background-color: #a0a0ab;}");
client.println("h2 {color: #e68a00;}");
client.println(".header {margin-left: 70px;width: 100%;height: 100%;border: 1px solid #f2f2f2;text-align: center;box-sizing: border-box;}");

client.println("body {background-color: #00091a;text-align: centre;width: 50%;}");
client.println("h1,p{color: #e68a00;} </style><title>1151_zw</title></head>");
            
            // Web Page Heading
           client.println("<body>");
client.println("  <div class=\"header\">");
client.println("    <h1>Welcome Home</h1><p>by Micheal Magaisa and Takundei Gora | JacobsHack! 2018</p>");
client.println("  </div>");
client.println("<audio id=\"mysound\" controls>");
client.println("  <source src=\"http://docs.google.com/uc?export=open&id=1fVtuFr0eE88ZD_JadEEDtn1C6nSFyq9n\" type=\"audio/mp3\">");
client.println("</audio>");
client.println("<br>");

client.println("<button onclick=\"VolumeDown()\" type=\"button\">Volume DOWN</button>");
client.println("<button onclick=\"VolumeUP(0)\" type=\"button\">Volume UP</button>");
client.println("<button onclick=\"player()\" type=\"button\">Play</button>");
client.println("<script>");
client.println("var vid = document.getElementById(\"mysound\");");
client.println("function player(){");
client.println("  newwindow2=window.open(\'\',\'theaudio\',\'height=200,width=400\');");
client.println("  var tmp = newwindow2.document;");
client.println("  tmp.write(\'<audio id=\"mysound\" controls autoplay><source src=\"http://docs.google.com/uc?export=open&id=1fVtuFr0eE88ZD_JadEEDtn1C6nSFyq9n\" type=\"audio/mp3\"></audio>');}");
client.println("function VolumeUP(setvolume) {");
client.println("  var pop_player = window.open(\'\',\'theaudio\');");
client.println("  var control = pop_player.document.getElementById(\"mysound\");");
client.println("if(setvolume == 0){");
client.println("  vid.volume = vid.volume + 0.1;");
client.println("  control.volume = vid.volume + 0.1;}");
client.println("else{vid.volume = setvolume;");
client.println("control.volume = setvolume;}}");
client.println("function VolumeDown() {");
client.println("    var pop_player = window.open(\'\',\'theaudio\');");
client.println("    var control = pop_player.document.getElementById(\"mysound\");");
client.println("  vid.volume = vid.volume - 0.1;");
client.println("    control.volume = vid.volume - 0.1;}");

client.println("</script>");
if(setvolume == 1){
  client.println("<script>VolumeUP(0.2);</script>");
  } else if(setvolume == 2)
  {
  client.println("<script>VolumeUP(0.4);</script>");
  }
  else if(setvolume == 3)
  {
  client.println("<script>VolumeUP(0.6);</script>");
  }
  else if(setvolume == 4)
  {
  client.println("<script>VolumeUP(0.8);</script>");
  }
setvolume = 0;
client.println("<h2>Power</h3>");
if(power == "on"){
  client.println("  <a><button class=\"button active\">ON</button></a>");
  client.println("  <a href=\"/power/off\"><button class=\"button off\">OFF</button></a>");
  client.println("<!--if on/off show correct status  -->");
}else{
  client.println("  <a href=\"/power/on\"><button class=\"button \">ON</button></a>");
  client.println("  <a><button class=\"button active\">OFF</button></a>");
  
  }
client.println("  <h2> Distance</h2>");
  if(delta == "on"){
  client.println("  <a><button class=\"button active\">ON</button></a>");
  client.println("  <a href=\"/distance/off\"><button class=\"button off\">OFF</button></a>");
  client.println("<!--if on/off show correct status  -->");
  }else{
  client.println("  <a href=\"/distance/on\"><button class=\"button \">YES</button></a>");
  client.println("  <a><button class=\"button active\">NO</button></a>");
  
  }
client.println("  <h2>Humidity/ Temperature</h2>");
client.println("  <a href=\"/temp/on\"><button class=\"button\">Yes</button></a>");
client.println("  <a href=\"/temp/off\"><button class=\"button off\">No</button></a>");
    
            
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            Serial.println("--> Ending HTML ");
            // Break out of the while loop
            break;
       } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }

      
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
delay(1000);
}

