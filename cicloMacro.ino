void lanzaCiclo()
{

  if (modoTrabajo==1 && !mainMotor && !nFotosMD) error1();
  else if (modoTrabajo==1 && mainMotor && !nFotosMD) error2();
  else if ((modoTrabajo==3 || modoTrabajo==ZIGZAG) && !nFotosMD) error1();
  else if ((modoTrabajo==3 || modoTrabajo==ZIGZAG) && !nFotosMD2) error2();
  else {

    if (modoTrabajo<3) {                  // Indico que carril esta funcionando
      if (!mainMotor) funcionando=1; 
      else funcionando2=1;
    }
    else funcionando = funcionando2 = 1;
    
    fotosHechas = fotosHechas2 = 0;
    totalPasos = totalPasos2 = 0;                 // Pongo a 0 el total de pasos dados, sirve para volver a la posicion inicial

    // Ajusto el numero de fotos a hacer en funcion del modo

    if (modoTrabajo == 2 || modoTrabajo == 4) {
      nFotos = nFotosMF;
      nFotos2 = nFotosMF2;
    }
    else {
      nFotos = nFotosMD;
      nFotos2 = nFotosMD2;
    }

    ponMensajeEstabilizando();

    delay(tEstab * 1000);           // Me espero el tiempo establecido para estabilizar al haber pulsado el boton del joystick

    ponMacro();                     // Muestro la pantalla de datos del ciclo de macro

  }

}


void cicloMacro()
{
  unsigned int pasos;

  // El programa esta funcionando, tiene que hacer una foto y esperar el tiempo establecido en pausa

  fotosHechas++;         // Incrementa el num de fotos hechas

  for (int i = 0; (i < fCiclo && digitalRead(J_BTN)); i++) {   // Ejecuto el ciclo de disparar la camara las veces establecidas en fCiclo

    sprintf(linea2, " %04d / %03d", (nFotos - fotosHechas), fCiclo - (i + 1));
    ponLinea(0, 1, 2);

    disparaCamara(tFoS);

    // Si LedOn > 0 hay que activar tambien la salida auxiliar para el control del LED/Flash

    if (LedOn>0) {

      delay(LedOn);               // Espera el tiempo indicado antes de "encender" la luz LED/Flash
      
      digitalWrite(CH_AUX, 0);    // Activo la salida del led
      delay(LedDur);       
      digitalWrite(CH_AUX, 1);    // Desactivo la salida del led
      
    }

    delay(pausa * 1000);                    // Me espero el tiempo de pausa establecido    

  }

  // Si se pulsado el boton, final de carrera o ha hecho todas las fotos -> fin de ciclo

  if (!digitalRead(J_BTN) || !digitalRead(FCA) || !digitalRead(FCB) || fotosHechas >= nFotos) finCicloMacro();
  else
  if (dPaso > 0) {

    // Si tipoMov = 0, es movimiento lineal, usa los pasos especificados en dPaso
    // Si tipoMov = 1, es movimiento rotativo, usa los pasos especificados en aRotacion

    pasos = (tipoMov)?aRotacion:dPaso; // Pasos = depende del tipo de movimiento, pasos para angulo de rotacion o pasos para movimiento lineal   
  
    digitalWrite(EN1, 0);              // Siempre activo el driver del motor

    if (cuantosPasos <= 0) mueveMotor(1, pasos, !sentido);      // Si es modo 2, el motor debe moverse en sentido contrario al normal ya que lo hemos colocado en el final del recorrido
    else if (cuantosPasos > 0) mueveMotor(1, pasos, sentido);   // mueve el motor con los pulsos calculados

    if (autoEN==2 && !UsaTMC2208_UART) digitalWrite(EN1, 1);   // Si es necesario desactivo el driver del motor
    
    totalPasos++;     // Guardo el num de pasos totales dados

  }

  if (fotosHechas<nFotos) ponMacro();

  delay(tEstab * 1000);

}


void finCicloMacro()
{
  unsigned int pasos;

  if (modoTrabajo==ZIGZAG || (volver && digitalRead(FCA) && digitalRead(FCB))) {     // Si debe volver a su posicion inicial y NO estaba pulsado un final de carrera

    pasos = (tipoMov)?aRotacion:dPaso; // Pasos = depende del tipo de movimiento, pasos para angulo de rotacion o pasos para movimiento lineal   

    // llevo el carril a la posicion original, moviendolo en sentido contrario los pasos que se movio

    digitalWrite(EN1, 0);               // Siempre activo el driver del motor

    for (int i = 0; i < totalPasos; i++) {
      if (cuantosPasos <= 0) mueveMotor(1, pasos, sentido);   // El motor debe moverse en sentido contrario al normal ya que lo hemos colocado en el final del recorrido
      else mueveMotor(1, pasos, !sentido);
    }

  } // if (volver && digitalRead(FCA)...

  if (autoEN != 0 && !UsaTMC2208_UART) digitalWrite(EN1, 1);   // Si es necesario desactivo el driver del motor

  // Enciendo el LCD por si se hubiera apagado

  lcd0x27.display(); lcd0x27.backlight();
  lcd0x3F.display(); lcd0x3F.backlight();
  Antes = millis();

  funcionando = nFotosMD = 0;     // Hago un reset de este carril

  if (modoTrabajo!=ZIGZAG) cuantosPasos = 0;     // Si el modo de trabajo no es el 5, hago un reset de la distancia recorrida
  
}



void cicloMacro2()
{
  unsigned int pasos;

  // El programa esta funcionando, tiene que hacer una foto y esperar el tiempo establecido en pausa

  fotosHechas2++;         // Incrementa el num de fotos hechas

  for (int i = 0; (i < fCiclo2 && digitalRead(J_BTN)); i++) {   // Ejecuto el ciclo de disparar la camara las veces establecidas en fCiclo

    sprintf(linea2, " %04d / %03d", (nFotos2 - fotosHechas2), fCiclo2 - (i + 1));

    if (modoTrabajo==1 || modoTrabajo==2) ponLinea(0, 1, 2);    // Si solo trabajo con un carril pongo los datos en la linea 2 
    else ponLinea(0, 3, 2);                                     // Si trabajo con los dos carriles actualizo la linea 3

    if (modoTrabajo != ZIGZAG) {    // Si el modo de trabajo no es zigzag, dispara la camara 2
      
      disparaCamara2(tFoS2);   

      // Si LedOn2 > 0 hay que activar tambien la salida auxiliar para el control del LED/Flash

      if (LedOn2>0) {

        delay(LedOn2);               // Espera el tiempo indicado antes de "encender" la luz LED/Flash
      
        digitalWrite(CH_AUX, 0);     // Activo la salida del led
        delay(LedDur2);       
        digitalWrite(CH_AUX, 1);     // Desactivo la salida del led
      
      }
      
      delay(pausa2 * 1000);         // Me espero el tiempo establecido
    }

  }

  // Si se pulsado el boton, final de carrera o ha hecho todas las fotos -> fin de ciclo

  if (!digitalRead(J_BTN) || !digitalRead(FCA) || !digitalRead(FCB) || fotosHechas2 >= nFotos2) finCicloMacro2();
  else
  if (dPaso2 > 0) {

    // Si tipoMov = 0, es movimiento lineal, usa los pasos especificados en dPaso
    // Si tipoMov = 1, es movimiento rotativo, usa los pasos especificados en aRotacion

    pasos = (tipoMov2)?aRotacion2:dPaso2; // Pasos = depende del tipo de movimiento, pasos para angulo de rotacion o pasos para movimiento lineal    
  
    digitalWrite(EN2, 0);                   // Siempre activo el motor

    if (cuantosPasos2 <= 0) mueveMotor(2, pasos, !sentido2);      // Si es modo 2, el motor debe moverse en sentido contrario al normal ya que lo hemos colocado en el final del recorrido
    else if (cuantosPasos2 > 0) mueveMotor(2, pasos, sentido2);   // mueve el motor con los pulsos calculados

    if (autoEN == 2 && !UsaTMC2208_UART) digitalWrite(EN2, 1);   // Si es necesario desactivo el driver del motor
    
    totalPasos2++;     // Guardo el num de pasos totales dados

  }

  if (fotosHechas2<nFotos2) ponMacro();

  if (modoTrabajo!=ZIGZAG) delay(tEstab2 * 1000);   // Si el modo no es zigzag, espero tiempo de estabilizacion del carril 2

}


void finCicloMacro2()
{
  unsigned int pasos;

  if (volver2 && digitalRead(FCA) && digitalRead(FCB)) {    // Si debe volver a su posicion inicial y NO estaba pulsado el final de carrera

    pasos = (tipoMov2)?aRotacion2:dPaso2; // Pasos = depende del tipo de movimiento, pasos para angulo de rotacion o pasos para movimiento lineal

    // llevo el carril adecuado a la posicion original, moviendolo en sentido contrario los pasos que se movio

    digitalWrite(EN2, 0);               // Siempre activo el driver del motor

    for (int i = 0; i < totalPasos2; i++) {
      if (cuantosPasos2 <= 0) mueveMotor(2, pasos, sentido2);   // Si es modo 2, el motor debe moverse en sentido contrario al normal ya que lo hemos colocado en el final del recorrido
      else mueveMotor(2, pasos, !sentido2);
    }

  } // if (volver && digitalRead(FCA)) ...

  if (autoEN != 0 && !UsaTMC2208_UART) digitalWrite(EN2, 1);   // Si es necesario desactivo el driver del motor

  // Enciendo el LCD por si se hubiera apagado

  lcd0x27.display(); lcd0x27.backlight();
  lcd0x3F.display(); lcd0x3F.backlight();
  Antes = millis();

  funcionando2 = nFotosMD2 = 0;     // Hago un reset de este carril

  cuantosPasos2 = 0;

}


// Dispara la camara con el modo programado

void disparaCamara(int tiempo)
{
  digitalWrite(CH7, 1);
  //Serial1.println("<SHUT1>");
  //Serial.println("Enviando <SHUT1> al HC-12");
  delay(tiempo * 10);         // Espera el tiempo establecido en decimas de segundo antes de desconectar los reles
  digitalWrite(CH7, 0);
}


void disparaCamara2(int tiempo)
{
  digitalWrite(CH8, 1);
  //Serial1.println("<SHUT2>");
  //Serial.println("Enviando <SHUT2> al HC-12");
  delay(tiempo * 10);         // Espera el tiempo establecido en decimas de segundo antes de desconectar los reles
  digitalWrite(CH8, 0);
}


// Mueve el motor los pasos en la direccion indicada

void mueveMotor(byte nmotor, int pasos, bool direccion)
{
  long pulsos;
  uint16_t delayPaP_aqui, dir_aqui, step_aqui;

  pulsos = long(pasos) * (UsaTMC2208_UART?256:32);

  if (nmotor == 1) {
    dir_aqui = DIR1;
    step_aqui = STEP1;
    delayPaP_aqui = delayPaP;
    pulsos = pulsos / precisionVer[precision];      // Ajusta los pulsos segun la precision 
  }
  else if (nmotor == 2) {
    dir_aqui = DIR2;
    step_aqui = STEP2;
    delayPaP_aqui = delayPaP2;
    pulsos = pulsos / precisionVer[precision2];      // Ajusta los pulsos segun la precision  
  }

  digitalWrite(dir_aqui, direccion);           // Fijamos una direcion

  for (long i = pulsos; i > 0; i--) {          // Mandamos pulsos al motor
    digitalWrite(step_aqui, HIGH);
    delayMicroseconds(delayPaP_aqui);          // Esperamos xxx microsegundos
    digitalWrite(step_aqui, LOW);
    delayMicroseconds(delayPaP_aqui);          // Esperamos xxx microsegundos
  }

}


// Mueve el motor los pasos en la direccion indicada y usando los micropulsos indicados

void mueveMotor(byte nmotor, uint16_t pasos, bool direccion, uint16_t micropulsos)
{

  long pulsos;
  uint16_t delayPaP_aqui, dir_aqui, step_aqui;

  pulsos = long(pasos) * (UsaTMC2208_UART?256:32);
  delayPaP_aqui = micropulsos;

  if (nmotor == 1) {
    dir_aqui = DIR1;
    step_aqui = STEP1;
    pulsos = pulsos / precisionVer[precision];      // Ajusta los pulsos segun la precision 
  }
  else if (nmotor == 2) {
    dir_aqui = DIR2;
    step_aqui = STEP2;
    pulsos = pulsos / precisionVer[precision2];      // Ajusta los pulsos segun la precision  
  }

  digitalWrite(dir_aqui, direccion);           // Fijamos una direcion

  for (long i = pulsos; i > 0; i--) {          // Mandamos pulsos al motor
    digitalWrite(step_aqui, HIGH);
    delayMicroseconds(delayPaP_aqui);          // Esperamos xxx microsegundos
    digitalWrite(step_aqui, LOW);
    delayMicroseconds(delayPaP_aqui);          // Esperamos xxx microsegundos
  }


}
