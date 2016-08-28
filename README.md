# battery-monitor

Particle [Photon](https://docs.particle.io/datasheets/photon-datasheet/) based WiFi 12v car battery monitor. The idea is to monitor the battery voltage in order to get early warning that the battery is becoming discharged.

The program uses the deep sleep mode of the Photon between voltage measurements, in this state the current draw from the battery is ~1mA. It takes about 10 seconds to wake-up, connect to WiFi, take a measurement and send an event containing the measured voltage.

![circuit diagram](circuit/battery-monitor.png?raw=true "Title")

## BOM

* Particle Photon
* Traco Power TSR-1-2450 DC/DC Step-Down Converter
* 1N4747 Zener Diode
* 1000uF 35V Electrolytic Capacitor
* 0.1uF 50V Ceramic Capacitor
* 150uH RF Choke 4.2ohm 175mA max
* 1K Resistor
* 220K Resistor
* 22K Resistor

## References

[Power Filter/Regulator Circuit](http://linuxcar.sone.jp/reg.en.html)
[Powering Arduino and switches from 12V automotive battery](http://www.eevblog.com/forum/projects/powering-arduino-and-switches-from-12v-automotive-battery/msg687838/#msg687838)
