#include <WiFi.h>
#include <PubSubClient.h>

#define LED_1 2
int BUTTON1 = 13;
int button_state = 0;

int VR = A0;
int VR_Value = 0;

unsigned long last_time = 0;
unsigned long period = 1000;
const char *ssid = "Petch wifi";
const char *password = "12345678";
// const char *ssid = "TrueGigatexFiber_A7A - 2.4G";
// const char *password = "0858281728";

const char *mqtt_broker = "broker.hivemq.com";
const char *topic = "Test/Petch/Table_2";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(LED_1, OUTPUT);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");

  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());

    Serial.printf("The client %s connects to mosquitto mqtt broker\n", client_id.c_str());

    if (client.connect(client_id.c_str())) {
      Serial.println("Public emqx mqtt broker connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  client.publish(topic, "อุปกรณ์เชือมต่อเเล้ว");
  client.subscribe(topic);
}


void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println(" - - - - - - - - - - - -");
}

void loop() {
  if (millis() - last_time >= period) {
    last_time = millis();
    client.loop();
    button_state = digitalRead(BUTTON1);
    digitalWrite(LED_1, button_state);
    Serial.println("b1val=" + String(button_state));
    VR_Value = analogRead(VR);

    client.publish(topic, String(VR_Value).c_str());

    if (button_state == 1) {
      // if ()
      client.publish(topic, "1");

      delay(3000);
    }
  }
}