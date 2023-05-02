# Sensor de nível de água com aviso sonoro + integração com Amazon Alexa

Fiz esse projeto para resolver um problema de nível de degelo da minha geladeira, que quando não é retirada a água periodicamente, acaba tendo o vazamento. O intuito do projeto, é que toda vez que o reservatório ficar cheio, ele soe um beep (para ser bem chato mesmo, e saber que tem que esvaziar o compartimento) e envie uma notificação através de uma rotina configurada dentro do aplicativo Amazon Alexa. 

Para realizar este projeto, utilizei as seguintes ferramentas:

 - 1 ESP32
 - 1 Speaker
 - 1 Sensor de nível d'água horizontal ([Link no mercado livre](https://produto.mercadolivre.com.br/MLB-2804772868-boia-sensor-nivel-lateral-magnetico-agua-arduino-aquario-_JM?matt_tool=40343894&matt_word=&matt_source=google&matt_campaign_id=14303413655&matt_ad_group_id=133855953276&matt_match_type=&matt_network=g&matt_device=c&matt_creative=584156655519&matt_keyword=&matt_ad_position=&matt_ad_type=pla&matt_merchant_id=314253506&matt_product_id=MLB2804772868&matt_product_partition_id=1816238256222&matt_target_id=pla-1816238256222&gclid=CjwKCAjwxr2iBhBJEiwAdXECwxI-NaACetl7EbXQMweXqBxHr41G6_Rrz2nXC2FO8iF5Mfdu3ZfbtRoCfTwQAvD_BwE))
 - 1 Protoboard

# Dependências

Para fazer todas as integrações, foi utilizada as seguintes libs:

- [Sinric Pro](https://sinric.pro/pt-index.html)

## Preparação

Toda a etapa de configuração da ESP32 dentro a IDE do arduino, pode ser encontrada [clicando aqui](https://thingsbr.com.br/introducao-ao-esp32-programe-o-esp32-usando-o-arduino-ide-para-piscar-um-led/#:~:text=No%20ESP32,%20o%20LED%20interno,o%20intervalo%20alterando%20o%20atraso.). E seguindo para o tópico ***Preparing ESP32 in Arduino IDE***.

Outro passo importante para realizar esse projeto, é ter conta nessa API de integração com a Amazon Alexa e outros assistentes de voz, pode ser encontrada [clicando aqui](https://sinric.pro/pt-index.html). Usei como referência o conteúdo deste artigo [aqui](https://www.instructables.com/ESP32-Smart-Home-System-With-Alexa-App-Manual-Swit/), tópico 11 em diante.


# Código
Para utilizar o projeto, basta fazer o clone e fazer algumas atualizações no código:

Para se conectar ao Wifi, basta colocar as informações da sua rede Wifi nesses campos:

    /*** WIFI CONFIG */
    #define  WIFI_SSID  "YOUR_WIFI_SSID_HERE"
    #define  WIFI_PASS  "YOUR_WIFI_PASSWORD_HERE"

Após ter se cadastrado e feito o cadastro do seu dispositivo no **[Sinric Pro](https://sinric.pro/pt-index.html)**, ele vai disponibilizar 3 códigos, basta substituilos aqui:

    /*** SINRIC CONFIG */
    #define APP_KEY "YOUR-APP-KEY" // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
	#define APP_SECRET "YOUR-APP-SECRET" // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"
	#define CONTACT_ID "YOUR-DEVICE-ID" // Should look like "5dc1564130xxxxxxxxxxxxxx"

Neste trecho, defina quais pinos serão usados para ler e emitir as informações:

    /*** PINS CONFIG */
    #define  FLOATER_PIN  4
    #define  BEEP_PIN  22
    #define  EMIT_BEEP  true // Change to false to disable beep
    
Após trocar essas informações, o projeto deverá se conectar em sua rede Wifi, depois em sua conta do Sinric Pro, e iniciar o processo de envio e recebimento de informações. 

Neste trecho da função *loop*, é feita a verificação para caso o equipamento  esteja com o reservatório cheio, ele possa emitir um beep.

    void  loop()  {
		SinricPro.handle();
		
		if  (SinricPro.isConnected()){
			handleContactSensor();
			if(reservoir_is_full() && EMIT_BEEP)  beep(500);
		}else  if(!SinricPro.isConnected() && reservoir_is_full() && EMIT_BEEP){
			beep(500);
		}
	}
Para desabilitar, basta trocar *"EMIT_BEEP"* para "*false*".

# Esquema!
![Esquema pinos](https://user-images.githubusercontent.com/73892235/235707780-dd10f86d-b0bf-4cd1-9610-205d2a727f35.jpg)

[Clique aqui](https://youtube.com/shorts/T29lxYrhNkw) para ver o vídeo do projeto funcionando.


# Referências
- https://www.instructables.com/ESP32-Smart-Home-System-With-Alexa-App-Manual-Swit/
- https://thingsbr.com.br/introducao-ao-esp32-programe-o-esp32-usando-o-arduino-ide-para-piscar-um-led/#:~:text=No%20ESP32%2C%20o%20LED%20interno,o%20intervalo%20alterando%20o%20atraso.
- https://docs.arduino.cc/built-in-examples/basics/DigitalReadSerial
