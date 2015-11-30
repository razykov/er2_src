# -*- coding:utf-8 -*-

 
import smbus
import time
import os
import RPi.GPIO as GPIO
 
# Define a class called Accel
class Accel:
    myBus=""
    if GPIO.RPI_INFO['P1_REVISION'] == 1:
        myBus=0
    else:
        myBus=1
    b = smbus.SMBus(myBus)
    def setUp(self):
      #self.b.write_byte_data(0x1D,0x16,0x55) # Setup the Mode
      self.b.write_byte_data(0x1D,0x10,0) # Calibrate
      self.b.write_byte_data(0x1D,0x11,0) # Calibrate
      self.b.write_byte_data(0x1D,0x12,0) # Calibrate
      self.b.write_byte_data(0x1D,0x13,0) # Calibrate
      self.b.write_byte_data(0x1D,0x14,0) # Calibrate
      self.b.write_byte_data(0x1D,0x15,0) # Calibrate
      self.b.write_byte_data(0x1D,0x16,0x45)
    def getValueX(self):
      return (self.b.read_byte_data(0x1D,0x01) << 8) and self.b.read_byte_data(0x1D,0x00)
    def getValueY(self):
      return self.b.read_byte_data(0x1D,0x07)
    def getValueZ(self):
      return self.b.read_byte_data(0x1D,0x08)
 
MMA7455 = Accel()
MMA7455.setUp()

f = open('mma', 'w')

for a in range(10000):
    x = MMA7455.getValueX()
    y = MMA7455.getValueY()
    z = MMA7455.getValueZ()
    
    #print("X=", x)
    #print("Y=", y)
    #print("Z=", z)
    
    f.write(str(a) + " " + str(x) + " " + str(y) + " " + str(z) + "\n")
    
    time.sleep(0.01)
    #os.system("clear")