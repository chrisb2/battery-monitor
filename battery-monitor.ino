#define VCC 3273
#define BATTERY_PIN A0
#define R1 221200 // Resistor R1 Ohms
#define R2 21440 // Resistor R2 Ohms
#define CONNECT_WAIT_MILLIS 5000
#define LOW_THRESHOLD 12000 // milliVolts
//#define SLEEP_SECS 15 // 15 seconds
//#define SLEEP_SECS 900 // 15 minutes
#define SLEEP_SECS 3600 // 1 hour

// Set manual mode so that if no network is available at startup, photon can go back to sleep
SYSTEM_MODE(MANUAL);

const float DIV_RATIO = (R1 + R2) / (float) R2;
const char VOLTAGE_EVENT[11] = "carVoltage";
const char LOW_EVENT[14] = "carLowVoltage";

// set to true when the photon can sleep
volatile bool canSleep = false;

void setup() {
    disableLed();

    Particle.connect();
    if (waitFor(Particle.connected, CONNECT_WAIT_MILLIS)) {
        subscribeToVoltageEvent();
        publishVoltageEvent();
        // Listen for voltage event to make sure it was published before sleeping
        waitForVoltageEvent();
    }

    System.sleep(SLEEP_MODE_DEEP, SLEEP_SECS);
}

void subscribeToVoltageEvent() {
    Particle.subscribe(VOLTAGE_EVENT, subscribeHandler, MY_DEVICES);
}

void publishVoltageEvent() {
    char eventData[5];
    uint16_t milliVolts = getBatteryMilliVolts();
    delay(10);
    milliVolts = getBatteryMilliVolts();
    sprintf(eventData, "%.1f", milliVolts / 1000.0);
    if (milliVolts < LOW_THRESHOLD) {
        Particle.publish(LOW_EVENT, eventData, 60, PRIVATE);
    }
    Particle.publish(VOLTAGE_EVENT, eventData, 60, PRIVATE);
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
