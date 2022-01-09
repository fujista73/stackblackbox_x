// Controla el movimiento por los menus

// modoMenu / 100 = 0 si estamos en el arbol principal
// modoMenu / 100 = 1...8 indica que estamos dentro de un submenu y modoMenu mod 100 = 1... indica la opcion dentro de ese submenu
// modoMenu % 100 = 1 si estamos en la opcion ".." para ir atras -> modoMenu = modoMenu div 100


void seleccionaAccion()
{

  int menuAnterior;

  menuAnterior = modoMenu;

  if (!menuIn) {    // Si no estamos modificando un valor

    if (bdown | bdowndown) modoMenu--;
    else if (bup | bupup) modoMenu++;
    else if (bjoy) {                      // Controlo las opciones al pulsar el boton sin estar en modo de ajuste de valores

      if (modoMenu == 1) lanzaCiclo();    // AQUI CONTROLA EL PULSAR LA OPCION "INICIAR"
      else if (modoMenu < 100) {

        if (opMax[modoMenu] > 0) modoMenu = (modoMenu * 100); // Si está en el menu principal y hay submenu, entro al submenu
        else {

          // AQUI TENGO QUE CONTROLAR LAS OPCIONES DEL MENU PRINCIPAL QUE NO TIENE SUBMENU ...

          if (modoMenu == 4) mainMotor = !mainMotor;
          else if (modoMenu == 9) hazReset();
          guardaSetup();
          menuPrincipalX();

        }

      }
      else if ((modoMenu % 100) == 0) modoMenu = modoMenu / 100;  // Si esta en menu secundario y en la opcion ".." volvemos atras
      else {
        controlaValores();        // AQUI ES DONDE VAN LAS ACCIONES SI ESTAMOS DENTRO DE UN SUBMENU
        guardaSetup();            // Guarda en EEPROM los ajustes modificados
        menuPrincipalX();
      }

    }

    opcionPrincipal = (modoMenu < 100) ? 0 : modoMenu / 100;      // Esta es la opcion del menu principal en el que estamos
    opcionSecundaria = (modoMenu < 100) ? 0 : modoMenu % 100;     // Esta es la opcion del submenu donde estamos

    if (modoMenu < 100) {   // Controla los limites en la primera opcion

      if (modoMenu < 1) modoMenu = opMax[opcionPrincipal];
      else if (modoMenu > opMax[opcionPrincipal]) modoMenu = 1;

    }
    else {

      //sprintf(linea, "ANTES: Principal:%3d    Secundaria:%3d      Menu:%3d     opMaX:%3d",opcionPrincipal,opcionSecundaria,modoMenu, opMax[opcionPrincipal]);
      //Serial.println(linea);

      if (opcionSecundaria == 99) modoMenu = ((opcionPrincipal + 1) * 100) + opMax[opcionPrincipal + 1];
      else if (modoMenu > ((opcionPrincipal * 100) + opMax[opcionPrincipal])) modoMenu = (opcionPrincipal * 100);

      opcionPrincipal = modoMenu / 100;        // Esta es la opcion del menu principal en el que estamos
      opcionSecundaria = modoMenu % 100;       // Esta es la opcion del submenu donde estamos

    }

  }               // if (!menuIn) ...
  else {          // Estamos en modo de modificacion de valores, tenemos que controlar lo que cambiamos

    if (bdown | bdowndown | bup | bupup | bmas | bmasmas | bmenos | bmenosmenos) {

      controlaValoresAnalog();                  // Si movemos el cursor ajusta los valores analogicos necesarios

      if (!moviendoEje) {
        guardaSetup();          // Guarda los valores modificados en la EEPROM
        menuPrincipalX();       // Refresca el menu si no estamos en las opciones de mover el carril. SIN ESTO EL MOVIMIENTO ES A SALTOS !!!!
      }
      else {
        if (modoMenu == 202) ponInicioCarril(1);        // Solo actualizamos la linea que pone la distancia de inicio del carril 1
        else if (modoMenu == 203) ponInicioCarril(2);   // Solo actualizamos la linea distancia carril 2
      }

    }
    else if (bjoy) {

      menuIn = 0;                   // Si pulsamos el boton salimos del modo edicion
      moviendoEje = 0;              // Ya seguro que no estamos moviendo ningun eje

      if (autoEN != 0 && !UsaTMC2208_UART) {    // Si AutoEN!=0 desactiva los motores
        digitalWrite(EN1, 1);
        digitalWrite(EN2, 1);
      }

      menuPrincipalX();             // Refresca el menu
    }

  }

  //sprintf(linea, "Principal:%3d    Secundaria:%3d      Menu:%3d     opMaX:%3d     menuAnterior:%3d",opcionPrincipal,opcionSecundaria,modoMenu, opMax[opcionPrincipal], menuAnterior);
  //Serial.println(linea);


  if (menuAnterior != modoMenu) menuPrincipalX();

}



void controlaValores()
{

  // Aqui se controla el ajuste de los valores de la opcion actual del menu al pulsar el boton

  switch (modoMenu) {

    // Primero controlamos las opciones que se activan pulsando el boton

    case 204: finCicloMacro(); finCicloMacro2(); break;    // Resetea la distancia programada para los carriles

    case 301: case 302: case 303: case 304: case 305: 
      modoTrabajo = (modoMenu - 300);           // El modo de trabajo coincide con la opcion del menu - 300
      modoMenu = 3;
      break;

    case 506:
      copiaAjustesCamara(2); break;       // Copia los ajustes actuales a la camara 2

    case 507: disparaCamara(tFoS); break;

    case 606:
      copiaAjustesCamara(1); break;       // Copia los ajustes actuales a la camara 1

    case 607: disparaCamara2(tFoS2); break;

    case 703: tipoDelay = (tipoDelay+1) % 3 ;           // Pone un delay preestablecido para simplificar 
    
      if (tipoDelay==0) delayPaP=(UsaTMC2208_UART)?50:100;
      else if (tipoDelay==1) delayPaP=(UsaTMC2208_UART)?200:200;
      else if (tipoDelay==2) delayPaP=(UsaTMC2208_UART)?400:400;

      if (DEBUG) {
        Serial.print("tipoDelay = "); Serial.println(tipoDelay);  
      }
          
      break;

    case 705:
      sentido = !sentido; break;

    case 708:
      volver = !volver; break;

    case 709: tipoMov = !tipoMov; calculaCuantosPasos(); break;      // Lineal - rotativo

    case 712: copiaAjustesCarril(2); break;   // Copia los ajustes actuales al carril 2

    case 713: digitalWrite(EN1, 0);                               // Activo el driver PaP
      mueveMotor(1, pvuelta, !sentido);                           // Envio pvuelta pasos al motor = 1 vuelta completa
      if (autoEN != 0 && !UsaTMC2208_UART) digitalWrite(EN1, 1);  // Si es necesario desactivo el driver
      break;   

    case 803: tipoDelay2 = (tipoDelay2 + 1) % 3;                  // Pone un delay preestablecido para simplificar 

      if (tipoDelay2==0) delayPaP2=(UsaTMC2208_UART)?20:100;
      else if (tipoDelay2==1) delayPaP2=(UsaTMC2208_UART)?70:200;
      else if (tipoDelay2==2) delayPaP2=(UsaTMC2208_UART)?120:300;
      break;

    case 805: sentido2 = !sentido2; break;

    case 808: volver2 = !volver2; break;
    
    case 809: tipoMov2 = !tipoMov2; calculaCuantosPasos2(); break;        // Lineal - rotativo

    case 812: copiaAjustesCarril(1); break;       // Copia los ajustes actuales al carril 1

    case 813: digitalWrite(EN2, 0);                               // Activo el driver PaP
      mueveMotor(2, pvuelta2, !sentido);                          // Envio 400 pasos al motor = 1 vuelta completa     
      if (autoEN != 0 && !UsaTMC2208_UART) digitalWrite(EN2, 1);  // Si es necesario desactivo el driver
      break;   

    default: menuIn = 1;                          // Si no es ninguna de las anteriores entra en modificacion de valores

  }

}



// Aqui se controla el cambio de los valores analogicos

void controlaValoresAnalog()
{

  int tmp;

  switch (modoMenu) {

    case 201: 
    
      if (cuantosPasos==0 && cuantosPasos2==0) {    // Para poder mover el carril no debe haber un final definido

        if (!moviendoEje) {
          moviendoEje = 1;
          digitalWrite(EN1, 0);       // Activo el driver PaP
          digitalWrite(EN2, 0);
        }
      
        if (bmas)             mueveMotor(1, 1, !sentido, delayPaP/2);
        else if (bmasmas)     mueveMotor(1, 1, !sentido,delayPaP/4);
        else if (bmenos)      mueveMotor(1, 1, sentido, delayPaP/2);
        else if (bmenosmenos) mueveMotor(1, 1, sentido, delayPaP/4);
        else if (bup)         mueveMotor(2, 1, sentido2, delayPaP2/2);
        else if (bupup)       mueveMotor(2, 1, sentido2, delayPaP2/4);
        else if (bdown)       mueveMotor(2, 1, !sentido2, delayPaP2/2);
        else if (bdowndown)   mueveMotor(2, 1, !sentido2, delayPaP2/4);
      
      }
      else indicaDistanciasDefinidas();  
      
      break;


    case 202:

      if (modoTrabajo == 1 || modoTrabajo == 3 || modoTrabajo == ZIGZAG) {

        if (!moviendoEje) {
          moviendoEje = 1;            // Esto hace que no haya pausa en los movimientos al refrescar la pantalla
          digitalWrite(EN1, 0);       // Activo el driver PaP
        }

        if (unidades!=0 || (unidades==0 && cuantosPasos<9970)) {   // Limita por arriba el movimiento del carril

          if (bmas)             { mueveMotor(1, dPaso, !sentido); cuantosPasos = cuantosPasos + dPaso; }
          else if (bmasmas)     { mueveMotor(1, dPaso, !sentido, delayPaP/2); cuantosPasos = cuantosPasos + dPaso; }
          else if (bdown)       { mueveMotor(1, dPaso, !sentido, delayPaP/2); cuantosPasos = cuantosPasos + dPaso; }
          else if (bdowndown)   { mueveMotor(1, dPaso, !sentido, delayPaP/4); cuantosPasos = cuantosPasos + dPaso; }
         
        }

        if (unidades!=0 || (unidades==0 && cuantosPasos>-9970)) {   // Limita por abajo el movimiento del carril
          
          if (bmenos)           { mueveMotor(1, dPaso, sentido); cuantosPasos = cuantosPasos - dPaso; }
          else if (bmenosmenos) { mueveMotor(1, dPaso, sentido, delayPaP/2); cuantosPasos = cuantosPasos - dPaso; }
          else if (bup)         { mueveMotor(1, dPaso, sentido, delayPaP/2); cuantosPasos = cuantosPasos - dPaso; }
          else if (bupup)       { mueveMotor(1, dPaso, sentido, delayPaP/4); cuantosPasos = cuantosPasos - dPaso; }

        }
      
        calculaCuantosPasos();
        calculaFotosMD();    // Calculo cuantas fotos debe hacer y asignarlas a nFotosM2, que se vera en la linea de nFotos si cuantosPasos>0

        if (DEBUG) {
          
          Serial.print("cuantosPasos = "); Serial.println(cuantosPasos);  

          Serial.print("dPaso = "); Serial.print(dPaso);
          Serial.print("; unPaso = "); Serial.print(unPaso);      
          Serial.print("; disPaso = "); Serial.println(disPaso);

          Serial.print("pEntera = "); Serial.print(pEntera);
          Serial.print("; pDecimal = "); Serial.print(pDecimal);
          Serial.print("; dEnteraT = "); Serial.print(pEnteraT);
          Serial.print("; dDecimalT = "); Serial.println(pDecimalT);

          Serial.print("nFotos = "); Serial.println(nFotosMD);
            
        } // if DEBUG         

      }
      else indicaModoIncorrecto();
      
    break;


    case 203:

      if (modoTrabajo == 1 || modoTrabajo == 3 || modoTrabajo == ZIGZAG) {

        if (!moviendoEje) {
          moviendoEje = 1;        // Esto hace que no haya pausa en los movimientos
          digitalWrite(EN2, 0);   // Activo el driver PaP
        }

        if (unidades!=0 || (unidades==0 && cuantosPasos2<9970)) {   // Limita por arriba el movimiento del carril
        
          if (bmas)             { mueveMotor(2, 1, !sentido2); cuantosPasos2++; }
          else if (bmasmas)     { mueveMotor(2, 1, !sentido2, delayPaP2/2); cuantosPasos2++; }
          else if (bdown)       { mueveMotor(2, 1, !sentido2, delayPaP2/2); cuantosPasos2++; }
          else if (bdowndown)   { mueveMotor(2, 1, !sentido2, delayPaP2/4); cuantosPasos2++; }

        }

        if (unidades!=0 || (unidades==0 && cuantosPasos2>-9970)) {   // Limita por abajo el movimiento del carril        

          if (bmenos)           { mueveMotor(2, 1, sentido2); cuantosPasos2--; }
          else if (bmenosmenos) { mueveMotor(2, 1, sentido2, delayPaP2/2); cuantosPasos2--; }
          else if (bup)         { mueveMotor(2, 1, sentido2, delayPaP2/2); cuantosPasos2--; }
          else if (bupup)       { mueveMotor(2, 1, sentido2, delayPaP2/4); cuantosPasos2--; }

        }

        calculaCuantosPasos2();
        calculaFotosMD2();    // Calculo cuantas fotos debe hacer y asignarlas a nFotosM2, que se vera en la linea de nFotos si cuantosPasos>0      

        if (DEBUG) {
          Serial.print("cuantosPasos2 = "); Serial.println(cuantosPasos2);  

          Serial.print("dPaso2 = "); Serial.print(dPaso2);
          Serial.print("; unPaso2 = "); Serial.print(unPaso2);      
          Serial.print("; disPaso2 = "); Serial.println(disPaso2);

          Serial.print("pEntera2 = "); Serial.print(pEntera2);
          Serial.print("; pDecimal2 = "); Serial.print(pDecimal2);
          Serial.print("; dEnteraT2 = "); Serial.print(pEnteraT2);
          Serial.print("; dDecimalT2 = "); Serial.println(pDecimalT2);

          Serial.print("nFotos2 = "); Serial.println(nFotosMD2);
            
        } // if DEBUG         
           
      }
      else indicaModoIncorrecto();      

      break;


    case 501:     // Ajusta el num de fotos de la camara 1 si se trabaja en modo de fotos
    
      if ((modoTrabajo == 2 || modoTrabajo == 4)) {
        nFotosMF = ajustaValor(nFotosMF,1,50,50,100,1,9999);
        calculaCuantosPasos();                                    // Saca la parte entera y decimal de la distancia total a recorrer
      }
      else indicaModoIncorrecto();
      
      break;


    case 502:       // Ajusta el num. de fotos por ciclo de la camara 1

      fCiclo = ajustaValor(fCiclo,1,5,10,20,1,999);
      break;


    case 503:

      pausa = ajustaValor(pausa,1,5,10,50,1,999);
      break;


    case 504:     // Tiempo de estabilizacion, tiempo desde que se mueve el motor hasta que se dispara la camara de nuevo en segundos

      tEstab = (byte) ajustaValor(tEstab,1,5,5,10,0,99);
      break;


    case 505:     // Tiempo de disparo, el tiempo que se activa el optoacoplador de disparo de camara, en decimas de segundo

      tFoS = (byte) ajustaValor(tFoS,1,5,5,10,1,99);   
      break;


    case 508:     // Tiempo de espera en centesimas de segundo para activar la salida LED/Flash 

      LedOn = ajustaValor(LedOn,1,5,10,50,0,5000);
      break;


    case 509:     // Tiempo en milesimas de segundo durante el que se activa la salida Auxiliar LED/Flash 

      LedDur = ajustaValor(LedDur,1,10,10,25,0,5000);      
      break;


    case 601:     // Ajusta el num de fotos de la camara 2

      if ((modoTrabajo == 2 || modoTrabajo == 4)) {
        nFotosMF2 = ajustaValor(nFotosMF2,1,10,50,100,1,9999);
        calculaCuantosPasos2();                                   // Aqui saca la parte entera y decimal de la distancia total a recorrer
      }
      else indicaModoIncorrecto();
            
      break;


    case 602:       // Ajusta el num. de fotos por ciclo de la camara 2

      fCiclo2 = ajustaValor(fCiclo2,1,5,10,20,1,999);
      break;


    case 603:       // Ajusta la pausa (segundos) desde que hace una foto hasta que mueve el motor

      pausa2 = ajustaValor(pausa2,1,5,5,10,1,999);
      break;


    case 604:     // Tiempo de estabilizacion, tiempo desde que se mueve el motor hasta que se dispara la camara 2 de nuevo en segundos

      tEstab2 = (byte) ajustaValor(tEstab2,1,5,5,10,1,99);    
      break;


    case 605:     // Tiempo de disparo, el tiempo que se activa el optoacoplador de disparo de camara 2 en decimas de segundo

      tFoS2 = (byte) ajustaValor(tFoS2,1,5,5,10,1,99);
      break;


    case 608:     // Tiempo de espera en centesimas de segundo para activar la salida LED/Flash 

      LedOn2 = ajustaValor(LedOn2,1,5,10,50,0,5000);
      break;


    case 609:     // Tiempo en milesimas de segundo durante el que se activa la salida Auxiliar LED/Flash 

      LedDur2 = ajustaValor(LedDur2,1,10,10,25,0,5000);
      break;


    case 701:

      if (!moviendoEje) {
        moviendoEje = 1;
        digitalWrite(EN1, 0);       // Activo el driver PaP
      }

      if (bmas)             mueveMotor(1, 1, !sentido);               // mueve el motor 1 paso hacia delante
      else if (bmasmas)     mueveMotor(1, 1, !sentido, delayPaP/2);   // mueve el motor 1 paso hacia delante un poco mas rapido
      else if (bmenos)      mueveMotor(1, 1, sentido);                // mueve el motor 1 paso hacia atras
      else if (bmenosmenos) mueveMotor(1, 1, sentido, delayPaP/2);    // mueve el motor 1 paso hacia atras un poco mas rapido
      else if (bup)         mueveMotor(1, 1, sentido, delayPaP/3);    // mueve el motor 1 paso hacia delante rapido
      else if (bupup)       mueveMotor(1, 1, sentido, delayPaP/4);    // mueve el motor 1 paso hacia delante mas rapido
      else if (bdown)       mueveMotor(1, 1, !sentido, delayPaP/3);   // mueve el motor 1 paso hacia detras rapido
      else if (bdowndown)   mueveMotor(1, 1, !sentido, delayPaP/4);   // mueve el motor 1 paso hacia detras mas rapido

      if (cuantosPasos != 0) {    // Si había posicion final establecida, la pone a 0 y recalcula todo
        cuantosPasos = 0;
        calculaCuantosPasos();
        calculaFotosMD();    
      }
      
      break;


    case 702:

      dPaso = ajustaValor(dPaso,1,5,10,20);
    
      if ((dPaso*unPaso)>99999) dPaso = 0;   //  En milimetros, la distancia real es como maximo 99 mm
      if (dPaso<0) dPaso = 99999/unPaso;        
      
      calculaUnidades();
      calculaUnPaso();
      calculaUnPaso2();

      if (cuantosPasos != 0) {
        calculaFotosMD();
        calculaCuantosPasos();        
      }

      if (DEBUG) {
        Serial.print("dPaso = "); Serial.print(dPaso);
        Serial.print("; unPaso = "); Serial.print(unPaso);      
        Serial.print("; disPaso = "); Serial.println(disPaso);

        Serial.print("pEntera = "); Serial.print(pEntera);
        Serial.print("; pDecimal = "); Serial.print(pDecimal);
        Serial.print("; dEnteraT = "); Serial.print(pEnteraT);
        Serial.print("; dDecimalT = "); Serial.println(pDecimalT);
      }
      
      break;


    case 704:

      if (bmas | bmasmas | bdown | bdowndown) precision++;
      else if (bmenos | bmenosmenos | bup | bupup) precision--;
    
      if ((UsaTMC2208_UART & precision > 8) | (!UsaTMC2208_UART & precision>5))  precision = 0;

      dPaso = 1;    // Si se cambia la precision, pone por defecto 1 paso como distancia de paso

      calculaUnidades();
      calculaUnPaso();
      
      if (cuantosPasos != 0) {
        calculaFotosMD();
        calculaCuantosPasos();    // Calcula la distancia total a recorrer y su parte entera y decimal             
      }

      break;


    case 706:                     // Establece el numero de pasos para una vuelta del motor (motores normales: 200 o 400)
    
      pvuelta = ajustaValor(pvuelta,10,20,40,80,100,9990);
      
      calculaUnidades();
      calculaUnPaso();            // Calcula la distancia de un paso y su parte entera y decimal

      if (DEBUG) {
        Serial.print("pvuelta = "); Serial.println(pvuelta);
        Serial.print("unPaso = "); Serial.print(unPaso); Serial.print("; disPaso = "); Serial.println(disPaso);
      }
      
      
      if (cuantosPasos != 0) {
        calculaFotosMD();
        calculaCuantosPasos();    // Calcula la distancia total a recorrer y su parte entera y decimal             
      }
      
      break;


    case 707:                     // Establece el lead, o distancia que se mueve por cada vuelta completa del motor, incrementos de 0.5mm 
    
      lead = ajustaValor(lead,1,4,8,16,1,100);   
      
      calculaUnidades();
      calculaUnPaso();            // Calcula la distancia de un paso y su parte entera y decimal
      
      if (cuantosPasos != 0) {
        calculaFotosMD();
        calculaCuantosPasos();    // Calcula la distancia total a recorrer y su parte entera y decimal             
      }
      
      break;


    case 710:
    
      aRotacion = ajustaValor(aRotacion,1,10,50,100,1,5550);
      calculaUnPaso();
      
      break;


    case 711:         // Ajusta la intensidad RMS del motor 1

      driverM1_I = ajustaValor(driverM1_I,5,10,25,50,5,999);    

      if (UsaTMC2208_UART) driverM1.rms_current((uint16_t)driverM1_I);   // Fijar corriente rms de trabajo

      if (DEBUG) {
        Serial.print("DRV1_I_RMS = ");
        Serial.println(driverM1.rms_current());
      }

      break;


    case 801:

      if (!moviendoEje) {
        moviendoEje = 1;
        digitalWrite(EN2, 0);       // Activo el driver PaP
      }

      if (bmas)             mueveMotor(2, 1, !sentido);               // mueve el motor 1 paso hacia delante
      else if (bmasmas)     mueveMotor(2, 1, !sentido, delayPaP2/2);  // mueve el motor 1 paso hacia delante un poco mas rapido
      else if (bmenos)      mueveMotor(2, 1, sentido);                // mueve el motor 1 paso hacia atras
      else if (bmenosmenos) mueveMotor(2, 1, sentido, delayPaP2/2);   // mueve el motor 1 paso hacia atras un poco mas rapido
      else if (bup)         mueveMotor(2, 1, sentido, delayPaP2/3);   // mueve el motor 1 paso hacia delante rapido
      else if (bupup)       mueveMotor(2, 1, sentido, delayPaP2/4);   // mueve el motor 1 paso hacia delante mas rapido
      else if (bdown)       mueveMotor(2, 1, !sentido, delayPaP2/3);  // mueve el motor 1 paso hacia detras rapido
      else if (bdowndown)   mueveMotor(2, 1, !sentido, delayPaP2/4);  // mueve el motor 1 paso hacia detras mas rapido

      if (cuantosPasos2 != 0) {    // Si había posicion final establecida, la pone a 0 y recalcula todo
        cuantosPasos2 = 0;
        calculaCuantosPasos2();
        calculaFotosMD2();    
      }
     
      break;

    case 802:

      dPaso2 = ajustaValor(dPaso2,1,5,10,20);
    
      if ((dPaso2*unPaso2)>99999) dPaso2 = 0;   //  En milimetros, la distancia real es como maximo 99 mm
      if (dPaso2<0) dPaso2 = 99999/unPaso2;

      calculaUnidades();
      calculaUnPaso2();
      calculaUnPaso2();
      
      if (cuantosPasos2 != 0) {
        calculaFotosMD2();
        calculaCuantosPasos2();
      }

      if (DEBUG) {
        Serial.print("dPaso2 = "); Serial.print(dPaso2);
        Serial.print("; unPaso2 = "); Serial.print(unPaso2);      
        Serial.print("; disPaso2 = "); Serial.println(disPaso2);

        Serial.print("pEntera2 = "); Serial.print(pEntera2);
        Serial.print("; pDecimal2 = "); Serial.print(pDecimal2);
        Serial.print("; dEnteraT2 = "); Serial.print(pEnteraT2);
        Serial.print("; dDecimalT2 = "); Serial.println(pDecimalT2);
      }
     
      break;


    case 804:

      if (bmas | bmasmas | bdown | bdowndown) precision2++;
      else if (bmenos | bmenosmenos | bup | bupup) precision2--;
    
      if ((UsaTMC2208_UART & precision2 > 8) | (!UsaTMC2208_UART & precision2>5))  precision2 = 0;

      dPaso2 = 1;    // Si se cambia la precision, pone por defecto 1 paso como distancia de paso

      calculaUnidades();
      calculaUnPaso2();
      
      if (cuantosPasos2 != 0) {
        calculaFotosMD2();
        calculaCuantosPasos2();    // Calcula la distancia total a recorrer y su parte entera y decimal             
      }

    break;


    case 806:                       // Establece el numero de pasos para una vuelta del motor (antes solo 200 o 400)

      pvuelta2 = ajustaValor(pvuelta2,10,20,40,80,100,9990);
   
      calculaUnidades();
      calculaUnPaso2();             // Calcula la distancia de un paso y su parte entera y decimal
      
      if (cuantosPasos2 != 0) {
        calculaFotosMD2();
        calculaCuantosPasos2();     // Calcula la distancia total a recorrer y su parte entera y decimal             
      }
      
      break;


    case 807:                       // Establece el lead, o distancia que se mueve por cada vuelta completa del motor, incrementos de 0.5mm

      lead2 = ajustaValor(lead2,1,4,8,16,1,100);   

      calculaUnidades();
      calculaUnPaso2();             // Calcula la distancia de un paso y su parte entera y decimal

      if (cuantosPasos2 != 0) {
        calculaFotosMD2();
        calculaCuantosPasos2();     // Calcula la distancia total a recorrer y su parte entera y decimal        
      }
      
      break;


    case 810:

      aRotacion2 = ajustaValor(aRotacion2,1,10,50,100,1,5550);
      calculaUnPaso2();
      
      break;


    case 811:         // Ajusta la intensidad RMS del motor 2

      driverM2_I = ajustaValor(driverM2_I,5,10,25,50,5,999);

      if (UsaTMC2208_UART) driverM2.rms_current((uint16_t)driverM2_I);   // Fijar corriente rms de trabajo

      if (DEBUG) {
        Serial.print("DRV2_I_RMS = ");
        Serial.println(driverM2.rms_current());
      }

      break;

  }

}


uint16_t ajustaValor(uint16_t variable, int mas, int masmas, int up, int upup, int limite_abajo, int limite_arriba)
{
  
  int temporal=(int) variable;    // Lo paso a entero para controlar valores < 0

  if (bmas)             temporal += mas;
  else if (bmasmas)     temporal += masmas;
  else if (bmenos)      temporal -= mas;
  else if (bmenosmenos) temporal -= masmas;
  else if (bup)         temporal -= up;
  else if (bupup)       temporal -= upup;
  else if (bdown)       temporal += up;
  else if (bdowndown)   temporal += upup;

  if (temporal < limite_abajo) temporal = limite_arriba;
  if (temporal > limite_arriba) temporal = limite_abajo;

  return ((uint16_t)temporal);
}


int ajustaValor(int variable, int mas, int masmas, int up, int upup)
{
  int temporal=variable;

  if (bmas)             temporal += mas;
  else if (bmasmas)     temporal += masmas;
  else if (bmenos)      temporal -= mas;
  else if (bmenosmenos) temporal -= masmas;
  else if (bup)         temporal -= up;
  else if (bupup)       temporal -= upup;
  else if (bdown)       temporal += up;
  else if (bdowndown)   temporal += upup;
  
  return (temporal);
}
