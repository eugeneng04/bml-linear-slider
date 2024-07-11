import serial, time

ser = serial.Serial("/dev/cu.usbmodem11101", baudrate = 9600)
time.sleep(2)

def setMaxSpeed(speed):
    ser.write(f's mrpm {speed}'.encode())

def setMaxVelocity(v):
    ser.write(f's mv {v}'.encode())

def setStepsPerRev(spr):
    ser.write(f's spr {spr}'.encode())

def setSpeed(speed):
    ser.write(f's sr {speed}'.encode())

def setVelocity(v):
    ser.write(f's sv {v}'.encode())

def moveDistance(d):
    ser.write(f'm d {d}'.encode())

def moveRotations(r):
    ser.write(f'm r {r}'.encode())