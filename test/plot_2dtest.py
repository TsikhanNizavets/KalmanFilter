import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("./build/test/2dtest_data.csv")

fig, axes = plt.subplots(2, 1, figsize=(10, 8), sharex=True)

axes[0].plot(df["time"], df["vel_x_est"], marker="o", label="vel x est - time", color="blue", linewidth=2)
axes[0].set_ylabel("Vel x est")
axes[0].legend()
axes[0].grid(True)

axes[1].plot(df["time"], df["vel_y_est"], marker="o", color="green", linewidth=2)
axes[1].set_xlabel("Time (s)")
axes[1].set_ylabel("Vel y est")
axes[1].set_title("vel y est - time")
axes[1].grid(True)

plt.tight_layout()
plt.show()