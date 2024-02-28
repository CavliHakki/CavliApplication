# CavliApplication
# CavliC16QSEVK-MQTT-1

Cavli Wireless C16QS Modülü için STM32 işlemcisi ile yapılmış ve Cavli Wireless C16QS EVK kartı ile çalışan kodları içerir.

Uygulamada NUCLEO-G071RB kartı kullanılmıştır. 

Uygulamada kullanacağınız MQTT broker bilgilerini CavliLib klasörü içerisinde bulunan C16QS.h header dosyasında ilgili alana girmeniz gerekir.

/* MQTT Broker Informations */

#define BrokerAddress			  "broker.test.net"

#define BrokerPortNumber		"1883"

#define UserName				    "UserName"

#define Password				    "123456"

#define ClientID				    "TEST"

#define LastWillTopic			  "LWT"

#define LastWillMessage			"LWM"

#define DestinationTopic		"TestTopic"

#define SubscribeTopic			"SubTopic"

Uygulama notunu Documents klasöründe bulabilirsiniz.
