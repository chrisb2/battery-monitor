#define VCC 3273
#define BATTERY_PIN A0
#define R1 21440 // Resistor R1 Ohms
#define R2 3242 // Resistor R2 Ohms
#define CONNECT_WAIT_MILLIS 5000
#define SLEEP_SECS 15
//#define SLEEP_SECS 3600

SYSTEM_MODE(MANUAL);

const float DIV_RATIO = (R2 + R1) / (float) R2;
const char VOLTAGE_EVENT[11] = "carVoltage";

volatile bool canSleep = false; // set to true when the photon can sleep

void setup() {
    disableLed();
    
    pinMode(BATTERY_PIN, INPUT);
    
    Particle.subscribe(VOLTAGE_EVENT, subscribeHandler, MY_DEVICES);
    Particle.connect();
    if (waitFor(Particle.connected, CONNECT_WAIT_MILLIS)) {
        publishVoltageEvent();
        waitForVoltageEvent();
    }
    System.sleep(SLEEP_MODE_DEEP, SLEEP_SECS);
}

void loop() {}

void publishVoltageEvent() {
    char mV[20];
    getBatteryMilliVolts();
    delay(10);
    sprintf(mV, "%d", getBatteryMilliVolts());
    Particle.publish(VOLTAGE_EVENT, mV, 60, PRIVATE);
}

void waitForVoltageEvent() {
    int waitCount = 0;
    while (!canSleep && waitCount++ < 100) {
        delay(100);
    }
}

void disableLed() {
    RGB.control(true);
    RGB.brightness(0);
}

void subscribeHandler(const char* event, const char* data) {
    canSleep = true;
}

uint16_t getBatteryMilliVolts() {
	uint16_t mV = readMilliVolts(BATTERY_PIN);
	return round(mV * DIV_RATIO);
}

uint16_t readMilliVolts(int pin) {
	return map(analogRead(pin), 0, 4095, 0, VCC);
}