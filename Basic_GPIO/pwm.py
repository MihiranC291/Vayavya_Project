import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)         # Use physical pin numbering
GPIO.setup(12, GPIO.OUT)         # Set pin 12 as output

led = GPIO.PWM(12, 50)           # Set up PWM on pin 12 at 50Hz
led.start(0)                     # Start PWM with 0% duty cycle

try:
    while True:
        for dc in range(0, 101, 5):      # Increase brightness
            led.ChangeDutyCycle(dc)
            time.sleep(0.1)
        for dc in range(100, -1, -5):    # Decrease brightness
            led.ChangeDutyCycle(dc)
            time.sleep(0.1)
except KeyboardInterrupt:
    pass
finally:
    led.stop()                  # Stop PWM
    GPIO.cleanup()             # Clean up GPIO settings

