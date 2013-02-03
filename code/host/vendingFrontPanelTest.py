from vendingFrontPanel import *
import time

myFrontPanel = vendingFrontPanel()

myFrontPanel.Open("COM22")

while True:
	myFrontPanel.RefreshInputs()
	print "Front Panel:\n"
	print "%d\t%d\t%d\t%d" % (myFrontPanel.switches[0], myFrontPanel.switches[1], myFrontPanel.switches[2], myFrontPanel.switches[3])
	print "%d\t%d\t%d\t%d" % (myFrontPanel.switches[4], myFrontPanel.switches[5], myFrontPanel.switches[6], myFrontPanel.switches[7])
	print "%d\t%d\t%d\t%d" % (myFrontPanel.switches[8], myFrontPanel.switches[9], myFrontPanel.switches[10], myFrontPanel.switches[11])
	print "%d\t%d\t%d\t%d" % (myFrontPanel.switches[12], myFrontPanel.switches[13], myFrontPanel.switches[14], myFrontPanel.switches[15])
	
	for x in range(0, 16):
		if myFrontPanel.switches[x] == 1:
			myFrontPanel.SetLED(x, 255, 0, 0)
		else:
			myFrontPanel.SetLED(x, 0, 0, 0)
	#time.sleep(1)