"""Log greenhouse readings from the Arduino to a CSV file.

The Arduino sketch (arduino/greenhouse_light_temp) prints two lines every
30 s over the serial port:

    Temperature (°C): 22.27
    Light Intensity (Raw): 998

This script reads those lines, pairs each temperature with the light value
that follows it, adds a timestamp and appends the row to a CSV file.

Usage examples
    Windows:        python log_arduino_data.py --port COM4
    Linux / macOS:  python log_arduino_data.py --port /dev/ttyACM0 --readings 60

Find your port in the Arduino IDE under Tools > Port. Close the Arduino
serial monitor first, because only one program can use the port at a time.
"""

import argparse
import csv
import sys
import time
from pathlib import Path

import serial

TEMP_PREFIX = "Temperature"
LIGHT_PREFIX = "Light Intensity"
HEADER = ["timestamp", "temperature_c", "light_raw"]


def parse_value(line):
    """Return the text after the colon, e.g. 'Light Intensity (Raw): 998' -> '998'."""
    return line.split(":", 1)[1].strip()


def log_readings(port, baud, output, n_readings):
    output = Path(output)
    output.parent.mkdir(parents=True, exist_ok=True)
    write_header = not output.exists() or output.stat().st_size == 0

    ser = None
    try:
        ser = serial.Serial(port, baud, timeout=2)
        time.sleep(2)  # the Arduino resets when the port opens; give it time to start

        with open(output, mode="a", newline="", encoding="utf-8") as file:
            writer = csv.writer(file)
            if write_header:
                writer.writerow(HEADER)

            print(f"Logging {n_readings} readings to {output}. Press Ctrl+C to stop...")
            temperature = None
            count = 0
            while count < n_readings:
                line = ser.readline().decode("utf-8", errors="replace").strip()
                if not line:
                    continue

                try:
                    if line.startswith(TEMP_PREFIX):
                        temperature = float(parse_value(line))
                    elif line.startswith(LIGHT_PREFIX) and temperature is not None:
                        light = int(parse_value(line))
                        timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
                        writer.writerow([timestamp, temperature, light])
                        file.flush()  # keep the data safe if the run is interrupted
                        count += 1
                        print(f"[{count}/{n_readings}] {timestamp}  {temperature} °C  light {light}")
                        temperature = None
                except (ValueError, IndexError):
                    # A half-received line right after start-up; skip it.
                    temperature = None

    except serial.SerialException as e:
        print(f"Serial port error: {e}")
        print("Check the port name and make sure no other program is using it.")
        sys.exit(1)
    except KeyboardInterrupt:
        print("Logging stopped.")
    finally:
        if ser is not None and ser.is_open:
            ser.close()


def main():
    parser = argparse.ArgumentParser(description="Log Arduino temperature and light readings to CSV.")
    parser.add_argument("--port", required=True, help="serial port, e.g. COM4 or /dev/ttyACM0")
    parser.add_argument("--baud", type=int, default=9600, help="must match Serial.begin() in the sketch (default 9600)")
    parser.add_argument("--output", default="arduino_readings.csv", help="CSV file to append to (default arduino_readings.csv)")
    parser.add_argument("--readings", type=int, default=120, help="number of readings to record; 120 = one hour at 30 s (default 120)")
    args = parser.parse_args()
    log_readings(args.port, args.baud, args.output, args.readings)


if __name__ == "__main__":
    main()
