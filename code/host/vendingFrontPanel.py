import serial
import time

class vendingFrontPanel:
	def __init__(self):
		self.switches = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
		self.button = 0
		self.serialObject = serial.Serial()

	def Open(self, portName):
		if not self.serialObject.isOpen():
			self.serialObject = serial.Serial(portName, 9600, timeout=1)
			time.sleep(2)
			line = self.serialObject.readline();
			
		
	def Close(self):
		self.serialObject.close()
		
	def RefreshInputs(self):
		if self.serialObject.isOpen():
			self.serialObject.flushOutput();
			self.serialObject.flushInput();
			self.serialObject.write("RS")
			line = self.serialObject.readline()
			
			if line[0] == 'S':
				if line[1] == ')':
					self.switches[0] = 1
				else:
					self.switches[0] = 0
				if line[2] == '!':
					self.switches[1] = 1
				else:
					self.switches[1] = 0
				if line[3] == '@':
					self.switches[2] = 1
				else:
					self.switches[2] = 0
				if line[4] == '#':
					self.switches[3] = 1
				else:
					self.switches[3] = 0
				if line[5] == '$':
					self.switches[4] = 1
				else:
					self.switches[4] = 0
				if line[6] == '%':
					self.switches[5] = 1
				else:
					self.switches[5] = 0
				if line[7] == '^':
					self.switches[6] = 1
				else:
					self.switches[6] = 0
				if line[8] == '&':
					self.switches[7] = 1
				else:
					self.switches[7] = 0
				if line[9] == '*':
					self.switches[8] = 1
				else:
					self.switches[8] = 0
				if line[10] == '(':
					self.switches[9] = 1
				else:
					self.switches[9] = 0
				if line[11] == 'A':
					self.switches[10] = 1
				else:
					self.switches[10] = 0
				if line[12] == 'B':
					self.switches[11] = 1
				else:
					self.switches[11] = 0
				if line[13] == 'C':
					self.switches[12] = 1
				else:
					self.switches[12] = 0
				if line[14] == 'D':
					self.switches[13] = 1
				else:
					self.switches[13] = 0
				if line[15] == 'E':
					self.switches[14] = 1
				else:
					self.switches[14] = 0
				if line[16] == 'F':
					self.switches[15] = 1
				else:
					self.switches[15] = 0
			
			self.serialObject.write("P")
			line = self.serialObject.readline()
			if line[0] == 'P':
				self.button = 1
			else:
				self.button = 0

	def SetLED(self, ledNum, redVal, greenVal, blueVal):
		if self.serialObject.isOpen():
			ledNum = ledNum % 16
			redVal = redVal % 256
			blueVal = blueVal % 256
			greenVal = greenVal % 256
			
			commandString = "L%(ledNum)X%(redVal)0.2X%(greenVal)0.2X%(blueVal)0.2X" % {"ledNum":ledNum, "redVal":redVal, "blueVal":blueVal, "greenVal":greenVal}
			
			self.serialObject.flushOutput()
			self.serialObject.flushInput()
			
			self.serialObject.write(commandString)
			line = self.serialObject.readline()
			