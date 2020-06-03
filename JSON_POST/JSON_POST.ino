#include <NTPClient.h>
#include <WiFiUdp.h>
#include "funciones.h"
#include <Ticker.h>
#include <ESP8266WiFi.h>

#define GMT_MEXICO -18000
#define OUTPUT_SIZE 500

//Declaracion de variables globales.
char output[OUTPUT_SIZE]; //Falta ajustar el tamaño para que no se desperdicie memoria.
String tanqueID, lugarID, fecha, stringOutput;

//Servidor para tiempo.
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Clases de tickers
Ticker tiempo;
Ticker conectividad;

String weekDays[7] = {"Domingo","Lunes","Martes","Miercoles","Jueves","Viernes","Sabado"};
String months[12] = {"Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre"};

//Actualizar tiempo local de servidor NTP
void actualizarTiempoLocal();

//Sumer tiempo local 5 minutos cada 5 minutos
void sumarTiempoLocal();

//Estructura de dato para guardar tiempo local
struct local_time {
 int hour;
 int minute;
 int second; 
}_time{0,0,0};


//Flag global del estado actual de la conexión
bool conexionFlagActual = false;
bool conexionFlagPasado = conexionFlagActual;

//Cambiar flag de conexión
void conexion(){
  conexionFlagActual = !conexionFlagActual;
}

void setup() {
  entablarConexiones(); //Hasta no entablar la conexion no se procede a generar recursos que no se pueden utilizar.
  timeClient.begin();
  timeClient.setTimeOffset(GMT_MEXICO);
  for (int i = 0; i < 10; i++){
    Serial.println("Updating NTP server time..");
    timeClient.update();
    delay(500);
  }
  actualizarTiempoLocal();
  conectividad.attach(600, conexion); //cada 10 segundos cambiamos el flag de conexión
  tiempo.attach(300, sumarTiempoLocal); //cada 5 segundos accionar la función sumarTiempoLocal
  
  //stringOutput = String();
  //tanqueID = String("EURO5149ZZ");
  //lugarID = String("AMC");
  //fecha = String("2122-10-03T10:00:00Z");
  //stringOutput = setTanqueEsta(tanqueID,lugarID,fecha); //Esta variable almacena el formato JSON que va a cargarse en el POST.
  //Serial.println(stringOutput);
  //hacerPeticion(stringOutput); //Hacer la peticion POST, no hace falta pasar "stringOutput" porque es una variable global.
}

void loop() {
  //Revisamos el estado de los flags de conexión para actualizar el timeClient y dar el estatus de conexión.
  if (conexionFlagPasado != conexionFlagActual){
    if (WiFi.status() == WL_CONNECTED){
    timeClient.update();
    actualizarTiempoLocal();
    Serial.println("Conectado");
  } else {
    Serial.println("Desconectado");
  }
  Serial.println(WiFi.status());
  conexionFlagPasado = !conexionFlagPasado;
  }
  
  //Esta vacio ya que no queremos mandar la misma peticion varias veces, por el momento solamente estamos mandandola una vez.
  //Serial.println(timeClient.getFormattedTime());
  //Serial.println(getDate(timeClient));
}

//Se suma a nuestro tiempo local 5 minutos cada 5 minutos.
void sumarTiempoLocal(){
  static byte _5minCount = 0;
  _time.minute += 5;
  if (_time.minute >= 60){
    _time.minute = 0;
    _time.hour += 1;
    if (_time.hour >= 24){
      _time.hour = 0;
    }
    _5minCount = 0;
  }
  _5minCount++;
  Serial.print("Tiempo local: ");
  Serial.print("HH:");
  Serial.print(_time.hour);
  Serial.print(" MM:");
  Serial.print(_time.minute);
  Serial.print(" SS:");
  Serial.print(_time.second);
  Serial.print(", minute counter: ");
  Serial.print(_5minCount);
  Serial.println();
}

//Se actualiza el tiempo local cuando sí hay conexión a internet.
void actualizarTiempoLocal(){
  _time.hour = timeClient.getHours();
  _time.minute = timeClient.getMinutes();
  _time.second = timeClient.getSeconds();
  Serial.print("Tiempo local: ");
  Serial.print("HH:");
  Serial.print(_time.hour);
  Serial.print(" MM:");
  Serial.print(_time.minute);
  Serial.print(" SS:");
  Serial.print(_time.second);
  Serial.println();
}
