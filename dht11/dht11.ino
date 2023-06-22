// Teste de funcionalidade sensor DHT11
#include "DHT.h"

#define DHTPIN 4     // Pino que se conecta ao sensor
#define DHTTYPE DHT11 

// Inicializa o sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT11 teste"));

  dht.begin();
}

void loop() {
  delay(2000);

  float h = dht.readHumidity();

  // Le a temperatura em graus celsius
  float t = dht.readTemperature();

  // Checa se alguma leitura falhou e sai antes (para tentar novamente)
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Falha na leitura do sensor DHT11!"));
    return;
  }
  // Computa indice de calor em ºC (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Umidade: "));
  Serial.print(h);
  Serial.print(F("%  Temperatura: "));
  Serial.print(t);
  Serial.print(F("°C Índice de calor: "));
  Serial.print(hic);
  Serial.println(F("°C"));
}