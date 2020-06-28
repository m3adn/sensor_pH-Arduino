## Informação
Tal como as primeiras 3 partes do projeto foram feitas sem o sensor necessário , o código que está nesta ultima parte será modificado ligeiramente e adaptado ao sensor em causa.
________________________________

## Explicação do codigo:
Esta parte serve para ligar  o MKR atravês do LoRa à gateway, avisar se estámos ligados à mesma ou não e posteriormente enviar os dados para a plataforma.
```
#include <MKRWAN.h> 
LoRaModem modem(Serial);
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
```
Esta parte serve para ler os dados do sensor:
```

#define SensorPin A0            //Saida analogica no pino 0 para o sensor pH
#define Offset 0.00            //Compensação do desvio
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
double averagearray(int* arr, int number);
int pHArray[ArrayLenth];   //Guarda o valor medio do sensor.
int pHArrayIndex=0;

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
```
Como não podemos enviar valores em float temos que encontrar uma maneira de passar para inteiros. Resolvi em multiplicar por 1000 e depois multiplicar por 3 (serve como chave para que se alguém quiser intercetar os dados que não consiga decifrar).   
`pHValue = pHValue*1000*3;`

Passamos para hexadecimal os dados e fazemos o respetivo print para nos assegurarmos que o valor hexadecimal enviado é o mesmo que é recebido:    
```
 txBuffer[0] = ((int)pHValue >> 8) & 0xff;
    txBuffer[1]  = (int)pHValue & 0xff;

    Serial.println(txBuffer[0], HEX);
    Serial.println(txBuffer[1], HEX);
    

```
