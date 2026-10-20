#include "Arduino.h"
#include "esp_camera.h"

// Brochage Freenove / ESP32-S3-EYE (confirmé par le scan I2C : OV2640 en 0x30)
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     15
#define SIOD_GPIO_NUM     4
#define SIOC_GPIO_NUM     5

#define Y9_GPIO_NUM       16
#define Y8_GPIO_NUM       17
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       12
#define Y5_GPIO_NUM       10
#define Y4_GPIO_NUM       8
#define Y3_GPIO_NUM       9
#define Y2_GPIO_NUM       11
#define VSYNC_GPIO_NUM    6
#define HREF_GPIO_NUM     7
#define PCLK_GPIO_NUM     13

void setup() {
  Serial.begin(115200);
  delay(1500);

  Serial.println("\n--- Début du test ESP32-S3-CAM ---");

  if (psramFound()) {
    Serial.printf("[SUCCÈS] PSRAM : %lu octets\n", (unsigned long)ESP.getPsramSize());
  } else {
    Serial.println("[ATTENTION] PSRAM non détectée.");
  }

  camera_config_t config = {};
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
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 12;
  config.fb_count = 2;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.grab_mode = CAMERA_GRAB_LATEST;

  Serial.println("Initialisation de la caméra...");
  esp_err_t err = esp_camera_init(&config);

  if (err != ESP_OK) {
    Serial.printf("[ERREUR] Échec init caméra (0x%x)\n", err);
  } else {
    Serial.println("[SUCCÈS] Caméra initialisée !");

    camera_fb_t *fb = esp_camera_fb_get();
    if (fb) {
      Serial.printf("[SUCCÈS] Image capturée : %ux%u, %u octets\n",
                    fb->width, fb->height, (unsigned)fb->len);
      esp_camera_fb_return(fb);
    } else {
      Serial.println("[ERREUR] Capture impossible.");
    }
  }

  Serial.println("--- Fin du test ---");
}

void loop() {
  delay(5000);
  Serial.println("L'ESP32-S3 tourne normalement...");
}