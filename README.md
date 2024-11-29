# Sleeping Bus Depot Simulation

This project simulates a **Sleeping Bus Depot Problem** where passengers arrive randomly at a depot, wait for a bus, and board it when the bus arrives. The depot has a limited capacity for waiting passengers, and the bus can carry a fixed number of passengers per trip. The simulation uses **threads**, **mutexes**, and **semaphores** for synchronization.

---

## Features

- Simulates a bus depot with:
  - A fixed depot capacity.
  - A bus with a maximum passenger capacity.
- Uses threads to simulate buses and passengers arriving randomly.
- Synchronization between bus and passenger threads using semaphores and mutexes.
- Logs the boarding process and handles scenarios like:
  - Depot being full.
  - Passengers leaving if no space is available.
  - Bus departing after all waiting passengers have boarded.

---

## Prerequisites

To run this simulation, ensure you have the following:

- A C compiler (e.g., GCC).
- A system supporting POSIX threads (`pthread`) and semaphores (most Linux/Unix systems).
- If on Windows, use a POSIX emulation layer like **Cygwin** or **WSL** (Windows Subsystem for Linux).

---

## How to Run

1. **Clone this repository**:
   ```bash
   git clone <repository-url>
   cd sleeping-bus-depot
2. **Compile the Code:**
   ```bash
   gcc sleeping_bus_depot.c -o sleeping_bus_depot -lpthread
3. Run the Simulation:
   ```bash
   ./sleeping_bus_depot

## Contributing
Contributions are welcome! Feel free to open an issue or submit a pull request.
