# CavliApplication
# STM32 Uygulamaları

Cavli Wireless C16QS Modülü için STM32 işlemcisi ile yapılmış ve Cavli Wireless C16QS EVK kartı ile çalışan kodları içerir.
Uygulamada NUCLEO-G071RB kartı kullanılmıştır.

Nucleo ile kullanımda Cavli C16QS EVK kartı üzerinde bulunan J12 jumper'larını çıkarmanız gerekmektedir.

## CavliC16QSEVK-MQTT-1

Ugulama ile ilgili Uygulama Notu dokümanını Proje klasörleri içerisinde bulunan Documents klasöründe bulabilirsiniz.
Uygulamada kullanacağınız MQTT broker bilgilerini CavliLib klasörü içerisinde bulunan C16QS.h header dosyasında ilgili alana girmeniz gerekir.

/* MQTT Broker Informations */
```c
#define BrokerAddress			"broker.test.net"

#define BrokerPortNumber		"1883"

#define UserName			"UserName"

#define Password			"123456"

#define ClientID			"TEST"

#define LastWillTopic			"LWT"

#define LastWillMessage			"LWM"

#define DestinationTopic		"TestTopic"

#define SubscribeTopic			"SubTopic"
```

## CavliC16QSEVK-TCP-1

Bu uygulamada normal modda TCP sunucusuna bağlantı yapılmış ve veri gönderilmiştir.

Ugulama ile ilgili Uygulama Notu dokümanını Proje klasörleri içerisinde bulunan Documents klasöründe bulabilirsiniz.
Uygulamada kullanıcağınız TCP sunucu bilgilerini CavliLib klasörü içerisinde bulunan C16QS.h header dosyasında ilgili alana girmeniz gerekir.

/* TCP Informations */
```c

#define DestinationIP			"111.222.333.444"

#define DestinationPort			(5555)

```

## CavliC16QSEVK-TCP-2

Bu uygulamada transparan modda TCP sunucusuna bağlantı yapılmış ve veri gönderilmiştir.

Ugulama ile ilgili Uygulama Notu dokümanını Proje klasörleri içerisinde bulunan Documents klasöründe bulabilirsiniz.
Uygulamada kullanıcağınız TCP sunucu bilgilerini CavliLib klasörü içerisinde bulunan C16QS.h header dosyasında ilgili alana girmeniz gerekir.

/* TCP Informations */
```c

#define DestinationIP			"111.222.333.444"

#define DestinationPort			(5555)
```

## CavliC16QSEVK-GPIO7-1

Bu uygulamada C16QS modülünün GPIO7 output pini 1sn aralıklar ile high ve low yapılmıştır. Bu pin çıkışı 1.8VDC olduğundan dolayı
bir LED sürmek için veya başka bir amaçta kullanım için transistör kullanılarak anahtarlama yapılması gerekmektedir.

Ugulama ile ilgili Uygulama Notu dokümanını Proje klasörleri içerisinde bulunan Documents klasöründe bulabilirsiniz.

# Arduino Uygulamaları

Cavli Wireless C16QS Modülü için Arduino uno ile yapılmış ve Cavli Wireless C16QS EVK kartı ile çalışan kodları içerir.
Uygulamada Arduino Uno kartı kullanılmıştır. Arduino Uno kartının donanımsal uart pinleri debug amaçlı kullanılmıştır. 
Cavli C16QS ile iletişimde Software Aerial kütüphanesi kullanıldığından dolayı Arduino uygulaması için C16QS modülünün
AT komut portunun hızı 19200 bps olarak değiştirilmiştir. Bu değişimi yapmak için Dokümanlar kalsöründe bulunan 
"C16QS Uart1 Baud Rate Değişimi Uygulama Notu 1.0" uygulama notunu kullanabilirisniz.


Arduno ile kullanımda Cavli C16QS EVK kartı üzerinde bulunan J12 jumper'larını çıkarmanız gerekmektedir.

## ArduinoC16QS-GPIO7-1

Bu uygulamada C16QS modülünün GPIO7 output pini 1sn aralıklar ile high ve low yapılmıştır. Bu pin çıkışı 1.8VDC olduğundan dolayı
bir LED sürmek için veya başka bir amaçta kullanım için transistör kullanılarak anahtarlama yapılması gerekmektedir.

Ugulama ile ilgili Uygulama Notu dokümanını Dokumanlar klasöründe bulabilirsiniz.


