## 1. Processing & Connectivity
| ID | Name and Model | Description | Quantity |
| :--- | :--- | :--- | :--- |
| **B-01** | NodeMCU ESP-32 (CH340) | Main CPU for Master, Slave nodes, and Bus Stop units. | 2 |
| **-** | Raspberry Pi 4 Model B | Main CPU for data collection & cloud transmission (Handover Target). | 3 |
| **B-13** | SIM Module A7670E Cat-1 | Provides onboard LTE internet connection via IoT SIM. | 3 |
| **B-07** | GPS Module GY NEO 6M | Provides GPS location capabilities for the bus. | 4 |
| **B-11** | IoT M2M SIM Card | Cellular data provider for GPS and Cloud sync. | 4 |
| **B-09** | RS485 to TTL (MAX3485) | Differential communication between Pi and ESP32. | 4 |
| **-** | SD card 16 GB A1 | Storage for Raspberry Pi OS and logs. | 3 |

## 2. Sensing & Interaction
| ID | Name and Model | Description | Quantity |
| :--- | :--- | :--- | :--- |
| **B-03** | ToF Sensor (VL53L0X) | High-accuracy infrared distance sensing for seat occupancy. | 8 |
| **B-02** | 5mm RG LED (Common Cathode) | Bi-color seat availability indicators (Red/Green). | 10 |
| **BS-02** | MH-ET LIVE Scanner v3.0 | QR code scanner for ticket/boarding validation. | 1 |
| **B-04** | 12V Buzzer SFM 27 | Audible alert for seat violations or system status. | 6 |
| **B-14** | 22" Flat Monitor (75Hz) | ETA Display for the Smart Bus Stop. | 3 |
| **-** | Ultrasonic Sensors | Temporary seat sensors used during prototype execution. | 8 |

## 3. Power Management (Solar & Regulation)
| ID | Name and Model | Description | Quantity |
| :--- | :--- | :--- | :--- |
| **B-18** | Monocrystalline Panel 100W | 18V Solar panel for renewable energy. | 3 |
| **B-19** | MPPT Controller BL-30A | Manages charging between solar panel and battery. | 3 |
| **B-20** | Solar Battery 15AH 12V | AGM battery for energy storage. | 3 |
| **B-10** | Buck-boost XL6009 | Provides stable 12V output for buzzers/monitors. | 4 |
| **B-21** | Buck-down LM2596 | Steps 12V battery voltage down to 5V for ESP32/Pi. | 8 |
| **B-16** | Capacitor 2200 µF | Electrolytic capacitor for voltage stability. | 10 |
| **B-17** | MLCC 0.1 µF | Decoupling capacitor for voltage stability. | 10 |

## 4. Discrete Electronics & Accessories
| ID | Name and Model | Description | Quantity |
| :--- | :--- | :--- | :--- |
| **B-05** | Transistor BJT 2N2222 | Switching driver for buzzers and high-current LEDs. | 9 |
| **B-06** | Resistor 1kΩ | Current limiting for BJT base protection. | 10 |
| **B-15** | DC barrel jack AWG18 | Power input pigtail for monitors. | 4 |
| **B-22** | Portable TV Stand | Mounting hardware for the ETA Display monitors. | 2 |
| **-** | Micro HDMI to HDMI | Connection cable for Pi 4 to Monitor. | 3 |
| **-** | Filament PLA | Material for 3D printed enclosures. | 2 |