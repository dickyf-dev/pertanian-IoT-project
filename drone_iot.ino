// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <SimpleTimer.h>
#define trigPin D8 //////////////////D8
#define echoPin D7//////////////////D7
#define led2 D6//////////////////////D6
#define led D5////////////////////D5
#define buzzer D0//////////////////D0

SimpleTimer timer;

int sound = 250;

// Replace with your network credentials
const char* ssid = "ding"; // Input your wifi network name
const char* password = "dickyf13"; // Input your wifi password

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
Servo relay3;
// Auxiliar variables to store the current output state
String relay1State = "off";
String relay2State = "off";
String relay3State = "off";
String relay4State = "off";

// Assign output variables to GPIO pins
const int relay1 = 5; // GPIO5 D1
const int relay2 = 4; // GPIO4 D2
//relay3.attach(D3); // GPIO0 D3
const int relay4 = 2; // GPIO2 D4

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  relay3.attach(D3);
  pinMode(relay4, OUTPUT);
  // Set outputs to HIGH. relay active LOW
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  relay3.write(0);
  digitalWrite(relay4, HIGH);

  //ultrasonik
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
//  pinMode(led3, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
 
 //tes flip flop lampu 
for (int i=0; i<12; i++) 
    {
digitalWrite(led , HIGH);
delay(100);
digitalWrite(led , LOW);
delay(100);
  }
    
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {///////////////////////////////////////////////////////////VOID LOOP

  

  

//ultrasonik/////////////////////////////////////////////////////////////////////////////////
  long duration, distance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

            //utrasonik
////////////////////////////////kondisi landing or take off
  if (distance <= 12) {
   // digitalWrite(led, HIGH);
}
//  else {
//    digitalWrite(led,LOW);
//  }


///////////////////////////////kondisi bahaya 10 sampai 20
 else if (distance > 12 && distance <= 20) {
      for (int i=0; i<6; i++) 
    {
    digitalWrite(buzzer,HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);    
    }
  //  digitalWrite(led, HIGH);
     //sound = 260;
      
}

///////////////////////////////kondisi bahaya 10 sampai 20
// else if (distance > 20) {
//      for (int i=0; i<6; i++) 
//    {
//    digitalWrite(buzzer,HIGH);
//    delay(100);
//    digitalWrite(buzzer, LOW);
//    delay(2000);    
//    }
//    digitalWrite(led, HIGH);
//     //sound = 260;
//      
//}

///////////////////////////kondisi aman
  else {

    digitalWrite(buzzer,HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(1000); 
//      digitalWrite(buzzer, LOW);
//      digitalWrite(led, LOW);
  }
/*  if (distance < 20) {
      digitalWrite(led3, HIGH);
      sound = 270;
} 
  else {
    digitalWrite(led3, LOW);
  }

  */
 /* if (distance < 15) {
    digitalWrite(led4, HIGH);
    sound = 280;
}
  else {
    digitalWrite(led4,LOW);
  }
  if (distance < 10) {
    digitalWrite(led5, HIGH);
    sound = 290;
}
  else {
    digitalWrite(led5,LOW);
  }
  if (distance < 5) {
    digitalWrite(led6, HIGH);
    sound = 300;
}
  else {
    digitalWrite(led6,LOW);
  }
 */

 /////////////////////////////////cek monitor
  if (distance > 40 || distance <= 0){
    Serial.println("Out of range");
    //noTone(buzzer);
  }
  else {
    Serial.print(distance);
    Serial.println(" cm");
    //tone(buzzer,sound);
    
  }
//  delay(100);
   
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
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
            digitalWrite(led , HIGH);//////////////////tambahan indikator

            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0)     ///////////////// port 1
            {
              Serial.println("GPIO 5 on");
              relay1State = "on";
              digitalWrite(relay1, LOW);
            }
            else if (header.indexOf("GET /5/off") >= 0)
            {
              Serial.println("GPIO 5 off");
              relay1State = "off";
              digitalWrite(relay1, HIGH);
            }
            else if (header.indexOf("GET /4/on") >= 0) { ///////////////// port 2
              Serial.println("GPIO 4 on");
              relay2State = "on";
              digitalWrite(relay2, HIGH);
            }
            else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              relay2State = "off";
              digitalWrite(relay2, LOW);
            }
            else if (header.indexOf("GET /0/on") >= 0)
            {
              Serial.println("GPIO 0 on");
              relay3State = "on";
              relay3.write(90);
               Serial.println("servo berJALAN 120 derajat");
            }
            else if (header.indexOf("GET /0/off") >= 0)
            {
              Serial.println("GPIO 0 off");
              relay3State = "off";
              relay3.write(0);
            }
            else if (header.indexOf("GET /2/on") >= 0) {
              Serial.println("GPIO 2 on");
              relay4State = "on";
              digitalWrite(relay4, LOW);
            }
            else if (header.indexOf("GET /2/off") >= 0) {
              Serial.println("GPIO 2 off");
              relay4State = "off";
              digitalWrite(relay4, HIGH);
            }
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css'>");
            client.println("<link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.7.0/css/all.css'integrity='sha384-lZN37f5QGtY3VHgisS14W3ExzMWZxybE1SJSEsQp9S+oqd12jhcu+A56Ebc1zFSJ' crossorigin='anonymous'>");
            
            
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 12px 24px;");
            client.println("text-decoration: none; font-size: 15px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}.button1{ width:600px; height:107px; background:#FAD7A0; display: inline-block; margin-left: 0px; margin-top: 6px;}");
            client.println(".b1{ width:250px; height:87px; background:#D5DBDB; display: inline-block; margin-right: 10px; margin-top: 10px;}");
            client.println(".b2{ width:250px; height:87px; background:#D5DBDB; display: inline-block; margin-left: 10px; margin-top: 10px;}");
             client.println(".button11{ width:600px; height:105px; background:#82E0AA; display: inline-block; margin-left: 0px; }");
             client.println("</style></head>");
 
            // Web Page Heading
            client.println("<body><div><img src='http://vayuyan.com/images/Agricultural%20Drones.jpg' style='width:600px;height:50px;text-align:center;'> </img> </div>    <!-- <h2>Aplikasi AGREDROBOT</h2>-->");

            // Display current state,and ON/OFF buttons for GPIO 5
            client.println("<div class='button11'><div class='b1'> Pupuk cair -><b> Status " + relay1State );
            client.println("</b>");
            // If the relay1State is off, it displays the ON button
            if (relay1State == "off") {
              client.println("<p></p> <a href=\"/5/on\"><button class=\"button\"><i class='fas fa-tint' style='font-size:18px;color:lightblue;'></i>  ON</button></a></div>");
            } else {
              client.println("<p></p><a href=\"/5/off\"><button class=\"button button2\"><i class='fas fa-tint' style='font-size:18px;color:lightblue;'></i>   OFF</button></a></div>");
            }
            // Display current state, and ON/OFF buttons for GPIO 4
            client.println(" <div class='b2'>Lampu indikator -> <b> Status " + relay2State );
            client.println("</b>");
            // If the relay2State is off, it displays the ON button
            if (relay2State == "off") {
              client.println("<p></p><a href=\"/4/on\"><button class=\"button\"><i class='far fa-lightbulb' style='font-size:18px;color:lightyellow;'></i>     ON</button></a></div></div>");
            } else {
              client.println("<p></p><a href=\"/4/off\"><button class=\"button button2\"><i class='far fa-lightbulb' style='font-size:18px;color:lightyellow;'></i>    OFF</button></a> </div></div>");
            }

            // Display current state, and ON/OFF buttons for GPIO 0
            client.println("<div class='button1'><div class='b1'>Pupuk Padat -><b> Status " + relay3State  );
            client.println("</b>");
            // If the relay1State is off, it displays the ON button
            if (relay3State == "off") {
              client.println("<p></p><a href=\"/0/on\"><button class=\"button\"><i class='fas fa-tint-slash' style='font-size:18px;color:lightblue;'></i>     ON</button></a></div>");
            } else {
              client.println("<p></p><a href=\"/0/off\"><button class=\"button button2\"><i class='fas fa-tint-slash' style='font-size:18px;color:lightblue;'></i>      OFF</button></a></div>");
            }

            // Display current state, and ON/OFF buttons for GPIO 2
            client.println("<div class='b2'>Baling-baling -><b> Status " + relay4State );
            client.println("</b>");
            // If the relay2State is off, it displays the ON button
            if (relay4State == "off") {
              client.println("<p></p><a href=\"/2/on\"><button class=\"button\"><i class='fa fa-spinner fa-spin' style='font-size:18px;color:pink;'></i>       ON</button></a></div> </div>");
            } else {
              client.println("<p></p><a href=\"/2/off\"><button class=\"button button2\"><i class='fa fa-spinner fa-spin' style='font-size:18px;color:pink;'></i>      OFF</button></a> </div></div>");
            }

            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
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
    //Serial.println("");
    
  }

//timer.setTimeout(500,coba);
}





void coba(){

}
