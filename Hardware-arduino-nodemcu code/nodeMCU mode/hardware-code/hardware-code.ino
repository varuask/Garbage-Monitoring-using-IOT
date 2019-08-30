



//**********************************adding all the important libraries******************//
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>




//**********************************defining the major pins****************************//
#define TRIGGERPIN1 D1
#define ECHOPIN1    D2
#define TRIGGERPIN2 D3
#define ECHOPIN2    D4
#define BLYNK_PRINT Serial

//***********************************mobile app config*******************************//
char auth[] = "76d218581cfb4345886eeeecea3c18a3";

//***********************************WiFi credentials *******************************//
char ssid[] = "lazybug.com";
char pass[] = "2822sarwal";
const char* host   = "nagarnigam.000webhostapp.com";

//***********************************mobile app settings****************************//
WidgetLCD lcd(V1);
WidgetLCD lcd1(V2);


void setup()
{

  
  Serial.begin(115200); //setting the data rate for communication with the computer
  delay(100);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  //**************************WiFi settings*****************************************//
  WiFi.begin(ssid,pass);
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  

  //****************************setting up pins for our function*******************//
  pinMode(TRIGGERPIN1, OUTPUT);//mode of trigger pin as output
  pinMode(TRIGGERPIN2, OUTPUT);
  pinMode(ECHOPIN1, INPUT);//mode of echopin is set as input as it takes the input
  pinMode(ECHOPIN2, INPUT);
  Blynk.begin(auth, ssid, pass); //to start the app
  //mobile app settings
  lcd.clear(); //Use it to clear the LCD Widget lcd
  lcd1.clear();//use it to clear the LCD widget lcd1
  lcd.print(0, 0, "Distance in cm"); // setting the caption for the LCD widget lcd before the looping begins to check response
  lcd1.print(0,0,"Garbage level"); //setting the caption for the LCD widget lcd1 before the looping begins to check the response
}

void loop()
{
  //*************************************mobile app settings*****************************//
  lcd.clear(); //clears away the garbage data from lcd
  lcd1.clear();//clears away the garbage data from lcd1
  lcd.print(0, 0, "Distance in cm"); //caption is again set
  lcd1.print(0,0,"GARBAGE LEVEL"); //caption is again set

  //*****************************************//
  float duration1, distance1,duration2,distance2; //setting the variables for our calculation of the data
  float percen1,percen2;
  String levels,levels2;
  
  digitalWrite(TRIGGERPIN1, LOW); //a low pulse is given out of the ultrasonic sensor 1
  digitalWrite(TRIGGERPIN2, LOW); //a low pulse is given out of the ultrasonic sensor 2
  delayMicroseconds(3); 
  digitalWrite(TRIGGERPIN1, HIGH); //a high pulse is given for 12 microseconds
  digitalWrite(TRIGGERPIN2, HIGH); //
  delayMicroseconds(12); 
  digitalWrite(TRIGGERPIN1, LOW); //then the pulse is again set to low
  digitalWrite(TRIGGERPIN2, LOW);
  duration1 = pulseIn(ECHOPIN1, HIGH); //reads the duration of time for which the echopin remains high
  duration2 = pulseIn(ECHOPIN2,HIGH);
  distance1 = (duration1/2) / 29.1; //converting that duration into distance using the formula d=s*t 
                                  //s is the speed of light in cm/s.
  lcd.print(7,1,distance1);
  Blynk.run();
  distance2 = (duration2/2) / 29.1;
  //********************lines of code for web******************************************//

  //****************************necessary calcualtions for tt dusbin 1********//
  if(distance1>35)
  {
    percen1=500;
    levels="uncertain";
  }
  else
  {
    percen1 = ((distance1)/35)*100;
    if(percen1>75)
    {
      levels="empty";
    }
    else if(percen1>40&&percen1<75)
    {
      levels="medium";
    }
    else
    {
      levels="full";
    }
  }


  //*************************************calcualtions for dusbin at sjt******************************************//
    if(distance2>35)
  {
    percen2=500;
    levels2="uncertain";
  }
  else
  {
    percen2 = ((distance2)/35)*100;
    if(percen2>75)
    {
      levels2="empty";
    }
    else if(percen2>40&&percen2<75)
    {
      levels2="medium";
    }
    else
    {
      levels2="full";
    }
  }
  //****************WiFi client object*************************************************//
  Serial.print("connecting to");
  Serial.println(host);
  WiFiClient client;
  const int httpPort = 80;
  if(!client.connect(host,httpPort))
  {
    Serial.println("connection failed");
    return;
  }
  //setting up the url for making the GET request....
  String url = "/api/insert.php?percent="+String(percen1)+"&"+"levels="+levels;
  String url2 = "/api/insert2.php?percent="+String(percen2)+"&"+"levels="+levels2;

  Serial.print("requesting url");
  Serial.println(url);
  

 //making a get request using the arduino..........
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  delay(5000);
  
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  delay(5000);
  
  Serial.println("req part 2");
  Serial.println(url2);
  client.print(String("GET ") + url2 + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  delay(5000);
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }             
 

  //*****************************************************mobile app displaying stuff****************************************************//
  if(distance1<=5)
  {
    lcd1.print(0,0,"LEVELS"); //caption is set
    lcd1.print(7,1,"FULL"); //displays full since the distance is less than 5 cm
    //this is a condition when the concerned authority needs to be notified about the levels via the email
    Blynk.email("priyan.jaiswal2016@vitstudent.ac.in","garbage monitor","please empty your dustbin as it is full");
  }
  else if(distance1>5&&distance1<25)
  {
    lcd1.print(0,0,"LEVELS");
    lcd1.print(7,1,"medium"); //displays medium accordingly
  }
  else
  {
    lcd1.print(0,0,"LEVELS");
    lcd1.print(7,1,"empty"); //displays empty accordingly
  }
  Serial.print(distance1); //to display the data on monitor
  Serial.println("Cm");
  lcd.print(7, 1, distance1); //to dislay the real time numerical data on the lcd widget
  lcd.print(7,1,"SAURAV");
 // Blynk.run(); //to run the blynk scripts

  delay(10000); //looping intervals are scheduled

}
