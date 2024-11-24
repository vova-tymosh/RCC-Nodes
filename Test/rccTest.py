import serial
import time
import subprocess
import select
import sys


def millis():
    return round(time.time() * 1000)

def get_unblocked_input():
    output, _, _ = select.select([sys.stdin], [], [], 0)
    if output:
        return sys.stdin.readline()
    return None

def findSerial():
    output = subprocess.check_output(["ls /dev/cu.usb*"], shell=True)
    usbmodem = output.decode('utf-8').strip().split('\n')
    if len(usbmodem) == 0:
        return None
    return usbmodem[0]

def openSerial(ser_name):
    return serial.Serial(ser_name, 115200)

def writeSerial(s, data):
    s.write(data.encode('utf-8'))

def readSerial(s, printLog=False):
    for i in range(1000):
        if s.in_waiting > 0:
            data = s.readline().decode('utf-8')
            if printLog and data:
                print('\t#Log:', data.strip())
