## Informação:
**Projeto realizado no ambito da cadeira: `Instrumentação e Sensores`  
Professor: `Antonio Valente`  
Autores: `Daniel Andronic - 67723 e Filipe Santos - 66539`**
Nota: Para ver o relatório do código do Arduino , por favor passar na pasta: `sensor_pH` onde está a parte do codigo correspondente ao arduino.

_____________________________________________________________

## Introdução:
Equipamento usado: 
  - Arduino MKR1300 
  - Sensor pH -> Marca:DfRobot | Modelo: SEN0161  

Plataformas usadas:
  - Arduino
  - ThethingsNetwork
  - Thingspeak
  - Twitter

_____________________________________________________________

## Configuração(ThethingsNetwork):
1. Encontrar a divisão Applications em Consoles:
![Passo1](https://github.com/m3adn/sensor_pH-Arduino/blob/master/img/thing1.png)
2. Criar uma aplicação:
![Passo2](https://github.com/m3adn/sensor_pH-Arduino/blob/master/img/thing2.png)
3. Conferir os dados:
![Passo3](https://github.com/m3adn/sensor_pH-Arduino/blob/master/img/thing3.png)
4. Registar um dispositivo:
![Passo4](https://github.com/m3adn/sensor_pH-Arduino/blob/master/img/thing4.png)
5. Conferir os dados:
![Passo5](https://github.com/m3adn/sensor_pH-Arduino/blob/master/img/thing5.png)
6. Copiar O nosso AppEui e AppKey para o codigo do Arduino:
![Passo6](https://github.com/m3adn/sensor_pH-Arduino/blob/master/img/thing7.png)
7. Introduzir a parte descodificadora do nosso código na secção PAYLOAD:
```
 function Decoder(bytes, port) {
  // Decode an uplink message from a buffer
  // (array) of bytes to an object of fields.
  var decoded = {};
  
 
    decoded.pHValue = (((bytes[0]) << 8| bytes[1])/(1000*3)).toFixed(2);
    decoded.Humidity = (((bytes[2]) << 8| bytes[3])).toFixed(2);
    decoded.ecValue = (((bytes[4]) << 8| bytes[5])*2/(1000));
    decoded.Temperature = (((bytes[6]) << 8| bytes[7])).toFixed(2);
  

  return {
    field1: decoded.Temperature,
    field2: decoded.Humidity,
    field4: decoded.ecValue,
    field3: decoded.pHValue
  }
}
```
![Passo7](https://github.com/m3adn/sensor_pH-Arduino/blob/master/img/thing6.png)  
8. Verificar se os valores recebidos e enviados são os mesmos:    
![Passo8](https://github.com/m3adn/sensor_pH-Arduino/blob/master/img/thing88.png)  
![Passo8](https://github.com/m3adn/sensor_pH-Arduino/blob/master/img/thing99.png)  
Como se pode ver , na secção data da segunda imagem vemos o mesmo valor em hexadecimal que está no Serial do arduino, ou seja o valor é enviado e recebido corretamente. Podemos ver também no field 3 que o valor foi descodificado bem, sendo este o valor inicial enviado pelo arduino.   
9. Interligar a conta do Thenetworkthings com a conta do ThingSpeak:  
Para tal tem que se clicar na divisão "Integrations" e de seguida clicar a aplicação do ThingSpeak para associar as contas.

_______________________________________________________________

## Configuração(ThingSpeak):
1. Fazer login na pagina principal    
2. Encontrar a divisão MyChanels na Categoria Chanels:  
![Passo1](https://github.com/m3adn/sensor_pH-Arduino/blob/master/img/speak12.png)
3. Criar um novo canal:  
![Passo2](https://github.com/m3adn/sensor_pH-Arduino/blob/master/img/speak22.png)
4. Se for necessário alterar algum field apos a configuração inicial, acessar a categoria *Settings* na pagina do proprio canal:  
![Passo3](https://github.com/m3adn/sensor_pH-Arduino/blob/master/img/speak32.png)
5. Verificar se o canal recebe os valores:    
![Passo4](https://github.com/m3adn/sensor_pH-Arduino/blob/master/img/speak13.png)  
Como se pode ver , tendo apenas o sensor de pH ligado , sendo este associado ao field3 podemos confirmar que o plataforma está a receber os valores enviados pelo ThethingsNetwork.

________________________________________________________________

## Configuração(ThingSpeak -> Twitter):
1. Encontrar divisão ThingTweet na categora Apps e interligar as contas:    
![Passo1](https://github.com/m3adn/sensor_pH-Arduino/blob/master/img/speak7.png)
2.Encontrar a divisão React:    
![Passo2](https://github.com/m3adn/sensor_pH-Arduino/blob/master/img/speak52.png)
3.Criar um react novo e alterar conforme quiser:     
![Passo3](https://github.com/m3adn/sensor_pH-Arduino/blob/master/img/speak62.png)
4.Resultado:    
![Passo4](https://github.com/m3adn/sensor_pH-Arduino/blob/master/img/speak92.png)

