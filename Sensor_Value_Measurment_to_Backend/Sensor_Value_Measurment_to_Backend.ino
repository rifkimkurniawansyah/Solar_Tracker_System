int sensorArus = 34;
int voltSensor = 36;
#include<Wire.h>

#include "WiFi.h"
#include "HTTPClient.h"

//Variabel wifi hotspot dan password
const char* ssid = "ssid name hotspot";
const char* pass = "password hotspot"; 


//const char* host = "IP Addres"; //localhost
const char* host = "monitoringoutputselsurya.000webhostapp.com"; //hosting

const int MPU_addr = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

int minVal = 265;
int maxVal = 402;

/*int minVal = -17000;
int maxVal = -17000;*/

double x;
double y;
double z;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //koneksi ke wiFi
  WiFi.begin(ssid,pass);
  Serial.println("Connecting.... ");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }

  
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

void loop() {
  // put your main code here, to run repeatedly:

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  int xAng = map(AcX, minVal, maxVal, -90, 90);
  int yAng = map(AcY, minVal, maxVal, -90, 90);
  int zAng = map(AcZ, minVal, maxVal, -90, 90);

  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  float sudut = map(z, 0, 360, 0, 270);
  
  float adcVolt = analogRead(voltSensor);
  float tegangan = ((0.0041*adcVolt)+0.5026);
  
  float adcArus = analogRead(sensorArus);
  //float arus = ((0.0089*adcArus)-16.646);
  float arus = ((0.0089*adcArus)-19);

  if(arus < 0){
    arus = 0;
  }

  float daya = (tegangan * arus);
  
  Serial.println("arus : " + String(arus));
  Serial.println("kemiringan : " + String(sudut));
  Serial.println("tegangan : " + String(tegangan));
  Serial.println("daya : " + String(daya));
  Serial.println("  ");
  delay(20000);



  //kirim data ke server
  WiFiClient client;
  //inisialisasi port web server pada xampp 80
  const int httpPort = 80;
  if( !client.connect(host, httpPort)){
    Serial.println("Connection Failed");
    return;
  }
  //kondisi pasti terkoneksi
  //kirim data sensor ke database dan web
  String Link;
  HTTPClient http;

  
  //Link = "http://" + String(host) + "/Monitoring_Sel_Surya/kirimdata.php?tegangan=" + String(tegangan) + "&arus=" + String(arus) + "&daya=" + String(daya) + "&kemiringan=" + String(z); //localhost
  Link = "http://" + String(host) + "/kirimdata.php?tegangan=" + String(tegangan) + "&arus=" + String(arus) + "&daya=" + String(daya) + "&kemiringan=" + String(sudut); //hosting
  http.begin(Link);
  http.GET();

  String respon = http.getString();
  Serial.println(respon);
  http.end();
  delay(20000);
  }
