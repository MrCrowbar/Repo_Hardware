#ifndef funciones
#define funciones

#include <NTPClient.h>
#include <ArduinoJson.h>
#include "ESP8266HTTPClient.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

void entablarConexiones();
bool hacerPeticion(String query);
//String getDate(NTPClient timeClient);
String setTanqueEsta(String tanqueID, String lugarID, String fecha);

void entablarConexiones(){
  //Aqui puse los ssid ya que no queremos que sean variables globales por ser credenciales de seguridad.
  const char* ssid = "INFINITUM9209_2.4"; //INFINITUM9209_2.4 INFINITUM8312_2.4
  const char* password = "4by3rg4JgD"; //4by3rg4JgD 76Mroar07m
  Serial.begin(115200);
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  while (!Serial) continue;
}

bool hacerPeticion(String query){
  bool respuesta = true;
  //Al ser la direccion del servidor tampoco queremos que este de manera global asi que la deje dentro de esta funcion.
  String TOKEN("eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJjb3JyZW8iOiJmbG9yb0BxdWltb2Jhc2ljb3MuY29tIiwiaWF0IjoxNTkxMjIzMzU3LCJleHAiOjE2MjI3ODA5NTd9.rnKJUiTUARs6mETAG2Y-08iw4V5ZMBsiWQ8KdY6Kyir7x_xbgiuBmBOI_BPsX4Zz5SquIumNC3jmjJJsYi6KfrIS-RNmaJRfIQRTBHEo5DjdngSzPLh_7UK9A-P-YqEl5Grf9Z8y6XfiQJMluqIRsKcsLC5wlCna0pC-lymvArYgyBpw5lUJrF8ZTQfsGVpbH2ZYYhswbDcHv0_tBUmIUv32HXDhA1sNLNEZsFnItqcr3jqFQwG-_0BosuJKj3B395dVxzb8wDrECjNlvFgRl-m6NuZrhFokWCD4aTlM_pw8heX_aYUQgSU8iABP68UluNmuQIgWpkVULvHDGBXpKA");
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    http.begin(client,"http://3.12.196.48:5201/graphql");//http://192.168.1.66:5000/arduino
    http.addHeader("authorization", TOKEN);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(query);
    if(httpResponseCode>0){
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    }else{
      Serial.print("Error on sending POST Request: ");
      Serial.println(httpResponseCode);
      respuesta = false;
   }
   http.end();
 }else{
    Serial.println("Error in WiFi connection");
    respuesta = false;
 }
 return respuesta;
}

String setTanqueEsta(byte tanqueID, String lugarID, String fecha){
  String output;
  DynamicJsonDocument root(500);
  root["query"].set("mutation($tank: TanqueEstaInput!, $id:String!){setTanqueEsta(tanqueEstaInput: $tank, idTanqueEstaOriginal: $id)}");
  JsonObject variables = root.createNestedObject("variables");
  JsonObject tank = variables.createNestedObject("tank");
  tank["id"].set(tanqueID);
  tank["idLugar"].set(lugarID);
  tank["fecha"].set(fecha);
  variables["id"].set(tanqueID);
  serializeJson(root, output);
  serializeJsonPretty(root, Serial);
  return output;
}

#endif
