# LoRa Disaster Shelter Occupancy System

Embedded IoT system for real-time disaster relief camp occupancy monitoring using **LoRa communication** and **multi-sensor fusion**.

---

## Overview

Natural disasters such as floods, earthquakes, cyclones, and landslides often force large populations into temporary relief shelters. Managing these shelters becomes difficult due to overcrowding and lack of real-time occupancy information.

This project implements a **Smart Disaster Shelter Entry and Capacity Monitoring System** that counts people entering and exiting a relief camp and sends the occupancy data to a remote monitoring station using **LoRa long-range wireless communication**, without relying on internet or cellular networks.

---

## Key Features

- Real-time shelter occupancy monitoring  
- Long-range wireless communication using **LoRa (433 MHz)**  
- Works without internet or cellular networks  
- Multi-sensor fusion to reduce false detections  
- Entry and exit detection using event-based logic  
- Low-power embedded system suitable for emergency deployment  

---

## System Architecture

```
Person at Shelter Entrance
        ↓
PIR Motion Sensor
        ↓
IR Break Beam Sensor
        ↓
Ultrasonic Distance Sensor
        ↓
Load Cell + HX711
        ↓
Microcontroller Processing
        ↓
Entry / Exit Detection
        ↓
Occupancy Count Update
        ↓
LoRa Transmission
        ↓
Receiver Node
        ↓
Monitoring Dashboard (Serial Monitor)
```

---

## Hardware Components

| Component | Purpose |
|-----------|--------|
| Arduino UNO / Nano | Main microcontroller |
| LoRa SX1278 Module | Long-range wireless communication |
| PIR Motion Sensor | Detects motion near entrance |
| IR Break Beam Sensor | Detects gate crossing |
| Ultrasonic Sensor (HC-SR04) | Confirms object presence |
| Load Cell | Measures weight to verify human presence |
| HX711 Amplifier | Interface for load cell |
| 18650 Battery | Portable power supply |

---

## Sensors Used

### PIR Sensor
Detects motion near the entrance.

### IR Break Beam Sensor
Detects when a person crosses the gate.

### Ultrasonic Sensor
Measures distance to confirm object presence.

### Load Cell
Measures weight to verify human presence.

The system uses **sensor fusion**, meaning multiple sensors must confirm an event before counting a person, which significantly reduces false detections.

---

## Entry / Exit Detection Logic

### Entry Detection
```
IR Triggered
↓
Load Cell Detects Weight
↓
PIR or Ultrasonic Confirms Motion
↓
Occupancy Count++
```

### Exit Detection
```
Load Cell Triggered First
↓
IR Triggered After
↓
Occupancy Count--
```

---

## Communication

The transmitter node sends the occupancy count via **LoRa packets**.

Example transmitted packet:

```
COUNT:5
```

Receiver output:

```
Camp Occupancy: 5
```

---

## Repository Structure

```
lora-disaster-shelter-occupancy-system
│
├── transmitter_node
│   └── shelter_transmitter.ino
│
├── receiver_node
│   └── shelter_receiver.ino
│
├── docs
│   ├── project_report.pdf
│   └── simulation_report.pdf
│
└── README.md
```

---

## Applications

- Disaster relief shelter monitoring
- Emergency evacuation management
- Refugee camp capacity tracking
- Crowd monitoring during emergency situations

---

## Future Improvements

- ESP32 implementation
- Low-power deep sleep mode
- Cloud-based monitoring dashboard
- Multi-shelter network monitoring

---

## Authors

**Team WATT**

- Pankaj Singh  
- K. Nithesh Kumar  
- Ram Prasad  
- Prisca Jean

---
