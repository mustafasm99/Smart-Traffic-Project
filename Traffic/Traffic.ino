#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WebServer.h>


// create connection to the network 
//  make sure that the wifi you will use name as project 
// the password is 12345678

const char* ssid        = "project";    // wifi name 
const char* password    = "12345678";  //  wifi password

// const char* ssid        = "Ares_Wifi";
// const char* password    = "SM99sm99";

WebServer server(80);                    // set the web server on port 80 internet port 

// these is the roud counter or the light sensor pins and deal with them 
const int sensorPins[4] = {32,33,34,35}; // four pins four roads
int carCounts[4]        =  {0,0,0,0};
int greenDurations[3]   = {5000 , 10000 , 15000};

unsigned long changeInterval  = 5000;    // interval in milliseconds
unsigned long lastChangeTime  = 0;       // time for each trafic     
unsigned long oneSideModeTime = 0;
int oneSideModeInterval       = 5000;   

String API_KEY                = "44ae615b-d3a5-4766-8c9e-f696e18cbe10"; // API KEY
bool EMR_MODE                 = false;
bool ONE_SIDE_MODE            = false;

const int RED                 = 0;
const int YELLOW              = 1;
const int GREEN               = 2;
int roudNumber                = 0; // 1 : west ; 2 : north ; 3 : east ; 4 : south;

// Traffic light state array
int trafficLightsState[4] = {RED, RED, RED, RED};
// these for the 4 sides      no   so  we   es 

// Pin assignments for the traffic lights
const int traffics[4][3] = {
    {23, 22, 21},                     // Traffic Light 1: red, yellow, green
    {15, 2, 0},                       // Traffic Light 2: red, yellow, green
    {4, 16, 17},                      // Traffic Light 3: red, yellow, green
    {5, 18, 19}                       // Traffic Light 4: red, yellow, green
};

// Sequence array representing the truth table you've provided
const int sequence[8][4] = {
    {GREEN,  RED,    RED,    RED   },  // Sequence 1
    {YELLOW, RED,    RED,    RED   },  // Sequence 2
    {RED,    RED,    GREEN,  RED   },  // Sequence 3
    {RED,    YELLOW, YELLOW, RED   },  // Sequence 4
    {RED,    GREEN,  RED,    RED   },  // Sequence 5
    {RED,    YELLOW, RED,    YELLOW},  // Sequence 6
    {RED,    RED,    RED,    GREEN },  // Sequence 7
    {YELLOW, RED,    RED,    YELLOW}   // Sequence 8
};

int currentSequence = 0; // index to track the current sequence

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);


  for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            pinMode(traffics[i][j], OUTPUT);
        }
    }
    lastChangeTime = millis();

  // Connect to Wi-Fi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // end of the connection 

  // set the server ends point 
  server.on("/" , homePage );
  server.on("/setmode" , setMode);
  server.on("/get_info" , get_info);
  server.begin();

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  // check if the wifi is connected 
  server.handleClient();
  unsigned long currentMillis   = millis(); // get the current time for each loop Cycle
  // check the time interval for change the lights 
  if (currentMillis - lastChangeTime >= changeInterval) {
        // Move to the next sequence
        currentSequence = (currentSequence + 1) % 8;
        // Update the traffic light states according to the sequence
        for (int i = 0; i < 4; i++) {
            trafficLightsState[i] = sequence[currentSequence][i];
            // check for green light to change the time 
            if(trafficLightsState[i] == GREEN){
                int cars = carCounts[i];
                if( cars > 0  && cars <= 3){
                  changeInterval = greenDurations[cars - 1];
                }
            }
        }
        // Reflect the changes on the physical traffic lights
        lastChangeTime = currentMillis;
    }
  updateTrafficLights();
 
}

void homePage (){                       // seting the home page code in html
  const char* htmlContent = "<!DOCTYPE html><html><head><title>ESP32 Web Server</title></head><body><h1>Hello from ESP32!</h1><p>This is a simple web server example.</p></body></html>";
  server.send(200 , "text/html" , htmlContent);
}

// return the number of cars in serten roud side 
void read_car_countes(){
  for(int i=0;i<4;i++){
    delay(50);
    int sensorValue  = analogRead(sensorPins[i]);
    if(sensorValue != 0){
      Serial.println(sensorValue);
    }
    if(sensorValue < 300 && sensorValue > 200){
      carCounts[i] = 1;
    }else if (sensorValue < 200 && sensorValue > 100){
      carCounts[i] = 2;
    }else if (sensorValue < 100){
      carCounts[i] = 3;
      if(roudNumber == i){
          oneSideModeInterval = 15000;
      }
    }
  }
}

void updateTrafficLights() {
    if (EMR_MODE == true){
      for(int light = 0 ; light < 4 ; light++){
        digitalWrite(traffics[light][RED]     , LOW);
        digitalWrite(traffics[light][YELLOW]  ,  HIGH);
        digitalWrite(traffics[light][GREEN]   , LOW);
      }
    }
    else if (ONE_SIDE_MODE == true){
      unsigned long  cuerrentOneSideMode = millis();
      if(cuerrentOneSideMode - oneSideModeTime > oneSideModeInterval ){
        ONE_SIDE_MODE       = false;
        EMR_MODE            = false;
        oneSideModeInterval = 5000;
      }
      for(int light = 0 ; light < 4 ; light++){
        if(light != roudNumber ){
          digitalWrite(traffics[light][RED]     , HIGH);
          digitalWrite(traffics[light][YELLOW]  ,  LOW);
          digitalWrite(traffics[light][GREEN]   , LOW);
        }else{
          digitalWrite(traffics[roudNumber][RED]     , LOW);
          digitalWrite(traffics[roudNumber][YELLOW]  , LOW);
          digitalWrite(traffics[roudNumber][GREEN]   , HIGH);
        }
      }
    }
    else {
        for (int light = 0; light < 4; light++) {
          for (int state = 0; state < 3; state++) {
              // Turn the light on if it matches the current state, otherwise turn it off
              digitalWrite(traffics[light][state], trafficLightsState[light] == state ? HIGH : LOW);
          }
        }
        if (currentSequence == 0 || currentSequence == 2 || currentSequence == 5 || currentSequence == 7){
          for(int i = 0 ; i < 4 ; i++){
            carCounts[i] = 0;
          }
        }
        
    }
  read_car_countes();
}


void setMode(){
    String apikey    = server.arg("apikey");
    String mode      = server.arg("mode");

    if(apikey != API_KEY){
      server.send(401 , "text/html" , "<h1> Unauthorized </h1>");
      return;
    }

    if(mode != ""){
      int modeNumber = mode.toInt();
      switch (modeNumber){
        case 0 : // normal mode 
          EMR_MODE = false;
          Serial.println("Normal Mode");
          break;
        case 1 : // emr mode 
          EMR_MODE = true; 
          Serial.println("EMR Mode");
          break; 
        case 2 : // one way mode 
          roudNumber  = server.arg("roudNumber").toInt();
          ONE_SIDE_MODE   = true;
          oneSideModeTime = millis();
          Serial.print("Oone side  Mode");
          Serial.println(roudNumber);
          break;
      };
      
    }
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200 , "text/html" , "<h1> the mdoe set to :"+mode+"</h1>");
}

void get_info(){
  StaticJsonDocument<512> doc;
  JsonArray CarCount      = doc.createNestedArray("CarCount");
  JsonArray traficstates  = doc.createNestedArray("TrafficStates");

  for(int i=0 ; i<4; i++){
    CarCount.add(carCounts[i]);
    traficstates.add(trafficLightsState[i]);
  }

  String output;
  serializeJson(doc , output);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200 , "application/json" , output);
}