/*
COFFEE MAKER - netbrew
PROTOTYPE VERSION 1.2A

Qalisar Tech Research - 04 June 2017
DANIEL Z. DIAS DE MORAES
update 30 Jun 2017
Last update 17 Aug 2017 - Refactoring Code - Added functions


*/

//////Variables
byte modeSel;
byte fade;
byte lastb1;
byte msg;
byte wmsg;
byte errorchk;

//////Store readings
byte serialRCV;
byte wsensRCV;
byte lsensRCV;
byte jsensRCV;
byte btn1READ;

//////LED Pins
const byte stdbyLed = 10;
const byte rdyLed = 11;
const byte faultLed = 9;


//////Actuators pins
const byte Relay = 2;

//////Sensors Pins
const byte Lsens = 7;
const byte Wsens = 8;
const byte Jsens = 4;
const byte btn1 = 12;


/////////////////////-------- SET-UP AND P.O.S.T. --------/////////////////////
void setup() {
	
// Serial Comms
Serial.begin(9600);

// SERIAL SPLASH SCREEN //////////////////////////////////////
Serial.println("QALISAR TECH RESEARCH 2017");
Serial.println("FIRMWARE NETBREW Ver 1.2A \n");
Serial.println("Executing P.O.S.T.");

//////////////////////////////////////////////////////////////

//////LEDs
// Serial Announce
Serial.println("I - LED Setup and Test");
//Stand by - LED
pinMode (stdbyLed, OUTPUT);
GpioOutTest(stdbyLed);
//Ready - LED
pinMode (rdyLed, OUTPUT);
GpioOutTest(rdyLed);
//Fault - LED
pinMode (faultLed, OUTPUT);
GpioOutTest(faultLed);

//////Actuator
Serial.println("I - RELAY Setup");
pinMode (Relay, OUTPUT);
GpioOutTest(Relay);

//////Sensors
Serial.println("I - SENSORS Setup");
pinMode (Wsens, INPUT);
Serial.println(" - Water Level Sensor.");
GpioInTest(Wsens);
pinMode (Lsens, INPUT);
Serial.println(" - Lid State Sensor.");
GpioInTest(Lsens);
pinMode (Jsens, INPUT);
Serial.println(" - Jar State Sensor");
GpioInTest(Jsens);


//////Variables
Serial.println("I - VARIABLES Setup");
lastb1 = LOW;
modeSel = 0;
msg = 0;
errorchk = 0;

//////End Set-Up
Serial.println(" - Setup FINISHED \n");
Serial.println(" - NETBREW HW READY");
}

/////////////////////-------- LOOP --------/////////////////////
void loop() {
  
	//---Read all sensors
	ReadSense();

	//---Set zero for all LEDs
	LedsOff();

	//---Button reading w\fast toggle prevention
	ReadButton();

	/////////////////////-------- OPERATING CYCLE
	//NOTE: CHECK FOR ERRORS, MAYBE CONVERTED 

	//CHECK SENSORS IF OK
	if ((lsensRCV == HIGH)&&(jsensRCV == HIGH))
	{
		//Error Check Message Reset
		errorchk = 0;
		
		//---Mode toggle---
		if (btn1READ == HIGH){
		  modeSel = modeSel + 1;
		  if (modeSel > 1){
			modeSel = 0;
			}
		  msg = 0;
		  Serial.print("OPERATION MODE: ");
		  if (modeSel == 1){
			Serial.println("COIL ON - HEATING/BREWING");
			}
		  if (modeSel == 0){
			Serial.println("COIL OFF - IDLE / STD BY");
			}
		  }
		//---StandBy and Ready to Brew State---
		if ((wsensRCV == LOW)&&(modeSel == 0)){
		  if(wsensRCV == LOW){
			Serial.println("WATER LVL - OK");
			}
		  if(msg == 0){
			Serial.println("STD BY - Ready to Brew!");
			msg = 1;
			}
		  
		  digitalWrite(Relay, LOW);
		  digitalWrite(stdbyLed, HIGH);
		  digitalWrite(rdyLed, HIGH);
		  }
		//---StandBy No Water---
		if ((wsensRCV == HIGH)&&(modeSel == 0)){
		  if(wsensRCV == HIGH){
			Serial.println("WATER LVL - EMPTY/LOW");
			}
		  if(msg == 0){
			Serial.println("STD BY - Heating Only AVLB - Check for WATER");
			msg = 1;
			}
		  digitalWrite(Relay, LOW);
		  digitalWrite(stdbyLed, HIGH);
		  digitalWrite(rdyLed, LOW);
		  }
		
		//---Brewer Mode---
		 if ((wsensRCV == LOW)&&(modeSel == 1)){
		  if(wsensRCV == LOW){
			Serial.println("WATER LVL - OK For brewing");
			}
		  if(msg == 0){
			Serial.println("BREWING - Hold On");
			msg = 1;
			}
		  digitalWrite(Relay, HIGH);
		  digitalWrite(stdbyLed, LOW);
		  
		  //**** FADE IN FADE OUT
		  LedFade(rdyLed,127,1);
		  
		  }
		
		//---Heating/Keep Warm Mode---
		 if ((wsensRCV == HIGH)&&(modeSel == 1)){
		  if(wsensRCV == HIGH){
			Serial.println("WATER LVL - Low/Empty");
			}
		  if(msg == 0){
			Serial.println("Heating Only - Check for WATER");
			msg = 1;
			}
		  digitalWrite(Relay, HIGH);
		  digitalWrite(stdbyLed, LOW);
		  
		  //**** FADE IN FADE OUT
		  LedFade(rdyLed,255,3);
		  }
		delay(100);
	}
	
	//ELSE SENSOR DETECT FAIL/ERROR   
	else
	{
		//If error occured - message will show once per occurence,
		//if it get reseted on normal mode, it can be shown again.
		ErrorMode();
			  
		//**** FADE IN FADE OUT
		LedFade(faultLed,255,3);
	}
}

//GPIO Digital Output Test (ON delay OFF)
int GpioOutTest(byte pinOUT)
{
	digitalWrite(pinOUT, HIGH);
	delay(50);
	digitalWrite(pinOUT, LOW);
}

//GPIO Digital Input Test
int GpioInTest(byte pinIN)
{
	Serial.println("Sensor read:" + pinIN);
}

//Turn Off Digital Pin
int LedsOff()
{
	digitalWrite(faultLed, LOW);
	digitalWrite(rdyLed, LOW);
	digitalWrite(stdbyLed, LOW);
}

//Read All Sensors
int ReadSense()
{
	//---Water sensor
	wsensRCV = digitalRead(Wsens);
	//---Lid sensor
	lsensRCV = digitalRead(Lsens);
	//---Jar sensor
	jsensRCV = digitalRead(Jsens);
}

//Read Button
int ReadButton()
{
		btn1READ = LOW;
	if (digitalRead(btn1) == HIGH){
	  if (lastb1 == LOW){
		  btn1READ = HIGH;
		  lastb1 = HIGH;
		}
	  }
	if (digitalRead(btn1) == LOW){
	  lastb1 = LOW;
	  }
}

//Fade-In Fade-Out
int LedFade(byte pinX, byte fadeVal, byte fadeSpeed)
{
	for (fade = 0; fade < fadeVal; fade ++)
		{
		analogWrite(pinX, fade);
		delay(fadeSpeed);
		}
		 
	for (fade = fadeVal; fade > 0; fade --)
		{
		analogWrite(pinX, fade);
		delay(fadeSpeed);
		}
}

//Error Mode and Serial Message
int ErrorMode()
{
	if(errorchk == 0)
	{
		Serial.println("FAULT - SYSTEM DISABLED");
		Serial.println("CHECK JAR OR OPEN LID.");
		errorchk = 1;
		msg = 0;
	}
	digitalWrite(Relay, LOW);
}