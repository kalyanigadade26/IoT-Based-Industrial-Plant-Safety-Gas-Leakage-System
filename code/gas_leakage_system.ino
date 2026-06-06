```cpp
/*************************************************
 * IoT Based Industrial Plant Safety
 * Gas Leakage Detection System
 *
 * Controller : NodeMCU ESP8266
 * Platform   : Blynk IoT
 * Language   : Embedded C (Arduino Framework)
 *************************************************/

#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Gas Leakage System"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_TOKEN"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Wi-Fi Credentials
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

// Blynk Authentication
char auth[] = BLYNK_AUTH_TOKEN;

// Virtual Pin for Device Uptime
#define PIN_UPTIME V6

// Gas Sensors
const int MQ6_PIN = D1;      // LPG Sensor
const int MQ135_PIN = D0;    // Air Quality / Benzene Sensor

// Buzzer
const int BUZZER_PIN = D2;

// Sensor States
int mq6State = 0;
int mq135State = 0;

// Timer Object
BlynkTimer timer;

/*************************************************
 * Send Device Uptime to Blynk
 *************************************************/
BLYNK_READ(PIN_UPTIME)
{
    Blynk.virtualWrite(PIN_UPTIME, millis() / 1000);
}

/*************************************************
 * Read Sensor Data and Update Blynk Dashboard
 *************************************************/
void sendSensorData()
{
    int analogValue = analogRead(A0);

    // Approximate PPM Conversion
    float gasPPM = map(analogValue, 0, 1023, 20, 500);

    Serial.print("Gas Concentration (PPM): ");
    Serial.println(gasPPM);

    // Send Data to Blynk
    Blynk.virtualWrite(V2, gasPPM);
    Blynk.virtualWrite(V0, mq6State);
    Blynk.virtualWrite(V1, mq135State);
}

/*************************************************
 * Setup Function
 *************************************************/
void setup()
{
    Serial.begin(9600);

    // Initialize Blynk
    Blynk.begin(auth, ssid, pass);

    // Configure Pins
    pinMode(MQ6_PIN, INPUT);
    pinMode(MQ135_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    // Run Sensor Update Every 1 Second
    timer.setInterval(1000L, sendSensorData);

    Serial.println("Gas Leakage Monitoring System Started...");
}

/*************************************************
 * Main Loop
 *************************************************/
void loop()
{
    // Read Sensor Status
    mq6State = digitalRead(MQ6_PIN);
    mq135State = digitalRead(MQ135_PIN);

    // Trigger Alarm if Gas Detected
    if (mq6State == HIGH || mq135State == HIGH)
    {
        digitalWrite(BUZZER_PIN, LOW);
        Serial.println("WARNING: Gas Leakage Detected!");
    }
    else
    {
        digitalWrite(BUZZER_PIN, HIGH);
    }

    Serial.print("MQ6 Status: ");
    Serial.println(mq6State);

    Serial.print("MQ135 Status: ");
    Serial.println(mq135State);

    Blynk.run();
    timer.run();
}
```
