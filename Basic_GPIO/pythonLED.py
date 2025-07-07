import RPi.GPIO as GPIO     # Import Raspberry Pi GPIO library
import time                 # Import time library for delays

GPIO.setmode(GPIO.BCM)      # Use BCM (Broadcom chip-specific) pin numbering
GPIO.setwarnings(False)     # Disable warning messages
GPIO.setup(18, GPIO.OUT)    # Set GPIO pin 18 as output
print("LED on")
GPIO.output(18, GPIO.HIGH)  # Turn the LED on (set pin HIGH)
time.sleep(3)               # Wait for 3 seconds
print("LED off")
GPIO.output(18, GPIO.LOW)   # Turn the LED off (set pin LOW)
time.sleep(3)               # Wait for 3 seconds
print("LED on")
GPIO.output(18, GPIO.HIGH)  # Turn the LED on again
time.sleep(3)
print("LED off")
GPIO.output(18, GPIO.LOW)   # Turn the LED off again
GPIO.cleanup()              # Reset all GPIO settings

