#include <DHT.h>
#include <SPI.h>
#include <WiFiNINA.h>

#define SENSOR_ERROR_VALUE (-404.0)

#define REQUEST_PERIOD (5000)

#define DURATION_LIMIT (60)
#define PERIOD_LIMIT (120)

#define __BOUNDARY "xxx"
#define __FORM_DATA "Content-Disposition: form-data; name="
#define __DELIMITER "\r\n--xxx\r\n"

//PINOUT
#define RELAY_PIN 7

//------Request variable---------------------
char PostResponse[300];
char PostHeader[300];
char PostData[2000];

//------Sensor variables --------------------
int counter = 0;

int fan = 0;
int period = 2;
int duration = 1;

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

//Sensor 7
float humidity7 = 0;
float tempc7 = 0;

//Sensor 8
float humidity8 = 0;
float tempc8 = 0;

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

#define DHTPIN7 8
#define DHTTYPE7 DHT22   
DHT dht7(DHTPIN7, DHTTYPE7);

#define DHTPIN8 9
#define DHTTYPE8 DHT22   
DHT dht8(DHTPIN8, DHTTYPE8);

//Timer to control fan
uint32_t fan_timer = millis();
uint32_t last_request = millis();

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

  //Init DHT sensors
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  dht5.begin();
  dht6.begin();
  dht7.begin();
  dht8.begin();
}

//---------LOOP---------------------------------------

void loop() {

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

    //Sensor 7
    float humidity7 = dht7.readHumidity();
    if (isnan(humidity7)) humidity7 = SENSOR_ERROR_VALUE;
    float tempc7 = dht7.readTemperature();
    if (isnan(tempc7)) tempc7 = SENSOR_ERROR_VALUE;

    //Sensor 8
    float humidity8 = dht8.readHumidity();
    if (isnan(humidity8)) humidity8 = SENSOR_ERROR_VALUE;
    float tempc8 = dht8.readTemperature();
    if (isnan(tempc8)) tempc8 = SENSOR_ERROR_VALUE;

    Serial.println(humidity1);
    Serial.println(humidity2);
    Serial.println(humidity3);
    Serial.println(humidity4);
    Serial.println(humidity5);
    Serial.println(humidity6);
    Serial.println(humidity7);
    Serial.println(humidity8);

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
            "%s%s\"humidity1\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"humidity2\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"humidity3\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"humidity4\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"humidity5\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"humidity6\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"humidity7\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"humidity8\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"tempc1\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"tempc2\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"tempc3\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"tempc4\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"tempc5\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"tempc6\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"tempc7\"\r\n\r\n%d.%01d\r\n"
            "%s%s\"tempc8\"\r\n\r\n%d.%01d\r\n"
            "--%s--",
            __DELIMITER, __FORM_DATA, fan,
            __DELIMITER, __FORM_DATA, period, 
            __DELIMITER, __FORM_DATA, duration, 
            __DELIMITER, __FORM_DATA, analogRead(1),
            __DELIMITER, __FORM_DATA, analogRead(2),
            __DELIMITER, __FORM_DATA, analogRead(3),
            __DELIMITER, __FORM_DATA, analogRead(4),
            __DELIMITER, __FORM_DATA, int(humidity1), (int)(humidity1*100)%100,
            __DELIMITER, __FORM_DATA, int(humidity2), (int)(humidity2*100)%100, 
            __DELIMITER, __FORM_DATA, int(humidity3), (int)(humidity3*100)%100, 
            __DELIMITER, __FORM_DATA, int(humidity4), (int)(humidity4*100)%100, 
            __DELIMITER, __FORM_DATA, int(humidity5), (int)(humidity5*100)%100, 
            __DELIMITER, __FORM_DATA, int(humidity6), (int)(humidity6*100)%100,
            __DELIMITER, __FORM_DATA, int(humidity7), (int)(humidity7*100)%100, 
            __DELIMITER, __FORM_DATA, int(humidity8), (int)(humidity8*100)%100,
            __DELIMITER, __FORM_DATA, int(tempc1), (int)(tempc1*100)%100,
            __DELIMITER, __FORM_DATA, int(tempc2), (int)(tempc2*100)%100, 
            __DELIMITER, __FORM_DATA, int(tempc3), (int)(tempc3*100)%100, 
            __DELIMITER, __FORM_DATA, int(tempc4), (int)(tempc4*100)%100, 
            __DELIMITER, __FORM_DATA, int(tempc5), (int)(tempc5*100)%100, 
            __DELIMITER, __FORM_DATA, int(tempc6), (int)(tempc6*100)%100,
            __DELIMITER, __FORM_DATA, int(tempc7), (int)(tempc7*100)%100, 
            __DELIMITER, __FORM_DATA, int(tempc8), (int)(tempc8*100)%100,
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