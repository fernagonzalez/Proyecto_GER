

// Includes
#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_INA219.h>



// Iniciadores
Adafruit_INA219 ina219;




// Declaracion de variables globales
float Array_Corriente[10] = {};
float Array_Integral_Parcial[9] = {};
float Q_ganado = 0; // Establezco cero como valor arbitrario de referencia
float Delta_Q = 0;
float SOC = 0;
float Actualizacion_Umbral_Positivo = 0;
// Actualización del valor máximo de tensión (en caso de ser necesario).
float Umbral_Positivo = 12.1;
int Periodo_Array_Corriente_Subrutina = 0;
float SOC_Ultimo;



// Árbol de subrutinas mias
// Adquisicion_Preliminar_I();
// SOC_Inicial();


// 888888888888888888888888888888888888888888888888888888888
void setup(){
  Serial.begin(9600);
  ina219.begin();
  //uint32_t currentFrequency;
            }

void loop() {
  // Declaracion de variables locales


  // Diseñado para tener una estimacion del SOC caada 5 seg.

  float V_bateria = ina219.getBusVoltage_V();

  // Adquisición premilinar de i_bateria
  float I_bateria = 0;
  //float I_bateria = Adquisicion_Preliminar_I();

  // Imprimo el dato de la tensión y la corriente
  Serial.print("La tensión en la batería es: ");
  Serial.print(V_bateria);  Serial.print("V  ");  Serial.println(" ");
  Serial.println("---------");
  Serial.print("La corriente en la batería es: ");
  Serial.print(I_bateria);  Serial.print("mA  ");  Serial.println(" ");
  Serial.println("---------");

  // Estimación inicial del SOC
  //SOC = SOC_Inicial();

  //888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888


  // Condicionales para determinar modos (carga, descarga, auto-descarga)

  if (I_bateria > -2 && I_bateria < 2)
  {
    Serial.println("Modo: Auto-descarga");
    Serial.println("---------");
  }

  if (I_bateria < -2)
  {
    Serial.println("Modo: Descarga");
    Serial.println("---------");
  }

  if (I_bateria > 2)
  {
    Serial.println("Modo: Carga");
    Serial.println("---------");

    //Modo_Carga();

  }
}
