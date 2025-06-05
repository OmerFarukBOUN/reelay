import json
import math
from scipy.stats import sem, t
import matplotlib.pyplot as plt  # Import matplotlib for plotting

def calculate_transfer_times(sender_data_path, receiver_data_path):
    # Load JSON data from files
    with open(sender_data_path, "r") as sender_file:
        sender_data = json.load(sender_file)

    with open(receiver_data_path, "r") as receiver_file:
        receiver_data = json.load(receiver_file)

    # Ensure both files have the same number of packets
    if len(sender_data) != len(receiver_data):
        print(f"Sender packets: {len(sender_data)}, Receiver packets: {len(receiver_data)}")
        raise ValueError("Mismatch in the number of packets between sender and receiver files.")

    # Calculate transfer times
    transfer_times = [
        receiver["recieve_time"] - sender["send_time"]
        for sender, receiver in zip(sender_data, receiver_data)
    ]
    send_times = [entry["send_time"] for entry in sender_data]
    receive_times = [entry["recieve_time"] for entry in receiver_data]

    # Calculate statistics
    mean_transfer_time = sum(transfer_times) / len(transfer_times)
    min_transfer_time = min(transfer_times)
    max_transfer_time = max(transfer_times)
    send_period = (send_times[-1] - send_times[0])/ (len(send_times) - 1) if len(send_times) > 1 else 0
    receive_period = (receive_times[-1] - receive_times[0]) / (len(receive_times) - 1) if len(receive_times) > 1 else 0
    print(f"Send Period: {send_period:.2f} µs")
    print(f"Receive Period: {receive_period:.2f} µs")

    # Calculate 95% confidence interval
    confidence = 0.95
    n = len(transfer_times)
    standard_error = sem(transfer_times)
    confidence_interval = t.interval(confidence, n - 1, loc=mean_transfer_time, scale=standard_error)

    # Output results
    print(f"sender_data_path: {sender_data_path}")
    print(f"receiver_data_path: {receiver_data_path}")
    print(f"Mean Transfer Time: {mean_transfer_time:.2f} µs")
    print(f"Min Transfer Time: {min_transfer_time:.2f} µs")
    print(f"Max Transfer Time: {max_transfer_time:.2f} µs")
    print(f"95% Confidence Interval: ({confidence_interval[0]:.2f} µs, {confidence_interval[1]:.2f} µs)")

    # # Plot transfer times
    # print("Plotting transfer times...")
    # plt.figure(figsize=(10, 6))
    # plt.plot(transfer_times, label="Transfer Times", marker="o", linestyle="-", color="b")
    # plt.axhline(mean_transfer_time, color="r", linestyle="--", label=f"Mean: {mean_transfer_time:.2f} µs")
    # plt.fill_between(
    #     range(len(transfer_times)),
    #     confidence_interval[0],
    #     confidence_interval[1],
    #     color="r",
    #     alpha=0.2,
    #     label="95% Confidence Interval"
    # )
    # plt.title("Transfer Times")
    # plt.xlabel("Packet Number")
    # plt.ylabel("Transfer Time (µs)")
    # plt.legend()
    # plt.grid(True)
    # plt.tight_layout()

    # # Show the plot
    # plt.show()

# Example usage
calculate_transfer_times("/zenoh-bridge/sender.json", "/shared_data/receiver.json")