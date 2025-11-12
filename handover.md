# 📝 Technical Handover Guide

## 1. Main Unit (Master) Integration
The Main Unit acts as the system Master, responsible for GPS tracking, LTE cloud communication, and managing the RS-485 bus to poll Sensor Nodes.

### ⚙️ Hardware Details
| Component | Function | Hardware Note |
| :--- | :--- | :--- |
| **Raspberry Pi 4** | System Controller | Main logic and UART management. |
| **A7670E Cat-1 HAT** | LTE Module | Cellular data transmission. |
| **NEO-6M GPS** | GPS Receiver | Real-time location tracking. |
| **MAX3485** | RS-485 Transceiver | Communication with Sensor Nodes. |
| **LM2596 Buck** | Power Regulator | Steps down bus voltage (12V) to 5V. |

### ⚠️ Power Logic & Safety
* **Calibration:** The LM2596 must be tuned to exactly **5V** BEFORE connecting to the Raspberry Pi 4.
* **Sensitivity:** The Raspberry Pi is highly sensitive to overvoltage. Always measure output with a multimeter before plugging in the Pi.
* **GPS Fix:** The NEO-6M requires a "Cold Start" which can take several minutes. Ensure the antenna has a clear line of sight to the sky.

---

## 2. Sensor Node (Slave) Integration
Each node monitors 4 seats, processes occupancy locally, and communicates with the Master via RS-485.

### 📊 The Bitmap System
To minimize data traffic, seat status is packed into a single byte (Bitmap):
* **Bit 0 (1):** Seat 1 (or 5, 9, etc.)
* **Bit 1 (2):** Seat 2
* **Bit 2 (4):** Seat 3
* **Bit 3 (8):** Seat 4
* **Example:** If Master sends `0110`, Seats 2 and 3 are marked as Booked.

### 📍 Pinout Reference (ESP32)
| Component | ESP32 Pin | Function | Hardware Note |
| :--- | :--- | :--- | :--- |
| **MAX3485 RO** | GPIO 17 | RXD2 | Connect to Receiver Output. |
| **MAX3485 DI** | GPIO 16 | TXD2 | Connect to Driver Input. |
| **MAX3485 RE/DE**| GPIO 2 | EN (Direction) | HIGH = Transmit, LOW = Receive. |
| **VL53L0X SDA** | GPIO 21 | I2C Data | All sensors share this line. |
| **VL53L0X SCL** | GPIO 22 | I2C Clock | All sensors share this line. |
| **XSHUT 1** | GPIO 18 | Reset 1 | Used to assign Address 0x30. |
| **XSHUT 2** | GPIO 19 | Reset 2 | Used to assign Address 0x31. |



---