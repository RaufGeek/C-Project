# 🏠 My Sweet Home (MSH)

**CENG-464 – Object-Oriented Design & Design Patterns**  
Term Project: **My Sweet Home (MSH)**

---

## 📌 Project Overview

**My Sweet Home (MSH)** is a console-based smart home simulation system developed in **C++ (C++98 standard)**.  
The project demonstrates the practical application of multiple **object-oriented design patterns** in a realistic system.

The system allows users to manage smart home devices, switch modes and states, simulate security/fire scenarios, and restore previous system states.

---

## 🎯 Key Features

- Device management (add, remove, configure, clone)
- Smart home modes (Normal, Evening, Party, Cinema)
- System states with undo/restore support
- Security and fire emergency simulations
- Logging system activity to file
- Fully modular, object-oriented architecture

---

## 🧩 Design Patterns Used

| Design Pattern | Usage |
|----------------|------|
| **Singleton** | `MSHSystem`, `Logger` |
| **Prototype** | Cloning devices (batch addition) |
| **Abstract Factory** | Detector family creation (Smoke + Gas) |
| **Factory Method** | Device creation logic |
| **Strategy** | Home mode behaviors |
| **Memento** | State save / restore (undo functionality) |
| **Chain of Responsibility** | Security & fire scenario handling |

---

## 🏗️ Project Structure

C-Project-main/
│
├── main.cpp
├── MSHSystem.h / MSHSystem.cpp
├── Device.h
├── Devices.h / Devices.cpp
├── DetectorFactory.h / DetectorFactory.cpp
├── Modes.h / Modes.cpp
├── Memento.h / Memento.cpp
├── Scenarios.h / Scenarios.cpp
├── Logger.h / Logger.cpp
├── manual.txt
├── msh_log.txt
├── .vscode/
│ └── tasks.json


## ▶️ How to Build and Run (Windows)

### ✅ Requirements
- **MSYS2**
- **g++ (UCRT64)** with C++98 support
- **PowerShell** or **VS Code Terminal**

---

### 🔧 Compile
Run the following command **inside the project directory**:

```powershell
& "C:\msys64\ucrt64\bin\g++.exe" -std=c++98 -Wall -Wextra -pedantic `
Logger.cpp Devices.cpp DetectorFactory.cpp Modes.cpp Memento.cpp Scenarios.cpp MSHSystem.cpp main.cpp `
-o msh.exe
