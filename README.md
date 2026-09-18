# Kalman Filter Demo

A small C++ project that compares a first-order low-pass filter with a
one-dimensional Kalman filter.

The program generates synthetic position data, adds Gaussian sensor noise,
filters the noisy measurements, and saves the results to a CSV file.

## Project Structure

- `src/main.cpp` - Generates sensor data and runs the filters
- `src/LowPassFilter.cpp` - Low-pass filter implementation
- `src/kalman_filter_1d.cpp` - One-dimensional Kalman filter implementation
- `include/` - Header files
- `plot_data.py` - Creates a graph from the generated CSV data
- `CMakeLists.txt` - CMake build configuration

## Requirements

- C++17 compiler
- CMake
- Python 3
- Python packages:
  - pandas
  - matplotlib

Install the Python packages with:

```bash
python3 -m pip install pandas matplotlib