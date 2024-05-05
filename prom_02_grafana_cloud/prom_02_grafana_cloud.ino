#include "config.h"
#include "certificates.h"
#include <PromLokiTransport.h>
#include <PrometheusArduino.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"


#define DHTPIN 25
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define LDR 32
#define BUZZER 13
#define LED 5
#define LED_2 12
#define LED_ALARMA 27
#define WATER_sensor 35
#define BUTTON 26

#define Thresh_LED 1700

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

int LED_state;
int alarm_active;

char buffer[256];

// set LCD address, number of columns and rows
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);


PromLokiTransport transport;
PromClient client(transport);


#include <PubSubClient.h>
#include <WiFi.h>

const char* ssid = "grafana_mimir";    //Enter SSID
const char* password = "abelpalomas";  //Enter Password
const char* mqtt_server = "167.235.199.00";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

// Create a write request for 4 series.
WriteRequest req(8, 2048);


TimeSeries temperature(30, "temperature", "{room=\"livingroom\"}");

TimeSeries humidity(30, "humidity", "{room=\"livingroom\"}");

TimeSeries light(30, "light", "{room=\"livingroom\"}");

TimeSeries heat_index(30, "heat_index", "{room=\"livingroom\"}");

TimeSeries light_control(30, "light_control", "{room=\"livingroom\"}");

TimeSeries buzz_control(30, "buzz_control", "{room=\"livingroom\"}");

TimeSeries water_level(30, "water_level", "{room=\"livingroom\"}");

TimeSeries alarm_service(30, "alarm_service", "{room=\"livingroom\"}");


// !!!!!
// IF YOU WANT TO INCREASE THE TOTAL AMOUNT OF TIME SERIES, ALSO INCREASE WriteRequest()
// !!!!!

// Note, metrics with the same name and different labels are actually different series and you would need to define them separately
//TimeSeries ts2(5, "heap_free_bytes", "job=\"esp32-test\",host=\"esp32\",foo=\"bar\"");

int loopCounter = 0;

int LED_control = 0;
int BUZZ_control = 0;

int water_value = 0;

int there_is_water = 0;

int last_button = 1;


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  if (payload[0] == '0' && payload[1] == '1') {
    digitalWrite(LED, HIGH);
    digitalWrite(BUZZER, LOW);
    LED_control = 1;
    BUZZ_control = 0;
  } else if (payload[0] == '1' && payload[1] == '0') {
    digitalWrite(LED, LOW);
    LED_control = 0;
    if (alarm_active == 1) {
      digitalWrite(BUZZER, HIGH);
      BUZZ_control = 1;
    }
  } else if (payload[0] == '1' && payload[1] == '1') {
    digitalWrite(LED, HIGH);
    LED_control = 1;
    if (alarm_active == 1) {
      digitalWrite(BUZZER, HIGH);
      BUZZ_control = 1;
    }
  } else {
    digitalWrite(LED, LOW);
    digitalWrite(BUZZER, LOW);
    LED_control = 0;
    BUZZ_control = 0;
  }
  if (payload[2] == '1') {
    digitalWrite(LED_ALARMA, HIGH);
    alarm_active = 1;
  }
  if (payload[2] == '0') {
    digitalWrite(LED_ALARMA, LOW);
    alarm_active = 0;
  }
  Serial.println();
}

void setup() {
  //WiFi.begin(ssid, password);
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  pinMode(LDR, INPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(WATER_sensor, INPUT);
  pinMode(BUTTON, INPUT);
  pinMode(LED_ALARMA, OUTPUT);

  // initialize LCD
  lcd.init();
  // turn on LCD backlight
  lcd.backlight();

  
  
  mqtt_client.setServer("commands", 1883);
  mqtt_client.setCallback(callback);

  // Wait 5s for serial connection or continue without it
  // some boards like the esp32 will run whether or not the
  // serial port is connected, others like the MKR boards will wait
  // for ever if you don't break the loop.
  uint8_t serialTimeout;
  while (!Serial && serialTimeout < 50) {
    delay(100);
    serialTimeout++;
  }

  Serial.println("Starting");
  Serial.print("Free Mem Before Setup: ");
  Serial.println(freeMemory());

  // Configure and start the transport layer
  transport.setUseTls(true);
  transport.setCerts(grafanaCert, strlen(grafanaCert));
  transport.setWifiSsid(WIFI_SSID);
  transport.setWifiPass(WIFI_PASSWORD);
  //transport.setDebug(Serial);  // Remove this line to disable debug logging of the client.
  if (!transport.begin()) {
    Serial.println(transport.errmsg);
    while (true) {};
  }

  // Configure the client
  client.setUrl(GC_URL);
  client.setPath((char*)GC_PATH);
  client.setPort(GC_PORT);
  client.setUser(GC_USER);
  client.setPass(GC_PASS);
  //client.setDebug(Serial);  // Remove this line to disable debug logging of the client.
  if (!client.begin()) {
    Serial.println(client.errmsg);
    while (true) {};
  }

  mqtt_client.setServer(mqtt_server, mqtt_port);

  // Add our TimeSeries to the WriteRequest
  req.addTimeSeries(temperature);
  req.addTimeSeries(humidity);
  req.addTimeSeries(light);
  req.addTimeSeries(heat_index);
  req.addTimeSeries(light_control);
  req.addTimeSeries(buzz_control);
  req.addTimeSeries(water_level);
  req.addTimeSeries(alarm_service);
  //req.setDebug(Serial);  // Remove this line to disable debug logging of the write request serialization and compression.


  Serial.print("Free Mem After Setup: ");
  Serial.println(freeMemory());

  dht.begin();

  // Intentar conectar al servidor MQTT
  while (!mqtt_client.connected()) {
    Serial.println("Conectando al servidor MQTT...");
    if (mqtt_client.connect("ESP32Client", "hackupc", "nicetry")) {
      Serial.println("Conexión al servidor MQTT establecida.");
    } else {
      Serial.print("Error al conectar al servidor MQTT, rc=");
      Serial.println(mqtt_client.state());
      delay(2000);
    }
  }
  mqtt_client.subscribe("commands");
  // Enviar un mensaje MQTT al servidor
  mqtt_client.publish("events", "Hola desde ESP32!");
  //mqtt_client.subscribe(commands)
};



void loop() {

  // Mantener la conexión MQTT activa
  if (!mqtt_client.connected()) {
    reconnect();
  }
  mqtt_client.loop();

  int64_t time;
  time = transport.getTimeMillis();

  int LDR_value = analogRead(LDR);

  if (LDR_value > Thresh_LED) {
    digitalWrite(LED_2, LOW);
    LED_state = 0;
  } else {
    digitalWrite(LED_2, HIGH);
    LED_state = 1;
  }

  //Serial.println(LDR_value);
  if (!digitalRead(BUTTON)) {
    if ((alarm_active == 1) && (last_button == 1)) {
      digitalWrite(LED_ALARMA, LOW);
      alarm_active = 0;
    } else if ((alarm_active == 0) && (last_button == 1)) {
      digitalWrite(LED_ALARMA, HIGH);
      alarm_active = 1;
    }
    last_button = 0;
  } else {
    last_button = 1;
  }
  Serial.println(alarm_active);
  //Sensor

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C  Heat index: "));
  Serial.print(hic);
  Serial.println(F("°C"));

  lcd.setCursor(0, 0);
  // print message
  sprintf(buffer, "T: %.1fC   LED=%d", t, LED_control);
  lcd.print(buffer);
  lcd.setCursor(0, 1);
  sprintf(buffer, "H: %.0f%c     LDR=%d", h, '%', LED_state);
  lcd.print(buffer);

  water_value = analogRead(WATER_sensor);
  if (water_value != 0) {
    there_is_water = 1;
  } else {
    there_is_water = 0;
  }

  // End sensor

  if (!temperature.addSample(time, t)) {
    Serial.println(temperature.errmsg);
  }
  if (!humidity.addSample(time, h)) {
    Serial.println(humidity.errmsg);
  }
  if (!light.addSample(time, LDR_value)) {
    Serial.println(light.errmsg);
  }
  if (!heat_index.addSample(time, hic)) {
    Serial.println(heat_index.errmsg);
  }
  if (!light_control.addSample(time, LED_control)) {
    Serial.println(light_control.errmsg);
  }
  if (!buzz_control.addSample(time, BUZZ_control)) {
    Serial.println(buzz_control.errmsg);
  }
  if (!water_level.addSample(time, there_is_water)) {
    Serial.println(water_level.errmsg);
  }
  if (!alarm_service.addSample(time, alarm_active)) {
    Serial.println(alarm_service.errmsg);
  }
  loopCounter++;

  if (loopCounter >= 3) {
    // Send data
    loopCounter = 0;
    PromClient::SendResult res = client.send(req);
    if (!res == PromClient::SendResult::SUCCESS) {
      Serial.println(client.errmsg);
    }
    temperature.resetSamples();
    humidity.resetSamples();
    light.resetSamples();
    heat_index.resetSamples();
    light_control.resetSamples();
    buzz_control.resetSamples();
    water_level.resetSamples();
    alarm_service.resetSamples();
  }

  sprintf(buffer, "%.1f %.2f %d %d %d %d %d", h, t, LED_state, LED_control, BUZZ_control, there_is_water, alarm_active);
  mqtt_client.publish("events", buffer);
  memset(buffer, '\0', 256);


  //delay(250);
};

void reconnect() {
  while (!mqtt_client.connected()) {
    Serial.println("Intentando reconectar al servidor MQTT...");
    if (mqtt_client.connect("ESP32Client", "hackupc", "nicetry")) {
      mqtt_client.setServer("commands", 1883);
      mqtt_client.setCallback(callback);
      mqtt_client.subscribe("commands");
      Serial.println("Conexión al servidor MQTT establecida.");
    } else {
      Serial.print("Error al reconectar al servidor MQTT, rc=");
      Serial.println(mqtt_client.state());
      delay(2000);
    }
  }
}
