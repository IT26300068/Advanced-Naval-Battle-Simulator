# ⚓ Advanced Naval Battle Simulator

A console-based naval battle simulation written in the **C programming language**.

The program allows the user to configure a battleship and escort ships, generate a battlefield, run different parts of the simulation, and save the simulation results to a text file.

## 🚀 Features

- ⚙️ Configure battleship properties
- 🛥️ Configure escort ship settings
- 🎯 Select escort ship types
- 🗺️ Set battlefield values
- 🌊 Generate the battlefield
- 📏 Calculate distances between ships
- 💥 Calculate projectile range
- 🎯 Check whether ships can hit each other
- ⚔️ Simulate attacks between the battleship and escort ships
- ❤️ Track ship health and shots fired
- 💾 Save simulation results to `simulation_results.txt`
- 📊 Display saved simulation statistics
- 📖 View instructions from the main menu

## 📁 Project Files

### 🧭 `main.c`
Controls the main menu and the overall flow of the program.

### 🌊 `battlefield.h`
Contains the structures and constants used to represent the battlefield, battleship, and escort ships.

### 🗺️ `battlefield.c`
Contains functions used to create, configure, and display the battlefield.

### 💥 `combat.h`
Contains function declarations related to combat calculations.

### 🎯 `combat.c`
Contains combat and physics calculations such as:

- 📏 Distance between two ships
- 💣 Projectile range
- ⏱️ Projectile flight calculations
- 🎯 Hit checking
- 💥 Damage calculations

### ⚔️ `simulation.h`
Contains function declarations used by the simulation.

### 🚢 `simulation.c`
Runs the different simulation parts and controls battles between ships.

### 💾 `fileio.h`
Contains function declarations for saving and reading simulation information.

### 📄 `fileio.c`
Writes battlefield and simulation information to `simulation_results.txt` and displays saved statistics.

## 🧱 Main Data Structures

The program mainly uses the following structures:

- 🚢 `Battleship`
- 🛥️ `EscortShip`
- 📋 `EscortSpec`
- 🗺️ `Battlefield`

These structures store information such as:

- 🏷️ Ship type
- 🚢 Ship name
- 🔫 Gun name
- 📍 Position
- 🚀 Velocity
- 📐 Firing angle
- 💥 Impact power
- 🧮 Gamma value
- ⏳ Reload time
- ❤️ Health
- ✅ Alive status
- 🎯 Number of shots fired

## ⌨️ Program Inputs

The user enters the required values while the program is running.

Examples of inputs include:

- 🚢 Battleship properties
- 🛥️ Escort ship type
- 🔢 Number of escort ships
- 📍 Ship positions
- 🚀 Projectile velocity
- 📐 Firing angles
- ⏳ Reload time
- 🧮 Gamma value
- 🗺️ Battlefield settings

## 🖥️ Program Outputs

The program displays information such as:

- 🌊 Current battlefield setup
- 📍 Ship positions
- 📏 Distances between ships
- 💣 Projectile information
- ⚔️ Attack results
- 💥 Damage caused
- ❤️ Remaining ship health
- ☠️ Whether a ship is alive or destroyed
- 📊 Simulation statistics

Results are also saved in:

```text
simulation_results.txt
```

## 🧮 Physics Used

### 📏 Distance Between Two Ships

The straight-line distance between two positions is calculated using:

```text
distance = sqrt((x2 - x1)^2 + (y2 - y1)^2)
```

### 💣 Projectile Range

Projectile range is calculated using:

```text
R = (v^2 × sin(2θ)) / g
```

where:

- `R` = projectile range
- `v` = projectile velocity
- `θ` = firing angle
- `g` = gravitational acceleration (`9.81 m/s²`)

## 🛠️ How to Compile

Open a terminal in the project folder and compile the source files using GCC:

```bash
gcc main.c battlefield.c combat.c simulation.c fileio.c -o naval_simulator -lm
```

The `-lm` option is required because the program uses functions from the C math library.

## ▶️ How to Run

On **macOS or Linux**:

```bash
./naval_simulator
```

On **Windows**:

```bash
naval_simulator.exe
```

## 🔄 Example Program Flow

1. ▶️ Start the program.
2. ⚙️ Open the setup menu.
3. 🚢 Enter battleship properties.
4. 🛥️ Enter escort ship settings.
5. 🎯 Select escort ship types.
6. 🗺️ Generate the battlefield.
7. ⚔️ Start the required simulation.
8. 🖥️ View the results on the screen.
9. 💾 Check `simulation_results.txt` for saved results.
10. 📊 View simulation statistics from the menu.

## ✅ Requirements

- 💻 C compiler such as GCC
- 📚 Standard C libraries
- 🧮 Math library (`math.h`)
- ⌨️ Terminal or command prompt

## 💻 Language

This project is written in **C**.

## 🎓 Purpose

This project was created as a programming assignment to demonstrate the use of:

- 🧮 Variables
- 🔀 Conditions
- 🔁 Loops
- 🧩 Functions
- 📦 Arrays
- 🔤 Strings
- 🧱 Structures
- 📁 Header files
- 🗂️ Multiple source files
- 💾 File handling
- ➗ Mathematical calculations
- ⚔️ Simulation logic

## 👨‍💻 Author

**Vihanga Gunarathne**
