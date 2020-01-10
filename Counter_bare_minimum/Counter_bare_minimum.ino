/*
 * 10.1.20, peter  p@qup.at
 * 
 * This is a test for the counter of the Arduino DUE
 * 
 * Capture Pins are 11(TIOA) and 12(TIOB) on the Arduino Due 
 * Copied as much as possible from here: https://forum.arduino.cc/index.php?topic=400516.0
 */

volatile uint32_t CaptureCountA;
volatile boolean CaptureFlag;

//Connect Pins 7, 11, 12 to each other to test with the rising and falling edge of the PWM
#define ANALOG_PIN 7
#define ANALOG_VALUE 123 // values in the interval [0,255] 


void setup() {
  SerialUSB.begin(115200);

  pinMode( ANALOG_PIN, OUTPUT); //connect to AN-7 for Testing
  analogWrite(ANALOG_PIN,ANALOG_VALUE);

  setupCounter(); //call the function below which does the Register magic
}

void loop() {
  if (CaptureFlag) {
    CaptureFlag = 0;
    SerialUSB.print("Capture. Capture Register A: " + String(CaptureCountA));
    SerialUSB.println(". That's: " + String((float)CaptureCountA*23.8095238/1000) + "µs."); //MCK=84MHz, TC_CMR_TCCLKS_TIMER_CLOCK1==42MHZ^=23,8095ns per countertick
  }
}


//This Handler gets called when the interrupt (Rising edge TIOB) happens.
void TC8_Handler() {
  uint32_t status = TC2->TC_CHANNEL[2].TC_SR;       // Read & Save satus register -->Clear status register
  if ((status & TC_SR_LDRAS) == TC_SR_LDRAS) {  // Capture Register A has been loaded
    CaptureCountA = TC2->TC_CHANNEL[2].TC_RA;        // get data from capture register A for TC2 channel 2 (Counter 8)
  }
  CaptureFlag = 1; // set flag indicating a new capture value is present
}

void setupCounter(){
  PMC->PMC_PCER1 |= PMC_PCER1_PID35;                      // Timer Counter Channel 8 is TC8 (Table 9-1. Peripheral Identifiers)
  TC2->TC_CHANNEL[2].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1   // capture mode, MCK/2, clk on rising edge                           
                              | TC_CMR_ETRGEDG_RISING      // The clock is gated by an rising edge of TIOB (TIOB default).
                              | TC_CMR_LDRA_FALLING;   // load RA (==save value) on falling edge of TIOA
                                    

  TC2->TC_CHANNEL[2].TC_IER |= TC_IER_LDRAS;               // Trigger interruption on Load RA 
  TC2->TC_CHANNEL[2].TC_CCR = TC_CCR_CLKEN ;             // Clock  enable and software trigger

  NVIC_ClearPendingIRQ(TC8_IRQn); //Clear Interrupts
  NVIC_SetPriority(TC8_IRQn, 0);  //Set the Interrupt Priority, lower values indicate a higher priority [0-255]
  NVIC_EnableIRQ(TC8_IRQn);         // Enable TC8 interrupts
}
