import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("./build/sensor_data.csv")

plt.figure(figsize=(10, 5))
plt.plot(df["time"], df["raw"], "ro", markersize=3, label="Raw Input")
plt.plot(df["time"], df["low_pass"], label="Low-Pass Filter", color="blue", linewidth=2)
plt.plot(df["time"], df["kalman"], label="Kalman Filter 1D", color="green", linewidth=2)
plt.plot(df["time"], df["true_position"], label="True Position", color="black", linewidth=2)

plt.xlabel("Time (s)")
plt.ylabel("Position")
plt.title("Low-Pass Filter vs Kalman Filter 1D")
plt.legend()
plt.grid(True)

plt.show()