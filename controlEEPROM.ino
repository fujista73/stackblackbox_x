// Pone los valores por defecto para el modo actual

void valoresDefecto()
{
  modoMenu = 1;
  modoTrabajo = 1;
  mainMotor = 0;
  dPaso = dPaso2 = 10;              // Pasos de desplazamiento entre fotos
  tEstab = tEstab2 = 5;             // segundos para estabilizar
  nFotosMF = nFotosMF2 = 50;        // Numero de fotos por defecto
  pausa = pausa2 = 7;               // Segundos de espera entre fotos
  sentido = sentido2 = 0;           // Sentido de desplazamiento, por defecto A->B
  tipoMov = tipoMov2 = 0;           // Tipo de movimiento, por defecto lineal
  tFoS = tFoS2 = 30;                // Tiempo en centesimas de segundo pulsado el disparador de camara
  LedOn = LedOn2 = 0;               // Tiempo de espera en milesimas de segundo para activar salida LED desde que se activa camara 1
  LedDur = LedDur2 = 20;            // Tiempo que  está activa la salida LED - 20 mS
  pvuelta = pvuelta2 = 400;         // por defecto trabajamos con motores de 400 pasos
  precision = precision2 = 0;       // Indica en el array la precision a usar (x1)
  delayPaP = delayPaP2 = 70;        // 70 uS entre micropulsos
  tipoDelay = tipoDelay2 = 1;       // Modos del delay. 0=20, 1=70, 2=120  - en microsegundos
  aRotacion = aRotacion2 = 10;      // 10 pasos en movimiento rotatorio = 18 grados o 9 grados
  fCiclo = fCiclo2 = 1;             // 1 foto por ciclo
  volver = volver2 = 0;             // No volver al principio
  lead = lead2 = 2;                 // Usillo de 1 mm/vuelta (lead cada 0.5 mm)
  autoEN = 1;                       // Por defecto solo se activa el motor durante el ciclo de macro
  tiempoLCD = 180;                  // 180 segundos sin hacer nada se apaga la luz del LCD, 360 segundos se desactiva totalmente el LCD

  driverM1_I = driverM2_I = 200;    // por defecto, 200mA Intensidad RMS

  delayPaP=(UsaTMC2208_UART)?70:200;
  delayPaP2=(UsaTMC2208_UART)?70:200;

  if (!UsaTMC2208_UART) {
    ajustaM012_Motor(1);        // Ajusta los micropasos del motor    
    ajustaM012_Motor(2);        // Ajusta los micropasos del motor
    autoEN=1;                   // Por defecto solo activa el motor durante el ciclo
  }
  else {
    autoEN=0;                   // Por defecto activa siempre el motor
    driverM1.rms_current(200);  // Fijar corriente rms de trabajo motor 1
    driverM1.microsteps(256);   // Fijar los micropulsos por defecto del motor 1
    driverM2.rms_current(200);  // Fijar corriente rms de trabajo motor 2    
    driverM2.microsteps(256);   // Fijas los micropulsos por defecto del motor 2
    tmc2208_init_M1();
    tmc2208_init_M2();    
  }
  
}


void leeValoresActuales()
{

  nFotosMF = EEPROMReadInt(2);
  dPaso = EEPROMReadInt(4);
  pausa = EEPROMReadInt(6);
  mainMotor = EEPROM.read(8);
  modoTrabajo = EEPROM.read(9);
  tFoS = EEPROM.read(10);
  sentido = EEPROM.read(11);
  tipoDelay = EEPROM.read(13);
  delayPaP = EEPROMReadInt(14);
  tipoMov = EEPROM.read(16);
  fCiclo = EEPROM.read(18);
  volver = EEPROM.read(19);
  lead = EEPROM.read(20);
  tEstab = EEPROM.read(23);
  precision = EEPROM.read(24);
  aRotacion = EEPROMReadInt(26);
  driverM1_I = EEPROMReadInt(28);
  pvuelta = EEPROMReadInt(30);
  LedDur = EEPROMReadInt(32);
  LedOn = EEPROMReadInt(34);

  nFotosMF2 = EEPROMReadInt(202);
  dPaso2 = EEPROMReadInt(204);
  pausa2 = EEPROMReadInt(206);
  tFoS2 = EEPROM.read(210);
  sentido2 = EEPROM.read(211);
  tipoDelay2 = EEPROM.read(213);
  delayPaP2 = EEPROMReadInt(214);
  tipoMov2 = EEPROM.read(216);
  fCiclo2 = EEPROM.read(218);
  volver2 = EEPROM.read(219);
  lead2 = EEPROM.read(220);
  tEstab2 = EEPROM.read(223);
  precision2 = EEPROM.read(224);
  aRotacion2 = EEPROMReadInt(226);
  driverM2_I = EEPROMReadInt(228);
  pvuelta2 = EEPROMReadInt(230);
  LedDur2 = EEPROMReadInt(236);
  LedOn2 = EEPROMReadInt(238);
 
}


void guardaSetup()
{
  EEPROMWriteInt(2, nFotosMF);
  EEPROMWriteInt(4, dPaso);
  EEPROMWriteInt(6, pausa);
  EEPROM.write(8, mainMotor);
  EEPROM.write(9, modoTrabajo);
  EEPROM.write(10, tFoS);
  EEPROM.write(11, sentido);
  EEPROM.write(12, LedOn);  
  EEPROM.write(13, tipoDelay);
  EEPROMWriteInt(14, delayPaP);
  EEPROM.write(16, tipoMov);
  EEPROM.write(18, fCiclo);
  EEPROM.write(19, volver);
  EEPROM.write(20, lead);
  EEPROM.write(21, autoEN);
  EEPROM.write(22, tiempoLCD);
  EEPROM.write(23, tEstab);
  EEPROM.write(24, precision);
  EEPROMWriteInt(26, aRotacion);
  EEPROMWriteInt(28, driverM1_I);  
  EEPROMWriteInt(30, pvuelta);
  EEPROMWriteInt(32, LedDur);  
  EEPROMWriteInt(34, LedOn);    

  EEPROMWriteInt(202, nFotosMF2);
  EEPROMWriteInt(204, dPaso2);
  EEPROMWriteInt(206, pausa2);
  EEPROM.write(210, tFoS2);
  EEPROM.write(211, sentido2);
  EEPROM.write(213, tipoDelay2);
  EEPROMWriteInt(214, delayPaP2);
  EEPROM.write(216, tipoMov2);
  EEPROM.write(218, fCiclo2);
  EEPROM.write(219, volver2);
  EEPROM.write(220, lead2);
  EEPROM.write(223, tEstab2);
  EEPROM.write(224, precision2);
  EEPROMWriteInt(226, aRotacion2);
  EEPROMWriteInt(228, driverM2_I);
  EEPROMWriteInt(230, pvuelta2);
  EEPROMWriteInt(232, LedDur2);  
  EEPROMWriteInt(234, LedOn2);    

}


void hazReset()
{

  ponMensajeReset();

  valoresDefecto();         // Carga los valores por defecto para el modo activo
  EEPROM.write(1, vEEPROM); // Indica que ya se ha ejecutado el programa
  
  // Inicializa las variables

  modoMenu = 1;

  tiempoLCD = 240;

  fotosHechas = fotosHechas2 = 0;
  moviendoEje = menuIn = funcionando = 0;
  iniCarrera = finCarrera = 0;
  cuantosPasos = cuantosPasos2 = 0;

  opcionPrincipal = 0;
  opcionSecundaria = 0;

  // Ajusta el delay para los micropulsos de los motores

  if (tipoDelay==0) delayPaP=(UsaTMC2208_UART)?20:100;
  else if (tipoDelay==1) delayPaP=(UsaTMC2208_UART)?70:200;
  else if (tipoDelay==2) delayPaP=(UsaTMC2208_UART)?120:300;

  if (tipoDelay2==0) delayPaP2=(UsaTMC2208_UART)?20:100;
  else if (tipoDelay2==1) delayPaP2=(UsaTMC2208_UART)?70:200;
  else if (tipoDelay2==2) delayPaP2=(UsaTMC2208_UART)?120:300;

  calculaUnidades();

  calculaUnPaso();              // Calcula la distancia de un paso para el carril 1
  calculaUnPaso2();             // Calcula la distancia de un paso para el carril 2
  
  calculaCuantosPasos();
  calculaCuantosPasos2();

  Antes = millis();     // Guardo el tiempo actual para la funcion "reposo LCD"

  newData = false;

}



// Este codigo de lectura y escritura de EEPROM se ha obtenido de la web de Arduino

void EEPROMWriteInt(int p_address, unsigned int p_value)
{
  byte lowByte = ((p_value >> 0) & 0xFF);
  byte highByte = ((p_value >> 8) & 0xFF);

  EEPROM.write(p_address, lowByte);
  EEPROM.write(p_address + 1, highByte);
}


unsigned int EEPROMReadInt(int p_address)
{
  byte lowByte = EEPROM.read(p_address);
  byte highByte = EEPROM.read(p_address + 1);

  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}
