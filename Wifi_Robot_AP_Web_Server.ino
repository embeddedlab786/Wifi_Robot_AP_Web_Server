/* https://arduino.esp8266.com/stable/package_esp8266com_index.json
 * ESP8266 as Web Server using WiFi Access Point (AP) mode
 * Connect to AP "Robot Wifi", password = "87654321"
 * Open browser, visit 192.168.4.1
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Hash.h>

//SSID and Password to your ESP Access Point
const char* ssid = "Robot Wifi";
const char* password = "87654321";

#define ENA   4     // Enable/speed motors Right    GPIO4(D2)
#define IN_1  0     // L298N in1 motors Right       GPIO0(D3)
#define IN_2  2     // L298N in2 motors Right       GPIO2(D4)
#define IN_3  12    // L298N in3 motors Left        GPIO12(D6)
#define IN_4  13    // L298N in4 motors Left        GPIO13(D7)
#define ENB   15    // Enable/speed motors Left     GPIO15(D8)

#define Light  16 // Light  GPIO16(D0)

int speedCar = 150; // 0 to 255
char Data;

static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name = "viewport" content = "width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
<title>ESP8266 Robot</title>
<style>
"body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }"
#JD {
  text-align: center;
}
#JD {
  text-align: center;
  font-family: "Lucida Sans Unicode", "Lucida Grande", sans-serif;
  font-size: 24px;
}
.foot {
  text-align: center;
  font-family: "Comic Sans MS", cursive;
  font-size: 30px;
  color: #F00;
}
.button {
    border: none;
    color: white;
    padding: 17px;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 16px;
    margin: 4px 2px;
    cursor: pointer;
    border-radius: 12px;
  width: 100%;
}
.red {background-color: #F00;}
.green {background-color: #090;}
.yellow {background-color:#F90;}
.blue {background-color:#03C;}
</style>
<script>
var websock;
function start() {
  websock = new WebSocket('ws://' + window.location.hostname + ':81/');
  websock.onopen = function(evt) { console.log('websock open'); };
  websock.onclose = function(evt) { console.log('websock close'); };
  websock.onerror = function(evt) { console.log(evt); };
  websock.onmessage = function(evt) {
    console.log(evt);
    var e = document.getElementById('ledstatus');
    if (evt.data === 'ledon') {
      e.style.color = 'red';
    }
    else if (evt.data === 'ledoff') {
      e.style.color = 'black';
    }
    else {
      console.log('unknown event');
    }
  };
}
function buttonclick(e) {
  websock.send(e.id);
}
</script>
</head>
<body onload="javascript:start();">
&nbsp;
<table width="100%" border="1">
  <tr>
    <td bgcolor="#FFFF33" id="JD">Robot Controller</td>
  </tr>
</table>
<table width="100" height="249" border="0" align="center">
<tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> 
<tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> 
            
    <td>&nbsp;</td>
    <td align="center" valign="middle"><form name="form1" method="post" action="">
      <label>
        <button id="F"  type="button" onclick="buttonclick(this);" class="button green">Forward</button> 
      </label>
    </form></td>
    <td>&nbsp;</td>
  </tr>
<tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> 
   
    <td align="center" valign="middle"><form name="form1" method="post" action="">
      <label>
        <button id="L"  type="button" onclick="buttonclick(this);" class="button green">Turn_Left</button> 
      </label>
    </form></td>
    <td align="center" valign="middle"><form name="form1" method="post" action="">
      <label>
        <button id="S"  type="button" onclick="buttonclick(this);" class="button red">Stop</button> 
      </label>
    </form></td>
    <td align="center" valign="middle"><form name="form1" method="post" action="">
      <label>
        <button id="R"  type="button" onclick="buttonclick(this);" class="button green">Turn_Right</button> 
      </label>
    </form></td>
  </tr>
<tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> 
  
    <td>&nbsp;</td>
    <td align="center" valign="middle"><form name="form1" method="post" action="">
      <label>
        <button id="B"  type="button" onclick="buttonclick(this);" class="button green">Backward</button> 
      </label>
    </form></td>     
    <td>&nbsp;</td>
  </tr>
<tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr>

   
        <td align="center" valign="middle"><form name="form1" method="post" action="">
      <label>
        <button id="a"  type="button" onclick="buttonclick(this);" class="button blue">Speed Low</button> 
      </label>
    </form></td>
        <td align="center" valign="middle"><form name="form1" method="post" action="">
      <label>
        <button id="b"  type="button" onclick="buttonclick(this);" class="button blue">Speed Medium</button> 
      </label>
    </form></td>
        <td align="center" valign="middle"><form name="form1" method="post" action="">
      <label>
        <button id="c"  type="button" onclick="buttonclick(this);" class="button blue">Speed High</button> 
      </label>
    </form></td>
</tr>
<tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr> <tr>

        <td align="center" valign="middle"><form name="form1" method="post" action="">
      <label>
        <button id="o"  type="button" onclick="buttonclick(this);" class="button yellow">Light On</button> 
      </label>
    </form></td>
      <td align="center" valign="middle"><form name="form1" method="post" action="">
&nbsp;
    </form></td>
        <td align="center" valign="middle"><form name="form1" method="post" action="">
      <label>
        <button id="f"  type="button" onclick="buttonclick(this);" class="button yellow">Light Off</button> 
      </label>
    </form></td>
  </tr>
  
</table>
<p class="foot">Muhammad Ansar</p>
</body>
</html>
)rawliteral";

WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer server(80);


void setup() {
 Serial.begin(115200);
 
 pinMode(ENA, OUTPUT); 
 pinMode(IN_1, OUTPUT);
 pinMode(IN_2, OUTPUT);
 pinMode(IN_3, OUTPUT);
 pinMode(IN_4, OUTPUT);
 pinMode(ENB, OUTPUT); 

 pinMode(Light, OUTPUT); 
 
  delay(1000);
  Serial.println (">> Setup");
  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security
 
  IPAddress myIP = WiFi.softAPIP(); //Get IP address
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");   
  Serial.println(myIP);

  server.on("/", [](){
  server.send(200, "text/html", INDEX_HTML);
  });
  
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

}

void loop() {

     if(Data=='a')speedCar = 70;
else if(Data=='b')speedCar = 150;
else if(Data=='c')speedCar = 255;
else if(Data=='o')digitalWrite(Light, HIGH); // light is on
else if(Data=='f')digitalWrite(Light, LOW);  // light is off
else if(Data=='F')forword();
else if(Data=='B')backword();
else if(Data=='R')turnRight();
else if(Data=='L')turnLeft();
else if(Data=='S')Stop();

analogWrite(ENA, speedCar);
analogWrite(ENB, speedCar);
      
   webSocket.loop();
   server.handleClient();
}



void webSocketEvent(uint8_t num, WStype_t type,uint8_t * Payload, size_t length){
  switch(type) {
    case WStype_DISCONNECTED:
      //Serial.printf("[%u] Disconnected!\r\n", num);
      break;
    case WStype_CONNECTED:
      {IPAddress ip = webSocket.remoteIP(num);}
      break;
    case WStype_TEXT:
      //Serial.printf("%s\r\n", Payload);
      Data = Payload[0];
      Serial.println(Data);
      // send data to all connected clients
      webSocket.broadcastTXT(Payload, length);
     
      break;
    case WStype_BIN:
      hexdump(Payload, length);
      // echo data back to browser
      webSocket.sendBIN(num, Payload, length);
      break;
      default:
      break;
  }
}

void forword(){  //forword
digitalWrite(IN_1, HIGH); //Right Motor forword Pin 
digitalWrite(IN_2, LOW);  //Right Motor backword Pin 
digitalWrite(IN_3, LOW);  //Left Motor backword Pin 
digitalWrite(IN_4, HIGH); //Left Motor forword Pin 
}

void backword(){ //backword
digitalWrite(IN_1, LOW);  //Right Motor forword Pin 
digitalWrite(IN_2, HIGH); //Right Motor backword Pin 
digitalWrite(IN_3, HIGH); //Left Motor backword Pin 
digitalWrite(IN_4, LOW);  //Left Motor forword Pin 
}

void turnRight(){ //turnRight
digitalWrite(IN_1, LOW);  //Right Motor forword Pin 
digitalWrite(IN_2, HIGH); //Right Motor backword Pin  
digitalWrite(IN_3, LOW);  //Left Motor backword Pin 
digitalWrite(IN_4, HIGH); //Left Motor forword Pin 
}

void turnLeft(){ //turnLeft
digitalWrite(IN_1, HIGH); //Right Motor forword Pin 
digitalWrite(IN_2, LOW);  //Right Motor backword Pin 
digitalWrite(IN_3, HIGH); //Left Motor backword Pin 
digitalWrite(IN_4, LOW);  //Left Motor forword Pin 
}

void Stop(){ //stop
digitalWrite(IN_1, LOW); //Right Motor forword Pin 
digitalWrite(IN_2, LOW); //Right Motor backword Pin 
digitalWrite(IN_3, LOW); //Left Motor backword Pin 
digitalWrite(IN_4, LOW); //Left Motor forword Pin 
}
