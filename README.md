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

# Esquema
![enter image description here](https://lh3.googleusercontent.com/qNMRFRwnUz0LkI6adyLxoOEOJPc7dhFehort_Zn1ngMfG2Xe07a_G5ebN_ox7P8JjJSBX91ehbNyRQn9mW76VL0Sv08IIgdqiYyGs3aUEYlJpq0saiRhw1wvmraEquFwOCsbVny-7MdjCEtUeKuALIqDq0I5VPhH6AA9sdvP6oqNfMGNd2R7ROXLh0ATiA43LSP1zFebsN8fR4vPdfUJIqy_TkAKBKdPk2wC0sQr8t786Le20mIIVQMAvU_AlXsjGQP7xw3L09F0S-2J4zTDmze9JavJJHjc64-r-A7OSGDqvTgAds135Z2tNnlbWbvZJTdsBsMP566EYKgBpochpyqlig1pIf6MH_4w__x2cLszf5ilPLX3Vbrkr0-21imLD4oZKIOPapBm6470YQLWq7qMRh9ARgfl0IfmFHHFUeP7viBXYK9MgbJQGxjO7d_bh3QUjZ6FMm20Pfq4TV18lYpiW8t7KUVKol_dLTZ55khI4VBvBhI0P24RRyyof9YmYkr4M_XRHWPm6CjmPn85_lFQAzEWvIfbs9iva9OWWNUayhcdNpJpLGA-ua3_7Vp2lqZ3pRW5IK5LMxQBSxqSQl8wodM8DL0QztVBRGfM_GDhOzJQ9jxy_Re09ubuX_u4w1uPuj0v9c3trn-DYq6Sl9Uzfu-Fmsk1HislygzXXFB0rAuGpLqPzE5zzpG_4xF7w6Qipws_cN_DoYlI5bDddWpi1m-6chOzWm_g04UQQZQMZlQHnz802ktZezCKPaz01m6knVac2hCron8k_0h3J9Vbe6fzOm1b1d2Cbww2Yokc2QuGX6Ryy5fRDBmcYEFrEhhZDV_sBb3cYPx1oEgShUJD8Mvai5mVgMYlyFxUaga_wXP-ZUMTPN_KRfITT7sonyOQHAZPS0PFVUPCoq4mBMRo6dZ7Oo_dKQ5NK2NtV97Wyc_52g=w576-h667-s-no?authuser=0)

[Clique aqui](https://youtube.com/shorts/T29lxYrhNkw) para ver o vídeo do projeto funcionando.


# Referências
- https://www.instructables.com/ESP32-Smart-Home-System-With-Alexa-App-Manual-Swit/
- https://thingsbr.com.br/introducao-ao-esp32-programe-o-esp32-usando-o-arduino-ide-para-piscar-um-led/#:~:text=No%20ESP32%2C%20o%20LED%20interno,o%20intervalo%20alterando%20o%20atraso.
- https://docs.arduino.cc/built-in-examples/basics/DigitalReadSerial
