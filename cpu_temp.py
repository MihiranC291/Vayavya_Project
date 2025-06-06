import time
import RPi.GPIO as GPIO
LED=18
THRESHOLD=44
INTERVAL=5
DURATION=25
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED,GPIO.OUT)
GPIO.output(LED,GPIO.LOW) 
def get_temp():
    print("Beginning system monitoring...")
    try:
        with open("/sys/class/thermal/thermal_zone0/temp","r") as f:
            temp_r=f.readline()
            return float(temp_r) / 1000.0
    except FileNotFoundError:
        print("Error: Cannot retreive temperature data")
        return None
try:
    for _ in range(DURATION // INTERVAL):
        temp = get_temp()
        if temp is not None:
            print(f"CPU temperature: {temp:.2f} degrees C")
            if temp > THRESHOLD:
                print("Warning: CPU temperature exceeded the threshold")
                GPIO.output(LED, GPIO.HIGH)
            else:
                GPIO.output(LED, GPIO.LOW)
        else:
            print("Temperature check failure")
        time.sleep(INTERVAL)
        GPIO.output(LED, GPIO.LOW)
finally:
    GPIO.cleanup()
    print("GPIO cleaned up. Monitoring completed")
