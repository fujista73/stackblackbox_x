void tmc2208_init_M1()
{
  uint32_t data;

  driverM1.beginSerial(38400);  
  driverM1.push();

  digitalWrite(EN1, true);          // Desactivo el driver hardware

  driverM1.pdn_disable(true);       // Use UART  
  driverM1.mstep_reg_select(true);  // Select microsteps with UART  
  driverM1.I_scale_analog(false);
  driverM1.rms_current(driverM1_I); //, HOLD_MULTIPLIER, R_SENSE);
  driverM1.microsteps(256);
  driverM1.blank_time(24);
  driverM1.toff(3);                 // stealthChop
  driverM1.tbl(2);
  driverM1.intpol(true);
  driverM1.TPOWERDOWN(128);         // ~2s until driver lowers to hold current
  driverM1.hysteresis_start(3);
  driverM1.hysteresis_end(5);
  driverM1.pwm_lim(12);
  driverM1.pwm_reg(8);
  driverM1.pwm_autograd(1);
  driverM1.pwm_autoscale(1);
  driverM1.pwm_freq(1);
  driverM1.pwm_grad(14);
  driverM1.pwm_ofs(36);
  driverM1.en_spreadCycle(false);
  driverM1.GSTAT(0b111); // Clear  

  delay(200);

  digitalWrite(EN1, false); // Activo el driver

  if (DEBUG) {
    Serial.print("DRV1_STATUS = 0x");
    driverM1.DRV_STATUS(&data);
    Serial.print(data, HEX);

    Serial.print("; DRV1_I_RMS = ");
    Serial.print(driverM1.rms_current());

    Serial.print("; DRV1 microSteps = ");
    Serial.println(driverM1.microsteps());
  }
  
}


void tmc2208_init_M2()
{
  uint32_t data;  

  driverM2.beginSerial(38400);  
  driverM2.push();

  digitalWrite(EN2, true);          // Desactivo el driver hardware
   
  driverM2.pdn_disable(true);       // Use UART
  driverM2.mstep_reg_select(true);  // Select microsteps with UART
  driverM2.I_scale_analog(false);
  driverM2.rms_current(driverM2_I); //, HOLD_MULTIPLIER, R_SENSE);
  driverM2.microsteps(256);
  driverM2.blank_time(24);
  driverM2.toff(3);
  driverM1.tbl(2);  
  driverM2.intpol(true);
  driverM2.TPOWERDOWN(128);         // ~2s until driver lowers to hold current
  driverM2.hysteresis_start(3);
  driverM2.hysteresis_end(5);
  driverM2.pwm_lim(12);
  driverM2.pwm_reg(8);
  driverM2.pwm_autograd(1);
  driverM2.pwm_autoscale(1);
  driverM2.pwm_freq(1);
  driverM2.pwm_grad(14);
  driverM2.pwm_ofs(36);
  driverM2.en_spreadCycle(false);
  driverM2.GSTAT(0b111); // Clear  
  
  delay(200);

  digitalWrite(EN2, false);         // Activo el driver

  if (DEBUG) {  
    Serial.print("DRV2_STATUS = 0x");
    driverM2.DRV_STATUS(&data);
    Serial.print(data, HEX);

    Serial.print("; DRV2_I_RMS = ");
    Serial.print(driverM2.rms_current());

    Serial.print("; DRV2 microSteps = ");
    Serial.println(driverM2.microsteps());

  }
  
}
