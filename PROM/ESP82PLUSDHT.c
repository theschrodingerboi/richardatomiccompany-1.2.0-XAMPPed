#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

const char* ssid = "****";
const char* password = "****";


#define DHTPIN D3         // Pin DHT22 (CONECTAR DHT22 LUEGO DEL ENCENDIDO DE LA PLACA)
#define DHTTYPE DHT22     


DHT dht(DHTPIN, DHTTYPE);

const int motorPin1 = D1;  // Motor GPIO5
const int motorPin2 = D2;  // GPIO4
const int servodirectionPin = D7;  // GPIO13
const int servoaxisPin = D5;  // GPIO14
const int servoliftPin = D6;  // GPIO12
const int servogripPin = D8;  // GPIO15

int motor = 0;
int servodirection = 0;
int servoaxis = 0;
int servolift = 0;
int servogrip = 0;

void setup() {
  Serial.begin(115200);


  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(servodirectionPin, OUTPUT);
  pinMode(servoaxisPin, OUTPUT);
  pinMode(servoliftPin, OUTPUT);
  pinMode(servogripPin, OUTPUT);


  dht.begin();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    String url = "http://x.x.x.x/richardatomiccompany/PHP/control.php?get_status=";

    String components[] = {"motor", "servodirection", "servoaxis", "servolift", "servogrip"};

    for (int i = 0; i < 5; i++) {
      http.begin(client, url + components[i]);

      int httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println("Payload for " + components[i] + ": " + payload);

          int statusIndex = payload.indexOf("\"status\":");
          if (statusIndex != -1) {
            int statusStart = payload.indexOf(":", statusIndex) + 1;
            int statusEnd = payload.indexOf(",", statusStart);
            if (statusEnd == -1) {
              statusEnd = payload.indexOf("}", statusStart);
            }

            String statusStr = payload.substring(statusStart, statusEnd);
            int status = statusStr.toInt();

            if (components[i] == "motor") {
                if (status == 1) {
                    analogWrite(motorPin1, 254);
                    analogWrite(motorPin2, 0);
                } else if (status == 0) {
                    analogWrite(motorPin1, 0);
                    analogWrite(motorPin2, 0);
                } else {
                    analogWrite(motorPin1, 0);
                    analogWrite(motorPin2, 254);
                }
            
            } else if (components[i] == "servodirection") {
                int pwmValue = map(status, -1, 1, 135, 45);
                analogWrite(servodirectionPin, pwmValue);

            } else if (components[i] == "servoaxis") {
              int pwmValue = map(status, -6, 6, 15, 175);
              analogWrite(servoaxisPin, pwmValue);

            } else if (components[i] == "servolift") {
              int pwmValue = map(status, -3, 3, 45, 145);
              analogWrite(servoliftPin, pwmValue);

            } else if (components[i] == "servogrip") {
              int pwmValue = map(status, -5, 0, 45, 90);
              analogWrite(servogripPin, pwmValue);
            }

            Serial.print(components[i]);
            Serial.print(" status: ");
            Serial.println(status);
          } else {
            Serial.println("Error: Status not found in response.");
          }
        }
      } else {
        Serial.printf("Error in request, HTTP code: %d\n", httpCode);
      }
      http.end();
    }


    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();  


    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print("°C, Humidity: ");
      Serial.print(humidity);
      Serial.println("%");


      String dhtUrl = "http://x.x.x.x/richardatomiccompany/PHP/dht22.php?temperature=" + String(temperature) + "&humidity=" + String(humidity);

      http.begin(client, dhtUrl);
      int httpCode = http.GET();

      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println("DHT data sent: " + payload);
        }
      } else {
        Serial.printf("Error sending DHT data, HTTP code: %d\n", httpCode);
      }
      http.end();
    }

  } else {
    Serial.println("WiFi disconnected");
  }

  delay(1000); 
}
