// Copia los ajustes de la camara actual en la indicada

void copiaAjustesCamara(byte ajuste_destino)
{

  if (ajuste_destino == 1) {
    nFotosMF = nFotosMF2;
    fCiclo = fCiclo2;
    pausa = pausa2;
    tEstab = tEstab2;
    tFoS = tFoS2;
    LedOn = LedOn2;
    LedDur = LedDur2;
  }
  else {
    nFotosMF2 = nFotosMF;
    fCiclo2 = fCiclo;
    pausa2 = pausa;
    tEstab2 = tEstab;
    tFoS2 = tFoS;
    LedDur2 = LedDur;
    LedOn2 = LedOn;
  }

  ponMensajeConfigCamara(ajuste_destino);

}


// Copia los ajustes del carril actual en el destino

void copiaAjustesCarril(byte ajuste_destino)
{

  if (ajuste_destino == 1) {

    dPaso = dPaso2;
    volver = volver2;
    lead = lead2;
    pvuelta = pvuelta2;
    //pulsosPaso = pulsosPaso2;
    precision = precision2;
    delayPaP = delayPaP2;
    tipoMov = tipoMov2;
    aRotacion = aRotacion2;
    sentido = sentido2;
    driverM1_I = driverM2_I;

    calculaUnidades();

    calculaUnPaso();
    calculaFotosMD();    
    calculaCuantosPasos();

    if (UsaTMC2208_UART) {
      driverM1.rms_current((uint16_t)driverM1_I);   // Fijar corriente rms de trabajo    
      driverM1.microsteps(256);
    }

  }
  else {

    dPaso2 = dPaso;
    volver2 = volver;
    lead2 = lead;
    pvuelta2 = pvuelta;
    //pulsosPaso2 = pulsosPaso;
    precision2 = precision;
    delayPaP2 = delayPaP;
    tipoMov2 = tipoMov;
    aRotacion2 = aRotacion;
    sentido2 = sentido;
    driverM2_I = driverM1_I;    

    calculaUnidades();

    calculaUnPaso2();
    calculaFotosMD2();    
    calculaCuantosPasos2();

    if (UsaTMC2208_UART) {
      driverM2.rms_current((uint16_t)driverM2_I);   // Fijar corriente rms de trabajo    
      driverM2.microsteps(256);      
    }

  }

  ponMensajeConfigCarril(ajuste_destino);

}
