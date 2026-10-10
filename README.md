# Kalman Filter Innovation Whiteness Test

A C++17/Eigen project implementing a linear Kalman filter and the
innovation-whiteness diagnostic from Section V of R. K. Mehra’s 1970 paper,
“On the Identification of Variances and Adaptive Kalman Filtering.”

The main example compares matched, too-large, and too-small assumed
process-noise covariances using identical synthetic position measurements.
The low-pass and scalar Kalman filters remain as supporting learning examples.
Covariance identification and automatic adaptation of Q and R are outside the
current implementation.

![Tracking and innovation correlations for larger, matched, and smaller assumed process-noise covariance](<docs/figures/plot_mehra_Q_comparison(33-1 33-3 33-5).png>)

In the seed-42 run, the matched filter had 2 of 100 correlations outside
the bounds and passed. The larger-Q filter had 5 and failed the strict
“less than 5%” rule; the smaller-Q filter had 100 and failed.

The test uses an approximate statistical rule. Passing means that the checked innovation correlations satisfy this rule; it does not prove that the innovations are white or that the assumed noise variances are correct.

### [Results: settings and measured results](docs/results.md)

## Requirements

For the C++ build:

- A compiler supporting C++17.
- CMake 3.14 or newer.
- Eigen 3.4 or a compatible later version.
- GoogleTest when building tests, which are enabled by default.

For plotting:

- Python 3.
- The packages listed in `scripts/requirements.txt`.

CMake checks for the required C++ libraries during configuration.
Install missing dependencies before configuring again.

## Install dependencies on Ubuntu 24.04

```bash
sudo apt update
sudo apt install build-essential cmake libeigen3-dev libgtest-dev python3 python3-venv
```

Run the remaining commands from the repository root.

Create and activate a Python virtual environment, then install the
plotting dependencies:

```bash
python3 -m venv .venv
source .venv/bin/activate
python -m pip install -r scripts/requirements.txt
```

Activate `.venv` again when opening a new terminal to run the plots.

## Configure and build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build --parallel
```

The first command finds dependencies and generates the build files.
The second compiles the libraries, examples, and tests.

## Run the tests

```bash
cd build
ctest --output-on-failure
cd ..
```

CTest runs the registered tests and displays details when a test fails.

## Run the main comparison

Generate the CSV files:

```bash
./build/examples/mehra_q_comparison
```

With the Python environment activated, display the comparison:

```bash
python scripts/plot_mehra_q_comparison.py
```

Generated data is written under `build/examples/`.
Run plotting commands from the repository root.

## Other examples

| Example | Executable | Plotting script |
|---|---|---|
| Single whiteness experiment | `build/examples/mehra_whiteness` | `scripts/plot_mehra_whiteness.py` |
| Filter comparison | `build/examples/filter_comparison` | `scripts/plot_filter_comparison.py` |
| 2D tracking | `build/examples/tracking_2d` | `scripts/plot_tracking_2d.py` |

Run the executable first, then its plotting script.

## Build without tests

GoogleTest is optional when tests are disabled:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
cmake --build build --parallel
```

## References

- R. K. Mehra, “On the Identification of Variances and Adaptive Kalman Filtering,”
  *IEEE Transactions on Automatic Control*, vol. AC-15, no. 2, April 1970,
  pp. 175–184.
  [DOI](https://doi.org/10.1109/TAC.1970.1099422). 
  
  Section V, printed pp. 177–178,
  provides the innovation-whiteness diagnostic implemented here.
  
- Becker, A. (2023). *Kalman filter from the ground up* (1st ed.).
  KalmanFilter.net. [Book information](https://kalmanfilter.net/book.html).

  Used as a reference for matrix notation and the equations of the scalar
  and linear Kalman filters. The 2D validation test uses measurements from
  Table 9.1 (p. 198) and reference state estimates from pp. 200–202 in
  Section 9.1, “Example 9 – vehicle location estimation.”
