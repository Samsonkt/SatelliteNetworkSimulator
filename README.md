# 🌐 Satellite Network Simulator

A **C++ application** simulating a constellation of low-Earth orbit satellites using an **AVL balanced binary search tree (BST)**. Built as part of **CMSC 341** at **UMBC (Fall 2023)**, this simulator is inspired by real-world satellite internet networks like **Starlink**, offering efficient and scalable satellite management.

> 📍 https://github.com/Samsonkt/SatelliteNetworkSimulator/tree/main

---

## 🚀 Overview

This command-line tool models a dynamic satellite network by:
- Efficiently handling satellite insertion, removal, and queries.
- Maintaining AVL balance for optimal performance.
- Simulating network resilience and responsiveness to failures or removals.

Originally designed to support a feasibility study for a startup exploring satellite-based internet infrastructure.

---

## ✨ Features

- **AVL Tree Structure:** Ensures `O(log n)` operations for insertion, removal, and search.
- **Dynamic Network Management:**
  - Insert/remove satellites.
  - Mark satellites as deorbited.
- **Query Support:**
  - Count satellites by inclination.
  - Check for specific satellite IDs.
- **Robust Testing:** Includes a custom test suite for functional and edge case validation.
- **Memory Safety:** Verified with **Valgrind** — no memory leaks.
- **Debug Output:** Optional `dumpTree()` for visualizing in-order traversal.

---

## 🧰 Technologies Used

- **C++** (Standard 11+)
- Optional: **CMake** for build system
- **Valgrind** for memory checks
- **Git** for version control

---

## 📦 Prerequisites

- C++ Compiler (e.g., `g++ 9.0+`)
- Optional: [CMake 3.14+](https://cmake.org/)
- Optional: [Valgrind](https://valgrind.org/)
- OS: Linux (preferred), macOS, or Windows (with some adjustments)

---

## 🛠️ Setup & Build

### Clone the Repo

```bash
git clone https://github.com/Samsonkt/SatelliteNetworkSimulator.git
cd SatelliteNetworkSimulator
```

> 🔁 Replace the URL above if your repo is different.

### Build the Project

#### Option 1: Using g++ directly

```bash
g++ -g -o simulator satnet.cpp mytest.cpp
```

#### Option 2: With CMake

```bash
mkdir build && cd build
cmake ..
make
```

---

## ▶️ Running the Simulator

Run the compiled program:

```bash
./simulator
```

The included test suite (`mytest.cpp`) will automatically execute test cases for AVL tree integrity, insertion/removal, and performance.

### Optional: Check for Memory Leaks

```bash
valgrind ./simulator
```

You should see a `no leaks are possible` message.

---

## 🧪 Testing

Test cases include:

- ✅ **Normal Cases:** Balanced insertions/removals.
- ⚠️ **Edge Cases:** Empty tree, one-node tree, etc.
- ❌ **Error Cases:** Duplicates, invalid IDs.
- 🚀 **Performance Cases:** Tests with 1000+ nodes.

Each test returns a pass/fail result. For debugging, enable `SatNet::dumpTree()`.

---

## 📁 Project Structure

```
├── satnet.h       // Class definitions for Sat and SatNet
├── satnet.cpp     // AVL tree logic and rebalancing
├── mytest.cpp     // Test suite (Tester class)
├── README.md      // Project description and setup
```

---

## 🤝 Contributing

Contributions are welcome! To contribute:

1. Fork the repo.
2. Create a feature branch:
   ```bash
   git checkout -b feature/your-feature
   ```
3. Commit your changes:
   ```bash
   git commit -m "Add your feature"
   ```
4. Push and open a PR:
   ```bash
   git push origin feature/your-feature
   ```

Please follow C++ best practices and comment your code meaningfully.

---

## 📜 License

This project is for **educational use only** under **UMBC's academic integrity policy**. No commercial license is applied unless explicitly stated.

---

## 🙏 Acknowledgments

- Built for **CMSC 341 (Fall 2023)** at **UMBC**
- Inspired by real-world constellations like **Starlink**

---
