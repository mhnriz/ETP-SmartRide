# ETP-SmartRide: Smart Transit & Monitoring System
**Status:** Functional Prototype Handed Over
**Date:** January 2026

## 📌 Project Overview
ETP-SmartRide is an IoT-based transit system designed to improve bus efficiency and passenger experience. The system integrates real-time seat occupancy tracking, GPS location monitoring, and a Smart Bus Stop ETA display. 

The project was executed in two phases:
1. **Phase 1 (Planning):** Research, Power Budgeting, and Circuit Drafting.
2. **Phase 2 (Execution):** Hardware integration, ESP-NOW communication, and Firebase Cloud syncing.

## ⚙️ System Architecture
Due to hardware logistics, the current prototype uses a **Modular ESP32 Mesh** rather than a Raspberry Pi master.

* **Master Node (ESP32):** Acts as the Cloud Gateway. It fetches booking data from Firebase via WiFi and broadcasts seat status to nodes.
* **Slave Nodes (ESP32):** Distributed across the bus. Each node manages 4 seats, controlling bi-color LEDs and ultrasonic sensors.
* **Communication:** Internal data is sent via **ESP-NOW** for low-latency, while external data uses **HTTPS/Firebase**.
* **Bus Stop Display:** Uses the same Master circuit to fetch ETA data and output to a 22" monitor.

## 🔋 Hardware & Power Specifications
The system is designed for mobility and sustainability:
- **Power Source:** 100W Monocrystalline Solar Panel [B-18].
- **Storage:** 15AH 12V AGM Solar Battery [B-20].
- **Regulation:** LM2596 Buck-down modules regulated to 5V for logic circuits.
- **Enclosures:** Custom 3D-printed PLA housings.

> **⚠️ HANDOVER NOTE:** All LM2596 buck converters must be verified with a multimeter for stable 5V output before connecting the ESP32 or Pi.

## 📂 Repository Structure
- `/firmware`: Contains `.ino` files for the Bus Master, Bus Stop, and Seat Nodes.
- `/hardware`: Contains the archived KiCad project files (`.zip`) and PDF schematics.
- `/hardware/parts_list.md`: Full Bill of Materials (BOM) with IDs.
- `/docs`: Power budgeting, initial planning documents, and architecture diagrams.
- `HANDOVER.md`: Technical guide for the incoming 8-month team.

## 🚀 Quick Start (Prototype Mode)
1. **Configure WiFi:** Update `ssid` and `password` in the `master_esp32.ino`.
2. **Firebase:** Ensure your `FIREBASE_TOKEN` and URL match your Firestore instance.
3. **Flash Nodes:** Flash the `slave_node.ino` to the respective ESP32s using the MAC addresses listed in the Master code.