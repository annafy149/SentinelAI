#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
  #include <Firebase_ESP_Client.h>

//Token generation process info.
#include "addons/TokenHelper.h"

//The RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#include "ws2812.h"
#include "twilio.hpp"
#include "sd_read_write.h"

#include "esp_camera.h"
#define CAMERA_MODEL_ESP32S3_EYE
#include "camera_pins.h"

// Values from Twilio (find them on the dashboard)
static const char *account_sid = "################################";
static const char *auth_token = "################################";
// Phone number should start with "+<countrycode>"
static const char *from_number = "+1844#######";

// You choose!
// Phone number should start with "+<countrycode>"
static const char *to_number = "+1267#######";
static const char *message = "Potential gun violance detected in your area.";

Twilio *twilio;

//Network credentials.
#define WIFI_SSID "HUNOSWTHAPENS"
#define WIFI_PASSWORD "###########"

//Firebase project API Key
#define API_KEY "###################################"

//RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://sentinelAI.firebaseio.com/" 

#define Buzzer_Pin 0
#define LED_Pin 45

char *CaptureTrigger = "Gun";
//Firebase Data object.
FirebaseData fbdo;

//Firebase authentication.
FirebaseAuth auth;

//Firebase configuration.
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
const long sendDataIntervalMillis = 10000;
int count = 0;
bool signupOK = false;

String response;
int Message = 0;


void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  Serial.println();

  pinMode(Buzzer_Pin, OUTPUT);
  pinMode(LED_Pin, OUTPUT);
  ws2812Init();
  sdmmcInit();
  removeDir(SD_MMC, "/SurveillenceFootage");
  createDir(SD_MMC, "/SurveillenceFootage");
  listDir(SD_MMC, "/SurveillenceFootage", 0);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to Wifi");
  Serial.print("Connecting to : ");
  Serial.println(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Successfully connected to : ");
  Serial.println(WIFI_SSID);
  Serial.print("IP : ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Assign the api key (required).
  config.api_key = API_KEY;

  // Assign the RTDB URL (required).
  config.database_url = DATABASE_URL;

  // Sign up.
  Serial.println();
  Serial.println("Sign up");
  Serial.print("Sign up SentinelDB user... ");
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Ok");
    signupOK = true;
  }

  if(cameraSetup()==1){
    ws2812SetColor(2);
  }

  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
    ws2812SetColor(1);
  }
  Serial.println("---------------");
  
  //Callback function for the long running token generation task.
  config.token_status_callback = tokenStatusCallback; //See addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  twilio = new Twilio(account_sid, auth_token);

  String response;
  bool success = twilio->send_message(to_number, from_number, message, response);
}


void loop() {

  if (CaptureTrigger == "Gun"){
    delay(20);
    camera_fb_t * fb = NULL;
    fb = esp_camera_fb_get();
    if (fb != NULL) {
      ws2812SetColor(3);
      int photo_index = readFileNum(SD_MMC, "/SurveillenceFootage");
      if(photo_index!=-1)
      {
        String path = "/SurveillenceFootage/" + String(photo_index) +".jpg";
        writejpg(SD_MMC, path.c_str(), fb->buf, fb->len);
      }
      esp_camera_fb_return(fb);
      digitalWrite(LED_Pin, HIGH);
      digitalWrite(Buzzer_Pin, HIGH);
    }
    else {
      Serial.println("Camera capture failed.");
    }
    ws2812SetColor(2);    
  }
  
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > sendDataIntervalMillis || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    if (Firebase.RTDB.setInt(&fbdo, "test/Timelapse", count)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    count++;
    
    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setFloat(&fbdo, "test/Gun Status", 1)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
}

int cameraSetup(void) {
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
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  // for larger pre-allocated frame buffer.
  if(psramFound()){
    config.jpeg_quality = 10;
    config.fb_count = 2;
    config.grab_mode = CAMERA_GRAB_LATEST;
  } else {
    // Limit the frame size when PSRAM is not available
    config.frame_size = FRAMESIZE_VGA;
    config.fb_location = CAMERA_FB_IN_DRAM;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return 0;
  }

  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  s->set_vflip(s, 1); // flip it back
  s->set_brightness(s, 1); // up the brightness just a bit
  s->set_saturation(s, 0); // lower the saturation

  Serial.println("Camera configuration complete!");
  return 1;
}
