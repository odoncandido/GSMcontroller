#ifndef GSM_CONTROLLER_H
#define GSM_CONTROLLER_H
#include <HardwareSerial.h>
#if CONFIG_IDF_TARGET_ESP32        // ESP32 original
  #define SIM800L_TX_PIN 17
  #define SIM800L_RX_PIN 16
  #define LED_BULTIN     2
#elif CONFIG_IDF_TARGET_ESP32C3    // ESP32-C3 (SuperMini, DevKit, etc)
  #define SIM800L_TX_PIN 0
  #define SIM800L_RX_PIN 1
 // #define LED_BUILTIN    8

#elif CONFIG_IDF_TARGET_ESP32S3    // ESP32-S3
  #define SIM800L_TX_PIN 20
  #define SIM800L_RX_PIN 19

#else
  #error "Placa ESP32 não suportada!"
#endif
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
class GSMController {
  private:
    HardwareSerial &sim800l;

  public:
    GSMController(HardwareSerial &serial);
    String* processGsmInformation(String response);
    String* processSMS(String response);
    String sendATCommand(String command);
    String obterNumeroChamada();
    String num="";
    bool numCallReturned = false;
    //String lerResposta();
    void callNumber(String number);
    void hangUpCall();
    void resetarChamada();
};

#endif
