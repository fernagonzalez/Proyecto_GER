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
float SOC;
float Umbral_Positivo = 12.5;
float Delta_Q = 0;
float Q_nominal = 9360; //Capacidad nominal (en Coulumb)
/* La capacidad nominal de la batería se calcula teniendo en cuenta la capacidad nominal en mAh,
    de la hoja de datos tenemos que la capadidad es 2.6 Ah para una sola bateria,
    pero como están conectadas en serie, el pack de tres baterias tiene la misma capacidad.
    Teniendo en cuenta la relación entre amper y coulumbs hacemos: 2.6 Ah = 2.6 (C/s)*3600 s =9360 C
*/


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
//              |--- Array_Corriente(); Lo cambié a Delta_Q porque eso retorna
//              |--- Valor_SOC_Coulumb_Counting();
// Modo_Descarga();

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
  SOC = SOC_Inicial();

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

  //8888888888888888888888888888888888888888888888888888888888888888888888888888
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

    void Modo_Descarga();

  }

  if (I_Bateria > 2)
  { Serial.println("Modo: Carga");
    Serial.println("---------");
    delay(1000);

    void Modo_Carga();
  }
}

//888888888888888888888888888888888888888888888888888888888888888888888888888888
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
    if (V_bateria  > Umbral_Positivo)   // Recalibración del estado de carga lleno
      {
      void Actualizacion_Umbral_Maximo_Tension();// Salir del bucle
      } else
            {
              float Q_ganado = 0;
              // Cada vez que el programa entra en el modo carga, inicializará en cero la carga perdida

              // Determinacion del SOC con Coulumb Counting
              Serial.println("check 1 "); // Borrar despues

              float Delta_Q_Subrutina();
              float Delta_Q1 = Delta_Q_Subrutina();

              // Imprimo el array de corrientes
              //*****eliminar esta impresion en el scketch final
              /*Serial.println("Los valores de las muestras de las corrientes son: ");
              int i = 0;
              for (i = 0; i <= 9; i++)
              {
               Serial.print(Array_Corriente[i]);
               Serial.print("mA  ");
               delay(100);
               }
                Serial.println(" ");
                Serial.println("---------");
                                              */

              /*En este modo el contador de coulumbs se presenta por Q_ganado, expresado en la siguite fórmula
              que representa la cantidad de cargas acumulada en T */


              Q_ganado += Delta_Q;

          float Delta_SOC = 100 * Q_ganado / Q_nominal;
          SOC += Delta_SOC;
          //SOC_Ultimo = SOC;

          float Valor_SOC_Coulumb_Counting();
        }

}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Actualizacion_Umbral_Maximo_Tension(void)
 {
    static float Actualizacion_Umbral_Positivo = V_bateria - Umbral_Positivo;
    Umbral_Positivo += Actualizacion_Umbral_Positivo;
    /* En la página 40 dice que la tension máxima del pack es de 12.9
    sin embrago adopto 12.5 como valor base, para que se vaya actualizando desde ahí con la variable Actualizacion_Umbral_Positivo*/
    Serial.print("SOC: "); Serial.print(100); Serial.println("% ");
    Serial.println("---------"); //Imprimir el SOC en 100 %

    Serial.print("Umbral positivo actualizado: ");Serial.print(Umbral_Positivo);Serial.println("V");
    Serial.println("---------");
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
float Delta_Q_Subrutina(void)
{
      int i = 0;
      static float  Array_Corriente[10]; // Redeclaracion de la variable, en la columna 108 se encuentraa la primera declaracion
      static int Periodo_Array_Corriente;
      Periodo_Array_Corriente = 350;
      for (i = 0; i <= 9; i++)
      {
        Array_Corriente[i] = ina219.getCurrent_mA();
        delay(Periodo_Array_Corriente);
        // El array tarda 3150 ms en formarse con 10 valores
      }

      Serial.println("check 2");

      // Acá empezaba array trapecios pero los unifiqué porque
      // no sé como integrar ambos, mas que nada no se como retornar el Array_Corriente
      float f_a = 0; float f_b = 0;
      float b_a = 350;   //b-a es dato, es el intervalo entre muestras sucesivas
      // b-a es igual a Periodo_Array_Corriente

      float Array_Integral_Parcial [10];
      Serial.println("Los valores de los áreas parciales son: "); // borrar despues
      for (i = 0; i < 9; i++)
      {
          f_a = Array_Corriente[i];
          f_b = Array_Corriente[i + 1];
          Array_Integral_Parcial [i] = {b_a * 0.5 * (f_a + f_b)}; // Tiene 9 elementos de área

          Serial.print(Array_Integral_Parcial [i]); //borar despues
          Serial.print("uC  "); //borar despues
          delay(200); //borar despues
      }
      Serial.println(" ");//borar despues
      Serial.println("---------");//borar despues
      Serial.println("check 2b");//borar despues
      //Acá tenemos el array de trapecios listo, por ende solo resta sumar las areas
      // Eliminar la siguiente impresión en el sketch final
      /*Serial.println("Los valores de los áreas parciales son: ");
      for (i = 0; i < 9; i++)
      {
          Serial.print(Array_Integral_Parcial [i]);
          Serial.print("uC  ");
          delay(500);
      } está en el buvle de arriba, tengo que borrar ambos*/


      // Área total
      float Delta_Q = 0;
      for (i = 0; i <= 8; i++)
      {
          Delta_Q += Array_Integral_Parcial[i];
      }
      Delta_Q = Delta_Q / 1000000; //paso de uC a C
      Serial.print("El valor del área total es: ");Serial.print(Delta_Q);Serial.print("C");
      Serial.println("---------");

      return Delta_Q;
  }


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

float Valor_SOC_Coulumb_Counting(void)
{
  // 12.1 hacer variable Tension_Maxima_Nominal
  if (SOC > 100 && V_bateria > Umbral_Positivo)
  {
    void Actualizacion_Umbral_Maximo_Tension();
  } else{
    Serial.print("SOC: ");Serial.print(SOC); Serial.println("% ");
    Serial.print(" "); Serial.println("---------");
    }
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Modo_Descarga(void)
{
  float Q_perdido = 0;
  // Cada vez que el programa entra en el modo descarga, inicializará en cero la carga perdida

  float Delta_DOD = 0; // Delta de Profundidad de descarga
  float DOD = 0; // Profundidad de descarga

  float Delta_Q = Delta_Q_Subrutina();
  Q_perdido += Delta_Q;

  Delta_DOD = (Q_perdido/Q_nominal)*100;
  DOD += Delta_DOD;

// Acá tiene que ir una subrutina parecida a Valor_SOC_Coulumb_Counting pero
// para el DOD
// Posteriormente trabajar con ficheros .h porque son muchas subrutinas ya y el código


}
