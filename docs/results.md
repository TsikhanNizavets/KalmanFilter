# Results

## Whiteness test and linear kalman filter

[mehra_q_comparison_demo.cpp](../examples/mehra_q_comparison_demo.cpp) and [mehra_whiteness_demo.cpp](../examples/mehra_whiteness_demo.cpp) use the test from Mehra's Section V, implemented in [innovation_analysis.cpp](../src/innovation_analysis.cpp), to analyse correlations between the innovations of a linear Kalman filter and examine how differences between $Q_{assumed}$ and $Q_{true}$ affect these correlations. The same test can also be used to examine the effect of differences between $R_{assumed}$ and $R_{true}$.

### Linear kalman filter 

Linear kalman filter is implemented in [linear_kalman_filter.cpp](../src/linear_kalman_filter.cpp). It uses following state model:

$$
x_i=F x_{i-1}+G u_i+w_i,
\qquad z_i=Hx_i+\epsilon_i.
$$

The Kalman filter performs the following prediction and update steps:

**Prediction**

$$
\begin{aligned}
\hat{x}_{i|i-1}
&= F\hat{x}_{i-1|i-1}+Gu_i
\\[1em]
P_{i|i-1}
&= FP_{i-1|i-1}F^T+Q_{\mathrm{assumed}}
\end{aligned}
$$

**Update**

$$
\begin{aligned}
K_i
&= P_{i|i-1}H^T
   \left(HP_{i|i-1}H^T+R_{\mathrm{assumed}}\right)^{-1}
\\[1em]
\hat{x}_{i|i}
&= \hat{x}_{i|i-1}
   +K_i\left(z_i-H\hat{x}_{i|i-1}\right)
\\[1em]
P_{i|i}
&= (I-K_iH)P_{i|i-1}(I-K_iH)^T
   +K_iR_{\mathrm{assumed}}K_i^T
\end{aligned}
$$

where

$$
\begin{array}{ll}
x_i \in \mathbb{R}^{n_x \times 1}\text{ — state vector}
&
\qquad F \in \mathbb{R}^{n_x \times n_x}\text{ — state transition matrix}
\\[1em]
u_i \in \mathbb{R}^{n_u \times 1}\text{ — control input}
&
\qquad G \in \mathbb{R}^{n_x \times n_u}\text{ — control matrix}
\\[1em]
z_i \in \mathbb{R}^{n_z \times 1}\text{ — measurement vector}
&
\qquad H \in \mathbb{R}^{n_z \times n_x}\text{ — observation matrix}
\\[1em]
w_i \sim \mathcal{N}(0_{n_x \times 1}, Q)\text{ — process noise}
&
\qquad \epsilon_i \sim \mathcal{N}(0_{n_z \times 1}, R)\text{ — measurement noise}
\\[1em]
Q \in \mathbb{R}^{n_x \times n_x}\text{ — process noise covariance}
&
\qquad R \in \mathbb{R}^{n_z \times n_z}\text{ — measurement covariance}
\\[1em]
K \in \mathbb{R}^{n_x \times n_z}\text{ — kalman gain}
&
\qquad P_i \in \mathbb{R}^{n_x \times n_x}\text{ — estimate covariance}
\end{array}
$$

$Q_{\mathrm{true}}$ and $R_{\mathrm{true}}$ are the covariances of the simulated process noise and measurement noise, respectively. $Q_{\mathrm{assumed}}$ and $R_{\mathrm{assumed}}$ are the corresponding covariances assumed in the filter equations and may differ from the true values.

The process and measurement noises are independent of each other and independent across samples.

The notation $\hat{x}_{i|i-1}$ denotes the predicted state estimate before using measurement $z_i$, while $\hat{x}_{i|i}$ denotes the corrected estimate after using it. The same distinction applies to $P_{i|i-1}$ and $P_{i|i}$.

The unadorned $x_i$ in the system model denotes the true state; hats mark estimates.

### Whiteness test

Following [Mehra's Section V](https://doi.org/10.1109/TAC.1970.1099422), printed pp. 177–178, the main experiment checks scalar innovations:

$$
\nu_i=z_i-H\hat{x}_{i|i-1}.
$$

After skipping the first $s$ of $T$ samples, $N=T-s$ samples remain.
For lags $\ell=0,\ldots,L$, where $L$ is the maximum lag, compute

$$
\begin{aligned}
\widehat C_\ell
&=\frac{1}{N}\sum_{i=s+\ell}^{T-1}\nu_i\nu_{i-\ell}^{T}
\\[2em]
[\widehat{\rho}_\ell]_{ij}
&=
\frac{[\widehat{C}_\ell]_{ij}}
{\left([\widehat{C}_0]_{ii}\,[\widehat{C}_0]_{jj}\right)^{1/2}}.
\end{aligned}
$$

Here $i,j$ are the row and column indices of the correlation matrix. The test checks only its diagonal elements, where $i=j$.

Initial samples are skipped to reduce startup effects. Steady-state operation and zero-mean innovations are assumed, following Mehra.

At lags $1,\ldots,L$, count the correlations outside the approximate 95% bounds $\pm1.96/\sqrt N$. The test passes only when fewer than 5% of the tested correlations lie outside these bounds. Lag zero supplies normalization and is excluded from the count.

Passing this finite-sample rule does not prove whiteness.




## Mehra Q comparison demo
Implementation: [mehra_q_comparison_demo.cpp](../examples/mehra_q_comparison_demo.cpp).

Visualisation: [plot_mehra_q_comparison.cpp](../scripts/plot_mehra_q_comparison.py).

### Setup

<div align="center">

| Setting | Value |
|:---:|:---:|
| Sampling interval $dt$ | 0.02 s |
| Measurements per filter | 4,000 |
| True initial state $x_0$ | $[0,0.5,0]^T$ |
| Initial state estimate $\hat{x}_{0\vert0}$ | $[0,0,0]^T$ |
| Control inputs $u_i$ | $[0]^T$ |
| Initial estimate covariance $P_{0\vert0}$ | $\begin{bmatrix}1&0&0\\0&1&0\\0&0&1\end{bmatrix}$ |
| State transition matrix $F$ | $\begin{bmatrix}1&\Delta t&\Delta t^2/2\\0&1&\Delta t\\0&0&1\end{bmatrix}$ |
| Control matrix $G$ | $[0,0,0]^T$ |
| Observation matrix $H$ | $[1,0,0]$ |
| Process noise covariance matrix $Q$ | $q_j\begin{bmatrix}\Delta t^6/36&\Delta t^5/12&\Delta t^4/6\\\Delta t^5/12&\Delta t^4/4&\Delta t^3/2\\\Delta t^4/6&\Delta t^3/2&\Delta t^2\end{bmatrix}$ |
| True process noise variance $q_{j,\mathrm{true}}$ |  $0.033$ |
| Assumed process noise variances | $3.3$, $0.033$ and $0.00033$ |
| True measurement noise variance $R_{\mathrm{true}}$ | $[0.0025]$ |
| Assumed measurement noise variance $R_{\mathrm{assumed}}$ | $[0.0025]$ |
| Startup innovations skipped | 200 |
| Tested lags | 1–100 |

</div>

[synthetic_data](../examples/data_generation/synthetic_data.cpp) generates the data.
The motion equations are implemented directly and correspond to the filter's state model.
The generator uses the true process and measurement noise variances.

### Results

The matched-noise case passes the whiteness diagnostic, with 2 of 100 tested correlations outside the bounds.
The case with the larger assumed variance fails, with 5 of 100 correlations outside the bounds.
The case with the smaller assumed variance has all 100 correlations outside the bounds, showing strong correlation between innovations at different times.

![Three filters using identical measurements: full tracking, a 65–70 s zoom, and normalized innovation correlations](<figures/plot_mehra_Q_comparison(33-1 33-3 33-5).png>)

These results illustrate how the assumed process-noise variance affects innovation correlations.
All three filters were tested on one generated dataset.
Different random data could give different results.
Passing does not prove whiteness.

## Mehra whiteness demo

Implementation: [mehra_whiteness_demo.cpp](../examples/mehra_whiteness_demo.cpp).

Visualisation: [plot_mehra_whiteness_demo.cpp](../scripts/plot_mehra_whiteness.py).

### Setup

The model setup is the same as in the [Mehra Q comparison demo](#mehra-q-comparison-demo), except that only one assumed process noise variance is used, which equals 3.3.
The data are generated by [synthetic_data.cpp](../examples/data_generation/synthetic_data.cpp) using the random-jerk model. Its motion equations correspond to the filter's state model.

### Results

The Kalman gains settle to approximately constant values before the correlation analysis begins at 4 s.
This supports skipping the first 200 samples to reduce startup effects in this experiment.
Despite the settled gains, the whiteness diagnostic fails, with 5 of 100 correlations outside the bounds.

![Position tracking, innovations, normalized innovation correlations and Kalman gains](<figures/plot_mehra_whiteness_6(true 33-3 assumed 33-1).png>)

![Position tracking zoomed in to 60–70 s](<figures/plot_mehra_whiteness_1(true 33-3 assumed 33-1).png>)

## Filter comparison

Implementation: [filter_comparison.cpp](../examples/filter_comparison.cpp).

Visualisation: [plot_filter_comparison.py](../scripts/plot_filter_comparison.py).

### Setup

For learning purposes, a scalar Kalman filter and a low-pass filter were also implemented in [scalar_kalman_filter.cpp](../src/scalar_kalman_filter.cpp) and [low_pass_filter.cpp](../src/low_pass_filter.cpp), respectively.
The bump curve is generated with [synthetic_data.cpp](../examples/data_generation/synthetic_data.cpp).

### Results

The graph shows three different filters.
The linear Kalman filter uses an assumed process noise variance that is higher than the true process noise variance.
Without this increase, the filter overshoots because the state transition matrix $F$ describes motion with constant acceleration, while the true trajectory follows a bump curve.

![Filter comparison](<figures/plot_filter_comparison.png>)

The zoomed-in graph also shows that the scalar Kalman filter and the low-pass filter have almost the same trajectory.
In this implementation, their trajectories can match exactly if the parameters and initial conditions are chosen appropriately.
Both filters show a small delay.

![Zoomed-in filter comparison](<figures/plot_filter_comparison_zoomed_in.png>)

## References

- R. K. Mehra, “On the Identification of Variances and Adaptive Kalman Filtering,”
  *IEEE Transactions on Automatic Control*, vol. AC-15, no. 2, April 1970,
  pp. 175–184. 
  [DOI](https://doi.org/10.1109/TAC.1970.1099422). Section V, printed pp. 177–178,
  supplies the innovation-whiteness diagnostic.
