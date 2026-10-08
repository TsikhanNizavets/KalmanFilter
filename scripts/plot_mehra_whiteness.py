import pandas as pd
import matplotlib.pyplot as plt

filter_output_data = pd.read_csv("./build/examples/mehra_whiteness_data.csv")
autocorrelation_data = pd.read_csv("./build/examples/normalized_autocorrelation_data.csv")

bound = autocorrelation_data["bound"].iloc[0]

fig, axes = plt.subplots(3, 2, figsize=(10, 8), sharex=False)

axes[0][0].plot(filter_output_data["time"], filter_output_data["true_position"], label="True Position", color="black", linewidth=1)
axes[0][0].plot(filter_output_data["time"], filter_output_data["raw_position"], label="Raw Position", color="red", linewidth=1)
axes[0][0].plot(filter_output_data["time"], filter_output_data["estimated_position"], label="Estimated Position", color="blue", linewidth=1)
axes[0][0].set_ylabel("Position")
axes[0][0].set_xlabel("Time (s)")
axes[0][0].legend()
axes[0][0].grid(True)

axes[1][0].plot(filter_output_data["time"], filter_output_data["innovation"], label="Innovation", color="black", linewidth=0.5)
axes[1][0].set_ylabel("Innovation")
axes[1][0].set_xlabel("Time (s)")
axes[1][0].legend()
axes[1][0].grid(True)

axes[0][1].plot(filter_output_data["time"], filter_output_data["pos_gain"], label="Position Gain", color="red", linewidth=2)
axes[0][1].set_ylabel("Position Gain")
axes[0][1].set_xlabel("Time (s)")
axes[0][1].legend()
axes[0][1].grid(True)

axes[1][1].plot(filter_output_data["time"], filter_output_data["vel_gain"], label="Velocity Gain", color="red", linewidth=2)
axes[1][1].set_ylabel("Velocity Gain")
axes[1][1].set_xlabel("Time (s)")
axes[1][1].legend()
axes[1][1].grid(True)

axes[2][1].plot(filter_output_data["time"], filter_output_data["acc_gain"], label="Acceleration Gain", color="red", linewidth=2)
axes[2][1].set_xlabel("Time (s)")
axes[2][1].set_ylabel("Acceleration Gain")
axes[2][1].legend()
axes[2][1].grid(True)

nonzero_lag_data = autocorrelation_data[autocorrelation_data["lag"] > 0]

outside_bounds = nonzero_lag_data["autocorrelation"].abs() > bound
outside_bound_data = nonzero_lag_data[outside_bounds]

axes[2][0].plot(nonzero_lag_data["lag"], nonzero_lag_data["autocorrelation"], marker="o", color="black", linewidth=2, label="Normalized autocorrelation")

axes[2][0].axhline(y=bound, color="red", linestyle="--", label="95% bounds")
axes[2][0].axhline(y=-bound, color="red", linestyle="--")

axes[2][0].scatter(outside_bound_data["lag"], outside_bound_data["autocorrelation"], color="red", zorder=3, label="Outside 95% bounds")

axes[2][0].set_xlabel("Lags")
axes[2][0].set_ylabel("Normalized autocorrelation")

values_checked = len(nonzero_lag_data)
values_outside_bounds = outside_bounds.sum()

outside_percentage = 100 * values_outside_bounds / values_checked
passes_test = values_outside_bounds < 0.05 * values_checked

if passes_test:
    axes[2][0].set_title(
        f"Outside bounds: {values_outside_bounds}/{values_checked} "
        f"({outside_percentage:.1f}%) < 5% — Whiteness test: PASS"
    )
else:
    axes[2][0].set_title(
        f"Outside bounds: {values_outside_bounds}/{values_checked} "
        f"({outside_percentage:.1f}%) >= 5% — Whiteness test: FAIL"
    )

axes[2][0].legend()
axes[2][0].grid(True)

fig.tight_layout()


zoom_start_time = 60.0
zoom_end_time = 70.0

# Keep only rows inside that interval.
zoom_data = filter_output_data[
    (filter_output_data["time"] >= zoom_start_time)
    & (filter_output_data["time"] <= zoom_end_time)
]

zoom_fig, zoom_axes = plt.subplots(figsize=(10, 5))

zoom_axes.plot(zoom_data["time"], zoom_data["raw_position"], color="red", linewidth=1, label="Raw Position")

zoom_axes.plot(zoom_data["time"], zoom_data["true_position"], color="black", linewidth=1.5, label="True Position")

zoom_axes.plot(zoom_data["time"], zoom_data["estimated_position"], color="blue", linewidth=1.5, label="Estimated Position")

zoom_axes.set_xlim(zoom_start_time, zoom_end_time)
zoom_axes.set_xlabel("Time (s)")
zoom_axes.set_ylabel("Position")
zoom_axes.set_title(f"Position tracking: {zoom_start_time:g}–{zoom_end_time:g} s")

zoom_axes.legend()
zoom_axes.grid(True)
zoom_fig.tight_layout()

plt.show(block=True)

# show kalman gains separate and detailed
