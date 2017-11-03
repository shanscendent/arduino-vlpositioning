# Visible Light Positioning
This page details a rudimentary visible light positioning system using multiple Arduino Unos based on [mchr3k]'s [Arduino Manchester Encoding][arduino-manchester] library.

Lamps and photodetector not included.

## Concepts
### VLC
Visible light communication (VLC) works by pulsing a light source on and off to send digital data to a receiver. A high frequency makes the flicker undetectable. Researchers have pushed bandwidths of over 1Gbit/s over these communication links, but this example only uses ~1200 baud as there is no need for high data rates.

### Positioning
Upon startup, each lamp (emitter) continuously transmits the same packet, which is a randomly generated 6-byte address unique to each lamp.
```c
uint8_t data[6] = {0x6D, 0x67, 0x93, 0xDD, 0x79, 0x7A};
```
The receiver (detector, filter and decoder) can then be positioned under the lamps to receive the transmissions. Additional processing is done and the receiver will notify the user which lamp it's under.

### Use Case
This VLP implementation can be used as an indoor positioning system (IPS) to get sub-meter positioning accuracy within a building.

## Getting Started
### Prerequisites
Install the required libraries.
- [Manchester Encoding][arduino-manchester] - Communication library
- [Timer1] - Easy timer library

### Usage
- There are 3 sketches, `transmitter`, `dcfilter` and `receiver`.
- The `transmitter` sketch uses the library transmission example without any delay between packets. It loops forever. This example is connected to a custom Arduino-based 10 channel light source which also accepts PWM control. You're free to use any light source down to simple LEDs. Connect the light source switch to pin `TX_PIN`.
- The `dcfilter` sketch filters the incoming analog voltage from a photodetector and converts it into a digital signal. Again, you're free to use your own, like visible light LED transceivers. I would recommend adding an amplifier circuit with automatic gain control (AGC) before the signal to the Arduino. Connect the photodetector output to `SENSOR_PIN` and the `PIN` output into the receiver. **Remember to not exceed 5V on the signal.**
- `receiver` decodes the received packets which are received from the digital signal going into `RX_PIN` from `dcfilter`'s `PIN` and updates the LEDs depending on which lamp is detected.
- Basically, **photodetector** > SENSOR_PIN > **filter** > PIN > RX_PIN > **receiver**.

### Receiver LED Output
|  | Red | Green | Yellow |
| ------ | ------ | ------ | ------|
| No lamp | Blinking | Off | Off |
| Lamp 1 | Off | On | Off |
| Lamp 2 | Off | Off | On |

### Pictures
- Here's an idea of how everything's connected together.
![Set-up](/images/Image01.jpg)
- The `dcfilter` sketch successfully converts a noisy 80mA peak-to-peak signal into a digital signal with a slight phase shift.
![Waveform](/images/Image02.png)

## Limitations
- The current version of the code only supports two lamps (not easily scalable)
- The light cones from each lamp should not overlap (there is no multiplexing scheme implemented)
- The checking algorithm is not very sophisticated yet, although there is some allowance for lost packets.
- Currently, the register level code is only only works on the Arduino Uno.

## Issues
- The Manchester Encoding library sometimes (at least 4/5 times) assumes incoming transmissions are of a different baudrate. In this case, it fails to update the receiving array even though it tells the user that a message has been received. To circumvent this, I have [bodged][bodge] a solution that checks whether the library has fallen into this state periodically and performs a *dirty* soft reboot if it is stuck.
- The filter and decoder run on separate Arduinos because there is heavy use of interrupts for both sketches. The sketches should be able to be merged if a lower baudrate is used.
- The `Serial.print` function cannot be used on the receiver as it seems to crash the Arduino Uno. LED outputs are used instead.

[//]: # (References)

[mchr3k]: <https://github.com/mchr3k>
[arduino-manchester]: <http://mchr3k.github.io/arduino-libs-manchester/>
[Timer1]: <https://playground.arduino.cc/Code/Timer1>
[bodge]: <https://en.wikipedia.org/wiki/Kludge#Computer_science>