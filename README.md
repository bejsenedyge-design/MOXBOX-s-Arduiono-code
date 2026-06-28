MOXBOX - Arduino Environmental Monitoring
Welcome to the MOXBOX project repository! This project focuses on environmental data collection and analysis using Arduino. It is designed to monitor key environmental parameters and display them in real-time.
📋 Project Overview
The MOXBOX system tracks several essential environmental factors:
 Temperature & Humidity: Measured via the DHT sensor.
 Fire/Flame Detection: Alerts in case of fire hazards using the Flame sensor.
 Soil Moisture: Monitors the water content in the soil for gardening/agricultural needs.
 Water Level: Tracks water levels to prevent overflow or detect shortages.
 Visualization: All data is processed and displayed on an LCD screen (I2C).
🛠 Tech Stack & Hardware
 Microcontroller: Arduino (compatible with Uno, Nano, etc.)
 Language: C++ (Arduino IDE)
 Communication: I2C protocol for the display
⚙️ How to Setup
1. Connect your hardware: Wire your sensors and the LCD display to the Arduino board according to your circuit diagram.
2. Install Libraries: Open your Arduino IDE, go to ⁠Sketch⁠ -> ⁠Include Library⁠ -> ⁠Manage Libraries⁠, and install:
 ⁠DHT sensor library⁠ (by Adafruit)
 ⁠LiquidCrystal I2C⁠ (by Frank de Brabander)
3. Upload: Copy the code from this repository into your Arduino IDE, select your board and port, and click Upload.
🚀 Future Improvements
 [ ] Adding data logging to an SD card.
 [ ] Integrating IoT features for remote monitoring.
