import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("./build/examples/mehra_whiteness_data.csv")

fig, axes = plt.subplots(3, 2, figsize=(10, 8), sharex=True)

axes[0][0].plot(df["time"], df["true_position"], label="True Position", color="black", linewidth=2)
axes[0][0].plot(df["time"], df["raw_position"], label="Raw Position", color="red", linewidth=2)
axes[0][0].plot(df["time"], df["estimated_position"], label="Estimated Position", color="blue", linewidth=2)
axes[0][0].set_ylabel("Position")
axes[0][0].legend()
axes[0][0].grid(True)

axes[1][0].plot(df["time"], df["innovation"], marker="o", label="Innovation", color="black", linewidth=2)
axes[1][0].set_ylabel("Innovation")
axes[1][0].legend()
axes[1][0].grid(True)

axes[0][1].plot(df["time"], df["pos_gain"], marker="o", label="Position Gain", color="red", linewidth=2)
axes[0][1].set_ylabel("Position Gain")
axes[0][1].legend()
axes[0][1].grid(True)

axes[1][1].plot(df["time"], df["vel_gain"], marker="o", label="Velocity Gain", color="red", linewidth=2)
axes[1][1].set_ylabel("Velocity Gain")
axes[1][1].legend()
axes[1][1].grid(True)

axes[2][1].plot(df["time"], df["acc_gain"], marker="o", label="Acceleration Gain", color="red", linewidth=2)
axes[2][1].set_xlabel("Time (s)")
axes[2][1].set_ylabel("Acceleration Gain")
axes[2][1].legend()
axes[2][1].grid(True)

plt.tight_layout()
plt.show()
