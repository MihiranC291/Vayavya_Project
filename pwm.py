import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)
GPIO.setup(12, GPIO.OUT)
led=GPIO.PWM(12, 50)
led.start(0)
while (1):
    for dc in range(0, 101, 5):
        led.ChangeDutyCycle(dc)
        time.sleep(0.1)
    for dc in range(100, -1, -5):
        led.ChangeDutyCycle(dc)
        time.sleep(0.1)
led.stop()
GPIO.cleanup()

