#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 4     // Pino que se conecta ao sensor
#define DHTTYPE DHT11 

// Inicializa o sensor
DHT dht(DHTPIN, DHTTYPE);

// Nome e senha da rede wi-fi conectada - TROCAR PARA OS DADOS DA REDE WI-FI ATUAL!
const char* ssid = "ESP32"; 
const char* password = "1234";

// Nome do dominio com URL
String serverName = "https://forecast-interface.vercel.app/api/";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(9600); 
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.println("Conectando");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado ao wi-fi com o IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(2000);

  float umi = dht.readHumidity();

  // Le a temperatura em graus celsius
  float temp = dht.readTemperature();

  // Checa se alguma leitura falhou e sai antes (para tentar novamente)
  if (isnan(umi) || isnan(temp)) {
    Serial.println(F("Falha na leitura do sensor DHT11!"));
    return;
  }

  // Manda um HTTP POST request a cada 10 minutos
  if ((millis() - lastTime) > timerDelay) {
    //Checa a conexão do wi-fi
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName + "?temp=" + String(temp) + "&umi=" + String(umi);
      http.begin(serverPath.c_str());
      
      // HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("Wi-Fi desconectado.");
    }
    lastTime = millis();
  }
}
