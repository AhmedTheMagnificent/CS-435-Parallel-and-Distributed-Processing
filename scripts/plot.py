"""
Reads timing.csv produced by ./benchmark and plots:
  1. Wall time: Serial vs OpenMP
  2. Speedup over serial

Usage: python3 scripts/plot.py
"""
import csv
import matplotlib.pyplot as plt
import numpy as np

sizes, serial, openmp = [], [], []

with open(r"build\timing.csv") as f:
    for row in csv.DictReader(f):
        sizes.append(int(row["N"]))
        serial.append(float(row["serial_s"]))
        openmp.append(float(row["openmp_s"]))

x = np.array(sizes)

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

ax1.plot(x, serial, "o-", label="Serial")
ax1.plot(x, openmp, "s-", label="OpenMP")
ax1.set_xlabel("Grid size N")
ax1.set_ylabel("Time (s)")
ax1.set_title("Wall Time")
ax1.set_xscale("log", base=2)
ax1.set_yscale("log")
ax1.legend()
ax1.grid(True, alpha=0.3)

ax2.plot(x, np.array(serial) / np.array(openmp), "s-", color="green")
ax2.axhline(1, color="gray", linestyle="--")
ax2.set_xlabel("Grid size N")
ax2.set_ylabel("Speedup")
ax2.set_title("OpenMP Speedup over Serial")
ax2.set_xscale("log", base=2)
ax2.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig("plot.png", dpi=150)
print("Saved plot.png")

