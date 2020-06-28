/*
 # Ver    : 1.0
 # Product: analog pH meter
 # SKU    : SEN0161
*/
#include <MKRWAN.h>  

#define SensorPin A0            //Saida analogica no pino 0 para o sensor pH
#define Offset 0.00            //Compensação do desvio
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection

LoRaModem modem(Serial);

double averagearray(int* arr, int number);

uint8_t txBuffer[2];
int pHArray[ArrayLenth];   //Guarda o valor medio do sensor.
int pHArrayIndex=0;

const char *appEui = "70B3D57ED0030FEA";
const char *appKey = "71114EA43603A768424D9013136EFE43";
   
// Escolha a região (AS923, AU915, EU868, KR920, IN865, US915, US915_HYBRID)
_lora_band region = EU868;
  
void setup(void)
{

  Serial.begin(115200);
    while (!Serial);
    if (!modem.begin(region)) {
      Serial.println("Erro ao iniciar!");
     
      while (1) {}
    };
   
      Serial.print("A versão do módulo é: ");
      Serial.println(modem.version());
      Serial.print("O EUI do dispositivo é: ");
      Serial.println(modem.deviceEUI());
   
    int connected = modem.joinOTAA(appEui, appKey);
    if (!connected) {
      Serial.println("Há algo de errado. Por favor , verifique a cobertura!");
      while (1) {}
    }
    //O MODEM PERMITE EVIAR UMA MENSAGEM A CADA 2 MINUTOS!?
    modem.minPollInterval(60);
  }
   
void loop(void)
{
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = averagearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+Offset;
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval)   //A cada 800 milisegundos imprimir o valor da tensão e pH.
  {
    Serial.print("Voltage:");
        Serial.print(voltage,2);
        Serial.print("    pH value: ");
    Serial.println(pHValue,2);
        printTime=millis();
  }
    pHValue = pHValue*1000*3;
    txBuffer[0] = ((int)pHValue >> 8) & 0xff;
    txBuffer[1]  = (int)pHValue & 0xff;

    modem.beginPacket();
    modem.write(txBuffer[0]);
    modem.write(txBuffer[1]);
  
   int err;
    err = modem.endPacket(txBuffer);
    if (err > 0) {
      Serial.println("Sucesso! A mensagem foi enviada corretamente!");
    } else {
      Serial.println("Erro ao enviar a mensagem :(");
      Serial.println("(só consegues enviar uma quantia limitada de mensagens por minuto dependendo da intensidade do sinal)");
      Serial.println("(a tal pode variar de uma mensagem por segundo até uma por minuto)");
    }
    delay(10000);
    }

double averagearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Erro no numero. Introduza um valor positivo.!/n");
    return 0;
  }
  if(number<5){   //Menor que 5.
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}
