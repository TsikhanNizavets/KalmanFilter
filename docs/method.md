# Method

This project implements a linear Kalman filter and the innovation-correlation
diagnostic in Section V of Mehra's 1970 paper. The main experiment changes the
assumed process-noise covariance while keeping the measurements fixed. It checks
whether the innovations — the differences between measurements and their predicted values — are consistent with white noise.
Covariance identification and automatic adaptation of $Q$ and $R$ are outside
the implementation. See [results](results.md) for the measured comparison.

## State model and kalman filter

The model in [mehra_q_comparison_demo.cpp](../examples/mehra_q_comparison_demo.cpp) and [mehra_whiteness_demo.cpp](../examples/mehra_whiteness_demo.cpp) tracks motion along one axis. 
At sample $i$, the state and measurement are defined as follows:

$$
x_i=F x_{i-1}+G u_i+w_i,
\qquad z_i=Hx_i+\epsilon_i.
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
\end{array}
$$

For the main experiment [mehra_q_comparison_demo.cpp](../examples/mehra_q_comparison_demo.cpp), the filter setup is shown below, including the corrected state estimate, model matrices, noise distributions, and initialization:

$$
\begin{gathered}
\begin{array}{ccc}
\hat{x}_{i|i}=\begin{bmatrix}\hat{p}_{i|i}\\\hat{v}_{i|i}\\\hat{a}_{i|i}\end{bmatrix}
&
\qquad u_i=\begin{bmatrix}0\end{bmatrix}
&
\qquad z_i=\begin{bmatrix}p_i^{\mathrm{meas}}\end{bmatrix}
\\[2em]
F=\begin{bmatrix}
1&\Delta t&\Delta t^2/2\\
0&1&\Delta t\\
0&0&1
\end{bmatrix}
&
\qquad G=\begin{bmatrix}0\\0\\0\end{bmatrix}
&
\qquad H=\begin{bmatrix}1&0&0\end{bmatrix}
\end{array}
\\[4em]
\begin{array}{cc}
w_i\sim\mathcal{N}\left(
\begin{bmatrix}0\\0\\0\end{bmatrix},Q_{\mathrm{true}}
\right)
&
\qquad \epsilon_i\sim\mathcal{N}(0,R_{\mathrm{true}})
\\[2em]
Q_{\mathrm{true}}=q_{\mathrm{true}}\begin{bmatrix}
\Delta t^6/36&\Delta t^5/12&\Delta t^4/6\\
\Delta t^5/12&\Delta t^4/4&\Delta t^3/2\\
\Delta t^4/6&\Delta t^3/2&\Delta t^2
\end{bmatrix}
&
\qquad R_{\mathrm{true}}=\begin{bmatrix}0.0025\end{bmatrix}
\\[2em]
\hat{x}_0=\begin{bmatrix}0\\0\\0\end{bmatrix}
&
\qquad P_0=\begin{bmatrix}
1&0&0\\
0&1&0\\
0&0&1
\end{bmatrix}
\end{array}
\end{gathered}
$$

- $p_i$ — position.
- $v_i$ — velocity.
- $a_i$ — acceleration.
- $\Delta t$ — sampling interval.
- $q_j$ — variance of the random jerk applied during each interval.

$Q_{\mathrm{true}}$ and $R_{\mathrm{true}}$ are the covariances of the simulated process noise; $Q_{\mathrm{assumed}}$ and $R_{\mathrm{assumed}}$ in the filter equations is the assumed process-noise covariance and may differ from it.
The process and measurement noises are independent of each other and independent across samples.

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

- $K_i$ — Kalman gain.
- $P$ — estimation-error covariance.
- $Q$ — process-noise covariance.
- $R$ — measurement-noise covariance.
- $I$ — identity matrix.

The notation $\hat{x}_{i|i-1}$ denotes the predicted state estimate before using measurement $z_i$, while $\hat{x}_{i|i}$ denotes the corrected estimate after using it.
The unadorned $x_i$ in the system model denotes the true state; hats mark estimates.

## Section V innovation-whiteness diagnostic

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
At lags $1,\ldots,L$, count correlations outside the approximate 95% bounds $\pm1.96/\sqrt N$.
Pass only when less than 5% are outside.
Lag zero supplies normalization and is excluded from the count.
Passing this finite-sample rule does not prove whiteness.

Implementation: [innovation_analysis.cpp](../src/innovation_analysis.cpp).


## References

- R. K. Mehra, “On the Identification of Variances and Adaptive Kalman Filtering,”
  *IEEE Transactions on Automatic Control*, vol. AC-15, no. 2, April 1970,
  pp. 175–184. 
  [DOI](https://doi.org/10.1109/TAC.1970.1099422). The implemented diagnostic is
  Section V, printed pp. 177–178, Eqs. (14)–(19) and the following test rule.
