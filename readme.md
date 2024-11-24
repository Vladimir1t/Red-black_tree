# Red-Black Tree Implementation

This project implements a Red-Black Tree with range query support, along with optional integration with the `std::set` container for debugging and testing purposes. The program also provides graph visualization functionality using Graphviz.
---

## Features

1. **Red-Black Tree:**  
   - Self-balancing binary search tree with insertion and range query operations.
   - Invariants maintained for O(log N) operations.

2. **Optional Debugging with `std::set`:**  
   - Range queries compared with `std::set` for correctness.

3. **Graph Visualization:**  
   - Outputs the Red-Black Tree structure as a `.png` file using Graphviz.

4. **Unit Testing:**  
   - A test suite is included for validating the implementation.

---

## Files Overview

- **`main.cpp`**: Contains the main program logic for interacting with the Red-Black Tree and optional `std::set`.
- **`red_black_tree.hpp`**: Header file defining the Red-Black Tree class and its operations.
- **`CMakeLists.txt`**: Build configuration for the project using CMake.
- **`tests.cpp`**: Provides unit tests to validate the functionality of the Red-Black Tree.
- **`tests/tests.txt`**: Test cases for the Red-Black Tree.
- **`.github/workflows/ci.yml`**: CI testing with github actions.

---

## Building the Project

### Prerequisites

- A C++17 compatible compiler.
- CMake (minimum version 3.11).
- Graphviz installed (`dot` command).

### Steps

1. Clone the repository.
2. Create a build directory and navigate into it:
   ```bash
   mkdir build && cd build
   ```
3. Configure the project using CMake:
   ```bash
   cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
   ```
4. Build the project:
   ```bash
   cmake --build build
   ```
5. Execute the program:
   ```bash
   build/rb_tree.x
   ```

---

## Running the Tests

1. Build the tests:
   ```bash
   cmake --build build --target test.x
   ```
2. Execute the test binary:
   ```bash
   build/test.x
   ```

---

## Example Usage

### Input Format
- **Commands:**
  - `k <key>`: Insert `<key>` into the Red-Black Tree.
  - `q <a> <b>`: Perform a range query for the range `[a, b]`.
- Example:
  ```plaintext
  k 10
  k 20
  k 15
  q 10 20
  ```

### Output
- For the above input:
  ```plaintext
  3
  ```

---

## Debugging with `std::set`

Enable the optional debugging mode using:
```bash
cmake -DSET_MODE_ENABLED=ON -DCUSTOM_SET_VALUE=DEBUG -S . -B build
cmake --build build
```
In this mode, range query results from the Red-Black Tree are compared against results from `std::set` and printed to the console.

---

## Graph Visualization

The program outputs a graph representation of the Red-Black Tree using Graphviz.  
1. Install Graphviz (`dot` command must be available).
2. After running the program, the tree is saved as a PNG in `graphviz/rb_tree.png`.

---

## Advanced Options

### Debug and Release Modes
Configure the build mode using:
```bash
cmake -DCMAKE_BUILD_TYPE=<Mode> -S . -B build
```
Replace `<MODE>` with `Debug` or `Release`.

### Custom Build Flags
Modify flags in `CMakeLists.txt` to fine-tune optimization and debugging behavior.
