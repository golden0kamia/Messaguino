#include <LiquidCrystal.h>
#include <NewRemoteReceiver.h>
#include <NewRemoteTransmitter.h>


//---------- Global variable ----------// DO NOT CHANGE
LiquidCrystal lcd(0, 1, 2, 3, 4, 5, 6);

const int MY1 = A0;	//Initialise les pin d'entrées
const int MY2 = A1;	//
const int MY3 = A2;	//
const int MY4 = A3;	//

const int MX1 = 10;	//Initialise les pins de sorties
const int MX2 = 11;	//
const int MX3 = 12;	//
const int RET = 7;	//
const int RX  = 8;	//
const int TX  = 13;	//

//---------- Other global variable ----------//
int i = 0;
unsigned long int adresse = 13709258;
int period = 253;
char unit = 0;
char repeat = 4;
char state[16];
String menuSelec[] = {"1:Change adresse",
					"2:Change period",
					"3:Change repeat"};
NewRemoteTransmitter transmitter(adresse, TX, period, repeat);


//---------- Functions ----------//
void menu();
char key();


//---------- Setup function ----------//
void setup()
{
	pinMode(MX1, OUTPUT);		//Mets la pin MX1 en OUTPUT
	pinMode(MX2, OUTPUT);		//Mets la pin MX2 en OUTPUT
	pinMode(MX3, OUTPUT); 		//Mets la pin MX3 en OUTPUT
	pinMode(RET, OUTPUT);		//Mets la pin RET en OUTPUT
	digitalWrite(MX1, LOW);
	digitalWrite(MX2, LOW);
	digitalWrite(MX3, LOW);
	digitalWrite(RET, HIGH);	//Active RET
	lcd.begin(16,2);			//Initialise le LCD
 Serial.begin(9600);
 for(i = 0; i < 16; i++)
 {
  state[i] = 0;
 }
	
	lcd.setCursor(3, 0);
	lcd.print("Intertechno");
	lcd.setCursor(6, 1);
	lcd.print("Remote");
	delay(2000);
	lcd.clear();
}

//---------- Main loop function ----------//
void loop() 
{
	lcd.print("Select Unit");
	lcd.setCursor(12, 1);
	lcd.print("Menu");
  unit = key();
  Serial.println(unit);
	if(unit == '*')
	{
		menu();
	}
	else if(unit == '#');
	{}
  state[unit] = !state[unit];
	transmitter.sendUnit(unit, state[unit]);
}

//---------- Main menu of the systeme ----------//
void menu()
{
	//Menu here
}

//---------- Get key pressed function ----------//
char key()
{
	char out = -1;
	do
	{
  Serial.println("wait key");
		digitalWrite(MX1, HIGH);
			if(digitalRead(MY1))
			{
				out = 1;
			}else if(digitalRead(MY2))
			{
				out = 4;
			}else if(digitalRead(MY3))
			{
				out = 7;
			}else if(digitalRead(MY4))
			{
				out = '*';
			}
		digitalWrite(MX1, LOW);
		digitalWrite(MX2, HIGH);
			if(digitalRead(MY1))
			{
				out = 2;
			}else if(digitalRead(MY2))
			{
				out = 5;
			}else if(digitalRead(MY3))
			{
				out = 8;
			}else if(digitalRead(MY4))
			{
				out = 0;
			}
		digitalWrite(MX2, LOW);
		digitalWrite(MX3, HIGH);
			if(digitalRead(MY1))
			{
				out = 3;
			}else if(digitalRead(MY2))
			{
				out = 6;
			}else if(digitalRead(MY3))
			{
				out = 9;
			}else if(digitalRead(MY4))
			{
				out = '#';
			}
		digitalWrite(MX3, LOW);
	}while(out == -1);
	delay(500);
	return out;
}
