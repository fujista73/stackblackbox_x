void ponCursor()
{

  int numLinea;

  // Quito la marca del menu de las 4 opciones

  ponLinea(0, 0, (char*)" ");
  ponLinea(0, 1, (char*)" ");
  ponLinea(0, 2, (char*)" ");
  ponLinea(0, 3, (char*)" ");

  // Coloco el cursor donde debe estar segun la opcion y pongo el cursor de posicion

  numLinea = (modoMenu < 99) ? modoMenu : (opcionSecundaria + 1);
  lcd0x27.setCursor(0, (numLinea % 4) - 1); lcd0x27.write(menuIn);
  lcd0x3F.setCursor(0, (numLinea % 4) - 1); lcd0x3F.write(menuIn);

  if (!moviendoEje && !menuIn) delay(20);   // Si no esta dentro de la opcion de mover el eje Z, espera un poco antes de volver a leer los botones

}


void ponPantallaInicio()
{

  strcpy(linea1, "********************");
  strcpy(linea2, "* StackBlackBox X  *");
  strcpy(linea3, "* (r)2020 / v4.22  *");
  strcpy(linea4, "********************");

  ponLinea(0, 0, 1);
  ponLinea(0, 1, 2);
  ponLinea(0, 2, 3);
  ponLinea(0, 3, 4);

  Serial.println(linea1);
  Serial.println(linea2);
  Serial.println(linea3);
  Serial.println(linea4);

  Serial.println("");
  Serial.println("Puerto Serie 1 (usb) conectado ...");

}


void ponMacro()
{
  float dActual = fotosHechas * disPaso,
        dActual2 = fotosHechas2 * disPaso2,
        gradosActual = fotosHechas * gradosTotales,
        gradosActual2 = fotosHechas2 * gradosTotales2;
  long dActualE, dActualE2, gradosActualE, gradosActualE2;
  int dActualD, dActualD2, nF, nF2, gradosActualD, gradosActualD2;

  // Aqui obtego los valores en caso de que se haya fijado una precisión

  dActual = dActual / float(precisionVer[precision]);
  dActual2 = dActual2 / float(precisionVer[precision2]);    

  // Aqui se calcula la parte entera y decimal del movimiento actual

  dActualE = long(dActual);                         // Parte entera de la distancia actual
  dActualD = int((dActual-long(dActual))*100);      // Parte con 2 decimales de la distancia actual
  
  dActualE2 = long(dActual2);
  dActualD2 = int((dActual2-long(dActual2))*100);

  gradosActualE = long(gradosActual);
  gradosActualD = (unsigned(gradosActual * 10)) % 10;

  gradosActualE2 = long(gradosActual2);
  gradosActualD2 = (unsigned(gradosActual2 * 10)) % 10;


  // Ajusto la distancia actual parcial si trabajamos en milimetros

  if (unidades) {                               // Si trabajo con milimetros ajusto la parte entera y decimales
    dActualD = (dActualE / 10) % 100;           // Primero se calcula la parte decimal ya que usa la parte entera original
    dActualE = dActualE / 1000;
  }

  if (unidades2) {                              // Si trabajo con milimetros ajusto la parte entera y decimales
    dActualD2 = (dActualE2 / 10) % 100;         // Primero se calcula la parte decimal ya que usa la parte entera original
    dActualE2 = dActualE2 / 1000;
  }


  // Aqui pongo las fotos que se verán en funcion del modo

  if (modoTrabajo == 2 || modoTrabajo == 4) {
    nF = nFotosMF;
    nF2 = nFotosMF2;
  }
  else {
    nF = nFotosMD;
    nF2 = nFotosMD2;
  }
    
  if (modoTrabajo<3 && mainMotor==1) {    // Si trabajamos en modos con un carril y es el carril 2, muestro sus datos

    if (!tipoMov2) sprintf(linea1, " %04lu,%02d / %04lu,%02d  ", dActualE2, dActualD2, pEnteraT2, pDecimalT2);
    else sprintf(linea1, " R.Tot: %4d,%1d grad ", unsigned(gradosActualE2), unsigned(gradosActualD2));
    
    sprintf(linea2, " %04d / %03d / %03d+%02d", (nF2 - fotosHechas2), fCiclo2, pausa2, tEstab2);
   
  }
  else {    // Si es carril 1 o modo de dos carriles, siempre mostramos los datos del carril 1

    if (!tipoMov) sprintf(linea1, " %04lu,%02d / %04lu,%02d  ", dActualE, dActualD, pEnteraT, pDecimalT);
    else sprintf(linea1, " R.Tot: %4d,%1d grad ", unsigned(gradosActualE), unsigned(gradosActualD));     
    
    sprintf(linea2, " %04d / %03d / %03d+%02d", (nF - fotosHechas), fCiclo, pausa, tEstab);

  }

  if (modoTrabajo > 2) {      // Si trabajamos con dos carriles tambien muestro los datos del segundo carril

    if (!tipoMov2) sprintf(linea3, " %04lu,%02d / %04lu,%02d  ", dActualE2, dActualD2, pEnteraT2, pDecimalT2);      // Refresca el movimiento lineal
    else sprintf(linea3, " R.Tot: %4d,%1d grad ", unsigned(gradosActualE2), unsigned(gradosActualD2));              // Refresca el movimiento rotatorio girado      

    sprintf(linea4, " %04d / %03d / %03d+%02d", (nF2 - fotosHechas2), fCiclo2, pausa2, tEstab2);

  }
  else {
    sprintf(linea3, "                    ");
    sprintf(linea4, "                    ");
  }

  ponLinea(0, 0, 1);
  ponLinea(0, 1, 2);
  ponLinea(0, 2, 3);
  ponLinea(0, 3, 4);

}


void ponFinCicloMacro()   // ***** Muestra una pantalla indicando que ha disparado la camara por entrada digital directa
{
  ponLinea(0, 0, (char*)"--------------------");
  ponLinea(0, 1, (char*)"  Fin ciclo macro   ");
  ponLinea(0, 2, (char*)"                    ");
  ponLinea(0, 3, (char*)"--------------------");
  delay(1000);
}


void indicaDisparo()    // ***** Muestra una pantalla indicando que ha disparado la camara por entrada digital directa
{
  ponLinea(0, 0, (char*)"--------------------");
  ponLinea(0, 1, (char*)"   Disparo directo  ");
  ponLinea(0, 2, (char*)"                    ");
  ponLinea(0, 3, (char*)"--------------------");
  delay(500);
}


void indicaModoIncorrecto()    // ***** Muestra una pantalla indicando que el modo de trabajo es incorrecto
{
  ponLinea(0, 0, (char*)"=ERROR==============");
  sprintf(linea2, " Modo de trabajo %1d ", modoTrabajo); ponLinea(0, 1, 2);  
  ponLinea(0, 2, (char*)" incorrecto!        ");
  ponLinea(0, 3, (char*)" Cambia de Modo     ");
  delay(2000);
}


void indicaDistanciasDefinidas()    // ***** Muestra una pantalla indicando que el modo de trabajo es incorrecto
{
  ponLinea(0, 0, (char*)"=ERROR==============");
  ponLinea(0, 1, (char*)" Hay distancias     ");
  ponLinea(0, 2, (char*)" definidas!         ");
  ponLinea(0, 3, (char*)" Haz Reset Carriles ");
  delay(2000);
}



void ponMensajePulsos(int pulsos)
{
  ponLinea(0, 0, (char*)"--------------------");
  sprintf(linea2, "Enviando %3d pulsos ", pulsos); ponLinea(0, 1, 2);
  ponLinea(0, 2, (char*)"                    ");
  ponLinea(0, 3, (char*)"--------------------");
  delay(500);
}


void ponMensajeConfigCamara(byte camara)
{
  ponLinea(0, 0, (char*)"--------------------");
  ponLinea(0, 1, (char*)" Config. copiada    ");
  sprintf(linea3, " a la camara %d      ", camara); ponLinea(0, 2, 3);
  ponLinea(0, 3, (char*)"--------------------");
  sprintf(linea, "Configuracion copiada a la camara %d", camara);
  Serial.println(linea);
  delay(800);
}


void ponMensajeConfigCarril(byte carril)
{
  ponLinea(0, 0, (char*)"--------------------");
  ponLinea(0, 1, (char*)" Config. copiada    ");
  sprintf(linea3, " al carril %d        ", carril); ponLinea(0, 2, 3);
  ponLinea(0, 3, (char*)"--------------------");
  sprintf(linea, "Configuracion copiada al carril %d", carril);
  Serial.println(linea);
  delay(800);
}


void ponMensajeModo(byte modo)
{
  ponLinea(0, 0, (char*)"--------------------");
  ponLinea(0, 1, (char*)" Establecido modo   ");
  sprintf(linea3, " de trabajo %d       ", modo); ponLinea(0, 2, 3);
  ponLinea(0, 3, (char*)"--------------------");
  sprintf(linea, "Establecido modo de trabajo %d", modo);
  Serial.println(linea);
  delay(800);
}


void ponMensajeEstabilizando()
{
  ponLinea(0, 0, (char*)"--------------------");
  ponLinea(0, 1, (char*)" Esperando tiempo   ");
  ponLinea(0, 2, (char*)" de estabilizacion  ");
  ponLinea(0, 3, (char*)"--------------------");
}


void ponMensajeReset()
{
  ponLinea(0, 0, (char*)"********************");
  ponLinea(0, 1, (char*)"* Reestableciendo  *");
  ponLinea(0, 2, (char*)"* los ajustes      *");
  ponLinea(0, 3, (char*)"********************");
}


void error1()   // ***** Muestra una pantalla indicando que ha disparado la camara por entrada digital directa
{
  ponLinea(0, 0, (char*)"=ERROR==============");
  ponLinea(0, 1, (char*)" Debe fijar inicio  ");
  ponLinea(0, 2, (char*)" del carril 1       ");
  ponLinea(0, 3, (char*)"====================");
  delay(1500);
  menuPrincipalX();
}


void error2()   // ***** Muestra una pantalla indicando que ha disparado la camara por entrada digital directa
{
  ponLinea(0, 0, (char*)"=ERROR==============");
  ponLinea(0, 1, (char*)" Debe fijar inicio  ");
  ponLinea(0, 2, (char*)" del carril 2       ");
  ponLinea(0, 3, (char*)"====================");
  delay(1500);
  menuPrincipalX();
}


void error3()
{
  ponLinea(0, 0, (char*)"=ERROR==============");
  ponLinea(0, 1, (char*)"  Final Carrera 1   ");
  ponLinea(0, 2, (char*)"  activado          ");
  ponLinea(0, 3, (char*)"====================");
}


void error4()
{
  ponLinea(0, 0, (char*)"=ERROR==============");
  ponLinea(0, 1, (char*)"  Final Carrera 2   ");
  ponLinea(0, 2, (char*)"  activado          ");
  ponLinea(0, 3, (char*)"====================");
}


void ponTodaPantalla()
{
  ponLinea(0, 0, linea1);
  ponLinea(0, 1, linea2);
  ponLinea(0, 2, linea3);
  ponLinea(0, 3, linea4);
}


void ponLinea(byte x, byte y, byte linea)
{
  switch (linea) {

    case 1: ponLinea(x, y, (char*)linea1); break;
    case 2: ponLinea(x, y, (char*)linea2); break;
    case 3: ponLinea(x, y, (char*)linea3); break;
    case 4: ponLinea(x, y, (char*)linea4); break;
  }
}


void ponLinea(byte x, byte y, char *cadena)
{
  lcd0x27.setCursor(x, y); lcd0x27.print(cadena);
  lcd0x3F.setCursor(x, y); lcd0x3F.print(cadena);
}
