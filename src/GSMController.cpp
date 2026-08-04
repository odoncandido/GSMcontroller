#include "GSMController.h"

/**
 * @brief configura o modem
 * @param void
 * @return void
 * */
GSMController::GSMController(HardwareSerial &serial)  : sim800l(serial) {
    sim800l.println("AT+CMGD=1,4"); 		//Apaga SMS da memória
    sim800l.println("AT+CFUN=1");	 	//Coloca o módulo em funcionamento completo
    sim800l.println("AT+CNMI=2,2,0,0,0"); 	//Configura como novos SMS serão entregues
    sim800l.println("AT+CMGF=1")	 	//Coloca SMS no modo texto
    sim800l.println("AT+CLIP=1")	 	//Ativa identificação do número de quem liga
    sim800l.println("AT+DDET=1,0,0")	 	//Ativa detecção de tons DTMF

}
/**
 * @brief Processa uma resposta recebida do modulo GSM
 * @param response Reposta retornada pelo GSM
 * @return Array contendo as informacoes processadas
 * */
String* GSMController::processGsmInformation(String response) {
  static String result[4];  
  if (response.indexOf("+CMT:") > -1) {  
    String* smsInfo = processSMS(response);
    result[0] = "SMS";         
    result[1] = smsInfo[0];     
    result[2] = smsInfo[1];     
    result[3] = smsInfo[2];     
  }else if (response.indexOf("RING") > -1){
    result[0] = "CALL";         
    result[1] = "Phone ringing";
    result[2] = obterNumeroChamada();
    result[3] = "";
  }else if (response.indexOf("NO CARRIER") > -1) {
    result[0] = "CALL_END";
    result[1] = "Missed Call!";
    result[2] = "";
    result[3] = "";
    resetarChamada();
  }else if(response.indexOf("BUSY") > -1){
    result[0] = "BUSY";
    result[1] = "Linha ocupada!";
    result[2] = "";
    result[3] = "";
  }else if(response.indexOf("NO DIALTONE") > -1){
    result[0] = "NO DIALTONE";
    result[1] = "Sem Linha";
    result[2] = "";
    result[3] = "";
  }else if(response.indexOf("NO ANSWER") > -1){
    result[0] = "NO ANSWER";
    result[1] = "Nao Atende";
    result[2] = "";
    result[3] = "";
  }else if(response.indexOf("UNDER_VOLTAGE_WARNING") > -1){
    result[0] = "UNDER_VOLTAGE_WARNING";
    result[1] = "Tensão aciama do limite! > 4.2 V";
    result[2] = "";
    result[3] = "";
  }else if(response.indexOf("LOW_VOLTAGE_WARINIG") > -1){
    result[0] = "LOW VOLTAGE WARNING";
    result[1] = "Tensão baixa!";
    result[2] = "";
    result[3] = "";
  }else{
    result[0] = "OTHER";
    result[1] = response;
    result[2] = "";
    result[3] = "";
  }

  return result;
}

String* GSMController::processSMS(String response) {
  static String smsInfo[3];
  int numStart = response.indexOf("\"") + 1;
  int numEnd = response.indexOf("\"", numStart);
  smsInfo[0] = response.substring(numStart, numEnd);  

  int dataStart = response.indexOf(",", numEnd) + 5;
  int dataEnd = response.lastIndexOf("\"") - 3;
  smsInfo[1] = response.substring(dataStart, dataEnd); 

  int corpoStart = response.indexOf("\n", dataEnd) + 1;
  smsInfo[2] = response.substring(corpoStart); 
  smsInfo[2].trim();
  return smsInfo;
}

String GSMController::sendATCommand(String command) {
  sim800l.println(command);
  delay(50);
  String response = "";
  while (sim800l.available()) {
    response += char(sim800l.read());
  }
  return response;
}

void GSMController::callNumber(String number) {
  sendATCommand("ATD" + number + ";");
}

void GSMController::hangUpCall() {
  sendATCommand("ATH");
}

String GSMController::obterNumeroChamada() {
  if (!numCallReturned){
    String response = sendATCommand("AT+CLCC");
    int start = response.indexOf("\"") + 1;
    int end = response.indexOf("\"", start);
  
    if (start > 0 && end > start) {
      numCallReturned = true;
      num = response.substring(start, end);  
      return num;
    } 
    return "Desconhecido "+ response;  
  }
  return num;
}

void GSMController::resetarChamada() {
  numCallReturned = false;
}

