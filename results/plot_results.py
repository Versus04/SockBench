import matplotlib.pyplot as plt
import csv
import os

base_dir = os.path.dirname(os.path.abspath(__file__))
csv_path = os.path.join(base_dir, "..", "results.csv")

message_sizes = []
latency_ms = []
throughput_mb = []

with open(csv_path, "r") as f:
    reader = csv.DictReader(f)
    for row in reader:
        message_sizes.append(int(row["message_size"]))
        latency_ms.append(float(row["avg_latency_ms"]))
        throughput_mb.append(float(row["throughput_MBps"]))

plt.figure()
plt.plot(message_sizes, latency_ms, marker='o')
plt.xscale('log')
plt.xlabel("Message Size (bytes)")
plt.ylabel("Average Latency (ms)")
plt.title("Latency vs Message Size")
plt.grid(True)
plt.savefig(os.path.join(base_dir, "latency.png"))

plt.figure()
plt.plot(message_sizes, throughput_mb, marker='o', color='orange')
plt.xscale('log')
plt.xlabel("Message Size (bytes)")
plt.ylabel("Throughput (MB/s)")
plt.title("Throughput vs Message Size")
plt.grid(True)
plt.savefig(os.path.join(base_dir, "throughput.png"))

print("Plots saved in results/ as latency.png and throughput.png")
