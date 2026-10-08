import pandas as pd
import matplotlib.pyplot as plt

filter_output_data_Q_smaller = pd.read_csv("./build/examples/mehra_whiteness_data_Q_smaller.csv")
autocorrelation_data_Q_smaller = pd.read_csv("./build/examples/normalized_autocorrelation_data_Q_smaller.csv")
bound_Q_smaller = autocorrelation_data_Q_smaller["bound"].iloc[0]

filter_output_data_Q_matched = pd.read_csv("./build/examples/mehra_whiteness_data_Q_matched.csv")
autocorrelation_data_Q_matched = pd.read_csv("./build/examples/normalized_autocorrelation_data_Q_matched.csv")
bound_Q_matched = autocorrelation_data_Q_matched["bound"].iloc[0]

filter_output_data_Q_larger = pd.read_csv("./build/examples/mehra_whiteness_data_Q_larger.csv")
autocorrelation_data_Q_larger = pd.read_csv("./build/examples/normalized_autocorrelation_data_Q_larger.csv")
bound_Q_larger = autocorrelation_data_Q_larger["bound"].iloc[0]


zoom_start_time = 65.0
zoom_end_time = 70.0

fig, axes = plt.subplots(3, 3, figsize=(18, 12), sharex=False)

# Larger Q
axes[0][0].plot(filter_output_data_Q_larger["time"], filter_output_data_Q_larger["true_position"], label="True Position", color="black", linewidth=1)
axes[0][0].plot(filter_output_data_Q_larger["time"], filter_output_data_Q_larger["raw_position"], label="Raw Position", color="red", linewidth=1)
axes[0][0].plot(filter_output_data_Q_larger["time"], filter_output_data_Q_larger["estimated_position"], label="Estimated Position", color="blue", linewidth=1)
axes[0][0].set_title("Larger Q", fontsize=14)
axes[0][0].set_ylabel("Position")
axes[0][0].set_xlabel("Time (s)")
axes[0][0].legend()
axes[0][0].grid(True)

zoom_data_Q_larger = filter_output_data_Q_larger[
    (filter_output_data_Q_larger["time"] >= zoom_start_time)
    & (filter_output_data_Q_larger["time"] <= zoom_end_time)
]

axes[1][0].plot(zoom_data_Q_larger["time"], zoom_data_Q_larger["raw_position"], color="red", linewidth=1, label="Raw Position")
axes[1][0].plot(zoom_data_Q_larger["time"], zoom_data_Q_larger["true_position"], color="black", linewidth=1.5, label="True Position")
axes[1][0].plot(zoom_data_Q_larger["time"], zoom_data_Q_larger["estimated_position"], color="blue", linewidth=1.5, label="Estimated Position")
axes[1][0].set_xlim(zoom_start_time, zoom_end_time)
axes[1][0].set_xlabel("Time (s)")
axes[1][0].set_ylabel("Position")
axes[1][0].set_title(f"Position tracking: {zoom_start_time:g}–{zoom_end_time:g} s")
axes[1][0].legend()
axes[1][0].grid(True)

nonzero_lag_data_Q_larger = autocorrelation_data_Q_larger[autocorrelation_data_Q_larger["lag"] > 0]

outside_bounds_Q_larger = nonzero_lag_data_Q_larger["autocorrelation"].abs() > bound_Q_larger
outside_bound_data_Q_larger = nonzero_lag_data_Q_larger[outside_bounds_Q_larger]

axes[2][0].plot(nonzero_lag_data_Q_larger["lag"], nonzero_lag_data_Q_larger["autocorrelation"], marker="o", color="black", linewidth=2, label="Normalized autocorrelation")
axes[2][0].axhline(y=bound_Q_larger, color="red", linestyle="--", label="95% bounds")
axes[2][0].axhline(y=-bound_Q_larger, color="red", linestyle="--")
axes[2][0].scatter(outside_bound_data_Q_larger["lag"], outside_bound_data_Q_larger["autocorrelation"], color="red", zorder=3, label="Outside 95% bounds")
axes[2][0].set_xlabel("Lags")
axes[2][0].set_ylabel("Normalized autocorrelation")

values_checked_Q_larger = len(nonzero_lag_data_Q_larger)
values_outside_bounds_Q_larger = outside_bounds_Q_larger.sum()

outside_percentage_Q_larger = 100 * values_outside_bounds_Q_larger / values_checked_Q_larger
passes_test_Q_larger = values_outside_bounds_Q_larger < 0.05 * values_checked_Q_larger

if passes_test_Q_larger:
    axes[2][0].set_title(
        f"Outside bounds: {values_outside_bounds_Q_larger}/{values_checked_Q_larger} "
        f"({outside_percentage_Q_larger:.1f}%) < 5%\nWhiteness test: PASS",
        fontsize=10
    )
else:
    axes[2][0].set_title(
        f"Outside bounds: {values_outside_bounds_Q_larger}/{values_checked_Q_larger} "
        f"({outside_percentage_Q_larger:.1f}%) >= 5%\nWhiteness test: FAIL",
        fontsize=10
    )

axes[2][0].legend()
axes[2][0].grid(True)


# Matched Q
axes[0][1].plot(filter_output_data_Q_matched["time"], filter_output_data_Q_matched["true_position"], label="True Position", color="black", linewidth=1)
axes[0][1].plot(filter_output_data_Q_matched["time"], filter_output_data_Q_matched["raw_position"], label="Raw Position", color="red", linewidth=1)
axes[0][1].plot(filter_output_data_Q_matched["time"], filter_output_data_Q_matched["estimated_position"], label="Estimated Position", color="blue", linewidth=1)
axes[0][1].set_title("Matched Q", fontsize=14)
axes[0][1].set_ylabel("Position")
axes[0][1].set_xlabel("Time (s)")
axes[0][1].legend()
axes[0][1].grid(True)

zoom_data_Q_matched = filter_output_data_Q_matched[
    (filter_output_data_Q_matched["time"] >= zoom_start_time)
    & (filter_output_data_Q_matched["time"] <= zoom_end_time)
]

axes[1][1].plot(zoom_data_Q_matched["time"], zoom_data_Q_matched["raw_position"], color="red", linewidth=1, label="Raw Position")
axes[1][1].plot(zoom_data_Q_matched["time"], zoom_data_Q_matched["true_position"], color="black", linewidth=1.5, label="True Position")
axes[1][1].plot(zoom_data_Q_matched["time"], zoom_data_Q_matched["estimated_position"], color="blue", linewidth=1.5, label="Estimated Position")
axes[1][1].set_xlim(zoom_start_time, zoom_end_time)
axes[1][1].set_xlabel("Time (s)")
axes[1][1].set_ylabel("Position")
axes[1][1].set_title(f"Position tracking: {zoom_start_time:g}–{zoom_end_time:g} s")
axes[1][1].legend()
axes[1][1].grid(True)

nonzero_lag_data_Q_matched = autocorrelation_data_Q_matched[autocorrelation_data_Q_matched["lag"] > 0]

outside_bounds_Q_matched = nonzero_lag_data_Q_matched["autocorrelation"].abs() > bound_Q_matched
outside_bound_data_Q_matched = nonzero_lag_data_Q_matched[outside_bounds_Q_matched]

axes[2][1].plot(nonzero_lag_data_Q_matched["lag"], nonzero_lag_data_Q_matched["autocorrelation"], marker="o", color="black", linewidth=2, label="Normalized autocorrelation")
axes[2][1].axhline(y=bound_Q_matched, color="red", linestyle="--", label="95% bounds")
axes[2][1].axhline(y=-bound_Q_matched, color="red", linestyle="--")
axes[2][1].scatter(outside_bound_data_Q_matched["lag"], outside_bound_data_Q_matched["autocorrelation"], color="red", zorder=3, label="Outside 95% bounds")
axes[2][1].set_xlabel("Lags")
axes[2][1].set_ylabel("Normalized autocorrelation")

values_checked_Q_matched = len(nonzero_lag_data_Q_matched)
values_outside_bounds_Q_matched = outside_bounds_Q_matched.sum()

outside_percentage_Q_matched = 100 * values_outside_bounds_Q_matched / values_checked_Q_matched
passes_test_Q_matched = values_outside_bounds_Q_matched < 0.05 * values_checked_Q_matched

if passes_test_Q_matched:
    axes[2][1].set_title(
        f"Outside bounds: {values_outside_bounds_Q_matched}/{values_checked_Q_matched} "
        f"({outside_percentage_Q_matched:.1f}%) < 5%\nWhiteness test: PASS",
        fontsize=10
    )
else:
    axes[2][1].set_title(
        f"Outside bounds: {values_outside_bounds_Q_matched}/{values_checked_Q_matched} "
        f"({outside_percentage_Q_matched:.1f}%) >= 5%\nWhiteness test: FAIL",
        fontsize=10
    )

axes[2][1].legend()
axes[2][1].grid(True)


# Smaller Q
axes[0][2].plot(filter_output_data_Q_smaller["time"], filter_output_data_Q_smaller["true_position"], label="True Position", color="black", linewidth=1)
axes[0][2].plot(filter_output_data_Q_smaller["time"], filter_output_data_Q_smaller["raw_position"], label="Raw Position", color="red", linewidth=1)
axes[0][2].plot(filter_output_data_Q_smaller["time"], filter_output_data_Q_smaller["estimated_position"], label="Estimated Position", color="blue", linewidth=1)
axes[0][2].set_title("Smaller Q", fontsize=14)
axes[0][2].set_ylabel("Position")
axes[0][2].set_xlabel("Time (s)")
axes[0][2].legend()
axes[0][2].grid(True)

zoom_data_Q_smaller = filter_output_data_Q_smaller[
    (filter_output_data_Q_smaller["time"] >= zoom_start_time)
    & (filter_output_data_Q_smaller["time"] <= zoom_end_time)
]

axes[1][2].plot(zoom_data_Q_smaller["time"], zoom_data_Q_smaller["raw_position"], color="red", linewidth=1, label="Raw Position")
axes[1][2].plot(zoom_data_Q_smaller["time"], zoom_data_Q_smaller["true_position"], color="black", linewidth=1.5, label="True Position")
axes[1][2].plot(zoom_data_Q_smaller["time"], zoom_data_Q_smaller["estimated_position"], color="blue", linewidth=1.5, label="Estimated Position")
axes[1][2].set_xlim(zoom_start_time, zoom_end_time)
axes[1][2].set_xlabel("Time (s)")
axes[1][2].set_ylabel("Position")
axes[1][2].set_title(f"Position tracking: {zoom_start_time:g}–{zoom_end_time:g} s")
axes[1][2].legend()
axes[1][2].grid(True)

nonzero_lag_data_Q_smaller = autocorrelation_data_Q_smaller[autocorrelation_data_Q_smaller["lag"] > 0]

outside_bounds_Q_smaller = nonzero_lag_data_Q_smaller["autocorrelation"].abs() > bound_Q_smaller
outside_bound_data_Q_smaller = nonzero_lag_data_Q_smaller[outside_bounds_Q_smaller]

axes[2][2].plot(nonzero_lag_data_Q_smaller["lag"], nonzero_lag_data_Q_smaller["autocorrelation"], marker="o", color="black", linewidth=2, label="Normalized autocorrelation")
axes[2][2].axhline(y=bound_Q_smaller, color="red", linestyle="--", label="95% bounds")
axes[2][2].axhline(y=-bound_Q_smaller, color="red", linestyle="--")
axes[2][2].scatter(outside_bound_data_Q_smaller["lag"], outside_bound_data_Q_smaller["autocorrelation"], color="red", zorder=3, label="Outside 95% bounds")
axes[2][2].set_xlabel("Lags")
axes[2][2].set_ylabel("Normalized autocorrelation")

values_checked_Q_smaller = len(nonzero_lag_data_Q_smaller)
values_outside_bounds_Q_smaller = outside_bounds_Q_smaller.sum()

outside_percentage_Q_smaller = 100 * values_outside_bounds_Q_smaller / values_checked_Q_smaller
passes_test_Q_smaller = values_outside_bounds_Q_smaller < 0.05 * values_checked_Q_smaller

if passes_test_Q_smaller:
    axes[2][2].set_title(
        f"Outside bounds: {values_outside_bounds_Q_smaller}/{values_checked_Q_smaller} "
        f"({outside_percentage_Q_smaller:.1f}%) < 5%\nWhiteness test: PASS",
        fontsize=10
    )
else:
    axes[2][2].set_title(
        f"Outside bounds: {values_outside_bounds_Q_smaller}/{values_checked_Q_smaller} "
        f"({outside_percentage_Q_smaller:.1f}%) >= 5%\nWhiteness test: FAIL",
        fontsize=10
    )

axes[2][2].legend()
axes[2][2].grid(True)


fig.tight_layout()

plt.show(block=True)
