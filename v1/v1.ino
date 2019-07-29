#include <SoftwareSerial.h>
//SoftwareSerial Serial(7,8);

void setup()
{
 Serial.begin(9600);  
 Serial.begin(9600);  
 delay(5000);
 
 Serial.println("AT+CGATT=1\r\n");
 delay(5000);
 printSerialData();
 
 Serial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n");//setting the SAPBR,connection type is GPRS
 delay(5000);
 printSerialData();
 
 Serial.println("AT+SAPBR=3,1,\"APN\",\"v-internet\"\r\n");//setting the APN,2nd parameter empty works for all networks 
 delay(5000);
 printSerialData();
 
 Serial.println("AT+SAPBR=1,1\r\n");
 delay(5000);
 printSerialData();

 Serial.println("AT+SAPBR=2,1\r\n");
 delay(5000);
 printSerialData();
 
 Serial.println("AT+HTTPINIT\r\n"); //init the HTTP request
 delay(5000); 
 printSerialData();

 Serial.println("AT+ CIFSR\r\n");
 delay(5000);
 printSerialData();
 
 Serial.println("AT+HTTPPARA=\"URL\",\"http://scg.sontg.net/pH/insert_pH.php?val=122\"\r\n");// setting the httppara, 
 //the second parameter is the website from where you want to access data 
 delay(5000);
 printSerialData();
 
 Serial.println("AT+HTTPACTION=1\r\n");//submit the POST request 
 delay(10000);//the delay is important if the return datas are very large, the time required longer.
 printSerialData();


 Serial.println("AT+HTTPTERM\r\n");// terminate HTTP service
// printSerialData();

}

void loop()
{
}


void printSerialData()
{
 while(Serial.available()!=0)
 Serial.write(Serial.read());
}
