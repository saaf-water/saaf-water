#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include "DFRobot_EC.h"
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// --------------------------------------------------------------------------------------------
//        UPDATE CONFIGURATION TO MATCH YOUR ENVIRONMENT
// --------------------------------------------------------------------------------------------

// IBM Watson IoT connection details
#define MQTT_HOST "XXXXX.messaging.internetofthings.ibmcloud.com" // Change XXXXXX -> Your Organisation ID
#define MQTT_PORT 1883
#define MQTT_DEVICEID "d:XXXXXX:YYYYYY:ZZZZZZ" // Change XXXXXX -> Your Organisation ID, YYYYYY -> Device Type, ZZZZZZ -> Device ID
#define MQTT_USER "use-token-auth"
#define MQTT_TOKEN "AAAAAAAAA" // Change AAAAAA -> Your Authentication Token
#define MQTT_TOPIC "iot-2/evt/status/fmt/json"
#define MQTT_TOPIC_DISPLAY "iot-2/cmd/display/fmt/json"

// Add GPIO pins used to connect devices
#define TDS_PIN 35
#define TURB_PIN 32
#define PH_PIN 33
#define EC_PIN 27
#define TEMP_PIN 4
#define RGB_PIN 5

#define LED_NUM 16

#define Vref 3.33
#define Ares 4096
// Add WiFi connection information
char ssid[] = "xxxxxxxx";     //  your network SSID (name) 
char pass[] = "xxxxxxxx";  // your network password

// --------------------------------------------------------------------------------------------
//        SHOULD NOT NEED TO CHANGE ANYTHING BELOW THIS LINE
// --------------------------------------------------------------------------------------------

// MQTT objects
void callback(char* topic, byte* payload, unsigned int length);
WiFiClient wifiClient;
PubSubClient mqtt(MQTT_HOST, MQTT_PORT, callback, wifiClient);

// Adafruit NeoPixel Object
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM, RGB_PIN, NEO_GRB + NEO_KHZ800);

// EC Object
DFRobot_EC ec;

// Onewire Object 
OneWire oneWire(TEMP_PIN);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

// variables to hold data
DynamicJsonDocument doc(1024);
StaticJsonDocument<1024> jsonDoc;
JsonObject payload = jsonDoc.to<JsonObject>();
JsonObject status = payload.createNestedObject("d");
static char msg[1024];

int wqs = 404;

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  
  // Start Neopixel
  strip.begin();
  strip.setBrightness(50);
  strip.show(); 
 // Start serial console
  Serial.begin(115200);
  Serial.setTimeout(2000);
  while (!Serial) { }
  Serial.println();
  Serial.println("Saaf water Hardware node [DEBUG MODE]");

  // Start WiFi connection
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");

  // Start connected devices
  sensors.begin();
  ec.begin();

  // Connect to MQTT - IBM Watson IoT Platform
  if (mqtt.connect(MQTT_DEVICEID, MQTT_USER, MQTT_TOKEN)) {
    Serial.println("MQTT Connected");
    mqtt.subscribe(MQTT_TOPIC_DISPLAY);

  } else {
    Serial.println("MQTT Failed to connect!");
    ESP.reset();
  }
}

void loop() {
  mqtt.loop();
  ringDisplay();
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Attempt to connect
    if (mqtt.connect(MQTT_DEVICEID, MQTT_USER, MQTT_TOKEN)) {
      Serial.println("MQTT Connected");
      mqtt.subscribe(MQTT_TOPIC_DISPLAY);
      mqtt.loop();
    } else {
      Serial.println("MQTT Failed to connect!");
      delay(5000);
    }
  }

  // Collect, Proceess and Access or Send data to Watson IoT Platform
  processBegin();

  // Pause for 2 hrs - but keep polling MQTT for incoming messages 
  for (int i = 0; i < 7200; i++) {
    mqtt.loop();
    delay(1000);
  }
}



void processBegin(){
  float tdsAvg = 0.0;
  float turbAvg = 0.0;
  float phAvg = 0.0;
  float ecAvg = 0.0;
  float tempAvg = 0.0;

  float tdsRT = 0.0;
  float turbRT = 0.0;
  float phRT = 0.0;
  float ecRT = 0.0;
  float tempRT = 0.0;

  for(int i=0; i<20; i++){
    tempRT += tempCal();
    delay(1000);
  }
  tempAvg = tempRT/20;

  for(int i=0; i<20; i++){
    tdsRT += tdsCal();
    delay(1000);
  }
  tdsAvg = tdsRT/20;

  for(int i=0; i<20; i++){
    turbRT += turbCal();
    delay(1000);
  }
  turbAvg = turbRT/20;

  for(int i=0; i<20; i++){
    phRT += phCal();
    delay(1000);
  }
  phAvg = phRT/20;

  for(int i=0; i<20; i++){
    ecRT += ecCal(tempAvg);
    delay(1000);
  }
  ecAvg = ecRT/20;

  printData(tdsAvg, turbAvg, phAvg, ecAvg, tempAvg);

  if(!mqtt.connect(MQTT_DEVICEID, MQTT_USER, MQTT_TOKEN)){
    assessData(tdsAvg, turbAvg, phAvg, ecAvg, tempAvg);
    ringDisplay();
  }
  else{
    sendData(tdsAvg, turbAvg, phAvg, ecAvg, tempAvg);
  }  
}

float tempCal(){
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  return temperatureC;
}

float tdsCal(){
  float voltage = 0;
  float tdsValue = 0;
  int sensorValue = analogRead(TDS_PIN);
  voltage = sensorValue*Vref/Ares;
  tdsValue=(133.42/(pow(voltage,3)) - 255.86*pow(voltage,2) + 857.39*voltage)*0.5;
  return tdsValue;
}

float turbCal(){
  float voltage, ntu;
  voltage = 0;
  for(int i=0; i<800; i++){
      voltage += ((float)analogRead(TURB_PIN)/Ares)*Vref;
  }
  voltage = voltage/800;
  age = round_to_dp(voltage,2);
  if(voltage < (Vref/2)){
    ntu = 3000;
  }
  else{
    ntu = -1120.4*pow(voltage,2)+5742.3*voltage-4353.8; 
  }
  return ntu;
}

float phCal(){
  float pH;
  int sensorValue;
  long sensorSum;
  for(int i=0; i < 50; i++){
      sensorValue = analogRead(PH_PIN);
      sensorSum += sensorValue;
  }
  sensorValue = sensorSum/50;
  pH = 7-1000*(sensorValue-372)*Vref/59.16/Ares;
  return pH;
}

float ecCal(float temperature){
  float voltage, ecValue;  
  ec.calibration(voltage,temperature);
  voltage = analogRead(EC_PIN)/Ares*(Vref*1000);  
  ecValue =  ec.readEC(voltage,temperature); 
  return ecValue*1000; // convert from milli to micro
}

float round_to_dp( float in_value, int decimal_place ){
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;
  return in_value;
}

void sendData(float tds, float turb, float ph, float ec, float temp){
  status["tds"] = tds;
  status["turb"] = turb;
  status["ph"] = ph;
  status["ec"] = ec;
  status["temp"] = temp;
  serializeJson(jsonDoc, msg, 1024);
  Serial.println(msg);
  if (!mqtt.publish(MQTT_TOPIC, msg)) {
    Serial.println("MQTT Publish failed");
  }
}

void printData(float tds, float turb, float ph, float ec, float temp){
  Serial.print("TDS Value = "); 
  Serial.print(tds);
  Serial.println(" PPM");
  
  Serial.print("Turbidity Value = "); 
  Serial.print(turb);
  Serial.println(" NTU");

  Serial.print("pH Value = "); 
  Serial.println(ph);

  Serial.print("EC Value = "); 
  Serial.print(ec);
  Serial.println(" uS/cm");

  Serial.print("Temperature Value = "); 
  Serial.print(temp);
  Serial.println(" °C");
}

void assessData(float tds, float turb, float ph, float ec, float temp){
  int paramSummary = [-1,-1,-1,-1];
  // TDS
  if(currentTds >= 0 && currentTds < 250){
      paramSummary[0] = 1;
  }
  else if(currentTds >= 250 && currentTds < 950){
      paramSummary[0] = 2;
  }
  else if(currentTds >= 950){
      paramSummary[0] = 3; 
  }
  // Turbidity
  if(currentTurbidity >= 0 && currentTurbidity <= 2){
      paramSummary[1] = 1;
  }
  
  else if(currentTurbidity > 2 && currentTurbidity <= 5){
      paramSummary[1] = 2;
  }
  else if(currentTurbidity > 5){
      paramSummary[1] = 3; 
  }
  // pH 
  if(currentPh >= 6.5 && currentPh <= 8.5){
      paramSummary[2] = 1;
  }
  else if(currentPh < 6.5 || currentPh > 8.5){
      paramSummary[2] = 3; 
  }
  // EC
  if(currentEc >= 0 && currentEc < 750){
      paramSummary[3] = 1;
  }
  else if(currentEc >= 750 && currentEc < 3000){
      paramSummary[3] = 2;
  }
  else if(currentEc >= 3000){
      paramSummary[3] = 3; 
  }

  int countY = 0;
  int Rflag =0;
  for(i = 0; i<4; i++){
      if(paramSummary[i] == 2){
          countY++;
      }
  }
  if(countY == 0 || countY == 1){
      wqs = 3;
  }
  else if(countY == 2){
      wqs = 2;
  }
  else if(countY == 3){
      wqs = 1;
  }
  for(i = 0; i<4; i++){
      if(paramSummary[i] == 3){
          wqs = 0;
          break;
      }
  }
}


void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] : ");

  payload[length] = 0; 
  Serial.println((char *)payload);// ensure valid content is zero terminated so can treat as c-string
  
  DeserializationError error = deserializeJson(doc, payload);
  if (error)
    return;
  wqs = doc["summary"];
  ringDisplay();
  
}

void ringDisplay(){
  switch (wqs){
    case 0:
      colorWipe(strip.Color(255, 0, 0), 50);
      break;
    case 1:
      colorWipe(strip.Color(255, 80, 0), 50);
      break;
    case 2:
      colorWipe(strip.Color(200, 255, 0), 50);
      break;
    case 3:
      colorWipe(strip.Color(0, 255, 0), 50);
      break;
    default:
      colorWipe(strip.Color(0, 0, 0), 50);
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}