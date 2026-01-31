import psutil
import serial
import time
import GPUtil

# Open COM5 at 115200 baud
ser = serial.Serial("COM5", 115200)

# Track previous network counters for speed calculation
prev_sent = psutil.net_io_counters().bytes_sent
prev_recv = psutil.net_io_counters().bytes_recv
prev_time = time.time()

def format_speed(bytes_per_sec):
    kbps = bytes_per_sec / 1024
    if kbps < 1024:
        return f"{int(kbps)} KB/s"
    else:
        mbps = kbps / 1024
        return f"{mbps:.1f} MB/s"

while True:
    # CPU and RAM
    cpu = psutil.cpu_percent()
    mem = psutil.virtual_memory().percent

    # GPU
    gpus = GPUtil.getGPUs()
    gpu = gpus[0].load * 100 if gpus else 0

    # Network speed calculation
    counters = psutil.net_io_counters()
    now = time.time()
    elapsed = now - prev_time
    sent_speed = (counters.bytes_sent - prev_sent) / elapsed
    recv_speed = (counters.bytes_recv - prev_recv) / elapsed
    prev_sent, prev_recv, prev_time = counters.bytes_sent, counters.bytes_recv, now

    msg = (
    f"CPU:{cpu:.0f}% RAM:{mem:.0f}%\n"
    f"Net up: {format_speed(sent_speed)}\n"
    f"Net dn: {format_speed(recv_speed)}\n"
    f"GPU:{gpu:.0f}%\n"
)

    ser.write(msg.encode())
    print("Sent:\n" + msg)
    time.sleep(1)
