/*

      1. Iniciar ciclo     - Inicia el ciclo de trabajo programado

      2. Fijar recorrido 
        200. ..
        201. Fijar posicion ZX    - Establece la posición inicial del ciclo de macro
        202. |-- 0,00 u/mm --|Z   - (modos 1, 3 y 5) Carril 1. Muestra información de la distancia que se recorrerá y del numero de fotos que se haran
        203. |-- 0,00 u/mm --|X   - (modos 1, 3 y 5) Carril 2. Muestra información de la distancia que se recorrerá y del numero de fotos que se haran        
        
        204. Reset carriles       - Pone a 0 el inicio de los dos carriles

      3. Modo Trabajo: 1 - 5

        Modo 1 - 1 carril indicando distancia = fijando inicio
        Modo 2 - 1 carril indicando numero de fotos
        Modo 3 - 2 carriles indicando distancia = fijando inicio
        Modo 4 - 2 carriles indicando numero de fotos
        Modo 5 - 2 carriles zigzag: carril 1 de inicio a fin -> avance carril 2 -> carril 1 de incio a fin -> avance carril 2 -> .....

      4. Motor principal: carril 1+camara 1 / carril 2 + camara 2

      5. Ajustes Camara 1
        500. ..
        501. Num.Fotos          - Numero de fotos ha hacer: calculadas o fijadas
        502. Fot.Ciclo          - Numero de fotos por ciclo
        503. Tp.Espera          - Tiempo de espera desde que se dispara la camara hasta que se mueve el motor
        
        504. Tp.Estab           - Tiempo desde que termina de moverse el motor hasta que dispara la camara
        505. Tp.Disparo         - Tiempo durante el que se acciona el optoacoplador para disparar la camara
        506. Copiar a Camara 2  - Copia los ajustes actuales a la camara 2
        507. Test Camara 1      - Hace un disparo de la camara 1

        508. Len On             - != 0 Indica el tiempo de espera desde que dispara la camara 1 para activar la salida auxiliar
        509. Led Dur            - Indica el tiempo durante el que se activa la salida auxiliar

      6. Ajustes Camara 2
        600. ..
        601. Num.Fotos          - Numero de fotos ha hacer: calculadas o fijadas
        602. Fot.Ciclo          - Numero de fotos por ciclo
        603. Tp.Espera          - Tiempo de espera desde que se dispara la camara hasta que se mueve el motor
        
        604. Tp.Estab           - Tiempo desde que termina de moverse el motor hasta que dispara la camara
        605. Tp.Disparo         - Tiempo durante el que se acciona el optoacoplador para disparar la camara
        606. Copiar a Camara 1  - Copia los ajustes actuales a la camara 1
        607. Test Camara 2      - Hace un disparo de la camara 2       

        608. Len On             - != 0 Indica el tiempo de espera desde que dispara la camara 2 para activar la salida auxiliar
        609. Led Dur            - Indica el tiempo durante el que se activa la salida auxiliar

      7. Ajustes Carril 1
        700. ..
        701. Ajustar posicion   - Ajustar la posicion del carril 2
        702. Dis.Paso           - Distancia de avance entre foto y foto, en pasos se pasa a micras
        703. Movimiento         - Tipo de movimiento del motor: lento, medio o rapido. TMC2208: 20, 70 o 120 uSegundos / A8825: 100, 200, 300

        704. Precision          - x1, x2, x4, ... x256                
        705. Sentido            - A->B o B->A
        706. TpMotor            - Nº de pasos por vuelta completa del motor (normal 200 o 400)
        707. mm/giro            - Distancia que se desplaza la base del carril en una vuelta completa del motor           

        708. Volver 0           - Indica si se vuelve a su posicion inicial al terminar el ciclo                       
        709. Tipo Mov           - movimiento lineal o rotativo
        710. An.Rotac           - Angulo de rotacion del carril en movimientos rotativos
        711. Irms1 mA           - Intensidad RMS - SOLO comunicacion UART TMC2208

        712. Copiar a Carril 2  - Copia los ajustes actuales al carril 2
        713. Test Carril 1      - Envia una vuelta al carril 1

      8. Ajustes Carril 2
        800. ..
        801. Ajustar posicion   - Ajustar la posicion del carril 1
        802. Dis.Paso           - Distancia de avance entre foto y foto
        803. Movimiento         - Tipo de movimiento del motor: lento, medio o rapido. TMC2208: 20, 70 o 120 uSegundos / A8825: 100, 200, 300
             
        804. Precision          - x1, x2, x4, ... x256                
        805. Sentido            - A->B o B->A
        806. TpMotor            - Nº de pasos por vuelta completa del motor (normal 200 o 400)      
        807. mm/giro            - Distancia que se desplaza la base del carril en una vuelta completa del motor   
        
        808. Volver 0           - Indica si se vuelve a su posicion inicial al terminar el ciclo      
        809. Tipo Mov           - movimiento lineal o rotativo
        810. An.Rotac           - Angulo de rotacion del carril en movimientos rotativos        
        811. Irms2 mA           - Intensidad RMS - SOLO comunicacion UART TMC2208       
         
        812. Copiar a Carril 1  - Copia los ajustes actuales al carril 1
        813. Test Carril 2      - Envia una vuelta al motor 2

      9. Reset

*/



void menuPrincipalX()
{
  unsigned nF;

  switch (modoMenu) {

    case 1: case 2: case 3: case 4:
      ponLinea(0, 0, (char*)" Iniciar ciclo      ");
      ponLinea(0, 1, (char*)" Fijar recorrido    ");     
      sprintf(linea3, " Modo de trabajo %1d  ", modoTrabajo); ponLinea(0, 2, 3);      
      if (modoTrabajo<3) { 
        if (!mainMotor) ponLinea(0, 3, (char*)" Carril 1 + Camara 1"); 
        else ponLinea(0, 3, (char*)" Carril 2 + Camara 2");
      }
      else ponLinea(0, 3, (char*)" Carril 1 + Carril 2");

      break;

    case 5: case 6: case 7: case 8:
      ponLinea(0, 0, (char*)" Ajustes camara 1   ");
      ponLinea(0, 1, (char*)" Ajustes camara 2   ");
      ponLinea(0, 2, (char*)" Ajustes carril 1   ");
      ponLinea(0, 3, (char*)" Ajustes carril 2   ");
      break;

    case 9: case 10: case 11: case 12:
      ponLinea(0, 0, (char*)" Reset              ");
      ponLinea(0, 1, (char*)"                    ");
      ponLinea(0, 2, (char*)"                    ");
      ponLinea(0, 3, (char*)"                    ");
      break;


    case 200: case 201: case 202: case 203:
      ponLinea(0, 0, (char*)" ..                 ");
      ponLinea(0, 1, (char*)" Fijar posicion ZX  ");      
      if (cuantosPasos == 0) {
        if (!unidades) ponLinea(0, 2, (char*)" |--    0,00 u  --|Z");
        else ponLinea(0, 2, (char*)" |--    0,00 mm --|Z");
      }
      else {
        if (!unidades) {
          if (cuantosPasos > 0) sprintf(linea3, " |<%4d.%02d u   #%04d", int(pEnteraT), int(pDecimalT), int(nFotosMD));
          else sprintf(linea3, " %4d.%02d u  >| #%04d", int(pEnteraT), int(pDecimalT), int(nFotosMD));         
        }
        else {
          if (cuantosPasos > 0) sprintf(linea3, " |<%4d.%02d mm  #%04d", int(pEnteraT), int(pDecimalT), int(nFotosMD));
          else sprintf(linea3, " %4d.%02d mm >| #%04d", int(pEnteraT), int(pDecimalT), int(nFotosMD));
        }      
        ponLinea(0, 2, 3);
      }

      if (cuantosPasos2 == 0) {
        if (!unidades2) ponLinea(0, 3, (char*)" |--    0,00 u  --|X");
        else ponLinea(0, 3, (char*)" |--    0,00 mm --|X");       
      }      
      else {
        if (!unidades2) {
          if (cuantosPasos2 > 0) sprintf(linea4, " |<%4d.%02d u   #%04d", int(pEnteraT2), int(pDecimalT2), int(nFotosMD2));
          else sprintf(linea4, " %4d.%02d u  >| #%04d", int(pEnteraT2), int(pDecimalT2), int(nFotosMD2));
        }
        else {
          if (cuantosPasos2 > 0) sprintf(linea4, " |<%4d.%02d mm  #%04d", int(pEnteraT2), int(pDecimalT2), int(nFotosMD2));
          else sprintf(linea4, " %4d.%02d mm >| #%04d", int(pEnteraT2), int(pDecimalT2), int(nFotosMD2));
        }      
        ponLinea(0, 3, 4);
      }    
      break;

    case 204: case 205: case 206: case 207:
      ponLinea(0, 0, (char*)" Reset Carriles     ");
      ponLinea(0, 1, (char*)"                    ");
      ponLinea(0, 2, (char*)"                    ");
      ponLinea(0, 3, (char*)"                    ");
      break;


    case 300: case 301: case 302: case 303:
      ponLinea(0, 0, (char*)" ..                 ");
      ponLinea(0, 1, (char*)" Modo1: 1 carr.dist.");
      ponLinea(0, 2, (char*)" Modo2: 1 car.#fotos");
      ponLinea(0, 3, (char*)" Modo3: 2 carr.dist.");
      break;


    case 304: case 305: case 306: case 307:
      ponLinea(0, 0, (char*)" Modo4: 2 car.#fotos");
      ponLinea(0, 1, (char*)" Modo5: 2 car.zigzag");
      ponLinea(0, 2, (char*)"                    ");
      ponLinea(0, 3, (char*)"                    ");
      break;


    case 500: case 501: case 502: case 503:
      ponLinea(0, 0, (char*)" ..                 ");
      if (modoTrabajo == 2 || modoTrabajo == 4) sprintf(linea2, " Num.Fotos: %03d     ", nFotosMF);
      else sprintf(linea2, " Num.Fotos: %03d     ", nFotosMD);
      ponLinea(0, 1, 2);
      sprintf(linea3, " Fot.Ciclo: %03d     ", fCiclo); ponLinea(0, 2, 3);
      sprintf(linea4, " Tp.Espera: %03d s   ", pausa); ponLinea(0, 3, 4);
      break;

    case 504: case 505: case 506: case 507:
      sprintf(linea1, " Tp.Estabil: %02d s   ", tEstab); ponLinea(0, 0, 1);
      sprintf(linea2, " Tp.Disparo: 0.%02d s ", tFoS); ponLinea(0, 1, 2);
      ponLinea(0, 2, (char*)" Copiar a Camara 2  ");
      ponLinea(0, 3, (char*)" Test Camara 1      ");
      break;

    case 508: case 509: case 510: case 511:
      sprintf(linea1, " Led On : %d.%03d s   ", LedOn/1000, LedOn%1000); ponLinea(0, 0, 1);      
      sprintf(linea2, " Led Dur: %d.%03d s   ", LedDur/1000, LedDur%1000); ponLinea(0, 1, 2);
      ponLinea(0, 2, (char*)"                    ");
      ponLinea(0, 3, (char*)"                    ");                  
      break;

    case 600: case 601: case 602: case 603:
      ponLinea(0, 0, (char*)" ..                 ");
      if (modoTrabajo == 2 || modoTrabajo == 4) sprintf(linea2, " Num.Fotos: %03d     ", nFotosMF2);
      else sprintf(linea2, " Num.Fotos: %03d     ", nFotosMD2);
      ponLinea(0, 1, 2);
      sprintf(linea3, " Fot.Ciclo: %03d     ", fCiclo2); ponLinea(0, 2, 3);
      sprintf(linea4, " Tp.Espera: %03d s   ", pausa2); ponLinea(0, 3, 4);
      break;

    case 604: case 605: case 606: case 607:
      sprintf(linea1, " Tp.Estabil: %02d s   ", tEstab2); ponLinea(0, 0, 1);
      sprintf(linea2, " Tp.Disparo: 0.%02d s ", tFoS2); ponLinea(0, 1, 2);
      ponLinea(0, 2, (char*)" Copiar a Camara 1  ");
      ponLinea(0, 3, (char*)" Test Camara 2      ");      
      break;

    case 608: case 609: case 610: case 611:
      sprintf(linea1, " Led On : %d.%03d s   ", LedOn2/1000, LedOn2%1000); ponLinea(0, 0, 1);      
      sprintf(linea2, " Led Dur: %d.%03d s   ", LedDur2/1000, LedDur2%1000); ponLinea(0, 1, 2);
      ponLinea(0, 2, (char*)"                    ");
      ponLinea(0, 3, (char*)"                    ");                  
      break;

    case 700: case 701: case 702: case 703:
      ponLinea(0, 0, (char*)" ..                 ");
      ponLinea(0, 1, (char*)" Ajustar posicion   ");

      if (!unidades) sprintf(linea3, " Dis.Paso: %3d.%02d u ", int(pEntera), pDecimal);   // Trabajamos con micras: 000,00 u
      else sprintf(linea3, " Dis.Paso: %02d.%02d mm ", int(pEntera), pDecimal);           // Trabajamos con milimetros: 00,00 mm 
      ponLinea(0, 2, 3); 

      if (tipoDelay==0) sprintf(linea4, " Movimiento rapido  ");
      else if (tipoDelay==1) sprintf(linea4, " Movimiento medio   ");
      else if (tipoDelay==2) sprintf(linea4, " Movimiento lento   ");    
      ponLinea(0, 3, 4);
     
      break;

    case 704: case 705: case 706: case 707:
      sprintf(linea1, " Precision x%03d     ", precisionVer[precision]); ponLinea(0, 0, 1);      
      if (!sentido) ponLinea(0, 1, (char*)" Sentido: A --> B   "); else ponLinea(0, 1, (char*)" Sentido: B --> A   ");
      sprintf(linea3, " TpMotor: %04d pasos", pvuelta); ponLinea(0, 2, 3);
      sprintf(linea4, " mm/giro: %02d.%02d mm  ", (lead/2),(lead%2)*50); ponLinea(0, 3, 4);      
      break;

    case 708: case 709: case 710: case 711:   

      if (volver) ponLinea(0, 0, (char*)" Volver 0: Si       "); else ponLinea(0, 0, (char*)" Volver 0: No       ");
      
      if (!tipoMov) sprintf(linea2, " Tipo Mov: Lineal   ");
      else sprintf(linea2, " Tipo Mov: Rotativo ");
      ponLinea(0, 1, 2);

      sprintf(linea3, " An.Rotac: %4u.%1u   ", unsigned(gradosTotalesE), unsigned(gradosTotalesD));
      ponLinea(0, 2, 3);

      sprintf(linea4, " Irms1 mA: %03u (%03u)", driverM1_I, driverM1.rms_current()); 
      ponLinea(0, 3, 4);      

      break;

    case 712: case 713: case 714: case 715:

      ponLinea(0, 0, (char*)" Copiar a Carril 2  ");
      ponLinea(0, 1, (char*)" Test Carril 1      ");
      ponLinea(0, 2, (char*)"                    ");
      ponLinea(0, 3, (char*)"                    ");      
      break;


    case 800: case 801: case 802: case 803:
      ponLinea(0, 0, (char*)" ..                 ");
      ponLinea(0, 1, (char*)" Ajustar posicion   ");
      
      if (!unidades2) sprintf(linea3, " Dis.Paso: %3d.%02d u", int(pEntera2), pDecimal2);  // Trabajamos con micras: 000,00 u
      else sprintf(linea3, " Dis.Paso: %2d.%02d mm ", int(pEntera2), pDecimal2);          // Trabajamos con milimetros: 00,00 mm 
      ponLinea(0, 2, 3);

      if (tipoDelay2==0) sprintf(linea4, " Movimiento rapido  ");
      else if (tipoDelay2==1) sprintf(linea4, " Movimiento medio   ");
      else if (tipoDelay2==2) sprintf(linea4, " Movimiento lento   ");    
      ponLinea(0, 3, 4);

      break;

    case 804: case 805: case 806: case 807:
      sprintf(linea1, " Precision x%03d     ", precisionVer[precision2]); ponLinea(0, 0, 1);
      if (!sentido2) ponLinea(0, 1, (char*)" Sentido: A --> B   "); else ponLinea(0, 1, (char*)" Sentido: B --> A   ");
      sprintf(linea3, " TpMotor: %04d pasos", pvuelta2); ponLinea(0, 2, 3);
      sprintf(linea4, " mm/giro: %02d.%02d mm  ", (lead2/2),(lead2%2)*50); ponLinea(0, 3, 4);
      break;

    case 808: case 809: case 810: case 811:

      if (volver2) ponLinea(0, 0, (char*)" Volver 0: Si       "); else ponLinea(0, 0, (char*)" Volver 0: No       ");
    
      if (!tipoMov2) sprintf(linea2, " Tipo Mov: Lineal   ");
      else sprintf(linea2, " Tipo Mov: Rotativo ");
      ponLinea(0, 1, 2);
    
      sprintf(linea3, " An.Rotac: %4d.%1d   ", unsigned(gradosTotalesE2), unsigned(gradosTotalesD2));
      ponLinea(0, 2, 3);

      sprintf(linea4, " Irms2 mA: %03u (%03u)", driverM2_I, driverM2.rms_current());       
      ponLinea(0, 3, 4);      

      break;

    case 812: case 813: case 814: case 815:
      sprintf(linea1, " Irms2 mA: %03u (%03u)", driverM2_I, driverM2.rms_current());       
      ponLinea(0, 0, (char*)" Copiar a Carril 1  ");
      ponLinea(0, 1, (char*)" Test Carril 2      ");
      ponLinea(0, 2, (char*)"                    ");
      ponLinea(0, 3, (char*)"                    ");      
      break;

  }

  ponCursor();

}



// Esto pone la linea de la posicion de inicio de los carriles
// Se hace aparte del resto para que el movimiento del carril sea fluido en las opciones
// de fijar inicio y ajustar posicion

void ponInicioCarril(byte nCarril)
{

  if (nCarril == 1) {

      if (cuantosPasos == 0) {
        if (!unidades) ponLinea(1, 2, (char*)"|--    0,00 u  --|Z");
        else ponLinea(1, 2, (char*)"|--    0,00 mm --|Z");
      }
      else {
        if (!unidades) {
          if (cuantosPasos > 0) {
            if (pEnteraT < 9999) sprintf(linea3, "|<%4d.%02d u   #%04d", int(pEnteraT), int(pDecimalT), nFotosMD);
            else {
              unidades = 1;
              calculaUnPaso();
              calculaCuantosPasos();
            }
          }
          else {
            if (pEnteraT < 9999) sprintf(linea3, "%4d.%02d u  >| #%04d", int(pEnteraT), int(pDecimalT), nFotosMD);
            else {
              unidades = 1;
              calculaUnPaso();
              calculaCuantosPasos();             
            }
          }
        }
        else {
          if (cuantosPasos > 0) sprintf(linea3, "|<%4d.%02d mm  #%04d", int(pEnteraT), int(pDecimalT), nFotosMD);
          else sprintf(linea3, "%4d.%02d mm >| #%04d", int(pEnteraT), int(pDecimalT), nFotosMD);
        }      
        ponLinea(1, 2, 3);
      }        
        
  }
  else if (nCarril == 2) {

      if (cuantosPasos2 == 0) {
        if (!unidades2) ponLinea(1, 3, (char*)"|--    0,00 u  --|X");
        else ponLinea(1, 3, (char*)"|--    0,00 mm --|X");       
      }      
      else {
        if (!unidades2) {
          if (cuantosPasos2 > 0) {
            if (pEnteraT2 < 9999) sprintf(linea4, "|<%4d.%02d u   #%04d", int(pEnteraT2), int(pDecimalT2), nFotosMD2);
            else {
              unidades2 = 1;
              calculaUnPaso2();
              calculaCuantosPasos2();             
            }
          }
          else {
            if (pEnteraT2 < 9999) sprintf(linea4, "%4d.%02d u  >| #%04d", int(pEnteraT2), int(pDecimalT2), nFotosMD2);
            else {
              unidades2 = 1;
              calculaUnPaso2();
              calculaCuantosPasos2();              
            }
          }
        }
        else {
          if (cuantosPasos2 > 0) sprintf(linea4, "|<%4d.%02d mm  #%04d", int(pEnteraT2), int(pDecimalT2), int(nFotosMD2));
          else sprintf(linea4, "%4d.%02d mm >| #%04d", int(pEnteraT2), int(pDecimalT2), (uint16_t)nFotosMD2);
        }      
        ponLinea(1, 3, 4);
      }

  }

}
