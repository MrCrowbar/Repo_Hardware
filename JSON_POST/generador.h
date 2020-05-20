#include <ArduinoJson.h>
#include "ESP8266HTTPClient.h"
#include "ESP8266WiFi.h"

class Generador{
  private:
    String output;
    
  public:
    String createTanqueEsta(String tanqueID, String lugarID, String fecha){
      //Generacion del JSON
      //Se genera unicamente el json correspondiente a "tanqueEstaInput" ya que posteriormente se le agregara el inicio y final del query
      DynamicJsonDocument  root(200);
      root["query"].set("mutation($tank: TanqueEstaInput){createTanqueEsta(tanqueEstaInput: $tank)}");
      
      JsonObject variables = root.createNestedObject("variables");
      JsonObject tank = variables.createNestedObject("tank");
      JsonObject id = tank.createNestedObject("id");
      
      id["idTanque"].set(tanqueID);
      id["idLugar"].set(lugarID);
      tank["fecha"].set(fecha);
    
      //Sacar el json root a la variable output
      serializeJson(root, output);
      //Sacar el json root a la terminal bien bonito
      serializeJsonPretty(root, Serial);
      return output;
    }
  
    String createTanque(String tanqueID, String calidad, String estadoValvula, float pesoActual, float peso, String fechaEsperadaRetorno, int idEtiqueta, String idContenido, String idDueno, String fechaIngreso, String observaciones){
      DynamicJsonDocument root(500);
      root["query"].set("mutation($tank: TanqueInput!){createTanque(tanqueInput: $tank)}");
      JsonObject variables = root.createNestedObject("variables");
      JsonObject tank = variables.createNestedObject("tank");
      tank["id"].set(tanqueID);
      tank["calidad"].set(calidad);
      tank["pesoActual"].set(pesoActual);
      tank["idContenido"].set(idContenido);
      tank["idDueno"].set(idDueno);
      tank["fechaIngreso"].set(fechaIngreso);
      tank["fechaEsperadaRetorno"].set(fechaEsperadaRetorno);
      tank["idEtiqueta"].set(idEtiqueta);
      tank["peso"].set(peso);
      tank["observaciones"].set(observaciones);
      variables["id"].set(tanqueID);
      serializeJson(root, output);
      //Sacar el json root a la terminal bien bonito
      serializeJsonPretty(root, Serial);
      return output;
    }
};
