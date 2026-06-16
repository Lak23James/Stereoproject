# StereoProject: 3D Crystallographic Projection Engine

**StereoProject** is a high-performance, web-based crystallographic laboratory. It visualizes 3D point group symmetries and maps them in real-time to a 2D stereographic projection (Wulff Net). 

Designed with a strict separation of concerns, the core physics and matrix mathematics are written entirely in **C++** and compiled to **WebAssembly (WASM)**, allowing native-speed execution directly in the browser alongside a **Three.js** interactive 3D dashboard.

**Author:** Lakshya Kohli | 1st-Year Metallurgy, IIT BHU

---

## 🔬 Core Features

* **Real-time 3D/2D Rendering:** Simultaneous visualization of point symmetries on a 3D reference sphere and a 2D stereogram.
* **WASM-Powered Math Engine:** All rotation ($C_n$), reflection ($m$), and inversion ($\bar{1}$) matrices are computed at the memory level in C++.
* **27 Automated Point Groups:** Instant generation of all non-cubic crystallographic point groups (Triclinic, Monoclinic, Orthorhombic, Tetragonal, Trigonal, Hexagonal).
* **Hemisphere Tracking:** Automatically distinguishes between Northern hemisphere points (rendered as solid dots) and Southern hemisphere points (rendered as open rings) on the Wulff Net using $X = x / (1 \pm z)$ logic.
* **Symmetry Chaining:** Manually chain primitive operations to build complex group orbits step-by-step.
<img width="1542" height="867" alt="image" src="https://github.com/user-attachments/assets/9b947dee-e0e5-44e4-bf90-3a2e207e6e22" />

---

## 🏗️ Architecture & Tech Stack

This project follows a strict "Engine vs. Dashboard" philosophy to isolate low-level math from UI logic.

1. **The Brain (Backend):** `C++17`
   * Handles 3D Point structures, 3x3 Matrices, linear algebra transformations, and group theory closure loops.
2. **The Bridge:** `Emscripten (WASM)`
   * Compiles the C++ logic into a binary format (`engine.wasm`) and generates an ES6 module (`engine.js`) using Embind.
3. **The Dashboard (Frontend):** `Three.js` + `HTML5 Canvas` + `JavaScript`
   * A pure visualization layer that feeds user input into the WASM binary and renders the resulting data coordinates.

---

## 🚀 Installation & Local Development

To run this project locally or modify the C++ engine, you will need the [Emscripten SDK (emsdk)](https://emscripten.org/docs/getting_started/downloads.html) and Python installed.

### 1. Clone the Repository
```bash
git clone [https://github.com/Lak23James/Stereoproject.git](https://github.com/Lak23James/Stereoproject.git)
cd Stereoproject
