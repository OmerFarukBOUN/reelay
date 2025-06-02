import json
import math
from scipy.stats import sem, t

# Load JSON data from files
with open("/zenoh-bridge/sender.json", "r") as sender_file:
    sender_data = json.load(sender_file)

with open("/zenoh-bridge/receiver.json", "r") as receiver_file:
    receiver_data = json.load(receiver_file)

# Ensure both files have the same number of packets
if len(sender_data) != len(receiver_data):
    raise ValueError("Mismatch in the number of packets between sender and receiver files.")

# Calculate transfer times
transfer_times = [
    receiver["recieve_time"] - sender["send_time"]
    for sender, receiver in zip(sender_data, receiver_data)
]

# Calculate statistics
mean_transfer_time = sum(transfer_times) / len(transfer_times)
min_transfer_time = min(transfer_times)
max_transfer_time = max(transfer_times)

# Calculate 95% confidence interval
confidence = 0.95
n = len(transfer_times)
standard_error = sem(transfer_times)
confidence_interval = t.interval(confidence, n - 1, loc=mean_transfer_time, scale=standard_error)

# Output results
print(f"Mean Transfer Time: {mean_transfer_time:.2f} µs")
print(f"Min Transfer Time: {min_transfer_time:.2f} µs")
print(f"Max Transfer Time: {max_transfer_time:.2f} µs")
print(f"95% Confidence Interval: ({confidence_interval[0]:.2f} µs, {confidence_interval[1]:.2f} µs)")