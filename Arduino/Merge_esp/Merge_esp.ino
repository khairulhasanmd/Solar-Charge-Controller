#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>

#define TRUE 1

// Update these with values suitable for your network.
const char* ssid = "AXIOM-Ground-Floor";
const char* password = "bismillah";
const char* mqtt_server = "192.168.100.101";
char Str1[15];
//void callback(char* topic, byte* payload, unsigned int length);

int incomingByte;  

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
float temp = 0;
int inPin = 5;



void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient_temperature_sensor")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void setup()
{
  Serial.begin(9600);
  setup_wifi(); 
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.subscribe("/ha/sred");
  pinMode(inPin, INPUT);
//  sensors.begin();
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 100) {
    lastMsg = now;

    if (Serial.available() > 0) {
      client.publish("/ha/NO", "Serial Data Receiving");
      client.publish("/ha/spub", String(Serial.readStringUntil('\n')).c_str(),TRUE);
//      while (Serial.readStringUntil('\n')) {
//        client.publish("/ha/A", String(Serial.readStringUntil('A')).c_str(),TRUE);
//        client.publish("/ha/V", String(Serial.readStringUntil('V')).c_str(),TRUE);
//        client.publish("/ha/W", String(Serial.readStringUntil('W')).c_str(),TRUE);
//      }
    }
    else{
      client.publish("/ha/NO", "No Serial Data Avilable");
    }
    client.subscribe("/ha/sred");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
 
//  Serial.print("Message arrived in topic: ");
//  Serial.println(topic);
 
//Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
//  Serial.println("-----------------------");
 
}
