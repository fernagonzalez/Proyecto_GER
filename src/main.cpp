// Control de Carga - Código principal

// Includes
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>



// Iniciadores
Adafruit_INA219 ina219;


// Declaracion de variables globales
//float Array_Corriente[10] = {};
/*
float Array_Integral_Parcial[9] = {};
float Q_ganado = 0; // Establezco cero como valor arbitrario de referencia
float Delta_Q = 0;
float SOC = 0;
float Actualizacion_Umbral_Positivo = 0;
*/

float V_bateria = ina219.getBusVoltage_V();
/*
// Actualización del valor máximo de tensión (en caso de ser necesario).
float Umbral_Positivo = 12.1;
int Periodo_Array_Corriente_Subrutina = 0;
float SOC_Ultimo;
*/


// Árbol de subrutinas mias
// Adquisicion_Preliminar_I();
// SOC_Inicial();
// Modo_Carga();
//   |---Actualizacion_Umbral_Maximo_Tension();

// Suma de delays
// 1000 ms en todas los modos


// 888888888888888888888888888888888888888888888888888888888
void setup()
{
  Serial.begin(9600);
  ina219.begin();
}

void loop()
 { // Diseñado para tener una estimacion del SOC caada 5 seg.

  // Declaracion de variables locales
  float Adquisicion_Preliminar_I(); // Va antes que I_Bateria porque se ejecuta primero
  float I_Bateria = Adquisicion_Preliminar_I();
  float SOC_Inicial();
  float SOC = SOC_Inicial();

  // Imprimo el dato de la tensión, la corriente y el SOC inicial
  Serial.print("V batería: ");
  Serial.print(V_bateria);  Serial.println("V");
  Serial.println("---------");
  Serial.print("I batería: ");
  Serial.print(I_Bateria);  Serial.println("mA");
  Serial.println("---------");
  Serial.print("SOC: ");
  Serial.print(SOC);  Serial.println("%");
  Serial.println("---------");

  //888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888
  // Condicionales para determinar modos (carga, descarga, auto-descarga)
  if (I_Bateria > -2 && I_Bateria < 2)
  { Serial.println("Modo: Auto-descarga");
    Serial.println("---------");
    delay(1000);
  }

  if (I_Bateria < -2)
  { Serial.println("Modo: Descarga");
    Serial.println("---------");
    delay(1000);
  }

  if (I_Bateria > 2)
  { Serial.println("Modo: Carga");
    Serial.println("---------");
    delay(1000);

    void Modo_Carga();
  }
}

//888888888888888888888888888888888888888888888888888888888888888888888888888888
// Declaracion de funciones

float Adquisicion_Preliminar_I(void)
{    // Adquisición premilinar de i_bateria
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

    return (I_Bateria);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

float SOC_Inicial(void)
{   // Polinomio calculado con MATLAB, que nos da el porcentaje de descarga
    // en función de la tensión en bornes de la batería x.
    float x = V_bateria;
    float SOC_Inicial = 1.0e+09 * (-0.0010*pow(x,5) + 0.0131*pow(x,4) - 0.1150*pow(x,3) + 0.6313*pow(x,2) - 1.9766*x + 2.7039);

    return (SOC_Inicial);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void Modo_Carga (void)
{
    Serial.println("Calculando SOC");
    if (V_bateria > Umbral_Positivo)   // Recalibración del estado de carga lleno
      {
      Actualizacion_Umbral_Maximo_Tension();// Salir del bucle
      } else
      {
    // Determinacion del SOC con Coulumb Counting
    Periodo_Array_Corriente_Subrutina = 350;
      Serial.println("check 1 ");

    Array_Corriente_Subrutina ();
    // Imprimo el array de corrientes
    //*****eliminar esta impresion en el scketch final
    Serial.println("Los valores de las muestras de las corrientes son: ");
    int i = 0;
      for (i = 0; i <= 9; i++)
      {
       Serial.print(Array_Corriente[i]);
       Serial.print("mA  ");
       delay(100);
      }
      Serial.println(" ");
      Serial.println("---------");

     Array_Trapecios();
    /*Serial.println("El valor del área total es: ");
      Serial.print(Delta_Q);
      Serial.println(" C");

      Serial.print(" ");
      Serial.println("---------");*/

    /* En este modo el contador de coulumbs se presenta por Q_ganado, expresado en la siguite fórmula
      que representa la cantidad de cargas acumulada en T */
    Q_ganado += Delta_Q;

    float Q_nominal = 9360; //Capacidad nominal (en Coulumb)
    /* La capacidad nominal de la batería se calcula teniendo en cuenta la capacidad nominal en mAh,
        de la hoja de datos tenemos que la capadidad es 2.6 Ah para una sola bateria,
        pero como están conectadas en serie, el pack de tres baterias tiene la misma capacidad.
        Teniendo en cuenta la relación entre amper y coulumbs hacemos: 2.6 Ah = 2.6 (C/s)*3600 s =9360 C
    */

    float Delta_SOC = 100 * Q_ganado / Q_nominal;
    SOC += Delta_SOC;
    SOC_Ultimo = SOC;

    Valor_SOC_Coulumb_Counting();
    }
  return 0;
}
