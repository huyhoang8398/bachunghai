

#include <SoftwareSerial.h>
#include <Wire.h>
#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;
float val;
SoftwareSerial Gsm(7,8);
void setup()
{
pinMode(LED,OUTPUT);
  Serial.begin(9600);
  Gsm.begin(9600);
   static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float val;
  static float voltage;
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      val = 3.5*voltage+Offset;
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    Serial.print("Voltage:");
        Serial.print(voltage,2);
        Serial.print("    pH value: ");
    Serial.println(val,2);
        digitalWrite(LED,digitalRead(LED)^1);
        printTime=millis();
  }
}
double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
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
 void loop() {
      Serial.println("HTTP get method :");
    Serial.println("AT+SAPBR=0,1\\r\\n");
    Gsm.println("AT+SAPBR=0,1"); /* close GPRS context */
    delay(1000);
    ShowSerialData();
    delay(500);
    /* Configure bearer profile 1 */
    Serial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\\r\\n");    
    Gsm.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");  /* Connection type GPRS */
    delay(500);
    ShowSerialData();
    delay(500);
    Serial.println("AT+SAPBR=3,1,\"APN\",\"v-internet\"\\r\\n");  
    Gsm.println("AT+SAPBR=3,1,\"APN\",\"v-internet\"");  /* APN of the provider */
    delay(500);
    ShowSerialData();
    delay(500);
    
    Serial.println("AT+SAPBR=1,1\\r\\n");
    Gsm.println("AT+SAPBR=1,1"); /* Open GPRS context */
    delay(2000);
    ShowSerialData();
    delay(500);
    Serial.println("AT+SAPBR=2,1\\r\\n");
    Gsm.println("AT+SAPBR=2,1"); /* Query the GPRS context */
    delay(1000);
    ShowSerialData();
    delay(1000);
    Serial.print("AT+HTTPINIT\\r\\n");
    Gsm.println("AT+HTTPINIT"); /* Initialize HTTP service */
    delay(1000); 
    ShowSerialData();
    delay(1000);
    Serial.print("AT+HTTPPARA=\"CID\",1\\r\\n");
    Gsm.println("AT+HTTPPARA=\"CID\",1");  /* Set parameters for HTTP session */
    delay(500);
    ShowSerialData();
    delay(500);
    Serial.println("AT+HTTPPARA=\"URL\",\"http://scg.sontg.net/pH/insert_pH.php\"\\r\\n");
    Gsm.print("AT+HTTPPARA=\"URL\",\"http://scg.sontg.net/pH/insert_pH.php?");
      Gsm.print("&val=");
    Gsm.print(val,2);
    Gsm.print("\"\r\n");
    delay(1000);
    ShowSerialData();
    delay(2000);
    Serial.print("AT+HTTPACTION=0\\r\\n");
    Gsm.println("AT+HTTPACTION=0");  /* Start GET session */
    delay(4000);
    ShowSerialData();
    delay(500);
    Serial.print("AT+HTTPTERM\\r\\n");  
    Gsm.println("AT+HTTPTERM");  /* Terminate HTTP service */
    delay(500);
    ShowSerialData();
    delay(2000);
   
}  
void ShowSerialData()
{
  while(Gsm.available()!=0)  /* If data is available on serial port */
  Serial.write(char (Gsm.read())); /* Print character received on to the serial monitor */
}
