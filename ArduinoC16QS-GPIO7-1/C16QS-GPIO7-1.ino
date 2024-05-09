#include <SoftwareSerial.h>

#define ATrxPin       2
#define ATtxPin       3

#define PowerKeyPin 9
#define ResetPin    7

SoftwareSerial c16qs(ATrxPin,ATtxPin);

typedef enum{
  SUCCESS = 0,
  ERROR = 1,
}ErrorStatus;

typedef enum {
	LogicLow = 0, LogicHigh = 1, LogicError = 2,
} LogicStatus_e;

typedef enum {
step_pwrkey,
step_rst,
step_echo_dis,
step_alive_control,
step_gpio7_status_control,
step_gpio7_set_low,
step_gpio7_set_high,
step_error,
} Step_e;

Step_e StepStatus;

void PowerKey_On();
void PowerKey_Off();
void PowerKeyFunction(void); 
void ResetKey_On();
void ResetKey_Off();
void ResetKeyFunction(void);
void Clear_Buffer(char* buf, int size);
void Echo_Disable(void);

ErrorStatus Alive_Control(void);
ErrorStatus Control_OK_Message(String buf);

LogicStatus_e GPIO7_Status_Control(void);
LogicStatus_e Control_GPIO7_Status_Message(String buf);

ErrorStatus GPIO7_Set(LogicStatus_e status);

void PowerKey_On(){
digitalWrite (PowerKeyPin, HIGH);
}

void PowerKey_Off(){
  digitalWrite (PowerKeyPin, LOW);
}

void PowerKeyFunction(void) {
	PowerKey_On();
	delay(100);
	PowerKey_Off();
}

void ResetKey_On(){
digitalWrite (ResetPin, HIGH);
}

void ResetKey_Off(){
  digitalWrite (ResetPin, LOW);
}

void ResetKeyFunction(void) {
  String received_data="";
	ResetKey_On();
	delay(200);
	ResetKey_Off();
  delay(1000);
  if(c16qs.available()){
    received_data = c16qs.readString();
  }
  Serial.print(received_data);
}

void Clear_Buffer(char* buf, int size){
  for(int i = 0;i<size;i++){
    buf[i] = 0;
  }
}

void Echo_Disable(void) {
  String received_data="";
	char buf[20];
	uint16_t size = 0;

  c16qs.print("ATE0\r\n"); 
	delay(500);
   if(c16qs.available()){
    received_data = c16qs.readString();
  }
  Serial.print(received_data);
}

ErrorStatus Alive_Control(void) {
  String received_data="";

	c16qs.print("AT\r\n");
	
	delay(500);
  if(c16qs.available()){
    received_data = c16qs.readString();
  }
  Serial.print(received_data);
	if (Control_OK_Message(received_data) == ERROR)
		return ERROR;

	return SUCCESS;
}

ErrorStatus Control_OK_Message(String buf) {
  if(buf == "\r\nOK\r\n"){
    return SUCCESS;
  }
	return ERROR;
}

LogicStatus_e GPIO7_Status_Control(void) {
	String received_data = "";
	LogicStatus_e status;
  c16qs.flush();
	c16qs.print("AT+GPSET?\r\n");
  Serial.print("AT+GPSET?\r\n");

	delay(1000);
  if(c16qs.available()){
    received_data = c16qs.readString();
  }
  Serial.print(received_data);
	status = Control_GPIO7_Status_Message(received_data);
  c16qs.flush();
	return status;
}

LogicStatus_e Control_GPIO7_Status_Message(String buf) {

	if (buf == "\r\n+GPSET: 0\r\n\r\nOK\r\n")
		return LogicLow;

	if (buf == "\r\n+GPSET: 1\r\n\r\nOK\r\n")
		return LogicHigh;

	return LogicError;
}

ErrorStatus GPIO7_Set(LogicStatus_e status) {
	String received_data = "";
  c16qs.flush();
	if(status == LogicHigh)
	  c16qs.print("AT+GPSET=1\r\n");

	if(status == LogicLow)
	  c16qs.print("AT+GPSET=0\r\n");

	delay(500);

  if(c16qs.available()){
    received_data = c16qs.readString();
  }
  Serial.print(received_data);
	//if (Control_OK_Message(received_data) == ERROR)
	//	return ERROR;
  c16qs.flush();
	return SUCCESS;
}


void setup() {
  Serial.begin(115200);
  c16qs.begin(19200);

  pinMode(PowerKeyPin, OUTPUT);
  pinMode(ResetPin, OUTPUT);
  StepStatus = step_pwrkey;
}

/* GPIO7 PIN UYGULAMASI */
void C16QS_GPIO7_Sequence(void) {

	switch (StepStatus) {
	case step_pwrkey:
		PowerKeyFunction();
		delay(500);
    Serial.print("PowerKeyFunction\r\n");
		StepStatus = step_rst;
		break;

	case step_rst:
    Serial.print("ResetKeyFunction\r\n");
		ResetKeyFunction();
		StepStatus = step_echo_dis;
		break;

	case step_echo_dis:
    Serial.print("Echo_Disable\r\n");
		Echo_Disable();
		StepStatus = step_alive_control;
		break;

	case step_alive_control:
		if (Alive_Control() == ERROR){
      Serial.print("ERROR\r\n");
      StepStatus = step_pwrkey;
    }		
		else{
      Serial.print("success\r\n");
			StepStatus = step_gpio7_status_control;
    }
		break;

	case step_gpio7_status_control:
		if (GPIO7_Status_Control() == LogicHigh) {
			delay(1000);
			StepStatus = step_gpio7_set_low;
      Serial.print("LogicLow\r\n");
			break;
		} else if (GPIO7_Status_Control() == LogicLow) {
			delay(1000);
			StepStatus = step_gpio7_set_high;
      Serial.print("LogicHigh\r\n");
			break;
		}
	//	StepStatus = step_error;
		break;

	case step_gpio7_set_low:
		if (GPIO7_Set(0) == ERROR) {
			StepStatus = step_error;
			break;
		}
		StepStatus = step_gpio7_status_control;
		break;

	case step_gpio7_set_high:
		if (GPIO7_Set(1) == ERROR) {
			StepStatus = step_error;
			break;
		}
		StepStatus = step_gpio7_status_control;
		break;

	case step_error:
		StepStatus = step_error;
		break;

	default:
		break;
	}

}

void loop() {
  C16QS_GPIO7_Sequence();
  delay(10);
}
