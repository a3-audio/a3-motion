// Input/Output declarations
#define POT_A_INPUT A0           //Analog input 1
#define POT_B_INPUT A1           //Analog input 2

// Constant value definitions

#define ADC_HYSTERESIS  8        //Must be 1 or higher. Noise filter, determines how big ADC change needed
#define MAX_POT_VALUE  127
#define POT_SENSITIVITY 8        //Higher number = more turns needed to reach max value

#define ADC_MAX_VALUE 1023

// Variables for potmeter
int ValuePotA = 0;            //Pot1 tap A value
int ValuePotB = 0;            //Pot1 tap B value
int PreviousValuePotA = 0;    //Used to remember last value to determine turn direction
int PreviousValuePotB = 0;    //Used to remember last value to determine turn direction
int DirPotA = 1;              //Direction for Pot 1 tap A
int DirPotB = 1;              //Direction for Pot1 tap B

int Direction  = 1;              //Final CALCULATED direction
int Value = 0;              //Final CALCULATED value

void setup() {
  Serial.begin(115200);
  PreviousValuePotA  = analogRead(POT_A_INPUT);   // Initialize Pot1 tap A value
  PreviousValuePotB  = analogRead(POT_B_INPUT);  // Initialize Pot1 tap B value
}

void loop()
{
  // Update ADC readings
  ValuePotA  = analogRead(POT_A_INPUT);
  ValuePotB  = analogRead(POT_B_INPUT);

  delay(50);
  /****************************************************************************
  * Step 1 decode each  individual pot tap's direction
  ****************************************************************************/
  // First check direction for Tap A
  if (ValuePotA > (PreviousValuePotA + ADC_HYSTERESIS))       // check if new reading is higher (by <debounce value>), if so...
  {
    DirPotA = 1;                                              // ...direction of tap A is up
  }
  else if (ValuePotA < (PreviousValuePotA - ADC_HYSTERESIS))  // check if new reading is lower (by <debounce value>), if so...
  {
    DirPotA = -1;                                             // ...direction of tap A is down
  }
  else
  {
    DirPotA = 0;                                              // No change
  }
  // then check direction for tap B
  if (ValuePotB > (PreviousValuePotB + ADC_HYSTERESIS))       // check if new reading is higher (by <debounce value>), if so...
  {
    DirPotB = 1;                                              // ...direction of tap B is up
  }
  else if (ValuePotB < (PreviousValuePotB - ADC_HYSTERESIS))  // check if new reading is lower (by <debounce value>), if so...
  {
    DirPotB = -1;                                             // ...direction of tap B is down
  }
  else
  {
    DirPotB = 0;                                              // No change
  }

  /****************************************************************************
  * Step 2: Determine actual direction of ENCODER based on each individual
  * potentiometer tap´s direction and the phase
  ****************************************************************************/
  if (DirPotA == -1 and DirPotB == -1)       //If direction of both taps is down
  {
    if (ValuePotA > ValuePotB)               // If value A above value B...
    {
      Direction = 1;                         // ...direction is up
    }
    else
    {
      Direction = -1;                        // otherwise direction is down
    }
  }
  else if (DirPotA == 1 and DirPotB == 1)    //If direction of both taps is up
  {
    if (ValuePotA < ValuePotB)               // If value A below value B...
    {
      Direction = 1;                         // ...direction is up
    }
    else
    {
      Direction = -1;                        // otherwise direction is down
    }
  }
  else if (DirPotA == 1 and DirPotB == -1)   // If A is up and B is down
  {
    if ( (ValuePotA > (ADC_MAX_VALUE/2)) || (ValuePotB > (ADC_MAX_VALUE/2)) )  //If either pot at upper range A/B = up/down means direction is up
    {
      Direction = 1;
    }
    else                                     //otherwise if both pots at lower range A/B = up/down means direction is down
    {
      Direction = -1;
    }
  }
  else if (DirPotA == -1 and DirPotB == 1)
  {
    if ( (ValuePotA < (ADC_MAX_VALUE/2)) || (ValuePotB < (ADC_MAX_VALUE/2)))   //If either pot  at lower range, A/B = down/up means direction is down
    {
      Direction = 1;
    }
    else                                     //otherwise if bnoth pots at higher range A/B = up/down means direction is down
    {
      Direction = -1;
    }
  }
  else
  {
    Direction = 0;                           // if any of tap A or B has status unchanged (0), indicate unchanged
  }

  /****************************************************************************
  * Step 3: Calculate value based on direction, how big change in ADC value,
  * and sensitivity. Avoid values around zero and max  as value has flat region
  ****************************************************************************/
  if (DirPotA != 0 && DirPotB != 0)         //If both taps indicate movement
  {
    if ((ValuePotA < ADC_MAX_VALUE*0.8) && (ValuePotA > ADC_MAX_VALUE*0.2))         // if tap A is not at endpoints
    {
      Value = Value + Direction*abs(ValuePotA - PreviousValuePotA)/POT_SENSITIVITY; //increment value
    }
    else                                    // If tap A is close to end points, use tap B to calculate value
    {
      Value = Value + Direction*abs(ValuePotB - PreviousValuePotB)/POT_SENSITIVITY;  //Make sure to add/subtract at least 1, and then additionally the jump in voltage
    }
    // Finally apply output value limit control
    if (Value <= 0)
    {
      Value = 0;
    }
    if (Value >= MAX_POT_VALUE)
    {
      Value = MAX_POT_VALUE;
    }                                           // Update prev value storage
    PreviousValuePotA = ValuePotA;          // Update previous value variable
    PreviousValuePotB = ValuePotB;          // Update previous value variable
  }
}
