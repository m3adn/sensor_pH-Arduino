## Informação
Tal como as primeiras 3 partes do projeto foram feitas sem o sensor necessário , o código que está nesta ultima parte será modificado ligeiramente e adaptado ao sensor em causa.  
**Sensor Usado: `DFROBOT - SEN0161`** -> [Mais Documentação](https://wiki.dfrobot.com/PH_meter_SKU__SEN0161_)
________________________________

## Explicação do codigo:
Esta parte serve para ligar o MKR1300 atravês do módulo LoRa à gateway,devolver o deviceEui para poder efetuar o registo na aplicação da plataforma, avisar se estámos ligados à mesma ou não e posteriormente enviar os dados para a plataforma. Usamos o appEui e AppKey fornecidos pela ThethingsNetwork para realizar a ligação à nossa aplicação na plataforma.
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
    
    modem.write(aqui_podiam_estar_valores);
    modem.write(aqui_podiam_estar_valores);
  
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
Esta parte serve para ler os dados do sensor e imprimir os mesmos. De notar que temos a função `averagearray`. A mesma serve para "calibrar" de uma certa maneira o sensor e fazer com que o valor final imprimido e enviado seja mais preciso.
```

#define SensorPin A0            //Saida analogica no pino 0 para o sensor pH
#define Offset 0.00            //Compensação do desvio
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    
double averagearray(int* arr, int number);
int pHArray[ArrayLenth];   //Guarda o valor medio lido.
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
  if(millis() - printTime > printInterval)   //A cada 800 milisegundos imprimie o valor da tensão e pH.
  {
    Serial.print("Voltage:");
        Serial.print(voltage,2);
        Serial.print("    pH value: ");
    Serial.println(pHValue,2);
        printTime=millis();
  }
  
 
```
Função `averagearray` mencionada anteriormente:
```
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
```
Como não podemos enviar valores em float temos que encontrar uma maneira de passar para inteiros. Resolvi em multiplicar por 1000 e depois multiplicar por 3(serve como chave para que se alguém quiser intercetar os dados que não consiga decifrar sem fazer esta conta).   
`pHValue = pHValue*1000*3;`

Passamos para hexadecimal os valores e mandamos o respetivo Print para a Serial do arduinoo para nos assegurarmos que o valor hexadecimal enviado é o mesmo que o recebido:    
```
    txBuffer[0] = ((int)pHValue >> 8) & 0xff;
    txBuffer[1]  = (int)pHValue & 0xff;

    Serial.println(txBuffer[0], HEX);
    Serial.println(txBuffer[1], HEX);
    

```
Como já temos o valor em hexadecimal , podemos mandá-lo para a cloud. Para tal , não podemos enviar a array `txBuffer` por inteiro. Temos que mandar os valores guardados em posições especificas da memória senão o valor a receber pela cloud será do Array inteiro:
```
    modem.write(txBuffer[0]);
    modem.write(txBuffer[1]);
```

## Melhorias
O código foi melhorado ligeiramente em relação ao enviado noutras fases. Como não havia maneira de testar em casa , para enviar para a cloud foi definida a função 'modem.write(txBuffer);'. No entanto ao testar deparei-me que os valores enviados não correspondiam aos recebidos. A solução foi colocar a posição do array em que guardamos os dados sendo estas posições 'modem.write(txBuffer[0]);' e 'modem.write(txBuffer[1]);'
