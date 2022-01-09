/*
   Autor: Daniel ST
   eMail: fujista73@gmail.com

   StackBlackBox(r) X
   Version 4.22

   Direcciones I2C para LCD A2004: 0x27 y 0x3F

*/

// *********************************************************************
// includes
// *********************************************************************

#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <TMC2208Stepper.h>
#include <TMC2208Stepper_REGDEFS.h>

// *********************************************************************
// defines
// *********************************************************************

#define UsaTMC2208_UART 1   // 0: Usa DRV A8825    // 1: Usa TMC2208 modo UART

#define JOYREVES  0         // Indica si el joystick está del reves 

#define vEEPROM 222

#define CAMBIO_U_MM   100   // Define el cambio de micras a milimetros

#define DEBUG   0       // Poner a 1 para ver mensajes de depuracion -> movimientos de ajuste a saltos

#define J_DY    0       // Entrada analogica Y de joystick
#define J_DX    1       // Entrada analogica X de joystick
#define J_BTN   2       // Boton de Joystick - usa la entrada digital 2. Deja las otras libres.

#define M10     3       // Salida M0 para control del driver A8825 - motor 1
#define M11     4       // Salida M1 para control del driver A8825
#define M12     9       // Salida M2 para control del driver A8825

#define DIR1    6       // Direccion para el motor paso a paso
#define STEP1   5       // Pasos para el motor paso a paso
#define EN1     13      // Salida para habilitar el driver del motor PaP

#define M20     27      // Salida M0 para control del driver A8825 - motor 2
#define M21     28      // Salida M1 para control del driver A8825
#define M22     29      // Salida M2 para control del driver A8825

#define DIR2    31      // Direccion para el motor paso a paso
#define STEP2   30      // Pasos para el motor paso a paso
#define EN2     26      // Salida para habilitar el driver del motor PaP

#define CH7     7       // Salida disparo camara 1
#define CH8     8       // Salida disparo camara 2

#define IN1     10      // Entrada 1 para disparar la camara por sensor - Nivel bajo
#define FCA     11      // Final de carrera A - activo por nivel bajo
#define FCB     12      // Final de carrera B - activo por nivel bajo

#define HC12    17      // PIN del SET del modulo HC-12. Poner a 0 para enviar comandos AT
#define CH_AUX  17      // PIN para activar la salida auxiliar para focos LED. Reutiliza la del modulo HC12

#define BTH     24      // PIN de estado del bluetooth

#define ZIGZAG  5       // Modo de trabajo zigzag

#define TXM1    50      // TX para el driver TMC2208 del motor 1
#define RXM1    51      // RX para el driver TMC2208 del motor 1

#define TXM2    52      // TX para el driver TMC2208 del motor 2
#define RXM2    53      // RX para el driver TMC2208 del motor 2


// *********************************************************************
// Motores TMC2208
// *********************************************************************

TMC2208Stepper driverM1 = TMC2208Stepper(RXM1, TXM1, 1);
TMC2208Stepper driverM2 = TMC2208Stepper(RXM2, TXM2, 1);


// *********************************************************************
// LCDML display settings
// *********************************************************************

LiquidCrystal_I2C lcd0x27(0x27, 20, 4);   // Pantalla LCD i2c con direccion 0x27 de 20 caracteres x 4 lineas
LiquidCrystal_I2C lcd0x3F(0x3F, 20, 4);   // Pantalla LCD i2c con direccion 0x3F de 20 caracteres x 4 lineas

byte cursor0[8] =
{
  0b01000,
  0b01100,
  0b01110,
  0b11111,
  0b01110,
  0b01100,
  0b01000,
  0b00000
};

byte cursor1[8] =
{
  0b00000,
  0b00100,
  0b01010,
  0b10001,
  0b01010,
  0b00100,
  0b00000
};

// *********************************************************************
// Global variables
// *********************************************************************

char  linea[100] = "                    ",
      linea1[21] = "                    ",
      linea2[21] = "                    ",
      linea3[21] = "                    ",
      linea4[21] = "                    ";

boolean funcionando,            // Esta haciendo fotos
        funcionando2,
        menuIn,                 // 0=Esta en modo de seleccion del menu, 1=Esta en modo de cambio de parametros
        moviendoEje,            // 1 = moviendo eje, no esperar para refrescar
        sentido, sentido2,      // Sentido del movimiento. 0 = INI->FIN / 1 = FIN->INI
        tipoMov, tipoMov2,      // Tipo de movimiento para el eje: 0 = lineal, 1 = rotativo
        mainMotor,              // Indica que motor + camara se usa en modos de un solo motor: 0 = motor 1 / 1 = motor 2
        bmas, bmasmas,
        bmenos, bmenosmenos,
        bup, bupup,
        bdown, bdowndown,
        bjoy,
        volver, volver2,
        iniCarrera, finCarrera,
        iniCarrera2, finCarrera2,
        unidades, unidades2;   // 0 = usa micras, 1 = usa milimetros

byte  opMax[12] = {9, 0, 4, 5, 0, 9, 9, 13, 13, 0},  // Numero de opciones de cada submenu. La primera indica las opciones del menu principal.
      tFoS, tFoS2,                // Tiempo que se activa el optoacoplador en ms
      tEstab, tEstab2,            // Tiempo para estabilizar la camara despues de mover el motor, de 1 a 99 segundos
      tipoDelay, tipoDelay2,      // Indica el delay preestablecido a 20, 70 o 120 (TMC2208) o a 100, 200 y 300 (A8825)
      lead, lead2,                // Paso del husillo
      tiempoLCD,                  // Tiempo en segundos que permanece activo el LCD. 0 = no se apaga nunca
      autoEN,                     // Si se pone a 1, solo habilita el motor cuando lo va a usar, a 0 siempre esta habilitado
      precision, precision2,      // Precision del movimiento, 1 = x1, 2 = x2, 3 = x4 (la mitad de micropasos al motor = la mitad del movimiento indicado)
      modoTrabajo;                // Modo de trabajo 1 ...

uint16_t opcionPrincipal, opcionSecundaria,
         precisionVer[9] = {1, 2, 4, 8, 16, 32, 64, 128, 256},
         LedOn, LedOn2,           // Tiempo de espera desde que se activa la camara para activar el led         
         LedDur, LedDur2,         // Tiempo en milisegundos que el LED está encendido
         pausa, pausa2,           // Tiempo de espera entre desde que se dispara la camara hasta que se mueve el motor        
         fotosHechas,             // Numero de fotos que lleva hechas
         fotosHechas2,
         fCiclo, fCiclo2,         // Num de fotos en cada ciclo de disparos
         delayPaP, delayPaP2,     // Tiempo de pausa para los pulsos del motor en microsegundos
         nFotos, nFotos2,         // Numero de fotos a hacer
         nFotosMF, nFotosMF2,     // Num. de fotos en modo de ajuste de fotos
         nFotosMD, nFotosMD2,     // Num. de fotos en modo de ajuste de distancia, fotos calculadas
         totalPasos, totalPasos2, // Num. total de pasos que se dan en un ciclo
         aRotacion, aRotacion2,   // Angulo de rotacion para movimientos rotatorios, valor * 1.8
         modoMenu,                // Modo actual del menu
         driverM1_I, driverM2_I,  // Intensidad rms de trabajo de los motores
         pvuelta, pvuelta2;       // Nº de pasos por vuelta para los motores      

                 
long    cuantosPasos, cuantosPasos2,      // Pasos a mover en el modo de rango, puede ser positivo o negativo
        dPaso, dPaso2;                    // Pasos que se mueve la camara despues de cada foto

float unPaso, unPaso2, disPaso, disPaso2, gradosTotales, gradosTotales2;

unsigned long pEntera, pEnteraT,          // Parte entera de la distacia total a recorrer carril 1
         pEntera2, pEnteraT2,             // Parte entera de la distacia total a recorrer carril 2
         pDecimal, pDecimalT,             // Parte decimal de la distancia total a recorrer
         pDecimal2, pDecimalT2,           // Parte decimal de la distancia total 2 a recorrer
         Antes, Ahora,                    // Para controlar el tiempo encendido de la pantalla
         gradosTotalesE, gradosTotalesE2, // Parte entera de los grados totales
         gradosTotalesD, gradosTotalesD2; // Parte decimal de los grados totales                           



// Variables used for incoming data

const byte maxDataLength = 20;
char receivedChars[21] ;
boolean newData = false;



// *********************************************************************
// Programa
// *********************************************************************

void setup()
{

  pinMode(J_BTN, INPUT_PULLUP); // Boton del Joystick

  pinMode(FCA, INPUT_PULLUP);   // Final de carrera A activo por nivel bajo
  pinMode(FCB, INPUT_PULLUP);   // Final de carrera B activo por nivel bajo
  pinMode(IN1, INPUT_PULLUP);   // Entrada 1 de disparo directo de camara por nivel bajo

  pinMode(CH8, OUTPUT);         // Opto 1 - Disparo C1
  pinMode(CH7, OUTPUT);         // Opto 2 - Disparo C2

  pinMode(DIR1, OUTPUT);        // Motor paso a paso - Direccion
  pinMode(STEP1, OUTPUT);       // Motor paso a paso - Pasos
  pinMode(EN1, OUTPUT);         // Enable del driver PaP

  pinMode(DIR2, OUTPUT);        // Motor paso a paso - Direccion
  pinMode(STEP2, OUTPUT);       // Motor paso a paso - Pasos
  pinMode(EN2, OUTPUT);         // Enable del driver PaP

  //pinMode(BTH, INPUT);        // Entrada del estado del bluetooth
  pinMode(CH_AUX, OUTPUT); 

  digitalWrite(CH7, 0);         // Desactivo el opto de disparo de camara 1.
  digitalWrite(CH8, 0);         // Desactivo el opto de disparo de camara 2.

  digitalWrite(CH_AUX, 1);      // Desactivo la salida del led
  
  Serial.begin(9600);          // Puerto serie usb para depuracion o control local
  //Serial1.begin(9600);         // Puerto modulo HC-12 433 MHz
  //Serial3.begin(9600);         // Puerto bluetooth

  lcd0x27.begin(); lcd0x3F.begin();
  lcd0x27.createChar(0, cursor0); lcd0x3F.createChar(0, cursor0);
  lcd0x27.createChar(1, cursor1); lcd0x3F.createChar(1, cursor1);

  // Si en la posicion 1 de la memoria EEPROM hay un valor especificado, el programa se ha ejecutado antes y lee los valores a las variables

  if (EEPROM.read(1) == vEEPROM) {
    leeValoresActuales();
  }
  else {
    valoresDefecto();            // Carga los valores por defecto para el modo activo
    EEPROM.write(1,vEEPROM);    // Indica que ya se ha ejecutado el programa
    guardaSetup();               // Guarda en EEPROM los valores actuales del modo activo
  }

  // Inicializa las variables

  modoMenu = 1;
  tiempoLCD = 240;
  fotosHechas = fotosHechas2 = 0;
  moviendoEje = menuIn = funcionando = 0;
  iniCarrera = finCarrera = 0;
  cuantosPasos = cuantosPasos2 = 0;
  opcionPrincipal = 0;
  opcionSecundaria = 0;

  if (!UsaTMC2208_UART) {       // Si NO usamos el driver TMC2208, recuperamos la configuracion del driver A8825
    pinMode(M10, OUTPUT);
    pinMode(M11, OUTPUT);
    pinMode(M12, OUTPUT);
    ajustaM012_Motor(1);        // Ajusta los micropasos del motor

    pinMode(M20, OUTPUT);
    pinMode(M21, OUTPUT);
    pinMode(M22, OUTPUT);
    ajustaM012_Motor(2);        // Ajusta los micropasos del motor
    autoEN = 1;                 // El motor solo se activa durante el ciclo.
  }
  else {
    autoEN = 0;
    tmc2208_init_M1();
    tmc2208_init_M2();
  }

  if (autoEN > 0) {
    digitalWrite(EN1, 1);       // Por defecto desactivo el driver PaP, solo se activa para mover el motor
    digitalWrite(EN2, 1);
  }

  // Ajusta el delay para los micropulsos de los motores

  if (tipoDelay==0) delayPaP=(UsaTMC2208_UART)?20:100;
  else if (tipoDelay==1) delayPaP=(UsaTMC2208_UART)?70:200;
  else if (tipoDelay==2) delayPaP=(UsaTMC2208_UART)?120:300;

  if (tipoDelay2==0) delayPaP2=(UsaTMC2208_UART)?20:100;
  else if (tipoDelay2==1) delayPaP2=(UsaTMC2208_UART)?70:200;
  else if (tipoDelay2==2) delayPaP2=(UsaTMC2208_UART)?120:300;

  calculaUnidades();
  calculaUnPaso();            // Calcula la distancia de un paso para el carril 1
  calculaUnPaso2();           // Calcula la distancia de un paso para el carril 2
  calculaCuantosPasos();
  calculaCuantosPasos2();

  Antes = millis();           // Guardo el tiempo actual para la funcion "reposo LCD"

  newData = false;

  ponPantallaInicio();
  delay(2000);

  menuPrincipalX();

}


void loop()
{
  boolean opcion;

  leeJoystick();    // Siempre lee las entradas del joystick

  if (!digitalRead(FCA) || !digitalRead(FCB)) {       // Si se activa algun final de carrera detiene el movimiento y desactiva los motores

    digitalWrite(EN1, 1);
    digitalWrite(EN2, 1);
    detieneCarril();

    if (!digitalRead(FCA)) error3();
    if (!digitalRead(FCB)) error4();

  }
  else if (funcionando || funcionando2) {             // Si esta funcionando en "ciclo Macro" ...

    if (modoTrabajo == 1 || modoTrabajo == 2) {       // Si el modo de trabajo es con un solo carril, hago el ciclo segun el carril (motor y camara) principal

      if (mainMotor == 0 && fotosHechas < nFotos) cicloMacro();           // Dispara la camara y mueve el motor
      else if (mainMotor == 1 && fotosHechas2 < nFotos2) cicloMacro2();

    }
    else if (modoTrabajo == 3 || modoTrabajo == 4) {  // Si el modo de trabajo es con dos carriles, siempre hago los dos ciclos

      if (fotosHechas < nFotos) cicloMacro();
      if (fotosHechas2 < nFotos2) cicloMacro2();

    }
    else if (modoTrabajo == ZIGZAG) {                 // Si el modo de trabajo es zigzag, tengo que controlarlo bien

      if (fotosHechas < nFotos) cicloMacro();         // Mientras haya fotos en el primer carril las hace,
      else {                                          // Si no, mueve el segundo carril y empieza otra vez

        if (fotosHechas2 < nFotos2) {

          cicloMacro2();

          if (funcionando2) {     // Si no llego al final del carril 2, debo mover otra vez el carril 1
            fotosHechas = 0;
            totalPasos = 0;
            //nFotos = nFotosMD;    // Pongo de nuevo el numero de fotos
            funcionando = 1;      // Pongo el carril 1 a funcionar
            ponMacro();           // Refresca la pantalla, me interesa para ver bien el carril 1
          }

          if (DEBUG) {
            sprintf(linea, "hechas2 %02d   nF2 %02d   fun2 %d  fun1 %d   hechas %d   nF %d", fotosHechas2, nFotos2, funcionando2, funcionando, fotosHechas, nFotos);
            Serial.println(linea);
          }

        }

      } // if (fotosHechas<nFotos) ... else ...

    } // else if (modoTrabajo==ZIGZAG) ...

    if (!funcionando && !funcionando2) {        // Si aun está funcionando
      ponFinCicloMacro();
      menuPrincipalX();
    }

  }
  else if (!digitalRead(IN1) && !menuIn) {      // Si se activa la entrada digital y no esta modificando un valor, dispara las dos camaras

    disparaCamara(tFoS);
    disparaCamara2(tFoS2);
    indicaDisparo();
    menuPrincipalX();

  }
  else  {                                       // Si se pulsa alguna tecla se trata la entrada digital y se muestran los valores

    opcion = (bjoy | bmas | bmasmas | bmenos | bmenosmenos | bup | bupup | bdown | bdowndown);

    if (opcion) seleccionaAccion();     // Si se ha pulsado algun boton o se ha movido el joystick hace lo necesario

  /*
    if (!funcionando) {
      recvWithStartEndMarkers();        // check to see if we have received any new data
      if (newData)  {
        processCommand();               // if we have new data see if it is a command
      }
    }

  */

  }

  //verMonitorSerie();    // Sirve para comunicar el puerto USB del arduino con los puertos bluetooth o HC-12, para configuracion

  // PARA TESTEAR LOS VALORES DEL JOYSTICK
  //char *linea="                   ";
  //sprintf(linea, "DX:%4d  DY:%4d",analogRead(J_DX),analogRead(J_DY));
  //Serial.println(linea);

}


void leeJoystick()
{

  bjoy = !digitalRead(J_BTN);

  bmas = bmasmas = bmenos = bmenosmenos = bup = bupup = bdown = bdowndown = 0;

  if (JOYREVES) {

    if (analogRead(J_DY) > 690) {
      if (analogRead(J_DY) < 1010) bmenos = 1;
      else bmenosmenos = 1;
    };

    if (analogRead(J_DY) < 330) {
      if (analogRead(J_DY) > 5) bmas = 1;
      else bmasmas = 1;
    };

    if (analogRead(J_DX) > 690) {
      if (analogRead(J_DX) < 1010) bup = 1;
      else bupup = 1;
    };

    if (analogRead(J_DX) < 330) {
      if (analogRead(J_DX) > 5) bdown = 1;
      else bdowndown = 1;
    };
    
  }
  else {

    if (analogRead(J_DY) > 690) {
      if (analogRead(J_DY) < 1010) bmas = 1;
      else bmasmas = 1;
    };

    if (analogRead(J_DY) < 330) {
      if (analogRead(J_DY) > 5) bmenos = 1;
      else bmenosmenos = 1;
    };

    if (analogRead(J_DX) > 690) {
      if (analogRead(J_DX) < 1010) bdown = 1;
      else bdowndown = 1;
    };

    if (analogRead(J_DX) < 330) {
      if (analogRead(J_DX) > 5) bup = 1;
      else bupup = 1;
    };

  };

  // Si hay alguna entrada modificada activo el LCD

  if (bjoy || bmas || bmasmas || bmenos || bmenosmenos || bup || bupup || bdown || bdowndown ) {
    lcd0x27.display(); lcd0x27.backlight();
    lcd0x3F.display(); lcd0x3F.backlight();
    Antes = millis();   // Actualizo el tiempo "inicial"
  }
  else if (tiempoLCD > 0) {
    Ahora = millis();
    if (((Ahora - Antes) / 1000) > tiempoLCD) {
      lcd0x27.noBacklight();  // Si ha pasado el tiempo especificado sin mover un boton desactiva la luz del LCD
      lcd0x3F.noBacklight();
    }
    if (((Ahora - Antes) / 1000) > (tiempoLCD * 2)) {
      lcd0x27.noDisplay();  // Si ha pasado el doble del tiempo especificado sin mover un boton apaga el display
      lcd0x3F.noDisplay();
    }
  }

}


void detieneCarril()      // Funcion para detener el movimiento del carril
{
  if (autoEN != 0  && !UsaTMC2208_UART) digitalWrite(EN1, 1);   // Desactiva el driver del motor
  iniCarrera = finCarrera = 0;                                  // Desactiva el movimiento del carril
}


void ajustaM012_Motor(byte nmotor)
{

  // Por defecto pone el driver A8825 a 32 micropulsos
  
  if (nmotor == 1) {
    digitalWrite(M10, 0);
    digitalWrite(M11, 1);
    digitalWrite(M12, 1);
  }
  else if (nmotor == 2) {
    digitalWrite(M20, 0);
    digitalWrite(M21, 1);
    digitalWrite(M22, 1);
  }

}


void calculaFotosMD()
{
  // Calculo el numero de fotos que debe hacer el carril 1

  nFotosMD = (abs(cuantosPasos) / dPaso) + 1;      // Hay que sumar 1 para que llegue al final del recorrido

}


void calculaFotosMD2()
{
  // Calculo el numero de fotos que debe hacer el carril 2

  nFotosMD2 = (abs(cuantosPasos2) / dPaso2) + 1;      // Hay que sumar 1 para que llegue al final del recorrido

}


void calculaUnPaso()
{

  // Obtengo la distancia de un paso y la que debe recorrer entre foto y foto, en micras

  unPaso = float(lead * 500.0F)/(float)pvuelta; // lead * 1000 (pasado a micras) /(nº pasos por vuelta) (y /2 porque en lead va de 0.5 en 0.5)
  disPaso = dPaso * unPaso;                     // La distancia entre fotos = # de pasos * distancia de 1 paso

  // Aqui obtego los valores en caso de que se haya fijado una precisión

  unPaso = unPaso / precisionVer[precision];
  disPaso = disPaso / precisionVer[precision];

  // Aqui saco la parte entera y decimal

  pEntera = long(disPaso);                      // Parte entera de un paso
  pDecimal = (disPaso - long(disPaso)) * 100;   // Parte decimal de un paso con 2 digitos

  if (unidades) {                               // Si trabajo con milimetros ajusto la parte entera y decimales
    pDecimal = (pEntera / 10) % 100;            // Primero se calcula la parte decimal ya que usa la parte entera original
    pEntera = pEntera / 1000;
  }

  // Tambien calculo los grados totales fijados segun el motor

  gradosTotales = ((float)aRotacion * 360.0F)/(float)pvuelta;
  gradosTotales = float(gradosTotales) / float(precisionVer[precision]);

  gradosTotalesE = long(gradosTotales);
  gradosTotalesD = (unsigned(gradosTotales * 10)) % 10;
  
  if (DEBUG) {
    Serial.print("gradosTotales = "); Serial.print(gradosTotales);
    Serial.print("; gradosTotalesE = "); Serial.print(gradosTotalesE);    
    Serial.print("; gradosTotalesD = "); Serial.println(gradosTotalesD);
  }

}


void calculaUnPaso2()
{

  // Obtengo la distancia que debe recorrer entre foto y foto, en micras

  unPaso2 = float(lead2 * 500.0F)/(float)pvuelta2;    // lead pasado a micras /(nº pasos por vuelta) (y /2 porque en lead va de 0.5 en 0.5)  
  disPaso2 = float(dPaso2) * float(unPaso2);          // La distancia entre fotos = # de pasos * distancia de 1 paso

  // Aqui obtego los valores en caso de que se haya fijado una precisión

  unPaso2 = unPaso2 / precisionVer[precision2];
  disPaso2 = disPaso2 / precisionVer[precision2];
  
  // Aqui saco la parte entera y decimal

  pEntera2 = long(disPaso2);                      // Parte entera de un paso
  pDecimal2 = (disPaso2 - long(disPaso2)) * 100;  // Parte decimal de un paso con 2 digitos

  if (unidades2) {                                // Si trabajo con milimetros ajusto la parte entera y decimales
    pDecimal2 = (pEntera2 / 10) % 100;            // Primero se calcula la parte decimal ya que usa la parte entera original
    pEntera2 = pEntera2 / 1000;
  }

  // Tambien calculo los grados totales fijados segun el motor

  gradosTotales2 = ((float)aRotacion2 * 360.0F)/(float)pvuelta2;
  gradosTotales2 = float(gradosTotales2) / float(precisionVer[precision2]);

  gradosTotalesE2 = long(gradosTotales2);
  gradosTotalesD2 = (unsigned(gradosTotales2 * 10)) % 10;

}


void calculaCuantosPasos()
{
  float dTotal;

  if (modoTrabajo == 2 || modoTrabajo == 4) dTotal = (nFotosMF - 1) * disPaso;   // En programa por num. de fotos calcula la distancia total a recorrer
  else dTotal = unPaso * abs(cuantosPasos);

  // Aqui saco la parte entera y decimal

  pEnteraT = long(dTotal);                      // Parte entera total
  pDecimalT = (dTotal - long(dTotal)) * 100;    // Parte decimal total con 2 digitos

  if (unidades) {                               // Si trabajo con milimetros ajusto la parte entera y decimales
    pDecimalT = (pEnteraT / 10) % 100;          // Primero se calcula la parte decimal ya que usa la parte entera original
    pEnteraT = pEnteraT / 1000;
  }

}


void calculaCuantosPasos2()
{
  float dTotal2;

  if (modoTrabajo == 2 || modoTrabajo == 4) dTotal2 = (nFotosMF2 - 1) * disPaso2;   // En programa por num. de fotos calcula la distancia total a recorrer
  else dTotal2 = unPaso2 * abs(cuantosPasos2);

  // Aqui saco la parte entera y decimal

  pEnteraT2 = long(dTotal2);                        // Parte entera total
  pDecimalT2 = (dTotal2 - long(dTotal2)) * 100;     // Parte decimal total con 2 digitos

  if (unidades2) {                                  // Si trabajo con milimetros ajusto la parte entera y decimales
    pDecimalT2 = (pEnteraT2 / 10) % 100;            // Primero se calcula la parte decimal ya que usa la parte entera original
    pEnteraT2 = pEnteraT2 / 1000;
  }

}



void calculaUnidades()
{

  // Ajusta las unidades en funcion de la distancia de paso de los carriles
  
  unidades = (((dPaso*float(lead * 500.0F)/(float)pvuelta)/precisionVer[precision])>(CAMBIO_U_MM))?1:0;       // Puede que no conozcamos unPaso y asi nos aseguramos de calcular las unidades
  unidades2 = (((dPaso2*float(lead2 * 500.0F)/(float)pvuelta2)/precisionVer[precision2])>(CAMBIO_U_MM))?1:0;   
  
}
