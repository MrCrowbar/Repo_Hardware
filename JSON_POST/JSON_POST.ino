#include <ArduinoQueue.h>
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

//Estructura de dato para guardar tanques detectados
struct tanque {
  String idLugar;
  String idTanque;
  String fecha;
};
ArduinoQueue<tanque> queue_tanque(10);

//Estructura de dato para guardar tiempo local
struct local_time {
  String tiempo;
  int year;
  byte hour;
  byte minute;
  byte second;
  byte day;
  byte month;
}_time;


//Flag global del estado actual de la conexión
bool conexionFlagActual = false;
bool conexionFlagPasado = conexionFlagActual;


//Sumer tiempo local 5 minutos cada 5 minutos
void sumarTiempoLocal();

//Cambiar flag de conexión
void conexion(){
  conexionFlagActual = !conexionFlagActual;
}

void setup() {
  entablarConexiones(); //Hasta no entablar la conexion no se procede a generar recursos que no se pueden utilizar.
  timeClient.begin();
  timeClient.setTimeOffset(GMT_MEXICO);
  while (!timeClient.update()){
    Serial.println("Updating NTP server time..");
    timeClient.forceUpdate();
  }
  //actualizarTiempoLocal();
  getDate();
  conectividad.attach(10, conexion); //cada 10 segundos cambiamos el flag de conexión
  tiempo.attach(5, sumarTiempoLocal); //cada 5 segundos accionar la función sumarTiempoLocal
  stringOutput = String();
  tanqueID = String("EURO5149ZZ");
  lugarID = String("AMC");
  //fecha = String("2020-06-05T10:40:17Z");//2020-6-5T19:40:17Z //2122-10-03T10:00:00Z
  //fecha = getDate();
  //Serial.println(fecha);
  //stringOutput = setTanqueEsta(tanqueID,lugarID,fecha); //Esta variable almacena el formato JSON que va a cargarse en el POST.
  //Serial.println(stringOutput);
  //hacerPeticion(stringOutput); //Hacer la peticion POST, no hace falta pasar "stringOutput" porque es una variable global.
}

void loop() {
  //Revisamos el estado de los flags de conexión para actualizar el timeClient y dar el estatus de conexión.
  if (conexionFlagPasado != conexionFlagActual){
    if (WiFi.status() == WL_CONNECTED){
    timeClient.update();
    while (!timeClient.update())timeClient.forceUpdate();
    //Serial.println(getDate());
    //actualizarTiempoLocal();
    getDate();
    Serial.print("Giempo local: ");
    Serial.print(_time.tiempo);
    Serial.println("Conectado");
  } else {
    Serial.println("Desconectado");
  }
  Serial.println(WiFi.status());
  conexionFlagPasado = !conexionFlagPasado;
  //Esto iría en la condicional que detecta cuando pasó un tanque
  //------------------
  crearTanque();
  stringOutput = setTanqueEsta(queue_tanque.getHead().idTanque,queue_tanque.getHead().idLugar,queue_tanque.getHead().fecha);
  if(hacerPeticion(stringOutput))queue_tanque.dequeue();
  else Serial.println("Queue not deleted");
  Serial.print("Queue item count: ");
  Serial.println(queue_tanque.itemCount());
  //------------------
  }
}


void crearTanque(){
  struct tanque _tanque;
  _tanque.idLugar = "AMC";
  _tanque.idTanque = "tanque1";
  _tanque.fecha = _time.tiempo;
  queue_tanque.enqueue(_tanque);
  String idTanque = queue_tanque.getHead().idTanque;
  String idLugar = queue_tanque.getHead().idLugar;
  String fecha = queue_tanque.getHead().fecha;
  Serial.print("ID tanque: ");
  Serial.print(idTanque);
  Serial.print("ID lugar: ");
  Serial.print(idLugar);
  Serial.print("Fecha: ");
  Serial.print(fecha);
  Serial.println();
}

//Se suma a nuestro tiempo local 5 minutos cada 5 minutos.
void sumarTiempoLocal(){
  String hora;
  String minuto;
  String dia;
  String mes;
 
  _time.minute += 5;
  if (_time.minute >= 60){
    _time.minute = 0;
    _time.hour += 1;
    if (_time.hour >= 24){
      _time.hour = 0;
    }
  }
  
  if (_time.hour < 10) hora = "0" + String(_time.hour);
  if (_time.minute < 10) minuto = "0" + String(_time.minute);
  if (_time.day < 10) dia = "0" + String(_time.day);
  if (_time.month < 10) mes = "0" + String(_time.month);
  
  _time.tiempo = String(_time.year) + mes + dia + "T" + hora + ":" + minuto + ":" + String(_time.second) + "Z";
  Serial.print("Tiempo local: ");
  Serial.print(_time.tiempo);
  Serial.println();
}

void getDate(){
  String hora; //HH:MM:SS
  String mes;
  String dia;
  hora = timeClient.getFormattedTime();
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *p_tm = gmtime((time_t *)&epochTime);
  int monthDay = p_tm->tm_mday;
  int currentMonth = p_tm->tm_mon+1;
  int currentYear = p_tm->tm_year+1900;
  if (monthDay < 10) dia = "0" + String(monthDay);
  if (currentMonth < 10) mes = "0" + String(currentMonth);
  _time.hour = timeClient.getHours();
  _time.minute = timeClient.getMinutes();
  _time.second = timeClient.getSeconds();
  _time.day = monthDay;
  _time.month = currentMonth;
  _time.year = currentYear;
  _time.tiempo = String(currentYear) + "-" + mes + "-" + dia + "T" + hora + "Z";
}
