import time
from rccTest import *

    
def test_f0_on(s):
    test_name = 'Test Function 0 ON'
    writeSerial(s, 'F100')
    printSerial(s)
    yn = input('\tIs Yellow light on? (Y/n)')
    return (yn.lower() != 'n', test_name)

def test_f0_blinking(s):
    test_name = 'Test Function 0 Blinking'
    writeSerial(s, 'F000')
    print('\tIs Yellow light blinking? (Y/n)')
    start = millis()
    flip = True
    while True:
        yn = get_unblocked_input()
        if yn != None:
            break
        if millis() - start > 500:
            writeSerial(s, 'F100' if flip else 'F000')
            flip = not flip
            start = millis()
    writeSerial(s, 'F000')
    printSerial(s)
    return (yn.strip().lower() != 'n', test_name)

def test_f1_blinking(s):
    test_name = 'Test Function 1 Blinking'
    writeSerial(s, 'F001')
    print('\tIs Blue light blinking? (Y/n)')
    start = millis()
    flip = True
    while True:
        yn = get_unblocked_input()
        if yn != None:
            break
        if millis() - start > 500:
            writeSerial(s, 'F101' if flip else 'F001')
            flip = not flip
            start = millis()
    # F1 is inverted, keep it off (set to 1)
    writeSerial(s, 'F101')
    printSerial(s)
    return (yn.strip().lower() != 'n', test_name)

def motor_blinker(s, direction):
    writeSerial(s, 'S%d000'%direction)
    speed = 20
    start = millis()
    yn = False
    while True:
        yn = get_unblocked_input()
        if yn != None:
            break
        if millis() - start > 100:
            if speed >= 200:
                speed = 20
            else:
                speed += 10
            if speed < 100:
                writeSerial(s, 'S%d%03d'%(direction, speed))
            else:
                writeSerial(s, 'S%d%03d'%(direction, 200 - speed))
            start = millis()
    writeSerial(s, 'S%d000'%direction)
    printSerial(s)
    return yn

def test_motor_forward(s):
    test_name = 'Test Motor forward'
    print('\tIs Green light fading in/out? (Y/n)')
    yn = motor_blinker(s, 1)
    return (yn.strip().lower() != 'n', test_name)

def test_motor_backward(s):
    test_name = 'Test Motor backward'
    print('\tIs White light fading in/out? (Y/n)')
    yn = motor_blinker(s, 2)
    return (yn.strip().lower() != 'n', test_name)

def test_motor_bemf(s):
    test_name = 'Test Motor BEMF'
    writeSerial(s, 'RB')
    data = readSerialFloat(s)
    print(f'\tHas to be non-zero value: {data}')
    return (data > 0, test_name)

def test_voltage(s):
    test_name = 'Test Power Meter Voltage'
    writeSerial(s, 'RV')
    data = readSerialFloat(s)
    print(f'\tHas to be higher than 10: {data}V')
    return (data > 10, test_name)

def test_current(s):
    test_name = 'Test Power Meter Current'
    writeSerial(s, 'RC')
    data = readSerialFloat(s)
    print(f'\tHas to be non-zero value: {data}mA')
    return (data > 0, test_name)

def test_current_with_load(s):
    test_name = 'Test Current With Load'
    writeSerial(s, 'S1100')
    printSerial(s)
    writeSerial(s, 'F100')
    printSerial(s)
    writeSerial(s, 'F001')
    printSerial(s)
    time.sleep(0.2)
    writeSerial(s, 'RC')
    data = readSerialFloat(s)
    writeSerial(s, 'S10')
    printSerial(s)
    writeSerial(s, 'F000')
    printSerial(s)
    writeSerial(s, 'F101')
    printSerial(s)
    print(f'\tHas to be higher than 30: {data}mA')
    return (data > 30, test_name)

