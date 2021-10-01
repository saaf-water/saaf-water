#include <Adafruit_ADS1X15.h>
#include "DFRobot_ESP_EC.h"
#include "DFRobot_ESP_PH_WITH_ADC.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "EEPROM.h"

#include <SPI.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
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

// Add WiFi connection information
char ssid[] = "------";     //  your network SSID (name) 
char pass[] = "------";  // your network password

#define LED_NUM 16
// --------------------------------------------------------------------------------------------
//        SHOULD NOT NEED TO CHANGE ANYTHING BELOW THIS LINE
// --------------------------------------------------------------------------------------------
WiFiClient wifiClient;

void callback(char* topic, byte* payload, unsigned int length);
PubSubClient mqtt(MQTT_HOST, MQTT_PORT, callback, wifiClient);

#define LED_NUM 16
#define RGB_PIN 19
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM, RGB_PIN, NEO_GRB + NEO_KHZ800);



DFRobot_ESP_EC ec;
#define ONE_WIRE_BUS 18
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DFRobot_ESP_PH_WITH_ADC ph;
Adafruit_ADS1115 ads;

#define VREF 5.0    
#define SCOUNT  30       


// variables to hold data
DynamicJsonDocument doc(1024);
StaticJsonDocument<1024> jsonDoc;
JsonObject payload = jsonDoc.to<JsonObject>();
JsonObject status = payload.createNestedObject("d");
static char msg[1024];

int wqs = 404;
float tdsAvg = 0.0;
float turbAvg = 0.0;
float phAvg = 0.0;
float ecAvg = 0.0;
float tempAvg = 0.0;

void setup(void){
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  
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

  
  
  ph.begin();
  sensors.begin();
  ec.begin();
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  Serial.println("Getting single-ended readings from AIN0..3");
  Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");                                                            
  ads.setGain(GAIN_TWOTHIRDS); 
  digitalWrite(13, HIGH);
  digitalWrite(14, HIGH);
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
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
  dht.begin();
//  pixel.begin();

  // Connect to MQTT - IBM Watson IoT Platform
  if (mqtt.connect(MQTT_DEVICEID, MQTT_USER, MQTT_TOKEN)) {
    Serial.println("MQTT Connected");
    mqtt.subscribe(MQTT_TOPIC_DISPLAY);

  } else {
    Serial.println("MQTT Failed to connect!");
    ESP.restart();
  }
}

void loop(void){
  //mqtt.loop();
  processBegin();
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqtt.connect(MQTT_DEVICEID, MQTT_USER, MQTT_TOKEN)) {
      Serial.println("MQTT Connected");
      mqtt.subscribe(MQTT_TOPIC_DISPLAY);
      mqtt.loop();
    } else {
      assessData(tdsAvg, turbAvg, phAvg, ecAvg, tempAvg);
      Serial.println("MQTT Failed to connect!");
      delay(5000);
    }
  }
  status["tds"] = tdsAvg;
  status["turb"] = turbAvg;
  status["ph"] = phAvg;
  status["ec"] = ecAvg;
  status["temp"] = tempAvg;
  serializeJson(jsonDoc, msg, 1024);
  Serial.println(msg);
  if (!mqtt.publish(MQTT_TOPIC, msg)) {
    Serial.println("MQTT Publish failed");
  }

  // Pause - but keep polling MQTT for incoming messages
  for (int i = 0; i < 7200; i++) {
    mqtt.loop();
    delay(1000);
  }
}
int getMedianNum(int bArray[], int iFilterLen) 
{
      int bTab[iFilterLen];
      for (byte i = 0; i<iFilterLen; i++)
      bTab[i] = bArray[i];
      int i, j, bTemp;
      for (j = 0; j < iFilterLen - 1; j++) 
      {
      for (i = 0; i < iFilterLen - j - 1; i++) 
          {
        if (bTab[i] > bTab[i + 1]) 
            {
        bTemp = bTab[i];
            bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
         }
      }
      }
      if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
      else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
      return bTemp;
}
void processBegin(){
  digitalWrite(13, HIGH);
  digitalWrite(14, HIGH);
  tempAvg = tempCal();
  delay(1000);
  phAvg = phCal(tempAvg);
  digitalWrite(13, LOW);
  digitalWrite(14, LOW);
  // mqtt.loop();
  delay(10000);
  //mqtt.loop();
  digitalWrite(13, LOW);
  digitalWrite(14, LOW);
  tdsAvg = tdsCal(tempAvg);
  digitalWrite(13, HIGH);
  digitalWrite(14, HIGH);
  //delay(1000);
  //mqtt.loop();
  ecAvg = ecCal(tempAvg, tdsAvg);
  // delay(30000);
  turbAvg = turbCal();
}

float tempCal(){
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  return temperatureC;
}
float tdsCal(float temperature){
  
  int analogBuffer[SCOUNT];   
  int analogBufferTemp[SCOUNT];
  int analogBufferIndex = 0,copyIndex = 0;
  
  float averageVoltage = 0,tdsValue = 0;
  int16_t adc;
  int Exit = 0;
  
  while(Exit != 1){
    Exit = 0;
    adc = ads.readADC_SingleEnded(2);
    analogBuffer[analogBufferIndex] = adc;    //read the analog value and store into the buffer
    analogBufferIndex++;
    delay(40);
    if(analogBufferIndex == SCOUNT){
      analogBufferIndex = 0;
      Exit = 1;
    }
  }
  for(copyIndex=0;copyIndex<SCOUNT;copyIndex++){
    analogBufferTemp[copyIndex]= analogBuffer[copyIndex];
  }
  averageVoltage = (getMedianNum(analogBufferTemp,SCOUNT) * 0.1875) / 1000.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
  float compensationCoefficient=1.0+0.02*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
  float compensationVolatge=averageVoltage/compensationCoefficient;  //temperature compensation
  tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5; //convert voltage value to tds value
  return tdsValue; 
}

float turbCal(){
  float volt = 0;
  float ntu;
  int16_t adc;
  for(int i=0; i<800; i++)
  {
      adc = ads.readADC_SingleEnded(3);
      volt += (adc*0.1875)/1000;
  }
  volt = volt/800;
  ntu = -305.81*volt+1015.29;
  return ntu;
}

float phCal(float temperature){
  int16_t adc;
  float phValue, voltage;
  for(int i=0;i<20;i++){
    voltage = ads.readADC_SingleEnded(1) / 10; // read the voltage
    phValue = ph.readPH(voltage, temperature); // convert voltage to pH with temperature compensation
    phValue -=1.2218;
    ph.calibration(voltage, temperature);
  }
  return phValue;
}

float ecCal(float temperature, float tds){
  float voltage, ecValue;
  int ecSwitch = 1;///////////////////////////////////////////////////////////////////////////////////////////
  if(ecSwitch == 1){
    voltage = ads.readADC_SingleEnded(3) / 10;
    ecValue = ec.readEC(voltage, temperature);
    ec.calibration(voltage, temperature);
    return ecValue*1000;     
  }
  else if(ecSwitch == 0){
    ecValue = 2*tds;
    return ecValue;
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

void assessData(float currentTds, float currentTurbidity, float currentPh, float currentEc, float temp){
  int Summary = -1;
  int paramSummary[] = {-1,-1,-1,-1};

  ///////   TDS    ////////////
  if(currentTds >= 0 && currentTds < 250){
    paramSummary[0] = 1;
  }

  else if(currentTds >= 250 && currentTds < 950){
    paramSummary[0] = 2;
  }
  else if(currentTds >= 950){
    paramSummary[0] = 3; 
  }

  ///////   Turbidity    ////////////
  if(currentTurbidity >= 0 && currentTurbidity <= 2){
    paramSummary[1] = 1;
  }

  else if(currentTurbidity > 2 && currentTurbidity <= 5){
    paramSummary[1] = 2;
  }
  else if(currentTurbidity > 5){
    paramSummary[1] = 3; 
  }

  ///////   pH    ////////////
  if(currentPh >= 6.5 && currentPh <= 8.5){
    paramSummary[2] = 1;
  }
  else if(currentPh < 6.5 || currentPh > 8.5){
    paramSummary[2] = 3; 
  }
  // else if(currentPh > 8.5){
  //     paramSummary[2] = 3; 
  // }
  ///////   Electrical Conductivity    ////////////
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

  for(int i = 0; i<4; i++){
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
  for(int i = 0; i<4; i++){
    if(paramSummary[i] == 3){
        wqs = 0;
    }
  }
  ringDisplay();
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
      strip.show();
      break;
    case 1:
      colorWipe(strip.Color(255, 80, 0), 50);
      strip.show();
      break;
    case 2:
      colorWipe(strip.Color(200, 255, 0), 50);
      strip.show();
      break;
    case 3:
      colorWipe(strip.Color(0, 255, 0), 50);
      strip.show();
      break;
    default:
      colorWipe(strip.Color(0, 0, 0), 50);
      strip.show();
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}