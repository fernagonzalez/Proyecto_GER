// Control de Carga - Código principal

// Includes
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>



// Iniciadores
Adafruit_INA219 ina219;


// Declaracion de variables globales
//float Array_Corriente[10] = {};
float Array_Integral_Parcial[9] = {};
float Q_ganado = 0; // Establezco cero como valor arbitrario de referencia
float Delta_Q = 0;
float SOC = 0;
float Actualizacion_Umbral_Positivo = 0;
//float Adquisicion_Preliminar_I();

// Actualización del valor máximo de tensión (en caso de ser necesario).
float Umbral_Positivo = 12.1;
int Periodo_Array_Corriente_Subrutina = 0;
float SOC_Ultimo;



// Árbol de subrutinas mias
// Adquisicion_Preliminar_I();
// SOC_Inicial();

// Suma de delays
// 1000 ms en todas los modos


// 888888888888888888888888888888888888888888888888888888888
void setup(){
  Serial.begin(9600);
  ina219.begin();

            }

void loop() {
  // Declaracion de variables locales
  float Adquisicion_Preliminar_I();

  // Diseñado para tener una estimacion del SOC caada 5 seg.

  float V_bateria = ina219.getBusVoltage_V();

  // Adquisición premilinar de i_bateria
  float I_Bateria = Adquisicion_Preliminar_I();

  // Imprimo el dato de la tensión y la corriente
  Serial.print("La tensión en la batería es: ");
  Serial.print(V_bateria);  Serial.print("V  ");  Serial.println(" ");
  Serial.println("---------");
  Serial.print("La corriente en la batería es: ");
  Serial.print(I_Bateria);  Serial.print("mA  ");  Serial.println(" ");
  Serial.println("---------");

  //888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888


  // Condicionales para determinar modos (carga, descarga, auto-descarga)

  if (I_Bateria > -2 && I_Bateria < 2)
  {
    Serial.println("Modo: Auto-descarga");
    Serial.println("---------");
    delay(1000);
  }

  if (I_Bateria < -2)
  {
    Serial.println("Modo: Descarga");
    Serial.println("---------");
    delay(1000);
  }

  if (I_Bateria > 2)
  {
    Serial.println("Modo: Carga");
    Serial.println("---------");
    delay(1000);

  }
}

//888888888888888888888888888888888888888888888888888888888888888888888888888888
// Declaracion de funciones

float Adquisicion_Preliminar_I(void) {
    //Tomo un promedio para evitar flutuaciones de la medición en un período muy corto de tiempo
    int i = 0;
    int Periodo_Adquisicion_Preliminar = 250;
    float I_Bateria = 0;
    float I_Bateria_Suma = 0;
    float  Array_Corriente[6];
    // Posible mejora usando Array_Corriente_Subrutina con variable M con M igual al orden del array

    for (i = 0; i <= 5; i++){
        Array_Corriente[i] = {ina219.getCurrent_mA()};
        delay(Periodo_Adquisicion_Preliminar);
          // Al cabo de 1.5 segundos tendremos 6 mediciones de corriente colocadas en el array
        }

    // Saco un promedio de las mediciones
    for (i = 0; i <= 5; i++){
        I_Bateria_Suma += Array_Corriente[i];
        }

    I_Bateria = I_Bateria_Suma / 6;

    return I_Bateria;
}
