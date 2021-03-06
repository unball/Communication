#include <SPI.h>
#include "RF24.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

struct DataStruct{
    int16_t Robot=0;
    int16_t Message=0;
};

DataStruct Report;
int ctr[4] = {0, 0, 0, 0}; 
int u=0;
char test[4];
int state[3];
int Data[3] = {0, 0, 0};
int CE = A0; //pro micro
int CS = 10; //pro micro

RF24 radio(CE,CS);
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


const uint64_t Channels[4] = { 0xABCDABCD71LL, 0x544d52687CLL , 0x644d52687CLL, 0x744d52687CLL};
uint64_t ChannelEnvia = Channels[0];
uint64_t ChannelRecebe = Channels[3];

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  //while(!Serial);
  Serial.println("passou aqui");
  RadioSetup();
  lcd.begin(16,2);
  lcd.setBacklight (HIGH);
  //Report.Robot = 1;
  state[0] = Data[0];
  state[1] = Data[1];
  state[2] = Data[2];
  lcd.setCursor (5,0);
  lcd.print ("UNBALL");
  lcd.setCursor(0,1);
  lcd.print("FUTEBOL DE ROBOS");

}

void loop() {
  // put your main code here, to run repeatedly:
  ReceiveReport();
  lcd.setCursor (11,0);
  sprintf(test, "%d", u++);
  lcd.print (" ");
  if(state[0] != Data[0] || state[1] != Data[1] || state[2] != Data[2]){
    Serial.println(Report.Robot);
    Serial.println(Report.Message);
    Print_lcd();
    state[0] = Data[0];
    state[1] = Data[1];
    state[2] = Data[2];
  }
  
  delay(25);
}

void RadioSetup(){
    radio.begin();                           // inicializa radio
    radio.setChannel(108);                   //muda para um canal de frequencia diferente de 2.4Ghz
    radio.setPALevel(RF24_PA_MAX);           //usa potencia maxima
    radio.setDataRate(RF24_2MBPS);           //usa velocidade de transmissao maxima

    radio.openWritingPipe(ChannelEnvia);        //escreve pelo pipe0 SEMPRE
    radio.openReadingPipe(1,ChannelRecebe);      //escuta pelo pipe1, evitar usar pipe0

    radio.enableDynamicPayloads();           //ativa payloads dinamicos(pacote tamamhos diferentes do padrao)
    radio.setPayloadSize(sizeof(DataStruct));   //ajusta os o tamanho dos pacotes ao tamanho da mensagem
                                                 //tamanho maximo de payload 32 BYTES
    
    radio.startListening();                 // Start listening
}

void NotReceivingFor(int id){
  ctr[id]++;
}

void ReceiveReport(){       //Function to receive data
  if(radio.available()){
    ctr[3] = 0;
    radio.read(&Report, sizeof(DataStruct));
    Serial.println("receiving");
    Data[Report.Robot] = Report.Message;
    ctr[Report.Robot] = 0;
    Serial.println(Data[Report.Robot]);
  }
  else{
    Serial.println("not receiving");
    NotReceivingFor(3);
    for(int i=0; i<3; i++){
    NotReceivingFor(i);
    if(ctr[i]>360){
      Data[i] = 0;
    }
  }
    if(ctr[3] > 400){
      ctr[3] = 401;
      Data[0] = 0;
      Data[1] = 0;
      Data[2] = 0;
    }
  }
}

void Clear_lcd(){
  lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("                ");
}

void Print_lcd(){
  if (Data[0] == 0 && Data[1] == 0 && Data[2] == 0){
    Clear_lcd();
    lcd.setCursor (5,0);
    lcd.print ("UNBALL");
    lcd.setCursor(0,1);
    lcd.print("FUTEBOL DE ROBOS");
  }
  else{
    Clear_lcd();
    lcd.setCursor (0,0);
    lcd.print ("R00");
    lcd.setCursor (6,0);
    lcd.print ("R01");
    lcd.setCursor (12,0);
    lcd.print ("R02");
  
    if (Data[0] == 1){
      lcd.setCursor (0,1);
      lcd.print ("LOST");
    }
    else if(Data[0] == 2){
      lcd.setCursor (0,1);
      lcd.print ("OK");
    }
    else{
      lcd.setCursor (0,1);
      lcd.print ("OFF");
    }
    
    if (Data[1] == 1){
      lcd.setCursor (6,1);
      lcd.print ("LOST");
    }
    else if(Data[1] == 2){
      lcd.setCursor (6,1);
      lcd.print ("OK");
    }
    else{
      lcd.setCursor (6,1);
      lcd.print ("OFF");
    }
    
    if (Data[2] == 1){
      lcd.setCursor (12,1);
      lcd.print ("LOST");
    }
    else if(Data[2] == 2){
      lcd.setCursor (12,1);
      lcd.print ("OK");
    }
    else{
      lcd.setCursor (12,1);
      lcd.print ("OFF");
    }
  }
}
