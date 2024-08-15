

const char* save_screen_html = R"save_screen( 
    <!DOCTYPE html>
    <html>
    <head>
        <title>Save Success</title>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <style>
            body {
                text-align: center;
                background-color: #f2f2f2;
                font-family: Arial, sans-serif;
                margin: 0;
                padding: 0;
                height: 100vh;
                display: flex;
                justify-content: center;
                align-items: center;
            }

            .message {
                padding: 20px;
                border-radius: 10px;
                background-color: #4CAF50;
                color: white;
                box-shadow: 0px 0px 10px 0px rgba(0, 0, 0, 0.2);
            }

            .message h1 {
                margin-bottom: 10px;
            }

            .message p {
                margin-top: 0;
            }

            .home-link {
                color: white;
                text-decoration: none;
                font-weight: bold;
                border: 2px solid white;
                padding: 10px 20px;
                border-radius: 5px;
                transition: background-color 0.3s ease;
            }

            .home-link:hover {
                background-color: rgba(255, 255, 255, 0.2);
            }
        </style>
    </head>
    <body>
        <div class="message">
            <h1>Save Successful!</h1>
            <p>Your configuration has been saved.</p>
            

         <form action="/" method="POST">
               <input type="submit" value="Go back to home" style="background-color: green; color: white; border: none; padding: 10px 20px; border-radius: 20px; cursor: pointer; font-size: 16px; transition: background-color 0.3s ease;" onmouseover="this.style.backgroundColor='darkgreen'" onmouseout="this.style.backgroundColor='green'">
          </form>


        </div>
    </body>
    </html>
)save_screen";

const char* htmlPage = R"page(
   <!DOCTYPE html>
    <html>
    <head>
        <title>Moisture Sensor Configuration</title>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <style>
            body {
                text-align: center;
                background-color: #f2f2f2;
                font-family: Arial, sans-serif;
                margin: 0;
                padding: 0;
            }
            .form:before {
                content: '';
                position: absolute;
                top: -20px;
                left: 0px;
                width: 100%;
                height: 40px;
                background-color: #4CAF50;
                border-top-left-radius: 10px;
                border-top-right-radius: 10px;
            }


            .container {
                display: flex;
                flex-direction: column;
                align-items: center;
                justify-content: center;
                height: 100vh;
                padding: 10px;
            }

            .form {
                width: 70%;
                max-width: 300px;
                background-color: #f2f2f2;
                padding: 40px;
                border-radius: 10px;
                box-shadow: 0px 0px 10px 0px rgba(0, 0, 0, 0.2);
                position: relative;
                color: #555;
            }

            .form h1 {
                margin-bottom: 30px;
                color: #4CAF50;
            }

            .form label {
                display: block;
                margin-bottom: 10px;
                color: #555;
                font-size: 16px;
                font-weight: bold;
            }

            .form input[type="text"],
            .form input[type="password"] {
                width: 100%;
                padding: 10px;
                margin-bottom: 20px;
                border-radius: 5px;
                border: none;
                background-color: #fff;
                font-size: 14px;
                color: #555;
            }

            .form input[type="submit"] {
                padding: 10px;
                margin-top: 10px;
                width: 100%; /* Set button width to match input fields */
                border-radius: 5px;
                border: none;
                background-color: #4CAF50;
                color: white;
                font-weight: bold;
                cursor: pointer;
                transition: background-color 0.3s ease;
            }

            .form input[type="submit"]:hover {
                background-color: #45a049;
            }

            .sensor-values {
                margin-top: 30px;
                text-align: left;
            }

            .sensor-values h2 {
                margin-bottom: 10px;
                color: #4CAF50;
            }

            .sensor-values p {
                margin-bottom: 10px;
            }

            .sensor-values span {
                font-weight: bold;
                color: #555;
            }

            .company-info {
                font-size: 12px;
                margin-top: 30px;
                color: #777;
            }

            .company-info a {
                color: #777;
                text-decoration: none;
            }

            .company-info a:hover {
                text-decoration: underline;
            }
        </style>
    </head>
    <body>
    <div class="container">
        <div class="form">
            <h1>Moisture Sensor Configuration</h1>
            <!-- Form for setting min and max thresholds -->
            <form action="/save" method="POST">
                <label for="moisture_min">Minimum Moisture Threshold:</label>
                <input type="text" id="moisture_min" name="moisture_min" placeholder="Input Number from 10-100" pattern="[0-9]*" title="Please enter only numbers">
                <label for="moisture_max">Maximum Moisture Threshold:</label>
                <input type="text" id="moisture_max" placeholder="Input Number Greater Than Threshold Min" name="moisture_max" pattern="[0-9]*" title="Please enter only numbers">
                <input type="submit" value="Save">
            </form>
    <!-- Display current sensor values and thresholds -->
            <div class="sensor-values">
                <h2>Current Sensor Values</h2>
                <div id="dynamic_soil_moisture_list">
                    <p>Soil Moisture: <span id="current_moisture"></span></p>
                </div>
                <h2>Threshold Settings</h2>
                <p>Minimum Threshold: <span id="min_threshold">_min_threashold</span></p>
                <p>Maximum Threshold: <span id="max_threshold">_max_threashold</span></p>
            </div>
            
            <!-- Company information -->
            <div class="company-info">
                <p>&copy; 2023 Ramlaxman Innovations. All rights reserved.</p>
            </div>
        </div>
    </div>
    <script>
      // Function to update soil moisture values
      function updateSoilMoistureList(data) {
          var listContainer = document.getElementById("dynamic_soil_moisture_list");
          // Clear previous content
          listContainer.innerHTML = "";

          // Loop through the list of moisture values and create list items
          for (var i = 0; i < data.length; i++) {
            if (data[i] > 5) { // Ignore sensors whose value are below 5%
                var listItem = document.createElement("p");
                listItem.textContent = (i + 1) + ". Soil Moisture: " + data[i] + "%"; // Add numbering
                listContainer.appendChild(listItem);
              }
          }
      }

      // Function to parse the received JSON data
      function parseMoistureData(jsonData) {
          var data = JSON.parse(jsonData);
          updateSoilMoistureList(data);
      }

      var receivedData = "_received_data";
      parseMoistureData(receivedData);
    </script>
    </body>
 </html> 
)page"; 
  

//Hotspot Local Server
const char* ssid = "RL_Moisture";           // SSID of the Hotspot
const char* password = "ramlaxman";         // Password of the network

//function prototypes
void blink_led(bool led_active);
void handleRoot();
void handleSave();


// ----------------------------- Start the hotspot and start the web-server
void hotspot(){
  WiFi.mode(WIFI_AP);
  Serial.println("\n[Info] Starting Hotspot...");
  if(WiFi.softAP(ssid, password)){
    IPAddress ipAddress = WiFi.softAPIP();
    
    String ip_address = ipAddress.toString();
    server.on("/", handleRoot);       //  Send the actual html page to display
    server.on("/save", handleSave);   //  Save the data into preferences once save is pressed.
    server.begin(); 

    server.begin();
    Serial.println("\n\n---------------- Hotspot Access Point Activated ------------------");
    Serial.println("[Info] IP Address: " + ip_address);
    hotspot_activated = true;
  }
  else {
    Serial.println("[Error] Failed to start Hotspot!");
    hotspot_activated = false;
  }
} // End this function

void handle_webserver(){
  static long hotspot_activation_timer = millis();
  static long start_time;

  if(hotspot_activated != true){
   blink_led(false);
    return; // close if hotspot not activated. 
  }
    blink_led(true);
    if(millis()-hotspot_activation_timer >= 3*60*1000 && WiFi.softAPgetStationNum() == 0){
      Serial.println("No clients connected for too long. Restarting...");
      hotspot_activated = false; 
      ESP.restart();
    } else if (WiFi.softAPgetStationNum() > 0) {
      hotspot_activation_timer = millis(); 
    }

    if(millis()-start_time > 2000){
      start_time = millis(); 
      Serial.println("[Info] Number of Clients: "+ (String)WiFi.softAPgetStationNum());
      Serial.print(".   ");
    }
    server.handleClient();
    yield();
}

void handleRoot() {  //Home Screen
  String response = String(htmlPage);
  // Replace placeholders with actual data
  String receivedData = "[";
  
  for(int i =0; i<num_moisture_sensors; i++){
    receivedData += soil_moisture_percent[i]; 
    if(i < (num_moisture_sensors-1)) 
      receivedData += ", ";
  }
  receivedData += "]";
  // Serial.println(receivedData);



  response.replace("_received_data", String(receivedData));
  response.replace("_min_threashold", String(threashold_min));
  response.replace("_max_threashold",  String(threashold_max));
  server.send(200, "text/html", response);
  // Serial.println(response);
}

void handleSave() { 
    String value_min = server.arg("moisture_min");
    String value_max = server.arg("moisture_max");

    int moisture_min = value_min.toInt();
    int moisture_max = value_max.toInt();
    Serial.printf("\nReceived Data RAW : Threashold Min: %d, Threashold Max: %d", moisture_min, + moisture_max);
   
    moisture_min  = constrain(moisture_min, 6, 90);
    moisture_max = constrain(moisture_max, moisture_min+10, 100);
    
    // make changes apply instantly
    threashold_min = moisture_min;
    threashold_max = moisture_max;

    Serial.printf("\nReceived Data Constrained\n[Info] Threashold Min: %d\n[Info] Threashold Max: %d", moisture_min, + moisture_max);
    preferences.begin("moisture_config", false);
    preferences.clear();
    preferences.putInt("moisture_min", moisture_min);
    preferences.putInt("moisture_max", moisture_max);
    preferences.end();
    Serial.println("[Info] Values Saved Successfully!!!\n"); 
    server.send(200, "text/html", save_screen_html);
}
 



void blink_led(bool led_active){

  unsigned long currentMillis = millis(); // Get the current time
  if (currentMillis - previousMillis >= 200) {
    previousMillis = currentMillis;
    ledState = !ledState;
    if (ledState) {
      leds[0] = COLOR; // Turn the LED on
      digitalWrite(indication_LED, HIGH);
    } else {
      leds[0] = CRGB::Black; // Turn the LED off
      digitalWrite(indication_LED, LOW);
    }
    
    if(led_active)
      FastLED.show();
    else{
      leds[0] = CRGB::Black; // Turn the LED off
      FastLED.show();
      digitalWrite(indication_LED, LOW);
    }
  }
}