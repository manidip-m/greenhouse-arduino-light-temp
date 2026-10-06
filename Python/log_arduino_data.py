import serial
import time
import csv
import os

# Configure the serial port
SERIAL_PORT = 'COM4'  # Replace with your Arduino's serial port
BAUD_RATE = 9600      # Must match the Arduino's Serial.begin() rate

# Specify the folder and file path
log_folder = r'C:\DataLogs'  # Folder to save the file
if not os.path.exists(log_folder):
    os.makedirs(log_folder)  # Create the folder if it doesn't exist
LOG_FILE = os.path.join(log_folder, 'sensor_data.csv')  # Full file path

try:
    # Open the serial connection
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)  # Wait for the serial connection to initialize

    # Open the CSV file for logging
    with open(LOG_FILE, mode='a', newline='') as file:
        writer = csv.writer(file)
        
        # Write the header row if the file is empty
        if file.tell() == 0:
            writer.writerow(["Timestamp", "Temperature (°C)", "Light Intensity (Raw)"])

        print("Logging data. Press Ctrl+C to stop...")
        count = 0
        while count < 122:  # Stop after 122 readings
            if ser.in_waiting > 0:
                # Read a line from the serial port
                line = ser.readline().decode('utf-8').strip()
                
                # Parse the data
                if line.startswith("Temperature (°C):"):
                    temperature = float(line.split(": ")[1])
                elif line.startswith("Light Intensity (Raw):"):
                    light_intensity = int(line.split(": ")[1])
                    
                    # Get the current timestamp
                    timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
                    
                    # Write the data to the CSV file
                    writer.writerow([timestamp, temperature, light_intensity])
                    print(f"Logged: {timestamp}, {temperature}°C, {light_intensity}")
                    count += 1  # Increment the counter

except serial.SerialException as e:
    print(f"Serial port error: {e}")
    print("Ensure the correct port is selected and no other program is using it.")
except KeyboardInterrupt:
    print("Logging stopped.")
finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()  # Close the serial connection
