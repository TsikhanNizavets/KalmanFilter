import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("./build/sensor_data.csv")

plt.figure(figsize=(10, 5))
plt.plot(df["time"], df["raw"], "ro", markersize=3, label="Raw Input")
plt.plot(df["time"], df["filtered"], label="Filtered Output", color="blue", linewidth=2)

plt.xlabel("Time (s)")
plt.ylabel("Position")
plt.title("Low-Pass Filter Output")
plt.legend()
plt.grid(True)

plt.show()