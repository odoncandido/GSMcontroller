#include "GSMController.h"

GSMController::GSMController(HardwareSerial &serial) 
  : sim800l(serial) {}
String* GSMController::processGsmInformation(String response) {
  static String result[4];  // Array para armazenar a informação processadaz
  /*if (response.indexOf("+CMTI:") > -1) {
    int startIndex = response.indexOf(",") + 1;
    String indice = response.substring(startIndex);
    indice.trim();  

    Serial.println("Novo SMS detectado, lendo...");
    Serial.println("Índice: " + indice);

    String comando = "AT+CMGR=" + indice;
    String respostaSMS = sendATCommand(comando);  // Obtém a resposta do AT+CMGR
    String* smsInfo = processSMS(respostaSMS);  // Processa a mensagem e 
    //String* smsInfo = processSMS(response);
    result[0] = "SMS";          // Tipo de evento
    result[1] = smsInfo[0];     // Número do remetente
    result[2] = smsInfo[1];     // Data/Hora
    result[3] = smsInfo[2];     // Corpo da mensagemretorna os dados
  }*/

  
  if (response.indexOf("+CMT:") > -1) {  
    String* smsInfo = processSMS(response);
    result[0] = "SMS";          
    result[1] = smsInfo[0];     
    result[2] = smsInfo[1];     
    result[3] = smsInfo[2];     
  } 
  else if (response.indexOf("RING") > -1){
    result[0] = "CALL";         
    result[1] = "Telefone chamando";
    result[2] = obterNumeroChamada();  //AT+CLCC
    result[3] = "";
  } 
  else if (response.indexOf("NO CARRIER") > -1) {
    result[0] = "CALL_END";
    result[1] = "Chamada perdida!";
    result[2] = "";
    result[3] = "";
    resetarChamada();
  } 
  else if(response.indexOf("BUSY") > -1){
    result[0] = "BUSY";
    result[1] = "Linha ocupada!";
    result[2] = "";
    result[3] = "";
  }
  else if(response.indexOf("NO DIALTONE") > -1){
    result[0] = "NO DIALTONE";
    result[1] = "Sem Linha";
    result[2] = "";
    result[3] = "";
  }
  else if(response.indexOf("NO ANSWER") > -1){
    result[0] = "NO ANSWER";
    result[1] = "Nao Atende";
    result[2] = "";
    result[3] = "";
  }
  
  else {
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
/*
String GSMController::obterNumeroChamada() {
  if (!numCallReturned) {
    String response = sendATCommand("AT+CLCC");

    // Quebra a resposta em linhas (caso o modem envie múltiplas)
    int clccIndex = response.indexOf("+CLCC:");
    if (clccIndex != -1) {
      // Isola a linha com +CLCC:
      int lineEnd = response.indexOf("\n", clccIndex);
      String clccLine = (lineEnd != -1) ? response.substring(clccIndex, lineEnd) : response.substring(clccIndex);

      // Extrai o número entre aspas
      int start = clccLine.indexOf("\"") + 1;
      int end = clccLine.indexOf("\"", start);
      
      if (start > 0 && end > start) {
        num = clccLine.substring(start, end);
        numCallReturned = true;
        return num;
      } else {
        return "Erro ao extrair número: " + clccLine;
      }
    } else {
      return "Sem resposta CLCC: " + response;
    }
  }

  return num; // Retorna o número salvo
}
*/

String GSMController::obterNumeroChamada() {
  //if (numCallReturned) {
  //  return "";  // Evita repetir a consulta
  //}

  if (!numCallReturned){
    String response = sendATCommand("AT+CLCC");
    int start = response.indexOf("\"") + 1;
    int end = response.indexOf("\"", start);
  
    if (start > 0 && end > start) {
      numCallReturned = true;  // Ativa a flag para evitar repetição
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

