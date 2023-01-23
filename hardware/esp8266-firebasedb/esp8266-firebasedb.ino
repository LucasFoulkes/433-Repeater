#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

int timestamp;

#define WIFI_SSID "TheRustic_ec"
#define WIFI_PASSWORD "TheRusticFood"
#define API_KEY "AIzaSyBTKvFNDLdZmG9-hlkZiLdIUmR6k4lWW5U"
#define USER_EMAIL "lucas@foulkes.studio"
#define USER_PASSWORD "771421Saddog@"
#define DATABASE_URL "https://rustic-restaurant-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "DATABASE_SECRET"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

static byte ndx = 0;
char endMarker = '\n';
char rc;
int count = 0;
boolean newData = false;
const byte numChars = 25;
char receivedChars[numChars];
String oldChars;
unsigned long dataMillis = 0;

unsigned long getTime()
{
    timeClient.update();
    unsigned long now = timeClient.getEpochTime();
    return now;
}

void setup()
{

    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.database_url = DATABASE_URL;
    Firebase.reconnectWiFi(true);
    fbdo.setResponseSize(4096);
    String base_path = "/UsersData/";
    config.token_status_callback = tokenStatusCallback;
    config.max_token_generation_retry = 5;
    Firebase.begin(&config, &auth);
    timeClient.begin();
}

void showNewData()
{
    if (newData == true)
    {
        newData = false;
    }
}

void loop()
{
    String path = "/rustic/";
    // path += auth.token.uid.c_str(); //<- user uid of current user that sign in with Emal/Password
    if (Firebase.ready())
        while (Serial.available() > 0 && newData == false)
        {
            rc = Serial.read();
            if (rc != endMarker)
            {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars)
                {
                    ndx = numChars - 1;
                }
            }
            else
            {
                receivedChars[ndx] = '\0';
                ndx = 0;
                showNewData();

                if (oldChars != String(receivedChars))
                {
                    newData = true;
                    Firebase.RTDB.setAsync(&fbdo, path + '/' + getTime(), String(receivedChars));
                    showNewData();
                }
                oldChars = String(receivedChars);
            }
        }
}
