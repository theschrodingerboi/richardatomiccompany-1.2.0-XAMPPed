#include "WiFi.h"
#include "esp_camera.h"
#include <WebServer.h>


const char* ssid = "****";
const char* password = "****";

WebServer server(80);


#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     21
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       19
#define Y4_GPIO_NUM       18
#define Y3_GPIO_NUM       5
#define Y2_GPIO_NUM       4
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22


void startCameraServer() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA; 
    config.jpeg_quality = 10; 
    config.fb_count = 2; 
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Error al iniciar la cámara: %d\n", err);
    return;
  }
  

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", "<h1>ESP32-CAM Configuracion</h1><p>Conéctate al stream en <a href='/stream'>stream</a></p>");
  });
  
 
  server.on("/stream", HTTP_GET, []() {
    server.send(200, "multipart/x-mixed-replace; boundary=frame");
    while (true) {
      camera_fb_t * fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Error al capturar imagen");
        break;
      }
      server.client().write("--frame\r\n");
      server.client().write("Content-Type: image/jpeg\r\n\r\n");
      server.client().write((const char *)fb->buf, fb->len);
      server.client().write("\r\n");
      esp_camera_fb_return(fb);
      if (!server.client().connected()) break;
    }
  });
  

  server.begin();
  Serial.println("Servidor web de configuración iniciado");
}

void setup() {
  Serial.begin(115200);
  Serial.println();


  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado a la red WiFi, IP: ");
  Serial.println(WiFi.localIP());
  

  startCameraServer();
}

void loop() {
  server.handleClient();
}
