#include <SoftwareSerial.h>

#define ATrxPin       2
#define ATtxPin       3

#define GNSSrxPin   4
#define GNSStxPin   8

#define PowerKeyPin 9
#define ResetPin    7

SoftwareSerial c16qs(ATrxPin,ATtxPin);

typedef enum{
  powerkey_step,s_reset,GPS_enable_step,GPS_enable_response_step,GPS_port_change,GPS_data_wait
}C16QS_state_e;

C16QS_state_e C16QS_state;

void PowerKey_On(){
digitalWrite (PowerKeyPin, HIGH);
}

void PowerKey_Off(){
  digitalWrite (PowerKeyPin, LOW);
}

void ResetKey_On(){
digitalWrite (ResetPin, HIGH);
}

void ResetKey_Off(){
  digitalWrite (ResetPin, LOW);
}

void setup() {
  Serial.begin(115200);
  c16qs.begin(19200);

  pinMode(PowerKeyPin, OUTPUT);
  pinMode(ResetPin, OUTPUT);
  C16QS_state = powerkey_step;

}


void C16QS_Process(){
  switch(C16QS_state) {
    // power key sinyalinin gonderilmesi
    case powerkey_step:
      PowerKey_On();
      delay(200);
      PowerKey_Off();
      delay(200);
      C16QS_state = s_reset;
    break;
    // reset sinyalinin gonderilmesi
    case s_reset:
      ResetKey_On();
      delay(200);
      ResetKey_Off();
      delay(1000);
      C16QS_state = GPS_enable_step;
      break;
    // GPS ozelliginin aktif edilmesi icin komut gonderilir.
    case GPS_enable_step:
      c16qs.print("AT+CGPS=1\r\n");  
      delay(500);    
      C16QS_state = GPS_enable_response_step;
    break;
    // GPS aktif etme komutunun cevabi alinir.
    case GPS_enable_response_step:
     if(c16qs.available()){
        String received_data = c16qs.readString();
        Serial.print(received_data); 
        C16QS_state = GPS_port_change;
      break;
      }
      C16QS_state = GPS_enable_response_step;
    break;
    // GPS verilerini AT komut portuna yonlendirir.
    case GPS_port_change:     
      c16qs.print("AT+GPSPORT=1\r\n");  
      delay(500);       
      C16QS_state = GPS_data_wait;
    break;
    // gelen GPS verilerini Serial Monitore gonderir.
    case GPS_data_wait:
      if(c16qs.available()){
        String received_data = c16qs.readStringUntil('$');
        Serial.print(received_data); 
        Serial.print("\r\n");
      break;
      }
      C16QS_state = GPS_data_wait;
    break;

    default:
    break;
  }

}

void loop() {
  C16QS_Process();
}
