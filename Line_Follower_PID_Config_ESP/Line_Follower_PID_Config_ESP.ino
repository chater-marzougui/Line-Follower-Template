#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include <ESP8266mDNS.h>

const char* ssid = "infinix7";
const char* password = "chater123";

ESP8266WebServer server(80);
SoftwareSerial arduino(D2, D3);

// Variables to store the values
int baseSpeed = 200;
float kp = 0.5;
float kd = 0.14;
String ExistingData = "";

void setup() {
  Serial.begin(115200);
  arduino.begin(4800);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("pidconfig")) {
    Serial.println("MDNS responder started");
  }
  // Define routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/update", HTTP_POST, handleUpdate);
  server.on("/getData", HTTP_POST, getData);
  server.on("/getQTRData", HTTP_POST, getQTRData);
  server.on("/toggleWorking", HTTP_POST, workAndStop);

  server.begin();

  arduino.println("getData");
  delay(50);
  while(arduino.available()>0){
    ExistingData = arduino.readStringUntil('\n');
  }
}

void loop() {
  server.handleClient();
  MDNS.update();
  
}

void handleRoot() {
 String html = getWebsite();


  server.send(200, "text/html", html);
}

void handleUpdate() {
  if (server.hasArg("baseSpeed")) {
    baseSpeed = server.arg("baseSpeed").toInt();
  }
  if (server.hasArg("kp")) {
    kp = server.arg("kp").toFloat();
  }
  if (server.hasArg("kd")) {
    kd = server.arg("kd").toFloat();
  }

  String response = "{\"baseSpeed\" :"+ String(baseSpeed) + ", \"kd\": " + String(kd) + ", \"kp\": " + String(kp) + "}";
  arduino.println("{\"baseSpeed\":\"" + String(baseSpeed) + "\",\"kp\":\"" + String(kp) + "\",\"kd\":\"" + String(kd) + "\"}");

  server.send(200, "text/plain", response);
}

void getData() {
  arduino.println("getData");
  delay(50);
  while(arduino.available()>0){
    ExistingData = arduino.readStringUntil('\n');
  }
  server.send(200, "text/plain", ExistingData);
}

void getQTRData() {
  arduino.println("QTR8RC");
  delay(50);
  while(arduino.available()>0){
    ExistingData = arduino.readStringUntil('\n');
  }
  server.send(200, "text/plain", ExistingData);
}

void workAndStop(){
  if (server.hasArg("work")) {
    String work = server.arg("work");
    arduino.println(work);
    delay(50);
  }
  String runor;
  while(arduino.available()>0){
    runor = arduino.readStringUntil('\n');
  }
  Serial.println("state :" + runor);
  server.send(200, "text/plain", runor);
}

String getWebsite(){
  String html = "<!DOCTYPE html>";
  html+= "<html lang='en'>";
  html+= "<head>";
  html+= "  <meta charset='UTF-8'>";
  html+= "  <meta name='viewport' content='width=device-width, initial-scale=1'>";
  html+= "  <meta http-equiv='X-UA-Compatible' content='ie=edge'>";
  html+= "  <meta name='author' content='Chater Marzougui'>";
  html+= "";
  html+= "  <title>ESP8266 Control Panel</title>";
  html+= "  <link rel='icon' href='https://www.arduino.cc/en/uploads/Trademark/ArduinoCommunityLogo.png' type='image/x-icon'>";
  html+= "  <style>";
  html+= "    body {";
  html+= "      font-family: Helvetica, Arial, sans-serif;";
  html+= "      display: flex;";
  html+= "      flex-direction: column;";
  html+= "      align-items: center;";
  html+= "      margin: 0;";
  html+= "      padding: 0;";
  html+= "      background-color: #f0f0f0;";
  html+= "    }";
  html+= "    h1 {";
  html+= "      margin-top: 20px;";
  html+= "      color: #333;";
  html+= "    }";
  html+= "    .row {";
  html+= "      display: flex;";
  html+= "      width: 100%;";
  html+= "      margin-top: 20px;";
  html+= "    }";
  html+= "    .container {";
  html+= "      background-color: #fff;";
  html+= "      padding: 20px;";
  html+= "      border-radius: 8px;";
  html+= "      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);";
  html+= "      text-align: center;";
  html+= "      margin: 10px;";
  html+= "    }";
  html+= "    .last {";
  html+= "      flex-grow: 1;";
  html+= "    }";
  html+= "    .input-group {";
  html+= "      margin: 10px 0;";
  html+= "    }";
  html+= "    .input-group label {";
  html+= "      display: block;";
  html+= "      margin-bottom: 5px;";
  html+= "      color: #555;";
  html+= "    }";
  html+= "    .input-group input {";
  html+= "      width: 100%;";
  html+= "      padding: 10px;";
  html+= "      border: 1px solid #c80d0ddb;";
  html+= "      border-radius: 4px;";
  html+= "    }";
  html+= "    .button {";
  html+= "      background-color: #195B6A;";
  html+= "      border: none;";
  html+= "      color: white;";
  html+= "      padding: 16px 40px;";
  html+= "      text-decoration: none;";
  html+= "      font-size: 16px;";
  html+= "      margin: 10px 0;";
  html+= "      cursor: pointer;";
  html+= "      border-radius: 4px;";
  html+= "    }";
  html+= "    .button2 {";
  html+= "      background-color: #77878A;";
  html+= "    }";
  html+= "    #response {";
  html+= "      margin-top: 20px;";
  html+= "      color: #333;";
  html+= "    }";
  html+= "    table {";
  html+= "      width: 100%;";
  html+= "      margin: 20px 0;";
  html+= "      border-collapse: collapse;";
  html+= "      background: white;";
  html+= "      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);";
  html+= "    }";
  html+= "    table#data-table-QTR th {";
  html+= "        background-color: #195B6A; /* Consistent with first table */";
  html+= "        color: white;";
  html+= "    }";
  html+= "    table#data-table-QTR tr:hover {";
  html+= "        background-color: #f1f1f1; /* Consistent hover effect */";
  html+= "    }";
  html+= "    th, td {";
  html+= "        padding: 12px;";
  html+= "        text-align: center;";
  html+= "        border-bottom: 1px solid #ddd;";
  html+= "    }";
  html+= "    th {";
  html+= "        background-color: #195B6A;";
  html+= "        color: white;";
  html+= "    }";
  html+= "    tr:hover {";
  html+= "        background-color: #f1f1f1;";
  html+= "    }";
  html+= "  </style>";
  html+= "</head>";
  html+= "<body>";
  html+= "  <h1>ESP8266 Control Panel</h1>";
  html+= "  <div class='row'>";
  html+= "    <div class='container'>";
  html+= "      <div class='input-group'>";
  html+= "        <label for='baseSpeed'>Base Speed:</label>";
  html+= "        <input type='text' id='baseSpeed' value=''>";
  html+= "      </div>";
  html+= "      <div class='input-group'>";
  html+= "        <label for='kp'>Kp:</label>";
  html+= "        <input type='text' id='kp' value=''>";
  html+= "      </div>";
  html+= "      <div class='input-group'>";
  html+= "        <label for='kd'>Kd:</label>";
  html+= "        <input type='text' id='kd' value=''>";
  html+= "      </div>";
  html+= "      <button class='button' onclick='sendValues()'>Update Values</button>";
  html+= "      <button class='button button2' onclick='getValues()'>Get Values</button>";
  html+= "      <p id='response'></p>";
  html+= "      <table id='data-table'>";
  html+= "          <thead>";
  html+= "              <tr>";
  html+= "                  <th>Base Speed</th>";
  html+= "                  <th>Kp</th>";
  html+= "                  <th>Kd</th>";
  html+= "              </tr>";
  html+= "          </thead>";
  html+= "          <tbody>";
  html+= "          </tbody>";
  html+= "      </table>";
  html+= "    </div>";
  html+= "    <div>";
  html+= "      <button class='button' id='toggleWorking' onclick='toggleWorking()'>Stop</button>";
  html+= "    </div>";
  html+= "    <div class='container last'>";
  html+= "      <div>";
  html+= "        <button class='button' id='toggleQTRButton' onclick='toggleQTRPolling()'>Start QTR Sensor Polling</button>";
  html+= "      </div>";
  html+= "      <table id='data-table-QTR'>";
  html+= "        <thead>";
  html+= "          <tr>";
  html+= "            <th>QTR Sensor Values</th>";
  html+= "            <th>Position</th>";
  html+= "          </tr>";
  html+= "        </thead>";
  html+= "        <tbody>";
  html+= "        </tbody>";
  html+= "      </table>";
  html+= "    </div>";
  html+= "  </div>";
  html+= "  <script>";
  html+= "    var work = true;";
  html+= "    function sendValues() {";
  html+= "      var baseSpeed = document.getElementById('baseSpeed').value;";
  html+= "      var kp = document.getElementById('kp').value;";
  html+= "      var kd = document.getElementById('kd').value;";
  html+= "      var xhr = new XMLHttpRequest();";
  html+= "      xhr.open('POST', '/update', true);";
  html+= "      xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');";
  html+= "      xhr.send('baseSpeed=' + baseSpeed + '&kp=' + kp + '&kd=' + kd);";
  html+= "      xhr.onload = function() {";
  html+= "        if (xhr.status >= 200 && xhr.status < 300) {";
  html+= "          addToTable(xhr.responseText);";
  html+= "        } else {";
  html+= "          document.getElementById('response').innerText = 'Error: ' + xhr.statusText;";
  html+= "        }";
  html+= "      };";
  html+= "    }";
  html+= "    function toggleWorking() {";
  html+= "      var xhr = new XMLHttpRequest();";
  html+= "      xhr.open('POST', '/toggleWorking', true);";
  html+= "      xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');";
  html+= "      var workString = work ? 'stop' : 'run';";
  html+= "      console.log(workString);";
  html+= "      xhr.send('work=' + workString);";
  html+= "      xhr.onload = function() {";
  html+= "        if (xhr.status >= 200 && xhr.status < 300) {";
  html+= "          work = !work;";
  html+= "          document.getElementById('toggleWorking').textContent = work ? 'Stop' : 'Start';";
  html+= "          document.getElementById('toggleWorking').style.backgroundColor = !work ? '#27bd0c' : '#d71717';";
  html+= "        } else {";
  html+= "          document.getElementById('response').innerText = 'Error: ' + xhr.statusText;";
  html+= "        }";
  html+= "      };";
  html+= "    }";
  html+= "    function getValues() {";
  html+= "      var xhr = new XMLHttpRequest();";
  html+= "      xhr.open('POST', '/getData', true);";
  html+= "      xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');";
  html+= "      xhr.send('');";
  html+= "      xhr.onload = function() {";
  html+= "        if (xhr.status >= 200 && xhr.status < 300) {";
  html+= "          console.log(xhr.responseText);";
  html+= "          addToTable(xhr.responseText, false);";
  html+= "        } else {";
  html+= "          document.getElementById('response').innerText = 'Error: ' + xhr.statusText;";
  html+= "        }";
  html+= "      };";
  html+= "    }";
  html+= "    function addToTable(data, isSent = true) {";
  html+= "        const tableBody = document.getElementById('data-table').getElementsByTagName('tbody')[0];";
  html+= "        const newRow = tableBody.insertRow(0);";
  html+= "";
  html+= "        const cellBaseSpeed = newRow.insertCell(0);";
  html+= "        const cellKp = newRow.insertCell(1);";
  html+= "        const cellKd = newRow.insertCell(2);";
  html+= "        newRow.style.backgroundColor = isSent ? 'rgb(177 29 29 / 25%)' : 'rgb(44 177 29 / 25%)';";
  html+= "        const adaptedData = JSON.parse(data);";
  html+= "        cellBaseSpeed.textContent = adaptedData.baseSpeed;";
  html+= "        cellKp.textContent = adaptedData.kp;";
  html+= "        cellKd.textContent = adaptedData.kd;";
  html+= "    }";
  html+= "    let isPolling = false;";
  html+= "    let pollingInterval;";
  html+= "";
  html+= "    function toggleQTRPolling() {";
  html+= "      isPolling = !isPolling;";
  html+= "      const button = document.getElementById('toggleQTRButton');";
  html+= "      ";
  html+= "      if (isPolling) {";
  html+= "        button.textContent = 'Stop QTR Sensor Polling';";
  html+= "        startQTRPolling();";
  html+= "      } else {";
  html+= "        button.textContent = 'Start QTR Sensor Polling';";
  html+= "        stopQTRPolling();";
  html+= "      }";
  html+= "    }";
  html+= "";
  html+= "    function startQTRPolling() {";
  html+= "      pollingInterval = setInterval(getQTRvalue, 200);";
  html+= "    }";
  html+= "";
  html+= "    function stopQTRPolling() {";
  html+= "      clearInterval(pollingInterval);";
  html+= "    }";
  html+= "    function getQTRvalue() {";
  html+= "      var xhr = new XMLHttpRequest();";
  html+= "      xhr.open('POST', '/getQTRData', true);";
  html+= "      xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');";
  html+= "      xhr.send('');";
  html+= "      xhr.onload = function() {";
  html+= "        if (xhr.status >= 200 && xhr.status < 300) {";
  html+= "          console.log(xhr.responseText);";
  html+= "          addToTableQTR(xhr.responseText);";
  html+= "        } else {";
  html+= "          document.getElementById('response').innerText = 'Error: ' + xhr.statusText;";
  html+= "        }";
  html+= "      };";
  html+= "    }";
  html+= "    function addToTableQTR(data) {";
  html+= "      const tableBody = document.getElementById('data-table-QTR').getElementsByTagName('tbody')[0];";
  html+= "      const newRow = tableBody.insertRow(0);";
  html+= "      const cellQTRValues = newRow.insertCell(0);";
  html+= "      const position = newRow.insertCell(1);";
  html+= "      const adaptedData = JSON.parse(data);  ";
  html+= "      cellQTRValues.textContent = adaptedData.qtrSensorValues.join('\t');";
  html+= "      position.textContent = adaptedData.position;";
  html+= "    }";
  html+= "  </script>";
  html+= "</body>";
  html+= "</html>";
  html+= "";

 return html;
}