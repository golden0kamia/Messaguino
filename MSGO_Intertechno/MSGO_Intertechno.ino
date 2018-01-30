#include <math.h>
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
int addr = 0;
double adresse = 13709258;
int period = 253;
char repeat = 4;
char unit = 0;
char state[16];
int selec = 0;
String menuSelec[] = {"1:Change adresse",
					  "2:Change period",
					  "3:Change repeat",
					  "4:RESET"};
int key = -1;
NewRemoteTransmitter transmitter(adresse, TX, period, repeat);


//---------- Functions ----------//
void menu();
void sub_menu();
char keys();
double keyInt();
double ctoi();


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
	
	lcd.setCursor(2, 0);
	lcd.print("Intertechno");
	lcd.setCursor(5, 1);
	lcd.print("Remote");
	delay(2000);
	lcd.clear();
}

//---------- Main loop function ----------//
void loop() 
{
	lcd.clear();
	lcd.print("Select Unit");
	lcd.setCursor(11, 1);
	lcd.print("#Menu");
	unit = 0;
	unit = keys();
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
	
	lcd.clear();
	lcd.print(menuSelec[selec]);
	lcd.setCursor(0, 1);
	lcd.print("ok          back");
	key = -1;
	key = keys();
	if(key == 2 && selec > 0)
	{
		selec--;
	}else if(key == 8 && selec < sizeof(menuSelec)-1)
	{
		selec++;
	}else if(key == '*')
		sub-menu(selec);
	{
	}else if(key == '#')
	{
		return;
	}
}

void sub_menu(int select)
{
	lcd.clear();
	switch(select)
	{
		case 0:
			lcd.print("Enter Adresse");
			//EEPROM.write(0, keyInt),
			break;
		case 1:
			lcd.print("Enter period");
			//EEPROM.write(1, keyInt),
			break;
		case 2:
			lcd.print("Enter repeat");
			//EEPROM.write(2, keyInt),
			break;
		case 3:
			//EEPROM.clear();
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		default:
			break;
	}
}

double keyInt()
{
	lcd.setCursor(0, 1);
	char inv[16];
	int invs = 0;
	key = -1;
	key = keys();
	do
	{
		switch(key)
		{
			case 0:
				inv[invs] = 0;
				lcd.print("0");
				invs++;
				break;
			case 1:
				inv[invs] = 1;
				lcd.print("1");
				invs++;
				break;
			case 2:
				inv[invs] = 2;
				lcd.print("2");
				invs++;
				break;
			case 3:
				inv[invs] = 3;
				lcd.print("3");
				invs++;
				break;
			case 4:
				inv[invs] = 4;
				lcd.print("4");
				invs++;
				break;
			case 5:
				inv[invs] = 5;
				lcd.print("5");
				invs++;
				break;
			case 6:
				inv[invs] = 6;
				lcd.print("6");
				invs++;
				break;
			case 7:
				inv[invs] = 7;
				lcd.print("7");
				invs++;
				break;
			case 8:
				inv[invs] = 8;
				lcd.print("8");
				invs++;
				break;
			case 9:
				inv[invs] = 9;
				lcd.print("9");
				invs++;
				break;
			case '#':
				if(!invs)
				{}else
				{
					invs--;
					inv[invs] = 0;
					lcd.setCursor(invs, 1);
					lcd.write(0);
					lcd.setCursor(invs, 1);
				}
				break;
			default:
				break;
		}
	}while(key != '*');
	return ctoi(inv);
}
	

//---------- Get key pressed function ----------//
char keys()
{
	char out = -1;
	do
	{
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


double ctoi(char inchar[])
{
	double output = 0;
	int length = sizeof(inchar);
	for(i = 0; i<length; i++)
	{
		switch(inchar[i])
		{
			case 1 || '1':
				output = output + 1*(pow(10, i));
				break;
			case 2 || '2':
				output = output + 2*(pow(10, i));
				break;
			case 3 || '3':
				output = output + 3*(pow(10, i));
				break;
			case 4 || '4':
				output = output + 4*(pow(10, i));
				break;
			case 5 || '5':
				output = output + 5*(pow(10, i));
				break;
			case 6 || '6':
				output = output + 6*(pow(10, i));
				break;
			case 7 || '7':
				output = output + 7*(pow(10, i));
				break;
			case 8 || '8':
				output = output + 8*(pow(10, i));
				break;
			case 9 || '9':
				output = output + 9*(pow(10, i));
				break;
			case 0 || '0':
				output = output + 0*(pow(10, i));
				break;
			default:
				break;
		}
	}
	return output;
}
