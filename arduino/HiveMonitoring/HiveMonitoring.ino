#include <DHT.h>
#include <SPI.h>
#include <WiFiNINA.h>

#define SENSOR_ERROR_VALUE (-404.0)

#define REQUEST_PERIOD (5000)
#define MOTION_PERIOD (100)

#define DURATION_LIMIT (60)
#define PERIOD_LIMIT (120)

#define __BOUNDARY "xxx"
#define __FORM_DATA "Content-Disposition: form-data; name="
#define __DELIMITER "\r\n--xxx\r\n"

//PINOUT
#define RELAY_PIN 7

#define MOTION_SENSOR_1_PIN 8
#define MOTION_SENSOR_2_PIN 9
#define MOTION_SENSOR_3_PIN 10
#define MOTION_SENSOR_4_PIN 11

int State1 = LOW;
int State2 = LOW;
int State3 = LOW;
int State4 = LOW;

//------Request variable---------------------
char PostResponse[300];
char PostHeader[300];
char PostData[2000];

//------Sensor variables --------------------
int counter = 0;

int fan = 0;
int period = 2;
int duration = 1;

//Motion sensors
int motion1 = 0;
int motion2 = 0;
int motion3 = 0;
int motion4 = 0;

//Sensor 1
float humidity1 = 0;
float tempc1 = 0;

//Sensor 2
float humidity2 = 0;
float tempc2 = 0;

//Sensor 3
float humidity3 = 0;
float tempc3 = 0;

//Sensor 4
float humidity4 = 0;
float tempc4 = 0;

//Sensor 5
float humidity5 = 0;
float tempc5 = 0;

//Sensor 6
float humidity6 = 0;
float tempc6 = 0;

//Temperature and humidity sensors
#define DHTPIN1 1
#define DHTTYPE1 DHT22   
DHT dht1(DHTPIN1, DHTTYPE1);

#define DHTPIN2 2
#define DHTTYPE2 DHT22   
DHT dht2(DHTPIN2, DHTTYPE2);

#define DHTPIN3 3
#define DHTTYPE3 DHT22   
DHT dht3(DHTPIN3, DHTTYPE3);

#define DHTPIN4 4
#define DHTTYPE4 DHT22   
DHT dht4(DHTPIN4, DHTTYPE4);

#define DHTPIN5 5
#define DHTTYPE5 DHT22   
DHT dht5(DHTPIN5, DHTTYPE5);

#define DHTPIN6 6
#define DHTTYPE6 DHT22   
DHT dht6(DHTPIN6, DHTTYPE6);

//Timers
unsigned long tmr1 = 0;
unsigned long tmr2 = 0;
unsigned long tmr3 = 0;
unsigned long tmr4 = 0;

//Timer to control fan
uint32_t fan_timer = millis();
uint32_t last_request = millis();
uint32_t last_motion = millis();

//-------WiFi variables ---------

//char ssid[] = "robomaster";
//char pass[] = "robomaster";

//char ssid[] = "Apis";
//char pass[] = "apismed2";

char ssid[] = "beekeeping";
char pass[] = "";

//char ssid[] = "Masha";
//char pass[] = "Stepa1988";

//char ssid[] = "Poopbot";
//char pass[] = "11111111";

int status = WL_IDLE_STATUS;
IPAddress server(134,209,90,29);

WiFiClient client;

//----------SETUP-----------------
void setup() {
  Serial.begin(9600);
    while (!Serial) { }
  
  //----------------setup wifi----------
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to WiFi");
  printWifiStatus();

  //------------------setup sensors -------
  //Fan output
  pinMode (RELAY_PIN, OUTPUT);

  //Motion sensors
  pinMode (8, INPUT); 
  pinMode (9, INPUT); 
  pinMode (10, INPUT);  
  pinMode (11, INPUT);

  //Init DHT sensors
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  dht5.begin();
  dht6.begin();

}

//---------LOOP---------------------------------------

void loop() {

//----------------------Read motion sensors -----------
  if (millis()-last_motion > MOTION_PERIOD) {
    last_motion = millis();

    //Read motion sensors
    int motionState1 = digitalRead(MOTION_SENSOR_1_PIN);
    if ( motionState1 == LOW) {
      if (State1 == LOW) {
        motion1++;
        State1 = HIGH;
      }
    }
    else {
      State1 = LOW;
    }

    int motionState2 = digitalRead(MOTION_SENSOR_2_PIN);
    if ( motionState2 == LOW) {
      if (State2 == LOW) {
        motion2++;
        State2 = HIGH;
      }
    }
    else {
      State2 = LOW;
    }

    int motionState3 = digitalRead(MOTION_SENSOR_3_PIN);
    if ( motionState3 == LOW) {
      if (State3 == LOW) {
        motion3++;
        State3 = HIGH;
      }
    }
    else {
      State3 = LOW;
    }

    int motionState4 = digitalRead(MOTION_SENSOR_4_PIN);
    if ( motionState4 == LOW) {
      if (State4 == LOW) {
        motion4++;
        State4 = HIGH;
      }
    }
    else {
      State4 = LOW;
    }
  }

//---------------------------------------
  if (millis()-last_request > REQUEST_PERIOD) {
    last_request = millis();

    const char* p1 = "duration=";
    const char* p2 = "period=";

    while (client.available()) {
      String line = client.readStringUntil('\n');
      char* begin = strstr(line.c_str(),p1);
      if (begin != NULL) {
        
        //Serial.println("@@@");
        Serial.println(begin);
        char* end = strchr(line.c_str(), '&');
        //Serial.println(end);
        int n = end - begin -strlen(p1);
        char d[n+1];
        strncpy(d, begin+strlen(p1), n);
        d[n]='\0';
        begin = end+1;
        //Serial.println(n);
        if (begin != NULL) {
          end = begin + strlen(begin);  
          n = end - begin -strlen(p2); 
         
          //Serial.println(n);
          char p[n+1];    
          strncpy(p, begin+strlen(p2), n); 
           p[n]='\0' ;
          //Serial.print("d: ");
          //Serial.println(d);
          //Serial.print("p: ");
          //Serial.println(p);
          //Serial.print("atoi(d): ");
          //Serial.println(atoi(d));
          //Serial.print("atoi(p): ");
          //Serial.println(atoi(p));
          int dt = atoi(d);
          int pt = atoi(p);
          if (dt>0 && dt<DURATION_LIMIT &&
              pt>0 && pt<PERIOD_LIMIT &&
              dt<=pt) {
                duration = dt;
                period = pt;
          }
        }
        
      }
    }


    Serial.print("duration=");
    Serial.println(duration);
    Serial.print("period=");
    Serial.println(period);

    //Fan control

    if (millis()-fan_timer < (uint32_t)duration*1000*60) {
      digitalWrite(RELAY_PIN, HIGH);
      fan = 1;
      Serial.print("FAN ");
    }
    else if (millis()-fan_timer > (uint32_t)period*1000*60) {
      fan_timer = millis();
      Serial.print("RESET ");
    }
    else {
      digitalWrite(RELAY_PIN, LOW);
      fan = 0;
      Serial.print("NO FAN ");  
    }
    Serial.print(" (");
    Serial.print(fan_timer);
    Serial.print(" / ");
    Serial.print(millis());
    Serial.println(")");

    //-----------------------------------
    //Read DHT sensors
    //Sensor 1
    float humidity1 = dht1.readHumidity();
    if (isnan(humidity1)) humidity1 = SENSOR_ERROR_VALUE;
    float tempc1 = dht1.readTemperature();
    if (isnan(tempc1)) tempc1 = SENSOR_ERROR_VALUE;

    //Sensor 2
    float humidity2 = dht2.readHumidity();
    if (isnan(humidity2)) humidity2 = SENSOR_ERROR_VALUE;
    float tempc2 = dht2.readTemperature();
    if (isnan(tempc2)) tempc2 = SENSOR_ERROR_VALUE;

    //Sensor 3
    float humidity3 = dht3.readHumidity();
    if (isnan(humidity3)) humidity3 = SENSOR_ERROR_VALUE;
    float tempc3 = dht3.readTemperature();
    if (isnan(tempc3)) tempc3 = SENSOR_ERROR_VALUE;

    //Sensor 4
    float humidity4 = dht4.readHumidity();
    if (isnan(humidity4)) humidity4 = SENSOR_ERROR_VALUE;
    float tempc4 = dht4.readTemperature();
    if (isnan(tempc4)) tempc4 = SENSOR_ERROR_VALUE;

    //Sensor 5
    float humidity5 = dht5.readHumidity();
    if (isnan(humidity5)) humidity5 = SENSOR_ERROR_VALUE;
    float tempc5 = dht5.readTemperature();
    if (isnan(tempc5)) tempc5 = SENSOR_ERROR_VALUE;

    //Sensor 6
    float humidity6 = dht6.readHumidity();
    if (isnan(humidity6)) humidity6 = SENSOR_ERROR_VALUE;
    float tempc6 = dht6.readTemperature();
    if (isnan(tempc6)) tempc6 = SENSOR_ERROR_VALUE;
    Serial.println(humidity1);
    Serial.println(humidity2);
    Serial.println(humidity3);
    Serial.println(humidity4);
    Serial.println(humidity5);
    Serial.println(humidity6);


    //-----------------------Communication part------------------------
    client.stop();
    
    counter++;

    sprintf(PostData,
            "%s%s\"fan\"\r\n\r\n%d"
            "%s%s\"period\"\r\n\r\n%d"
            "%s%s\"duration\"\r\n\r\n%d"
            "%s%s\"audio1\"\r\n\r\n%d"
            "%s%s\"audio2\"\r\n\r\n%d"
            "%s%s\"audio3\"\r\n\r\n%d"
            "%s%s\"audio4\"\r\n\r\n%d"
            "%s%s\"motion1\"\r\n\r\n%d"
            "%s%s\"motion2\"\r\n\r\n%d"
            "%s%s\"motion3\"\r\n\r\n%d"
            "%s%s\"motion4\"\r\n\r\n%d"
            "%s%s\"humidity1\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"humidity2\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"humidity3\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"humidity4\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"humidity5\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"humidity6\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"tempc1\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"tempc2\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"tempc3\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"tempc4\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"tempc5\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"tempc6\"\r\n\r\n%d.%01d\r\n"
            "--%s--",
            __DELIMITER, __FORM_DATA, fan,
            __DELIMITER, __FORM_DATA, period, 
            __DELIMITER, __FORM_DATA, duration, 
            __DELIMITER, __FORM_DATA, analogRead(1),
            __DELIMITER, __FORM_DATA, analogRead(2),
            __DELIMITER, __FORM_DATA, analogRead(3),
            __DELIMITER, __FORM_DATA, analogRead(4),
            __DELIMITER, __FORM_DATA, motion1,
            __DELIMITER, __FORM_DATA, motion2,
            __DELIMITER, __FORM_DATA, motion3,
            __DELIMITER, __FORM_DATA, motion4,
            __DELIMITER, __FORM_DATA, int(humidity1), (int)(humidity1*100)%100,
            __DELIMITER, __FORM_DATA, int(humidity2), (int)(humidity2*100)%100, 
            __DELIMITER, __FORM_DATA, int(humidity3), (int)(humidity3*100)%100, 
            __DELIMITER, __FORM_DATA, int(humidity4), (int)(humidity4*100)%100, 
            __DELIMITER, __FORM_DATA, int(humidity5), (int)(humidity5*100)%100, 
            __DELIMITER, __FORM_DATA, int(humidity6), (int)(humidity6*100)%100,
            __DELIMITER, __FORM_DATA, int(tempc1), (int)(tempc1*100)%100,
            __DELIMITER, __FORM_DATA, int(tempc2), (int)(tempc2*100)%100, 
            __DELIMITER, __FORM_DATA, int(tempc3), (int)(tempc3*100)%100, 
            __DELIMITER, __FORM_DATA, int(tempc4), (int)(tempc4*100)%100, 
            __DELIMITER, __FORM_DATA, int(tempc5), (int)(tempc5*100)%100, 
            __DELIMITER, __FORM_DATA, int(tempc6), (int)(tempc6*100)%100,
            __BOUNDARY
    );
    
    if (client.connect(server, 80)) {
      //Serial.println("POST Request:");
      sprintf(PostHeader, 
              "POST /monitoring/publish/ HTTP/1.1\r\n"
              "Host: 134.209.90.29:80\r\n"
              "User-Agent: ArduinoWiFi/1.1\r\n"
              "Accept:*/*\r\n"
              "Accept-Encoding: gzip, deflate, br\r\n"
              "Connection: keep-alive\r\n"
              "Content-Length: %d\r\n"
              "Content-Type: multipart/form-data; boundary=%s\r\n",
              strlen(PostData), __BOUNDARY
      );
      
      //Serial.print(PostHeader);
      //Serial.println(PostData);

      client.print(PostHeader);
      client.println(PostData);

    } else {
      // if you couldn't make a connection:
      Serial.println("connection failed");
    }
  }
  
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}