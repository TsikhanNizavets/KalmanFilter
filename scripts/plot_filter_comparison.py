import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("./build/examples/filter_comparison_data.csv")

plt.figure(figsize=(10, 5))
plt.plot(df["time"], df["raw_position"], "ro", markersize=3, label="Raw Input")
plt.plot(df["time"], df["low_pass"], label="Low-Pass Filter", color="blue", linewidth=2)
plt.plot(df["time"], df["kalman"], label="Scalar Kalman Filter", color="green", linewidth=2)
plt.plot(df["time"], df["kalman_const_acc"], label="Linear Kalman Filter", color="purple", linewidth=2)
plt.plot(df["time"], df["true_position"], label="True Position", color="black", linewidth=2)

plt.xlim(df["time"].iloc[0], df["time"].iloc[-1])
plt.xlabel("Time (s)")
plt.ylabel("Position")
plt.title("Low-Pass Filter vs Scalar Kalman Filter vs Linear Kalman Filter")
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.show()