# CavliApplication

Cavli Wireless C16QS Modülü için STM32 işlemcisi ile yapılmış ve Cavli Wireless C16QS EVK kartı ile çalışan kodları içerir.
Uygulamada NUCLEO-G071RB kartı kullanılmıştır.

# CavliC16QSEVK-MQTT-1

Ugulama ile ilgili Uygulama Notu dokümanını Proje klasörleri içerisinde bulunan Documents klasöründe bulabilirsiniz.
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


# CavliC16QSEVK-TCP-1

Bu uygulamada normal modda TCP sunucusuna bağlantı yapılmış ve veri gönderilmiştir.

Ugulama ile ilgili Uygulama Notu dokümanını Proje klasörleri içerisinde bulunan Documents klasöründe bulabilirsiniz.
Uygulamada kullanıcağınız TCP sunucu bilgilerini CavliLib klasörü içerisinde bulunan C16QS.h header dosyasında ilgili alana girmeniz gerekir.

/* TCP Informations */

#define DestinationIP			"111.222.333.444"

#define DestinationPort			(5555)

# CavliC16QSEVK-TCP-2

Bu uygulamada transparan modda TCP sunucusuna bağlantı yapılmış ve veri gönderilmiştir.

Ugulama ile ilgili Uygulama Notu dokümanını Proje klasörleri içerisinde bulunan Documents klasöründe bulabilirsiniz.
Uygulamada kullanıcağınız TCP sunucu bilgilerini CavliLib klasörü içerisinde bulunan C16QS.h header dosyasında ilgili alana girmeniz gerekir.

/* TCP Informations */

#define DestinationIP			"111.222.333.444"

#define DestinationPort			(5555)

