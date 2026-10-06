# Arduino-based Greenhouse Microclimate Monitoring

Here, I present a low-cost Arduino UNO system that measures temperature and light intensity inside a greenhouse, which was tested against the greenhouse's calibrated in-house sensors. 

This is a beginner-friendly sensor project for anyone curious about how technology is used in agriculture. You do not need to have used an Arduino before. Follow the steps in order, and take your time with the wiring.

## What you will learn

- How an Arduino reads a sensor and turns it into a number
- How light and temperature sensors work, and how to calculate a temperature from a voltage
- How to use LEDs and a buzzer as simple alerts
- How to record sensor data on your laptop with a Python script
- How to compare a low-cost sensor with a calibrated one, and why calibration matters

## Why measure a greenhouse?

A greenhouse gives plants a controlled environment, so they can grow independent of the location and time of year. It also protects them from weather, insects and diseases. The catch is that greenhouses use large amounts of energy, water and agrochemicals. Sensors and automation help growers watch the greenhouse microclimate and use these resources more carefully.

**Temperature** matters because it needs to be adjusted to the growing stage of a crop.

**Light** matters because sunlight affects the temperature, humidity and plant growth inside a greenhouse. In a closed space, sunlight can heat a greenhouse beyond ideal conditions. Light sensors measure visible light between 400 and 700 nm, which is the photosynthetically active radiation. Growers can use this information to control how much sunlight the plants get using greenhouse films, or to switch on artificial grow lights during overcast, low-light conditions, or in locations that receive low sunlight hours during winters.

This project explores the usability of an Arduino UNO with a basic temperature sensor and a light-dependent resistor (LDR) in a greenhouse, and compares its readings against calibrated, industrial in-house sensors.

## Defining some terms for better understanding

| Term | What it means |
|---|---|
| Microcontroller | A tiny computer on a single chip. The Arduino UNO is a board built around one. |
| Sensor | A part that measures something in the real world, such as light or temperature. |
| LDR (light-dependent resistor) | A resistor whose resistance changes with the amount of light falling on it. |
| ADC (analog-to-digital converter) | The part of the Arduino that turns a voltage into a number between 0 and 1023. |
| Serial port | The USB connection the Arduino uses to send data to your laptop. |
| Baud rate | The speed of that connection. This project uses 9600. |
| CSV file | A simple table saved as text, which Excel and Google Sheets can open. |
| Calibration | Checking a sensor against a trusted reference so its readings can be trusted. |

## What you need

**Parts**

- Arduino UNO and a USB cable
- LDR (light sensor)
- MCP9700A temperature sensor
- Blue LED (low light)
- Yellow LED (high light)
- Red LED (high temperature)
- Green LED (low temperature)
- Buzzer
- Four 220 Ω resistors (one for each LED)
- 10 kΩ resistor (connected with the LDR)
- Breadboard and jumper wires

**Software (all free)**

- Arduino IDE, to program the board
- Python, to record the data on your laptop

## Step 1: Build the circuit

> **Safety first:** unplug the USB cable while you wire. 

### Pin connections

The sketch expects each part on a specific pin of the Arduino:

| Pin | Connected to |
|---|---|
| A0 | LDR |
| A1 | Temperature sensor |
| D8 | Blue LED (low light) |
| D9 | Yellow LED (high light) |
| D10 | Red LED (high temperature) |
| D11 | Buzzer |
| D12 | Green LED (low temperature) |

Each of the four LEDs is connected to the circuit through a 220 Ω resistor. The LDR is connected through a 10 kΩ resistor, which makes it sensitive to the light intensity.

### Wiring the parts

Connect the Arduino's **5V** pin to the red power rail of the breadboard and one of its **GND** pins to the blue ground rail. Then add the parts one at a time.

1. **Each LED.** Connect its Arduino pin (from the table) to a 220 Ω resistor, then to the LED's longer leg. Connect the LED's shorter leg to GND. The resistor stops too much current from flowing through the LED.
2. **LDR.** Connect one leg of the LDR to 5V. Connect the other leg to pin A0, and also to one end of the 10 kΩ resistor. Connect the other end of the resistor to GND. Together, the LDR and the resistor share the 5 V between them. When the light intensity changes, the LDR's resistance changes, and so does the voltage that pin A0 reads. This is how a resistor makes the LDR "sensitive" to light. With this arrangement the reading goes up as it gets brighter, which is what the sketch expects.
3. **Temperature sensor.** It has three legs: power, signal and ground. Connect power to 5V, ground to GND and the signal leg to pin A1. Check the sensor's datasheet for which leg is which before you power it, because a sensor connected the wrong way round can be damaged.
4. **Buzzer.** Connect one pin to D11 and the other to GND. If your buzzer has a + mark, connect that side to D11.

Compare your build with the photo below.

  <p align="center">
    <img src="Images/fig1_arduino_circuit.jpg" alt="Arduino temperature readings" width="400">
  </p>

**Figure 1.** Arduino microcontroller designed using the Arduino student kit which was used to measure temperature and light intensity in the greenhouse.

## Step 2: Set up the Arduino and your laptop

### Install the Arduino IDE

1. Go to [arduino.cc/en/software](https://www.arduino.cc/en/software) [1] and download the Arduino IDE for your operating system.
2. Run the installer and follow the instructions.

### Connect the board

Plug the Arduino into your laptop with the USB cable. A power light on the board should come on. The USB cable powers the board and also carries the data.

### Open the sketch

1. Open the Arduino IDE.
2. Choose **File → Open** and select `arduino/greenhouse_light_temp/greenhouse_light_temp.ino`.

The sketch sits inside a folder with the same name. The Arduino IDE needs this.

### Choose your board and port

1. In the **Tools** menu, choose **Board** and select **Arduino UNO**.
2. In the **Tools** menu, choose **Port** and select the port your Arduino is on. On Windows it looks like `COM4`. If you are not sure which one it is, unplug the board, open the Port menu, plug the board back in, and see which entry appears. **Remember this name. You will need it in Step 3.**

### Upload

1. Click the **Verify** button (the tick) to check the code for mistakes.
2. Click the **Upload** button (the arrow) to send the code to the board.
3. Wait for the message that the upload is done.

### See the readings

1. Open the **Serial Monitor** from the **Tools** menu.
2. Set the baud rate at the bottom of the window to **9600**.
3. You should see two lines appear, and then two more every 30 seconds:

```
Temperature (°C): <value>
Light Intensity (Raw): <value>
```

### Try some experiments

The Arduino reads the temperature sensor (pin A1) and the LDR (pin A0) every 30 s and prints both values to the serial monitor (9600 baud). LEDs and a buzzer indicate the current conditions:

| Condition | Indicator |
|---|---|
| Light below 800 (raw) | Blue LED |
| Light above 980 (raw) | Yellow LED |
| Temperature below 20 °C | Green LED |
| Temperature above 25 °C | Red LED |
| Temperature above 30 °C | Buzzer (500 Hz) |

Light thresholds are raw analog readings (0–1023). Between 20 and 25 °C, which is ambient room temperature, no temperature indicator is switched on.

You can check if the setup and readings on the serial monitor works by trying the following: 
- Cover the LDR with your hand. What happens to the light number? Does an LED switch on?
- Hold the temperature sensor between your fingers. What happens to the temperature reading?

## Step 3: Record your data on your laptop

The Serial Monitor is great for watching live, but it does not save anything. The Python script in this repository saves every reading, with a timestamp, to a file.

### Install Python and the serial library

1. Download Python from [python.org/downloads](https://www.python.org/downloads/) [2] and install it. On Windows, tick **Add python.exe to PATH** in the installer.
2. Open a terminal (Command Prompt on Windows) in this project's folder and run:

```
pip install -r python/requirements.txt
```

### Set your port

1. Open `python/log_arduino_data.py` in any text editor.
2. Find the line `SERIAL_PORT = 'COM4'` and replace `COM4` with the port name you noted in Step 2.

### Run it

1. **Close the Serial Monitor.** Only one program can use the port at a time.
2. In the terminal, run:

```
python python/log_arduino_data.py
```

3. You will see `Logging data. Press Ctrl+C to stop...` and then a `Logged:` line for every reading. The script stops after 122 readings, which takes about an hour. (This was specific to my study; you can set your data collection duration as per your requirement by changing the number 122 in the line `while count < 122:`.) Press Ctrl+C to stop it earlier.

### Find your data

The script saves the readings in `sensor_data.csv` in the `C:\DataLogs` folder, and creates the folder if it does not exist. Open the file in Excel or Google Sheets. It has three columns: `Timestamp`, `Temperature (°C)` and `Light Intensity (Raw)`.

> **Using macOS or Linux?** The script was written on Windows, so you need to change two things at the top of the file. Use your own port name (it looks like `/dev/ttyACM0` on Linux and `/dev/cu.usbmodem…` on macOS), and change `log_folder` to a folder that exists on your computer.

## How the code works

The sketch has a short loop that repeats forever: read the temperature, read the light, print both, switch the LEDs and buzzer, then wait 30 seconds. A 0.1 s delay between the two sensor readings prevents them from interfering with each other.

**From voltage to number.** The Arduino's ADC measures a voltage between 0 and 5 V and turns it into a number from 0 to 1023. That means each step is about 4.9 mV (5 V ÷ 1024).

**From number to temperature.** The MCP9700A gives 500 mV at 0 °C and 10 mV for every extra degree [3]. The sketch uses these two facts:

```cpp
float tempVoltage = tempRaw * (5.0 / 1024.0);
float temperatureC = (tempVoltage - TEMP_SENSOR_V_OFFSET) / TEMP_SENSOR_SCALE;
```

Try it yourself with a raw reading of 153:

1. Voltage = 153 × 5 ÷ 1024 = 0.747 V
2. Temperature = (0.747 − 0.5) ÷ 0.01 = 24.7 °C

**Light.** The light value is just the raw number from the ADC. It has no unit. To compare it with the in-house light meter, the study converted the raw readings to klux.

## Look closely at the data

Open `data/arduino_readings_2025-02-26.csv`. Look at the temperature column. The values do not change smoothly: neighbouring values are always about 0.49 °C apart, for example 17.38, 17.87 and 18.36.

Why? One ADC step is about 4.9 mV, and the sensor gives 10 mV per degree, so one step is about 0.49 °C. The Arduino cannot see anything smaller than that. This is called the **resolution** of a measurement, and it is one reason why a sensor's readings can look jumpy.

## What happened in a real greenhouse

### Study design

Data was collected from a greenhouse at the Rosemount Environmental Institute, UCD, Dublin, Ireland. The Arduino was placed on a table in a partially shaded area, away from direct sunlight, and recorded for one hour on each of three days (25, 26, and 27 February 2025) at 30 s intervals. The serial output was recorded on a laptop using the Python script in this repository.

  <p align="center">
    <img src="Images/fig2_device_setup_greenhouse.jpg" alt="Arduino temperature readings" width="600">
  </p>

**Figure 2.** Device setup and data collection in the greenhouse. Arduino microcontroller was placed in a partially shaded area of the greenhouse, connected to the laptop, and the Python script was run to record data from the serial monitor.

The Arduino readings were compared with data from sensors already installed in the greenhouse, which logs temperature and light intensity every 8 min. The in-house temperature sensor is a digital DHT-22. Because the LDR readings are raw values, they were converted to klux to match the in-house light meter. As the two systems log at different intervals, only the time points where the in-house sensor recorded data were matched for analysis. The data was analysed in Microsoft Excel, using Pearson's correlation coefficient to assess how similar the Arduino and in-house readings were. A correlation coefficient close to 1 means two sets of readings rise and fall together.

### Temperature results

The highest temperature measured in the greenhouse was 30.1 °C on 25 February (clear and sunny day). The lowest was 16.8 °C on 26 February (overcast day). The temperature on 27 February was consistent.

<p align="center">
    <img src="Images/fig3_arduino_temperature.jpg" alt="Arduino temperature readings" width="600">
  </p>

**Figure 3.** Temperature readings from Arduino microcontroller (measured every 30 s) showing the fluctuations in an hour inside the greenhouse over three consecutive days.

<p align="center">
    <img src="Images/fig4_inhouse_temperature.jpg" alt="Arduino temperature readings" width="600">
  </p>

**Figure 4.** Temperature readings from in-house sensor (measured every 8 min) showing the fluctuations in an hour inside the greenhouse over three consecutive days.

The Arduino temperature readings fluctuated more than those of the in-house sensor, which is a digital DHT-22 that is more accurate and better housed in the sensor console. The large fluctuations in the Arduino readings are also due to the breadboard heating up in sunlight and to cool breezes entering the greenhouse.

Pearson’s correlation coefficient (r) for temperature readings was 0.43, 0.66, and 0.36 for 25th, 26th, and 27th February, respectively, indicating that the temperature readings from the two sensors were poorly correlated.

### Light results

The light intensity readings were mostly stable on 25 and 27 February. On 26 February they fluctuated slightly because of overcast weather, which later gave way to partly cloudy conditions.

<p align="center">
    <img src="Images/fig5_arduino_light.jpg" alt="Arduino temperature readings" width="600">
  </p>

**Figure 5.** Light intensity readings from Arduino microcontroller showing the fluctuations in an hour inside the greenhouse over three consecutive days.

<p align="center">
    <img src="Images/fig6_inhouse_light.jpg" alt="Arduino temperature readings" width="600">
  </p>

**Figure 6.** Light intensity readings from the in-house sensor showing the fluctuations in an hour inside the greenhouse over three consecutive days.

The correlation values between the LDR and the in-house sensor varied widely, and most of the values obtained with the Arduino's LDR exceeded 95 klux, which is unrealistic.

## Why didn't the Arduino match the greenhouse sensors?

A result that did not work out is still a good result, because it shows what to improve.

**Temperature.** The in-house DHT-22 has a stated accuracy of ±0.5 °C. It is pre-calibrated, compensates for non-linearity and temperature drift, has less noise and does not require voltage conversion to determine temperature. The Arduino sensor is low-cost, but its readings were affected by heating in sunlight.

**Light.** The LDR performed poorly: its readings were nowhere close to the in-house light meter. Light meters are usually pre-calibrated, designed to match the sensitivity of the human eye, give a linear output and are stable across temperature ranges. LDRs have a non-linear resistance–light intensity relationship, which makes measuring intensity with them challenging. The LDR would need to be calibrated against a light meter with known values, with the resulting model incorporated in the sketch.

**Practicality in a greenhouse.** The Arduino provides an interactive, low-cost platform to learn and build sensors. However, its bulky design, loose wires and exposed breadboard make it impractical in a greenhouse, because the breadboard can warm up and affect the temperature and light readings.

## How would you improve it?

- Solder the sensors to the microcontroller and house the whole setup in a case to protect it from the environment.
- Replace the temperature sensor with a DHT-22, which is pre-calibrated and can also monitor humidity.
- Replace the LDR with an Adafruit TSL2591 High Dynamic Range Digital Light Sensor, a digital, pre-calibrated sensor with a wide detection range that is resilient against temperature and voltage fluctuations.

Both suggested sensors are cheap and can be used with an Arduino, which would make it convenient to build affordable microcontrollers for greenhouses for farmers in rural areas and developing countries.

## Repository structure

```
├── arduino/
│   └── greenhouse_light_temp/
│       └── greenhouse_light_temp.ino   Arduino sketch
├── python/
│   ├── log_arduino_data.py             Logs serial output to CSV
│   └── requirements.txt
├── data/
│   └── arduino_readings_2025-02-26.csv Arduino readings, 26 Feb 2025
├── Images/                             Figures used in this README
└── README.md
```

## About the data

`data/arduino_readings_2025-02-26.csv` contains the Arduino readings recorded on 26 February 2025, one of the three days in the study. The in-house sensor data and the other two days are not included in this repository.

## References

1. Arduino. *Arduino software* (Arduino IDE download page). https://www.arduino.cc/en/software
2. Python Software Foundation. *Download Python*. https://www.python.org/downloads/
3. Microchip Technology. *MCP9700A* (product page and datasheet). https://www.microchip.com/en-us/product/MCP9700A
