import time
import RPi.GPIO as GPIO
# Constants
LED = 18               # GPIO pin for LED (BCM numbering)
THRESHOLD = 44         # Temperature threshold in °C
INTERVAL = 5           # Time (in seconds) between readings
DURATION = 25          # Total monitoring time in seconds
# GPIO setup
GPIO.setmode(GPIO.BCM)          # Use BCM (GPIO) pin numbering
GPIO.setup(LED, GPIO.OUT)       # Set the LED pin as output
GPIO.output(LED, GPIO.LOW)      # Ensure that the LED is initially off
# Function to read CPU temperature
def get_temp():
    print("Beginning system monitoring...")
    try:
        with open("/sys/class/thermal/thermal_zone0/temp", "r") as f:
            temp_r = f.readline()                   # Read temperature value
            return float(temp_r) / 1000.0           # Convert to degrees Centigrade
    except FileNotFoundError:
        print("Error: Cannot retrieve temperature data")
        return None
# Main monitoring loop
try:
    for _ in range(DURATION // INTERVAL):
        temp = get_temp()
        if temp is not None:
            print(f"CPU temperature: {temp:.2f} degrees C")
            if temp > THRESHOLD:
                print("Warning: CPU temperature exceeded the threshold")
                GPIO.output(LED, GPIO.HIGH)         # Turn on the LED if temp exceeds threshold
            else:
                GPIO.output(LED, GPIO.LOW)          # Turn off the LED if below threshold
        else:
            print("Temperature check failure")
        time.sleep(INTERVAL)                        # Wait before the next check
        GPIO.output(LED, GPIO.LOW)                  # Turn off the LED between intervals
finally:
    GPIO.cleanup()                                  # Release GPIO resources
    print("GPIO cleaned up. Monitoring completed")

