## Informação
Tal como as primeiras 3 partes do projeto foram feitas sem o sensor necessário , o código que está nesta ultima parte será modificado ligeiramente e adaptado ao sensor em causa.
________________________________

## Explicação do codigo:
Esta parte serve para ligar um o MKR atravês do LoRa à gateway e posteriormente avisar que estámos ligados à mesma ou não
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
