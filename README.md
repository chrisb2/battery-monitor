# battery-monitor

Particle [Photon](https://docs.particle.io/datasheets/photon-datasheet/) based WiFi 12v car battery monitor. The idea is to monitor the battery voltage in order to get early warning that the battery is becoming discharged.

The program uses the deep sleep mode of the Photon between voltage measurements, in this state the current draw from the battery is ~1mA. It takes about 10 seconds to wake-up, connect to WiFi, take a measurement and send an event containing the measured voltage.

![circuit diagram](circuit/battery-monitor.png?raw=true "Title")
