#include "Messenger.h"

// Values from Twilio (find them on the dashboard)
static const char *account_sid = "AC4ba512861d5427950f277d013e27942f";
static const char *auth_token = "1cc6025ede598805328e6d3ebd99d0bf";
// Phone number should start with "+<countrycode>"
static const char *from_number = "+18444196455";

// You choose!
// Phone number should start with "+<countrycode>"
static const char *to_number = "+12673618594";

Messenger::Messenger() {
  twilio = new Twilio(account_sid, auth_token);
}

Messenger::~Messenger() {
  delete twilio;
}

void Messenger::sendMessage(String msg) {

  if (WiFi.status() != WL_CONNECTED) return;

  String response;
  bool success = twilio->send_message(to_number, from_number, msg, response);
  if (success) {
    Serial.println("Sent message successfully!");
  } else {
    Serial.println(response);
  }
}