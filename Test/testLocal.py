from rccTest import *


def test_power(s):
    test_name = 'Test general power'
    yn = input('\tIs Red light on? (Y/n)')
    return (yn.lower() != 'n', test_name)
    
def test_f0_on(s):
    test_name = 'Test Function 0 ON'
    writeSerial(s, 'F100')
    readSerial(s)
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
    readSerial(s)
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
    readSerial(s)
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
    readSerial(s)
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


tests = [test_power, test_f0_on, test_f0_blinking, test_f1_blinking, test_motor_forward, test_motor_backward]

if __name__ == '__main__':
    ser_name = findSerial()
    if ser_name == None:
        print('No serial port found')
        exit(1)
    s = openSerial(ser_name)

    w = 80
    for test in tests:
        result, name = test(s)
        if result:
            print(name + 'ok'.rjust(w - len(name), '.'))
        else:
            print(name + 'FAIL'.rjust(w - len(name), '.'))

    s.close()