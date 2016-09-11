#define VCC 3273
#define BATTERY_PIN A0
#define R1 221200 // Resistor R1 Ohms
#define R2 21440 // Resistor R2 Ohms
#define CONNECT_WAIT_MILLIS 10000
#define LOW_THRESHOLD 12000 // milliVolts
#define SLEEP_SECS 3600 // 1 hour
//#define SLEEP_SECS 15

// Set manual mode so that if no network is available, photon can go back to sleep
SYSTEM_MODE(MANUAL);

const float DIV_RATIO = (R1 + R2) / (float) R2;
const char VOLTAGE_EVENT[11] = "carVoltage";
const char LOW_EVENT[14] = "carLowVoltage";
const char FLASH_REQUEST[13] = "flashRequest";
const char FLASH_READY[13] = "flashReady";

// set to true when the photon can sleep
volatile bool canSleep = false;
// set to true when photon is waiting for OTA update
volatile bool flashing = false;

void setup() {
    disableLed();

    Particle.connect();
    if (waitFor(Particle.connected, CONNECT_WAIT_MILLIS)) {
        subscribeToFlashEvent();
        subscribeToVoltageEvent();
        publishVoltageEvent();
        // Listen for voltage event to make sure it was published before sleeping
        waitForVoltageEvent();
        waitForFlashEvent();
    }

    if (!flashing) {
        System.sleep(SLEEP_MODE_DEEP, SLEEP_SECS);
    }
}

void loop() {
    signalReadyForFlash();
    if (flashing) {
        publishFlashReadyEvent();
        flashing = false;
    }
}

void signalReadyForFlash() {
    RGB.color(0, 255, 0);
    delay(500);
    RGB.brightness(5);
    delay(50);
    RGB.brightness(0);
    Particle.process();
}

void subscribeToVoltageEvent() {
    Particle.subscribe(VOLTAGE_EVENT, subscribeHandler, MY_DEVICES);
}

void subscribeToFlashEvent() {
    Particle.subscribe(FLASH_REQUEST, flashEventHandler);
}

void publishFlashReadyEvent() {
    Particle.publish(FLASH_READY, PRIVATE);
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

void waitForFlashEvent() {
    int waitCount = 0;
    while (!flashing && waitCount++ < 50) {
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

void flashEventHandler(const char* event, const char* data) {
    flashing = true;
}

uint16_t getBatteryMilliVolts() {
	uint16_t mV = readMilliVolts(BATTERY_PIN);
	return round(mV * DIV_RATIO);
}

uint16_t readMilliVolts(int pin) {
	return map(analogRead(pin), 0, 4095, 0, VCC);
}
