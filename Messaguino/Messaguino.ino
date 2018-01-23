#include <LiquidCrystal.h>
#include <VirtualWire.h>

LiquidCrystal lcd(0, 1, 2, 3, 4, 5, 6);

byte BOK[8]{    //Crée le charatère OK
  B00000,
  B00000,
  B00000,
  B00001,
  B00010,
  B10100,
  B01000,
  B00000
};
byte BEL[8]{    //Crée le caratère BEL
  B00000,
  B00000,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00000
};
byte BER[8]{    //Crée le caratère BER
  B00000,
  B00001,
  B00011,
  B00011,
  B00100,
  B01000,
  B11111,
  B00000
};
byte BEC[8]{    //Crée le charatère BEC
  B00000,
  B11111,
  B01110,
  B10101,
  B11011,
  B11111,
  B11111,
  B00000
};
const int MY1 = A0;   //Initialise les pin d'entrées
const int MY2 = A1;   //
const int MY3 = A2;   //
const int MY4 = A3;   //

const int MX1 = 10;    //Initialise les pins de sorties
const int MX2 = 11;    //
const int MX3 = 12;    //
const int RET = 7;   //
const int RX  = 8;   //
const int TX  = 13;   //

const int DEL = 0;    //Initialise les valeurs
const int OK = 1;     //
const int ER = 2;     //
const int REC = 3;    //

int VAL = 0;          //
int POS[] = {0, 0};   //
char LET;             //
int POSMOT = 0;       //
char MOT[20] = {};    //
char MOTREC[40][30] = {}; //
unsigned int SET = 0; //
int SLOT = 0;         //
int SLOTW = 1;        //
int i = 0;            //
int j = 0;            //
int WR = 0;           //
bool WRITE = 0;       //
bool READ = 0;        //
byte RECU = 0;        //
char CLI = REC;       //
bool CLIGN = 0;       //

bool RECIEVE();       //
void SEND(char MOTSEND[20]);

void setup()
{
  pinMode(MX1, OUTPUT);     //Mets la pin MX1 en OUTPUT
  pinMode(MX2, OUTPUT);     //Mets la pin MX2 en OUTPUT
  pinMode(MX3, OUTPUT);     //Mets la pin MX3 en OUTPUT
  pinMode(RET, OUTPUT);     //Mets la pin RET en OUTPUT
  digitalWrite(RET, HIGH);  //Active RET
  vw_set_tx_pin(TX);        //Initialise la comunication 433Mhz
  vw_set_rx_pin(RX);        //
  vw_set_ptt_pin(9);        //
  vw_set_ptt_inverted(true);//
  vw_rx_start();            //
  vw_setup(4800);           //Vitesse de transmission 4.8kbps
  for(i = 0; i < 40; i++)
  {
    for(j = 0; j < 30; j++)
    {
      MOTREC[i][j] = ' ';        //Mets MOTREC à ' '
    }
  }
  lcd.begin(16,2);          //Initialise le LCD
  lcd.createChar(OK, BOK);  //Crée le charatère OK
  lcd.createChar(DEL, BEL); //Crée le charatère DEL
  lcd.createChar(ER, BER);  //Crée le charatère ER
  lcd.createChar(REC, BEC); //Crée le charatère REC
}

///////////////////////////////////////////////////////////////LOOP///////////////////////////////////////////////////////////////
void loop() 
{
  lcd.setCursor(3, 0);      //Affiche le menu du système
  lcd.write("Messaguino");  //
  lcd.setCursor(0, 1);      //
  lcd.write(ER);            //
  lcd.setCursor(6, 1);      //
  lcd.write("V1.0");        //
  lcd.setCursor(15, 1);     //
  lcd.write(CLI);           //
  
  digitalWrite(MX1, HIGH);        //Active MX1
  while(digitalRead(MY4) == HIGH) //Lis la valeur de MY4
  {
    WRITE = 1;                      //Mets WRITE à 1
    SET = 0;                        //Mets SET à 0
    lcd.clear();                    //Efface le LCD
    delay(10);                      //Attends 10 ms
  }
  digitalWrite(MX1, LOW);         //Désactive MX1
  digitalWrite(MX3, HIGH);        //Active MX2
  while(digitalRead(MY4) == HIGH) //Lis la valeur de MY4
  {
    READ = 1;                       //Mets READ à 1
    SET = 0;                        //Mets SET à 0
    lcd.clear();                    //Efface le LCD
    delay(10);                      //Attends 10 ms
  }
  digitalWrite(MX3, LOW);         //Désactive MX3
  delay(1);                       //Attends 1ms
  SET++;                          //Ajoute 1 à SET
  if(SET % 60 == 0 && RECU == 1)
  {
    if(CLIGN == 0)
    {
      CLI = ' ';                      //Mets CLI à ' '
      CLIGN = 1;                      //Mets CLIGN à 1
    }
    else
    {
      CLI = REC;                      //Mets CLI à REC
      CLIGN = 0;                      //Mets CLIGN à 0
    }
  }
  //////////////////////////////////////////////////////////SLEEP//////////////////////////////////////////////////////////
  else if(SET >= 1136)            //Test SET (L'écran lcd s'étend au bout de 10 seconde)
  {
    lcd.setCursor(0, 0);            //Met les curseur à 0, 0
    lcd.write('S');                 //Ecrit le charactère 'S'
    while(digitalRead(MY4) == LOW)  //Lis la valeur de MY4
    {
      digitalWrite(RET, LOW);         //Désactive RET (désactive le rétroéclairage)
      digitalWrite(MX3, HIGH);        //Active MX3
      SET = 0;                        //Remets SET à 0
      if(RECIEVE() == 1)
      {
        RECU = 1;                       //Mets RECU à 1
      }
    }
    while(digitalRead(MY4) == HIGH) //Lis la valeur de MY4
    {
      digitalWrite(RET, HIGH);        //Active RET (active le rétroéclairage)
      lcd.setCursor(0, 0);            //Mets le courseur à 0, 0
      lcd.write(' ');                 //Ecrit le charactère ' '
    }
  }
  if(RECIEVE() == 1)
  {
    RECU = 1;                       //METs RECU à 1
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////Ecriture///////////////////////////////////
  while(WRITE == 1){             //Test WRITE

  /////////////////////////////////////////////////////////////////////////////////////////////MX1///////////////////////////////////
    digitalWrite(MX1, HIGH);          //Active MX1
    
    /////////////////////////////////////////////////////////////////////////////////////////MX1/MY1////////////////////////////////////
    if(digitalRead(MY1) == HIGH)      //Lis la valeur de MY1
    {
      if(POS[0] == 1 && POS[1] == 1)    //Test la position du clavier
      {
        SET = 0;                          //Remets SET à 0
        if(VAL == 1)                      //Test la valeur de la touche
        {
          LET = 'B';                        //Mets LET à 'B'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 2;                          //Mets la valeur de la touche à 2
          while(digitalRead(MY1) == HIGH)   //Lis la valeur de MY1
          {
            POS[0] = 1;                       //Mets la position du clavier sur 1, 1
            POS[1] = 1;                       //
          }
        }
        else if(VAL == 2)                 //Test la valeur de la touche
        {
          LET = 'C';                        //Mets LET à 'C'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 3;                          //Mets la valeur de la touche à 3
          while(digitalRead(MY1) == HIGH)   //Lis la valeur de MY1
          {
            POS[0] = 1;                       //Mets la position du clavier sur 1, 1
            POS[1] = 1;                       //
          }
        }
        else if(VAL == 3)                 //Test la valeur de la touche
        {
          LET = '1';                        //Mets LET à '1'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 4;                          //Mets la valeur de la touche à 4
          while(digitalRead(MY1) == HIGH)   //Lis la valeur de MY1
          {
            POS[0] = 1;                       //Mets la position du clavier sur 1, 1
            POS[1] = 1;                       //
          }
        }
        else
        {
          LET = 'A';                        //Mets LET à 'A'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 1;                          //Mets la valeur de la touche à 1
          while(digitalRead(MY1) == HIGH)   //Lis la valeur de MY1
          {
            POS[0] = 1;                       //Mets la position du clavier sur 1, 1
            POS[1] = 1;                       //
          }
        }
      }
      else
      {
        SET = 0;                          //Remets SET à 0
        if(VAL != 0)                      //Test la valeur de VAL
        {
          POSMOT ++;                      //Ajoute 1 à POSMOT
        }
        LET = 'A';                        //Mets LET à 'A'
        MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
        VAL = 1;                          //Mets la valeur de la touche à 1
        while(digitalRead(MY1) == HIGH)   //Lis la valeur de MY1
        {
          POS[0] = 1;                       //Mets la position du clavier sur 1, 1
          POS[1] = 1;                       //
        }
      }
    }
    /////////////////////////////////////////////////////////////////////////////////////////MX1/MY2////////////////////////////////////
    else if(digitalRead(MY2) == HIGH) //Lis la valeur de MY2
    {
      if(POS[0] == 1 && POS[1] == 2)    //Test la position du clavier
      {
        SET = 0;                          //Remets SET à 0
        if(VAL == 1)                      //Test la valeur de la touche
        {
          LET = 'K';                        //Mets LET à 'K'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 2;                          //Mets la valeur de la touche à 2
          while(digitalRead(MY2) == HIGH)   //Lis la valeur de MY2
          {
            POS[0] = 1;                       //Mets la position du clavier sur 1, 2
            POS[1] = 2;                       //
          }
        }
        else if(VAL == 2)                 //Test la valeur de la touche
        {
          LET = 'L';                        //Mets LET à 'L'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 3;                          //Mets la valeur de la touche à 3
          while(digitalRead(MY2) == HIGH)   //Lis la valeur de MY2
          {
            POS[0] = 1;                       //Mets la position du clavier sur 1, 2
            POS[1] = 2;                       //
          }
        }
        else if(VAL == 3)                 //Test la valeur de la touche
        {
          LET = '4';                        //Mets LET à '4'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 4;                          //Mets la valeur de la touche à 4
          while(digitalRead(MY2) == HIGH)   //Lis la valeur de MY2
          {
            POS[0] = 1;                       //Mets la position du clavier sur 1, 2
            POS[1] = 2;                       //
          }
        }
        else
        {
          LET = 'J';                        //Mets LET à 'J'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 1;                          //Mets la valeur de la touche à 1
          while(digitalRead(MY2) == HIGH)   //Lis la valeur de MY2
          {
            POS[0] = 1;                       //Mets la position du clavier sur 1, 2
            POS[1] = 2;                       //
          }
        }
      }
      else
      {
        SET = 0;                          //Remets SET à 0
        if(VAL != 0)                      //Test la valeur de VAL
        {
          POSMOT ++;                      //Ajoute 1 à POSMOT
        }
        LET = 'J';                        //Mets LET à 'J'
        MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
        VAL = 1;                          //Mets la valeur de la touche à 1
        while(digitalRead(MY2) == HIGH)   //Lis la valeur de MY2
        {
          POS[0] = 1;                       //Mets la position du clavier sur 1, 2
          POS[1] = 2;                       //
        }
      }
    }
    /////////////////////////////////////////////////////////////////////////////////////////MX1/MY3////////////////////////////////////
    else if(digitalRead(MY3) == HIGH) //Lis la valeur de MY3
    {
      if(POS[0] == 1 && POS[1] == 3)    //Test la position du clavier
      {
        SET = 0;                          //Remets SET à 0
        if(VAL == 1)                      //Test la valeur de la touche
        {
          LET = 'T';                        //Mets LET à 'T'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 2;                          //Mets la valeur de la touche à 2
          while(digitalRead(MY3) == HIGH)   //Lis la valeur de MY3
          {
            POS[0] = 1;                       //Mets la position du clavier sur 1, 3
            POS[1] = 3;                       //
          }
        }
        else if(VAL == 2)                 //Test la valeur de la touche
        {
          LET = 'U';                        //Mets LET à 'U'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 3;                          //Mets la valeur de la touche à 3
          while(digitalRead(MY3) == HIGH)   //Lis la valeur de MY3
          {
            POS[0] = 1;                       //Mets la position du clavier sur 1, 3
            POS[1] = 3;                       //
          }
        }
        else if(VAL == 3)                 //Test la valeur de la touche
        {
          LET = '7';                        //Mets LET à '7'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 4;                          //Mets la valeur de la touche à 4
          while(digitalRead(MY3) == HIGH)   //Lis la valeur de MY3
          {
            POS[0] = 1;                       //Mets la position du clavier sur 1, 3
            POS[1] = 3;                       //
          }
        }
        else
        {
          LET = 'S';                        //Mets LET à 'S'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 1;                          //Mets la valeur de la touche à 1
          while(digitalRead(MY3) == HIGH)   //Lis la valeur de MY3
          {
            POS[0] = 1;                       //Mets la position du clavier sur 1, 3
            POS[1] = 3;                       //
          }
        }
      }
      else
      {
        SET = 0;                          //Remets SET à 0
        if(VAL != 0)                      //Test la valeur de VAL
        {
          POSMOT ++;                      //Ajoute 1 à POSMOT
        }
        LET = 'S';                        //Mets LET à 'S'
        MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
        VAL = 1;                          //Mets la valeur de la touche à 1
        while(digitalRead(MY3) == HIGH)   //Lis la valeur de MY3
        {
          POS[0] = 1;                       //Mets la position du clavier sur 1, 3
          POS[1] = 3;                       //
        }
      }
    }
    /////////////////////////////////////////////////////////////////////////////////////////MX1/MY4////////////////////////////////////
    else if(digitalRead(MY4) == HIGH)//Lis la valeur de MY4
    {
      while(digitalRead(MY4) == HIGH)
      {
        SEND(MOT);                      //Envoie MOT
        delay(25);                      //delay de 25 ms
      }
      for(i = 0; i <= 19; i++)
      {
        MOT[i] = 0;                     //Efface MOT
      }
      POSMOT = 0;                     //Mets POSMOT à 0
      VAL = 0;                        //Mets VAL à 0
      POS[0] = 0;                     //Mets POS à0, 0
      POS[1] = 0;                     //
      SET = 0;                        //Mets SET à 0
      lcd.clear();                    //Efface le LCD
      WRITE = 0;                      //Mets WRITE à 0
    }
    delay(1);                         //Attends 1ms
    digitalWrite(MX1, LOW);           //Désactive MX1
    
  ///////////////////////////////////////////////////////////////////////////////////////////////MX2///////////////////////////////////  
    digitalWrite(MX2, HIGH);          //Active MX2
    
    /////////////////////////////////////////////////////////////////////////////////////////MX2/MY1////////////////////////////////////
    if(digitalRead(MY1) == HIGH)      //Lis la valeur de MY1
    {
      if(POS[0] == 2 && POS[1] == 1)    //Test la position du clavier
      {
        SET = 0;                          //Remets SET à 0
        if(VAL == 1)                      //Test la valeur de la touche
        {
          LET = 'E';                        //Mets LET à 'E'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 2;                          //Mets la valeur de la touche à 2
          while(digitalRead(MY1) == HIGH)   //Lis la valeur de MY1
          {
            POS[0] = 2;                       //Mets la position du clavier sur 2, 1
            POS[1] = 1;                       //
          }
        }
        else if(VAL == 2)                 //Test la valeur de la touche
        {
          LET = 'F';                        //Mets LET à 'F'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 3;                          //Mets la valeur de la touche à 3
          while(digitalRead(MY1) == HIGH)   //Lis la valeur de MY1
          {
            POS[0] = 2;                       //Mets la position du clavier sur 2, 1
            POS[1] = 1;                       //
          }
        }
        else if(VAL == 3)                 //Test la valeur de la touche
        {
          LET = '2';                        //Mets LET à '2'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 4;                          //Mets la valeur de la touche à 4
          while(digitalRead(MY1) == HIGH)   //Lis la valeur de MY1
          {
            POS[0] = 2;                       //Mets la position du clavier sur 2, 1
            POS[1] = 1;                       //
          }
        }
        else
        {
          LET = 'D';                        //Mets LET à 'D'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 1;                          //Mets la valeur de la touche à 1
          while(digitalRead(MY1) == HIGH)   //Lis la valeur de MY1
          {
            POS[0] = 2;                       //Mets la position du clavier sur 2, 1
            POS[1] = 1;                       //
          }
        }
      }
      else
      {
        SET = 0;                          //Remets SET à 0
        if(VAL != 0)                      //Test la valeur de VAL
        {
          POSMOT ++;                        //Ajoute 1 à POSMOT
        }
        LET = 'D';                        //Mets LET à 'D'
        MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
        VAL = 1;                          //Mets la valeur de la touche à 1
        while(digitalRead(MY1) == HIGH)   //Lis la valeur de MY1
        {
          POS[0] = 2;                       //Mets la position du clavier sur 2, 1
          POS[1] = 1;                       //
        }
      }
    }
    /////////////////////////////////////////////////////////////////////////////////////////MX2/MY2////////////////////////////////////
    else if(digitalRead(MY2) == HIGH) //Lis la valeur de MY2
    {
      if(POS[0] == 2 && POS[1] == 2)    //Test la position du clavier
      {
        SET = 0;                          //Remets SET à 0
        if(VAL == 1)                      //Test la valeur de la touche
        {
          LET = 'N';                        //Mets LET à 'N'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 2;                          //Mets la valeur de la touche à 2
          while(digitalRead(MY2) == HIGH)   //Lis la valeur de MY2
          {
            POS[0] = 2;                       //Mets la position du clavier sur 2, 2
            POS[1] = 2;                       //
          }
        }
        else if(VAL == 2)                 //Test la valeur de la touche
        {
          LET = 'O';                        //Mets LET à 'O'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 3;                          //Mets la valeur de la touche à 3
          while(digitalRead(MY2) == HIGH)   //Lis la valeur de MY2
          {
            POS[0] = 2;                       //Mets la position du clavier sur 2, 2
            POS[1] = 2;                       //
          }
        }
        else if(VAL == 3)                 //Test la valeur de la touche
        {
          LET = '5';                        //Mets LET à '5'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 4;                          //Mets la valeur de la touche à 4
          while(digitalRead(MY2) == HIGH)   //Lis la valeur de MY2
          {
            POS[0] = 2;                       //Mets la position du clavier sur 2, 2
            POS[1] = 2;                       //
          }
        }
        else
        {
          LET = 'M';                        //Mets LET à 'M'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 1;                          //Mets la valeur de la touche à 1
          while(digitalRead(MY2) == HIGH)   //Lis la valeur de MY2
          {
            POS[0] = 2;                       //Mets la position du clavier sur 2, 2
            POS[1] = 2;                       //
          }
        }
      }
      else
      {
        SET = 0;                          //Remets SET à 0
        if(VAL != 0)                      //Test la valeur de VAL
        {
          POSMOT ++;                        //Ajoute 1 à POSMOT
        }
        LET = 'M';                        //Mets LET à 'M'
        MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
        VAL = 1;                          //Mets la valeur de la touche à 1
        while(digitalRead(MY2) == HIGH)   //Lis la valeur de MY2
        {
          POS[0] = 2;                       //Mets la position du clavier sur 2, 2
          POS[1] = 2;                       //
        }
      }
    }
    /////////////////////////////////////////////////////////////////////////////////////////MX2/MY3////////////////////////////////////
    else if(digitalRead(MY3) == HIGH) //Lis la valeur de MY3
    {
      if(POS[0] == 2 && POS[1] == 3)    //Test la position du clavier
      {
        SET = 0;                          //Remets SET à 0
        if(VAL == 1)                      //Test la valeur de la touche
        {
          LET = 'W';                        //Mets LET à 'W'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 2;                          //Mets la valeur de la touche à 2
          while(digitalRead(MY3) == HIGH)   //Lis la valeur de MY3
          {
            POS[0] = 2;                       //Mets la position du clavier sur 2, 3
            POS[1] = 3;                       //
          }
        }
        else if(VAL == 2)                 //Test la valeur de la touche
        {
          LET = 'X';                        //Mets LET à 'X'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 3;                          //Mets la valeur de la touche à 3
          while(digitalRead(MY3) == HIGH)   //Lis la valeur de MY3
          {
            POS[0] = 2;                       //Mets la position du clavier sur 2, 3
            POS[1] = 3;                       //
          }
        }
        else if(VAL == 3)                 //Test la valeur de la touche
        {
          LET = '8';                        //Mets LET à '8'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 4;                          //Mets la valeur de la touche à 4
          while(digitalRead(MY3) == HIGH)   //Lis la valeur de MY3
          {
            POS[0] = 2;                       //Mets la position du clavier sur 2, 3
            POS[1] = 3;                       //
          }
        }
        else
        {
          LET = 'V';                        //Mets LET à 'V'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 1;                          //Mets la valeur de la touche à 1
          while(digitalRead(MY3) == HIGH)   //Lis la valeur de MY3
          {
            POS[0] = 2;                       //Mets la position du clavier sur 2, 3
            POS[1] = 3;                       //
          }
        }
      }
      else
      {
        SET = 0;                          //Remets SET à 0
        if(VAL != 0)                      //Test la valeur de VAL
        {
          POSMOT ++;                        //Ajoute 1 à POSMOT
        }
        LET = 'V';                        //Mets LET à 'V'
        MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
        VAL = 1;                          //Mets la valeur de la touche à 1
        while(digitalRead(MY3) == HIGH)   //Lis la valeur de MY3
        {
          POS[0] = 2;                       //Mets la position du clavier sur 2, 3
          POS[1] = 3;                       //
        }
      }
    }
    /////////////////////////////////////////////////////////////////////////////////////////MX2/MY4////////////////////////////////////
    else if(digitalRead(MY4) == HIGH) //Lis la valeur de MY3
    {
      if(POS[0] == 2 && POS[1] == 4)    //Test la position du clavier
      {
        SET = 0;                          //Remets SET à 0
        if(VAL == 1)                      //Test la valeur de la touche
        {
          LET = '?';                        //Mets LET à '0'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 2;                          //Mets la valeur de la touche à 2
          while(digitalRead(MY4) == HIGH)   //Lis la valeur de MY3
          {
            POS[0] = 2;                       //Mets la position du clavier sur 2, 4
            POS[1] = 4;                       //
          }
        }
        else if(VAL == 2)                 //Test la valeur de la touche
        {
          LET = '!';                        //Mets LET à '?'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 3;                          //Mets la valeur de la touche à 3
          while(digitalRead(MY4) == HIGH)   //Lis la valeur de MY4
          {
            POS[0] = 2;                       //Mets la position du clavier sur 2, 4
            POS[1] = 4;                       //
          }
        }
        else if(VAL == 3)                 //Test la valeur de la touche
        {
          LET = '0';                        //Mets LET à '!'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 4;                          //Mets la valeur de la touche à 4
          while(digitalRead(MY4) == HIGH)   //Lis la valeur de MY4
          {
            POS[0] = 2;                       //Mets la position du clavier sur 2, 4
            POS[1] = 4;                       //
          }
        }
        else
        {
          LET = ' ';                        //Mets LET à ' '
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 1;                          //Mets la valeur de la touche à 1
          while(digitalRead(MY4) == HIGH)   //Lis la valeur de MY4
          {
            POS[0] = 2;                       //Mets la position du clavier sur 2, 4
            POS[1] = 4;                       //
          }
        }
      }
      else
      {
        SET = 0;                          //Remets SET à 0
        if(VAL != 0)                      //Test la valeur de VAL
        {
          POSMOT ++;                        //Ajoute 1 à POSMOT
        }
        LET = ' ';                        //Mets LET à ' '
        MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
        VAL = 1;                          //Mets la valeur de la touche à 1
        while(digitalRead(MY4) == HIGH)   //Lis la valeur de MY4
        {
          POS[0] = 2;                       //Mets la position du clavier sur 2, 4
          POS[1] = 4;                       //
        }
      }
    }
    delay(1);
    digitalWrite(MX2, LOW);           //Désactive MX2
    
    ///////////////////////////////////////////////////////////////////////////////////////////////MX3///////////////////////////////////
    digitalWrite(MX3, HIGH);          //Active MX3
    
    /////////////////////////////////////////////////////////////////////////////////////////MX3/MY1////////////////////////////////////
    if(digitalRead(MY1) == HIGH)      //Lis la valeur de MY1
    {
      if(POS[0] == 3 && POS[1] == 1)    //Test la position du clavier
      {
        SET = 0;                          //Remets SET à 0
        if(VAL == 1)                      //Test la valeur de la touche
        {
          LET = 'H';                        //Mets LET à 'H'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 2;                          //Mets la valeur de la touche à 2
          while(digitalRead(MY1) == HIGH)   //Lis la valeur de MY1
          {
            POS[0] = 3;                       //Mets la position du clavier sur 3, 1
            POS[1] = 1;                       //
          }
        }
        else if(VAL == 2)                 //Test la valeur de la touche
        {
          LET = 'I';                        //Mets LET à 'I'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 3;                          //Mets la valeur de la touche à 3
          while(digitalRead(MY1) == HIGH)   //Lis la valeur de MY1
          {
            POS[0] = 3;                       //Mets la position du clavier sur 3, 1
            POS[1] = 1;                       //
          }
        }
        else if(VAL == 3)                 //Test la valeur de la touche
        {
          LET = '3';                        //Mets LET à '3'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 4;                          //Mets la valeur de la touche à 4
          while(digitalRead(MY1) == HIGH)   //Lis la valeur de MY1
          {
            POS[0] = 3;                       //Mets la position du clavier sur 3, 1
            POS[1] = 1;                       //
          }
        }
        else
        {
          LET = 'G';                        //Mets LET à 'G'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 1;                          //Mets la valeur de la touche à 1
          while(digitalRead(MY1) == HIGH)   //Lis la valeur de MY1
          {
            POS[0] = 3;                       //Mets la position du clavier sur 3, 1
            POS[1] = 1;                       //
          }
        }
      }
      else
      {
        SET = 0;                          //Remets SET à 0
        if(VAL != 0)                      //Test la valeur de VAL
        {
          POSMOT ++;                        //Ajoute 1 à POSMOT
        }
        LET = 'G';                        //Mets LET à 'G'
        MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
        VAL = 1;                          //Mets la valeur de la touche à 1
        while(digitalRead(MY1) == HIGH)   //Lis la valeur de MY1
        {
          POS[0] = 3;                       //Mets la position du clavier sur 3, 1
          POS[1] = 1;                       //
        }
      }
    }
    /////////////////////////////////////////////////////////////////////////////////////////MX3/MY2////////////////////////////////////
    else if(digitalRead(MY2) == HIGH) //Lis la valeur de MY2
    {
      if(POS[0] == 3 && POS[1] == 2)    //Test la position du clavier
      {
        SET = 0;                          //Remets SET à 0
        if(VAL == 1)                      //Test la valeur de la touche
        {
          LET = 'Q';                        //Mets LET à 'Q'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 2;                          //Mets la valeur de la touche à 2
          while(digitalRead(MY2) == HIGH)   //Lis la valeur de MY2
          {
            POS[0] = 3;                       //Mets la position du clavier sur 3, 2
            POS[1] = 2;                       //
          }
        }
        else if(VAL == 2)                 //Test la valeur de la touche
        {
          LET = 'R';                        //Mets LET à 'R'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 3;                          //Mets la valeur de la touche à 3
          while(digitalRead(MY2) == HIGH)   //Lis la valeur de MY2
          {
            POS[0] = 3;                       //Mets la position du clavier sur 3, 2
            POS[1] = 2;                       //
          }
        }
        else if(VAL == 3)                 //Test la valeur de la touche
        {
          LET = '6';                        //Mets LET à '6'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 4;                          //Mets la valeur de la touche à 4
          while(digitalRead(MY2) == HIGH)   //Lis la valeur de MY2
          {
            POS[0] = 3;                       //Mets la position du clavier sur 3, 2
            POS[1] = 2;                       //
          }
        }
        else
        {
          LET = 'P';                        //Mets LET à 'P'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 1;                          //Mets la valeur de la touche à 1
          while(digitalRead(MY2) == HIGH)   //Lis la valeur de MY2
          {
            POS[0] = 3;                       //Mets la position du clavier sur 3, 2
            POS[1] = 2;                       //
          }
        }
      }
      else
      {
        SET = 0;                          //Remets SET à 0
        if(VAL != 0)                      //Test la valeur de VAL
        {
          POSMOT ++;                        //Ajoute 1 à POSMOT
        }
        LET = 'P';                        //Mets LET à 'P'
        MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
        VAL = 1;                          //Mets la valeur de la touche à 1
        while(digitalRead(MY2) == HIGH)   //Lis la valeur de MY2
        {
          POS[0] = 3;                       //Mets la position du clavier sur 3, 2
          POS[1] = 2;                       //
        }
      }
    }
    /////////////////////////////////////////////////////////////////////////////////////////MX3/MY3////////////////////////////////////
    else if(digitalRead(MY3) == HIGH) //Lis la valeur de MY3
    {
      if(POS[0] == 3 && POS[1] == 3)    //Test la position du clavier
      {
        SET = 0;                          //Remets SET à 0
        if(VAL == 1)                      //Test la valeur de la touche
        {
          LET = 'Z';                        //Mets LET à 'Z'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 2;                          //Mets la valeur de la touche à 2
          while(digitalRead(MY3) == HIGH)   //Lis la valeur de MY3
          {
            POS[0] = 3;                       //Mets la position du clavier sur 3, 3
            POS[1] = 3;                       //
          }
        }
        else if(VAL == 2)                 //Test la valeur de la touche
        {
          LET = '-';                        //Mets LET à '9'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 3;                          //Mets la valeur de la touche à 3
          while(digitalRead(MY3) == HIGH)   //Lis la valeur de MY3
          {
            POS[0] = 3;                       //Mets la position du clavier sur 3, 3
            POS[1] = 3;                       //
          }
        }
        else if(VAL == 3)                 //Test la valeur de la touche
        {
          LET = '9';                        //Mets LET à '-'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 4;                          //Mets la valeur de la touche à 4
          while(digitalRead(MY3) == HIGH)   //Lis la valeur de MY3
          {
            POS[0] = 3;                       //Mets la position du clavier sur 3, 3
            POS[1] = 3;                       //
          }
        }
        else
        {
          LET = 'Y';                        //Mets LET à 'Y'
          MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
          VAL = 1;                          //Mets la valeur de la touche à 1
          while(digitalRead(MY3) == HIGH)   //Lis la valeur de MY3
          {
            POS[0] = 3;                       //Mets la position du clavier sur 3, 3
            POS[1] = 3;                       //
          }
        }
      }
      else
      {
        SET = 0;                          //Remets SET à 0
        if(VAL != 0)                      //Test la valeur de VAL
        {
          POSMOT ++;                      //Ajoute 1 à POSMOT
        }
        LET = 'Y';                        //Mets LET à 'Y'
        MOT[POSMOT] = LET;                //Ajoute LET au tableau MOT
        VAL = 1;                          //Mets la valeur de la touche à 1
        while(digitalRead(MY3) == HIGH)   //Lis la valeur de MY3
        {
          POS[0] = 3;                       //Mets la position du clavier sur 3, 3
          POS[1] = 3;                       //
        }
      }
    }
    /////////////////////////////////////////////////////////////////////////////////////////MX3/MY4////////////////////////////////////
    else if(digitalRead(MY4) == HIGH) //Lis la valeur de MY4
    {
      SET = 0;                            //Remets SET à 0
      if(POSMOT == -1)
      {
        WRITE = 0;
      }
      POS[0] = 0;                       //Mets POS à 0, 0
      POS[1] = 0;                       //
      MOT[POSMOT] = 0;                  //Efface la lettre du LCD
      POSMOT --;                          //Reviens sur la case précédente
      RECU = 0;
      while(digitalRead(MY4) == HIGH)     //Lis la valeur de MY4
      {
        if(POSMOT < -1)                      //Test POSMOT
        {
          POSMOT++;                           //Ajoute 1 à POSMOT
        }
      }
      lcd.clear();                        //Efface le LCD
      
    }
    delay(1);                         //Attends 1 ms
    digitalWrite(MX3, LOW);           //Désactive MX1
    
  ///////////////////////////////////////////////////////////////////////////////////////////////Affichage LCD///////////////////////////////////
    delay(1);                         //Attends 1ms
    SET ++;                           //Ajoute 1 à SET
    if(SET == 100)                    //Test SET
    {
      POS[0] = 0;                       //Remets les valeur de POS à 0
      POS[1] = 0;
    }
    if(POSMOT > 20)
    {
      POSMOT = 20;                      //Mets POSMOT à 27
    }
    lcd.setCursor(0, 0);              //Remets le curseur à 0, 0
    lcd.write(MOT);                   //Affiche MOT sur le LDC
    lcd.setCursor(0, 1);              //Met le curseur sur 0, 1
    lcd.write(OK);                    //Ecris le caratère OK
    if(POSMOT > 15)                   //
    {
      lcd.setCursor(1, 1);              //Mets le curseur du LCD à 1, 1
      for(i = 16; i <= POSMOT; i++)     //
      {
        lcd.write(MOT[i]);            //Affiche MOT sur le LCD
      }
    }
    lcd.setCursor(15, 1);             //Met le curseur sur 15, 1
    lcd.write(DEL);                   //Ecris le caratère DEL
  }
  
  ///////////////////////////////////////////////////////////////////////////////////////////////Lecture////////////////////////////////////////
  while(READ == 1)                  //Test la valeur de READ
  {
    lcd.setCursor(0, 0);              //Mets le curseur du LCD à 0, 0
    for(i = 0; i < 16; i++)           //
    {
      lcd.write(MOTREC[SLOTW][i]);         //Affiche MOTREC sur le LCD
    }
    lcd.setCursor(0, 1);              //Mets le surseur du LCD à 0, 1
    lcd.write(OK);                    //Affiche le caratère OK sur le LCD
    lcd.setCursor(1, 1);              //Mets le curseur du LCD à 1, 1
    for(i = 16; i < 30; i++)          //
    {
      lcd.write(MOTREC[SLOTW][i]);      //Affiche MOTREC sur le LCD
    }
    if(SLOTW < 10)
    {
      lcd.setCursor(14, 1);             //Mets le curseur du LCD à 14, 1
    }
    else
    {
      lcd.setCursor(13, 1);             //Mets le curseur du LCD à 13, 1
    }
    lcd.print(SLOTW);                 //Affiche SLOTW sur le lcd
    delay(10);                        //Attends 10 ms
    lcd.write(DEL);                   //Affiche le caratère DEL
    digitalWrite(MX1, HIGH);          //Active MX1
    while(digitalRead(MY2) == HIGH)   //
    {
      SLOTW = 1;
    }
    while(digitalRead(MY4) == HIGH)   //
    {
      READ = 0;                         //Mets READ à 0
      lcd.clear();                      //Efface le LCD
    }
    digitalWrite(MX1, LOW);           //Désactive MX1
    digitalWrite(MX2, HIGH);          //Active MX2
    if(digitalRead(MY1) == HIGH)
    {
      SLOTW --;                         //Enlève 1 à SLOTW
      if(SLOTW < 1)
      {
        SLOTW = 1;                        //Mets SLOTW à 1
      }
      delay(200);                       //Attends 500ms
    }
    else if(digitalRead(MY3) == HIGH)
    {
      SLOTW ++;                           //Ajoute 1 à SLOTW
      if(SLOTW > SLOT && SLOT != 0)
      {
        SLOTW = SLOT;                       //Mets SLOTW à la valeur de SLOT
      }
      else if(SLOT == 0)
      {
        SLOTW = 1;
      }
      delay(200);                         //Attends 500 ms
    }
    digitalWrite(MX2, LOW);           //Désactive MX2
    digitalWrite(MX3, HIGH);          //Active MX3
    while(digitalRead(MY2) == HIGH)
    {
      if(SLOT == 0)
      {
        SLOTW = 1;                        //Mets SLOTW à 1
      }
      else
      {
        SLOTW = SLOT;                     //Mets SLOTW à la valeur de SLOT
      }
    }
    if(digitalRead(MY4) == HIGH)
    {
      while(digitalRead(MY4) == HIGH)
      {
        lcd.clear();                      //Efface le LCD
        for(i = 0; i <= 30; i++)          //
        {
          MOTREC[SLOTW][i] = ' ';           //Efface MOTREC
        }
      }
      if(SLOT != 0 && SLOTW == SLOT)
      {
        SLOT --;                          //Enlève 1 à SLOT
        SLOTW = SLOT;                     //Mets SLOTW à la valeur de SLOT
      }
      else
      {
        for(i = SLOTW; i < SLOT; i++)
        {
          for(j = 0; j < 30; j++)
          {
            MOTREC[i][j] = MOTREC[i+1][j];      //Décale les messages dans la mémoire
          }
        }
        SLOT --;
      }
    }
    digitalWrite(MX3, LOW);           //Désactive MX3
    CLI = REC;                        //Mets CLI à REC
    RECU = 0;                         //Mets RECU à 0
    RECIEVE();
  }
}

  ///////////////////////////////////////////////////////////////////////////////////////////////Récéption////////////////////////////////////////
bool RECIEVE()                        //Fonction de récéption du message
{
  byte buf[VW_MAX_MESSAGE_LEN];      //Récéption du message
  byte buflen = VW_MAX_MESSAGE_LEN;  //
  
  if (vw_get_message(buf, &buflen))     //
  {
    SLOT ++;
    if(SLOT >= 40)
    {
      SLOT = 39;
      for(i = 0; i < 39; i++)
      {
        for(j = 0; j < 30; j++)
        {
          MOTREC[i][j] = MOTREC[i+1][j];      //Décale les messages dans la mémoire
        }
      }
    }
      for(i = 0; i < buflen; i++)         //
    {
      MOTREC[SLOT][i] = buf[i];             //MOTREC est égal au message reçu
    }
    for(i = buflen; i < 30; i++)          //
    {
      MOTREC[SLOT][i] = ' ';                //Rempli MOTREC d'espace
    }
    if(READ != 1)
    {
      SLOTW = SLOT;                         //Mets SLOTW à la valeur de SLOT
    }
    if(MOTREC[SLOT][0] != ' ')            //
    {
      return 1;                             //RECIEVE retourne 1
    }
    else
    {
      return 0;                             //RECIEVE retourne 0
    }
  }
}

  ///////////////////////////////////////////////////////////////////////////////////////////////Emmition////////////////////////////////////////
void SEND(char MOTSEND[20])         //Fonction de réception du message
{
  vw_send((uint8_t *)MOTSEND, 20);    //Envoe le message MOTSEND avec la librairie VirtualWire
  vw_wait_tx();                       //Attends que le message soit envoyé
}
