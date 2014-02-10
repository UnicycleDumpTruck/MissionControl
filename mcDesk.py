from time import sleep
import pygame.mixer
import serial 
import subprocess
import threading

#serialFromArduino = serial.Serial("/dev/ttyUSB1", 115200)
serialFromArduino = serial.Serial("/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A900aepy-if00-port0", 115200)
serialFromArduino.flush()

pygame.mixer.quit()
pygame.mixer.init(48000, -16, 2, 1024) #was 1024
warning = False

cws = pygame.mixer.Sound("cws.wav")
cws.set_volume(0.2)
cwsC = pygame.mixer.Channel(1)
thirteenSound = pygame.mixer.Sound("11event.wav")

teiPushed = 0
spsPushed = 0
suitCompPushed = 0
abortArmed = 0
lightningStruck = 0
sceRestored = False
sceAux = False
cwsPower = False
thirteen = False

locations=['/dev/ttyUSB0','/dev/ttyUSB1','/dev/ttyUSB2','/dev/ttyUSB3','/dev/ttyS0','/dev/ttyS1','/dev/ttyS2','/dev/ttyS3']

def lightningStrike():
	global lightningStruck
	global sceAux
	#lightningStruck = 1
	while True:
		if (lightningStruck == 1):
			if not sceAux:
				serialFromArduino.write(b'1,4,4,1,1\n')
				sleep(0.1)
				serialFromArduino.write(b'1,4,4,1,7\n')
				cwsC.play(cws)
				sleep(2)
			if not sceAux:		
				serialFromArduino.write(b'1,4,4,2,5\n')
				sleep(0.1)
				serialFromArduino.write(b'1,4,4,1,7\n')
				cwsC.play(cws)
				sleep(2)
			if not sceAux:
				serialFromArduino.write(b'1,4,4,5,0\n')
				sleep(0.1)
				serialFromArduino.write(b'1,4,4,1,7\n')
				cwsC.play(cws)
				sleep(1)
			if not sceAux:
				serialFromArduino.write(b'1,4,4,3,5\n')
				sleep(0.1)
				serialFromArduino.write(b'1,4,4,1,7\n')
				cwsC.play(cws)
				sleep(1)
			if not sceAux:
				serialFromArduino.write(b'1,4,4,0,3\n')
				sleep(0.1)
				serialFromArduino.write(b'1,4,4,1,7\n')
				cwsC.play(cws)
				sleep(2)
			if not sceAux:
				serialFromArduino.write(b'1,4,4,3,1\n')
				sleep(0.1)
				serialFromArduino.write(b'1,4,4,1,7\n')
				cwsC.play(cws)
				sleep(1)
			if not sceAux:
				serialFromArduino.write(b'1,4,4,5,3\n')
				sleep(0.1)
				serialFromArduino.write(b'1,4,4,1,7\n')
				cwsC.play(cws)
				sleep(1)
			if not sceAux:
				serialFromArduino.write(b'1,4,4,1,4\n')
				sleep(0.1)
				serialFromArduino.write(b'1,4,4,1,7\n')
				cwsC.play(cws)
				sleep(1)
			if not sceAux:
				serialFromArduino.write(b'1,4,4,3,3\n')
				sleep(0.1)
				serialFromArduino.write(b'1,4,4,1,7\n')
				cwsC.play(cws)
				sleep(1)
			if not sceAux:
				serialFromArduino.write(b'1,4,4,0,5\n')
				sleep(0.1)
				serialFromArduino.write(b'1,4,4,1,7\n')
				cwsC.play(cws)
				sleep(1)
			if not sceAux:
				serialFromArduino.write(b'1,4,4,3,0\n')
				sleep(0.1)
				serialFromArduino.write(b'1,4,4,1,7\n')
				cwsC.play(cws)
				sleep(1)
			if not sceAux:
				serialFromArduino.write(b'1,4,4,1,3\n')
				sleep(0.1)
				serialFromArduino.write(b'1,4,4,1,7\n')
				cwsC.play(cws)
				sleep(1)
			if not sceAux:
				serialFromArduino.write(b'1,4,4,5,2\n')
				sleep(0.1)
				serialFromArduino.write(b'1,4,4,1,7\n')
				cwsC.play(cws)
			lightningStruck = 0
			sleep(1)

def sceRestore():
	global sceRestored
	while True:
		if sceRestored:
			cwsC.stop()
			serialFromArduino.write(b'0,4,4,1,1\n')
			sleep(0.1)
			serialFromArduino.write(b'0,4,4,2,5\n')
			sleep(0.1)
			serialFromArduino.write(b'0,4,4,5,0\n')
			sleep(0.1)
			serialFromArduino.write(b'0,4,4,3,5\n')
			sleep(0.1)
			serialFromArduino.write(b'0,4,4,0,3\n')
			sleep(0.1)
			serialFromArduino.write(b'0,4,4,3,1\n')
			sleep(0.1)
			serialFromArduino.write(b'0,4,4,5,3\n')
			sleep(0.1)
			serialFromArduino.write(b'0,4,4,1,4\n')
			sleep(0.1)
			serialFromArduino.write(b'0,4,4,3,3\n')
			sleep(0.1)
			serialFromArduino.write(b'0,4,4,0,5\n')
			sleep(0.1)
			serialFromArduino.write(b'0,4,4,3,0\n')
			sleep(0.1)
			serialFromArduino.write(b'0,4,4,1,3\n')
			sleep(0.1)
			serialFromArduino.write(b'0,4,4,5,2\n')
			sceRestored = False
		sleep(0.5)

def thirteenEvent():
	global thirteen
	global thirteenSound
	global cwsC
	global cws
	while True:
		if thirteen:
			serialFromArduino.write(b'1,4,4,4,7\n') # Light next to switch
			offbit = 0
			thirteenSound.play()
			sleep(4.5)
			serialFromArduino.write(b'1,4,4,1,5\n') # Ox flow high
			sleep(0.1)
			serialFromArduino.write(b'1,4,4,1,7\n') # Illuminated Master Alarm Pushbutton
			cwsC.play(cws)
			sleep(0.1)
		#	serialFromArduino.write(b'2,11,0,0,0\n')
		#	sleep(1)
		#	serialFromArduino.write(b'2,10,0,0,0\n')
		#	sleep(1)
			serialFromArduino.write(b'2,9,0,0,0\n')
			sleep(0.5)
			serialFromArduino.write(b'2,9,6,0,0\n')
			sleep(0.5)
			serialFromArduino.write(b'2,8,0,0,0\n')
			sleep(0.5)
			serialFromArduino.write(b'2,8,6,0,0\n')
			sleep(0.5)
			serialFromArduino.write(b'1,4,4,2,3\n')	# Main Bus B Undervolt
			sleep(0.1)
			serialFromArduino.write(b'1,4,4,1,7\n') # Illuminated Master Alarm Pushbutton
			cwsC.play(cws)
			sleep(0.5)
	
			serialFromArduino.write(b'2,7,0,0,0\n')
			sleep(0.5)
			serialFromArduino.write(b'2,7,6,0,0\n')
			sleep(0.5)
			serialFromArduino.write(b'2,6,0,0,0\n')
			sleep(0.5)
			serialFromArduino.write(b'2,6,6,0,0\n')
			sleep(0.5)
			serialFromArduino.write(b'2,5,0,0,0\n')
			sleep(0.5)
			serialFromArduino.write(b'2,5,6,0,0\n')
			sleep(0.5)
			serialFromArduino.write(b'2,4,0,0,0\n')
			sleep(0.5)
			serialFromArduino.write(b'2,4,6,0,0\n')
			sleep(0.5)
			serialFromArduino.write(b'2,3,0,0,0\n')
			sleep(0.5)
			serialFromArduino.write(b'2,3,6,0,0\n')
			sleep(0.5)
			serialFromArduino.write(b'2,2,0,0,0\n')
			sleep(0.5)
			serialFromArduino.write(b'2,2,6,0,0\n')
			sleep(0.5)
			thirteen = False
		sleep(0.1)



def mainLoop():

	global teiPushed
	global spsPushed
	global suitCompPushed
	global abortArmed
	global lightningStruck
	global sceAux
	global sceRestored
	global airComp
	global cws
	global cwsPower
	global thirteen
	
	tliPushed = 0
	sicPushed = 0
	siiPushed = 0
	sivbPushed = 0
	miPushed = 0
	miiPushed = 0
	miiiPushed = 0
	glycolPushed = 0
	
	aborted = pygame.mixer.Sound("aborted.wav")

	liftoff = pygame.mixer.Sound("liftoff.wav")
	stagetwo = pygame.mixer.Sound("stagetwo.wav")
	stagethree = pygame.mixer.Sound("stagethree.wav")
	switchorbits = pygame.mixer.Sound("switchorbits.wav")
	tde = pygame.mixer.Sound("tde.wav")
	landeva = pygame.mixer.Sound("landeva.wav")
	headhome = pygame.mixer.Sound("headhome.wav")
	reentry = pygame.mixer.Sound("reentry.wav")
	splashdown = pygame.mixer.Sound("splashdown.wav")
	poll = pygame.mixer.Sound("poll.wav")

	sequenceC = pygame.mixer.Channel(2)
	
	shortexp = pygame.mixer.Sound("shortexp.wav")
	medexp = pygame.mixer.Sound("medexp.wav")
	les = pygame.mixer.Sound("les.wav")
	pexp = pygame.mixer.Sound("pexp.wav")
	hiexp = pygame.mixer.Sound("hiexp.wav")
	drogue = pygame.mixer.Sound("drogue.wav")
	main = pygame.mixer.Sound("main.wav")
	fpump = pygame.mixer.Sound("fpump.wav")
	fpump.set_volume(0.7)
	fan = pygame.mixer.Sound("fan.wav")
	heat = pygame.mixer.Sound("heat.wav")
	sps = pygame.mixer.Sound("sps.wav")
	tei = pygame.mixer.Sound("tei.wav")
	tli = pygame.mixer.Sound("tli.wav")
	sic = pygame.mixer.Sound("sic.wav")
	sii = pygame.mixer.Sound("sii.wav")
	sivb = pygame.mixer.Sound("sivb.wav")
	mi = pygame.mixer.Sound("mi.wav")
	mii = pygame.mixer.Sound("mii.wav")
	miii = pygame.mixer.Sound("miii.wav")
	flush = pygame.mixer.Sound("flush.wav")
	aircomp = pygame.mixer.Sound("aircomp.wav")
	aircomp.set_volume(0.5)
	dieselpump = pygame.mixer.Sound("dieselpump.wav")
	dieselpump.set_volume(0.7)
	extend = pygame.mixer.Sound("extend.wav")
	retract = pygame.mixer.Sound("retract.wav")
	qin = pygame.mixer.Sound("qin.wav")
	qin.set_volume(0.3)
	qout = pygame.mixer.Sound("qout.wav")
	qout.set_volume(0.3)
	hflow = pygame.mixer.Sound("lantern.wav")
	cfan = pygame.mixer.Sound("cfan.wav")
	cfan.set_volume(0.3)
	#explosion = pygame.mixer.Sound("explosion")
	#engine = pygame.mixer.Sound("engine.wav")
	

#	 for device in locations:	 
#		 try:	 
#			 print "Trying...",device  
#			 serialFromArduino = serial.Serial(device,115200)	
#			 break	
#		 except:	
#			 print "Failed to connect on",device	 

#	pygame.mixer.set_num_channels(5)

	serialFromArduino.write(b'0,4,4,6,0\n')
	sleep(0.1)
	serialFromArduino.write(b'0,4,4,6,1\n')
	sleep(0.1)
	serialFromArduino.write(b'0,4,4,6,2\n')
	sleep(0.1)
	serialFromArduino.write(b'0,4,4,6,3\n')
	sleep(0.1)
	serialFromArduino.write(b'0,4,4,6,4\n')
	sleep(0.1)
	serialFromArduino.write(b'0,4,4,7,0\n')
	sleep(0.1)
	serialFromArduino.write(b'0,4,4,7,1\n')
	sleep(0.1)
	serialFromArduino.write(b'0,4,4,7,2\n')
	sleep(0.1)
	serialFromArduino.write(b'0,4,4,7,3\n')
	sleep(0.1)
	serialFromArduino.write(b'0,4,4,7,4\n')


	print "Sampler Ready."

	# Loop while waiting for a keypress
	while True:
		try:
			digit = ord(serialFromArduino.read())
			offBit = 0
#			 if (digit < 128):
				#print(digit)
			if (digit > 128):
				digit = digit - 128
				#print digit, "off"
				offBit = 1
			if (digit == 6):
				if (offBit):
					offBit = 0
					medexp.play()
			elif (digit == 21): # Abort button
				if (offBit):
					offBit = 0
				else:
					if abortArmed:
						aborted.play()
						serialFromArduino.write(b'0,4,4,7,5\n')
						subprocess.call("halt", shell=True)
			elif (digit == 46): #Master Alarm button
				if (offBit):
					offBit = 0
				else:
					cwsC.stop()
					serialFromArduino.write(b'0,4,4,1,7\n')
					warning = False

			elif (digit == 19):
				if (offBit):
					offBit = 0
					cwsPower = True
					serialFromArduino.write(b'0,4,4,2,4\n')
					cwsC.stop()
				else:
					cwsPower = False
					serialFromArduino.write(b'1,4,4,2,4\n')
					sleep(0.1)
					cwsC.play(cws)
					serialFromArduino.write(b'1,4,4,1,7\n')
			elif (digit == 16): #                      Initiate Apollo 12 Lightning Strike
				if (offBit):
					offBit = 0
				else:
					if not cwsPower:
						if not lightningStruck:
							#global sceAux
							if not sceAux:
								lightningStruck = 1
			elif (digit == 0):
				if (offBit):
					offBit = 0
				else:
					sequenceC.play(switchorbits)
					serialFromArduino.write(b'1,4,4,6,4\n')
			elif (digit == 1):
				if (offBit):
					offBit = 0
				else:
					sequenceC.play(stagethree)
					serialFromArduino.write(b'1,4,4,6,3\n')
			elif (digit == 5):
				if (offBit):
					offBit = 0
				else:
					sequenceC.play(liftoff)
					serialFromArduino.write(b'1,4,4,6,1\n')
			elif (digit == 4):
				if (offBit):
					offBit = 0
				else:
					sequenceC.play(stagetwo)
					serialFromArduino.write(b'1,4,4,6,2\n')
			elif (digit == 41):
				if (offBit):
					offBit = 0
				else:
					sequenceC.play(splashdown)
					serialFromArduino.write(b'1,4,4,7,4\n')
			elif (digit == 42):
				if (offBit):
					offBit = 0
				else:
					sequenceC.play(reentry)
					serialFromArduino.write(b'1,4,4,7,3\n')
			elif (digit == 43):
				if (offBit):
					offBit = 0
				else:
					sequenceC.play(headhome)
					serialFromArduino.write(b'1,4,4,7,2\n')
			elif (digit == 44):
				if (offBit): 
					offBit = 0
				else:					 
					sequenceC.play(landeva)
					serialFromArduino.write(b'1,4,4,7,1\n')
			elif (digit == 45):
				if (offBit):
					offBit = 0
				else:
					sequenceC.play(tde)
					serialFromArduino.write(b'1,4,4,7,0\n')
			elif (digit == 3):
				if (offBit):
					offBit = 0
					hiexp.play()
			elif (digit == 2):
				if (offBit):
					offBit = 0
				else:
					main.play()
					serialFromArduino.write(b'1,4,4,0,1\n')
			if (digit == 7):
				if (offBit):
					offBit = 0
				else:
					les.play()
			elif (digit == 8):	 
				if (offBit):
					offBit = 0
				else:
					sequenceC.play(poll)
					serialFromArduino.write(b'1,4,4,6,0\n')
			elif (digit == 9):
				if (offBit):
					offBit = 0
				else:
					drogue.play()
					serialFromArduino.write(b'1,4,4,0,0\n')
			if (digit == 10):
				if (offBit):
					offBit = 0
					pexp.play()
			elif (digit == 11):
				if (offBit):
					offBit = 0
					shortexp.play()
			elif (digit == 12):	 #                           Apollo 13 explosion
				if (offBit):
					thirteen = True
				else:
					serialFromArduino.write(b'0,4,4,4,7\n')
					sleep(0.1)
					serialFromArduino.write(b'2,9,0,0,0\n')
					sleep(0.1)
					serialFromArduino.write(b'2,9,6,0,0\n')
					sleep(0.1)
					serialFromArduino.write(b'0,4,4,1,5\n') # Ox flow high
					sleep(0.1)
					serialFromArduino.write(b'0,4,4,2,3\n')	# Main Bus B Undervolt

			elif (digit == 13):
				if (offBit):
					serialFromArduino.write(b'1,4,4,5,7\n')
					offbit = 0
					fan.play()
				else:
					serialFromArduino.write(b'0,4,4,5,7\n')
					fan.fadeout(1000)
			elif (digit == 14):
				if (offBit):
					serialFromArduino.write(b'1,4,4,6,7\n')
					offbit = 0
					fpump.play()
				else:
					serialFromArduino.write(b'0,4,4,6,7\n')
					fpump.fadeout(1000)
			elif (digit == 15):
				if (offBit):
					serialFromArduino.write(b'1,4,4,7,7\n')
					offbit = 0
					heat.play()
				else:
					serialFromArduino.write(b'0,4,4,7,7\n')
					heat.fadeout(500)
			elif (digit == 24):
				if (offBit): 
					offBit = 0
					serialFromArduino.write(b'0,4,4,2,0\n')
				else:
					tei.play()	
					serialFromArduino.write(b'1,4,4,2,0\n')
					sleep(0.1)
					teiPushed = teiPushed + 1
					if teiPushed == 6:
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,4,1\n')
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,1,7\n')
						cwsC.play(cws)
			elif (digit == 25):
				if (offBit): 
					offBit = 0
					serialFromArduino.write(b'0,4,4,2,0\n')
				else:
					sps.play()	
					serialFromArduino.write(b'1,4,4,2,0\n')
					sleep(0.1)
					spsPushed = spsPushed + 1
					if (spsPushed % 10) == 0:
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,0,4\n')
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,1,7\n')
						cwsC.play(cws)
					if spsPushed == 5:
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,0,2\n')
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,1,7\n')
						cwsC.play(cws)
					if spsPushed == 25:
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,0,3\n')
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,1,7\n')
						cwsC.play(cws)
			elif (digit == 26):
				if (offBit): 
					offBit = 0
					serialFromArduino.write(b'0,4,4,2,0\n')
				else:
					miii.play()	 
					serialFromArduino.write(b'1,4,4,2,0\n')
					sleep(0.1)
					miiiPushed = miiiPushed + 1
					if miiiPushed == 6:
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,0,4\n')
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,1,7\n')
						cwsC.play(cws)

			elif (digit == 27):
				if (offBit): 
					offBit = 0
					serialFromArduino.write(b'0,4,4,2,0\n')
				else:
					sivb.play()	 
					serialFromArduino.write(b'1,4,4,2,0\n')
					sivbPushed = sivbPushed + 1
					if sivbPushed == 6:
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,0,5\n')
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,1,7\n')
						cwsC.play(cws)
			elif (digit == 28):
				if (offBit): 
					offBit = 0
					serialFromArduino.write(b'0,4,4,2,0\n')
				else:
					sii.play()	
					serialFromArduino.write(b'1,4,4,2,0\n')
					siiPushed = siiPushed + 1
					if siiPushed == 6:
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,4,0\n')
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,1,7\n')
						cwsC.play(cws)
			elif (digit == 29):
				if (offBit): 
					offBit = 0
					serialFromArduino.write(b'0,4,4,2,0\n')
				else:
					sic.play()	
					serialFromArduino.write(b'1,4,4,2,0\n')
					sicPushed = sicPushed + 1
					if sicPushed == 6:
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,3,1\n')
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,1,7\n')
						cwsC.play(cws)
			elif (digit == 30):
				if (offBit): 
					offBit = 0
					serialFromArduino.write(b'0,4,4,2,0\n')
				else:
					mi.play()  
					serialFromArduino.write(b'1,4,4,2,0\n')
					miPushed = miPushed + 1
					if miPushed == 6:
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,3,2\n')
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,1,7\n')
						cwsC.play(cws)
			elif (digit == 31):
				if (offBit): 
					offBit = 0
					serialFromArduino.write(b'0,4,4,2,0\n')
				else:
					mii.play()	
					serialFromArduino.write(b'1,4,4,2,0\n')
					miiPushed = miiPushed + 1
					if miiPushed == 6:
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,4,2\n')
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,1,7\n')
						cwsC.play(cws)
			elif (digit == 47):
				if (offBit):
					offBit = 0 
					serialFromArduino.write(b'0,4,4,2,0\n')
				else:
					tli.play()
					serialFromArduino.write(b'1,4,4,2,0\n')
					tliPushed = tliPushed + 1
					if tliPushed == 6:
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,5,0\n')
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,1,7\n')
						cwsC.play(cws)
			elif (digit == 22):
				if (offBit):
					serialFromArduino.write(b'1,4,4,7,5\n')
					offbit = 0
					abortArmed = 1
				else:
					serialFromArduino.write(b'0,4,4,7,5\n')
					abortArmed = 0
			elif (digit == 23):
				if (offBit):
					serialFromArduino.write(b'1,4,4,7,6\n')
					offbit = 0
					flush.play()
				else:
					serialFromArduino.write(b'0,4,4,7,6\n')
					flush.fadeout(2000)
			elif (digit == 20):
				if (offBit):
					serialFromArduino.write(b'0,4,4,6,5\n')
					offBit = 0
					qout.play()
				else:
					serialFromArduino.write(b'1,4,4,6,5\n')
					qin.play()
			elif (digit == 32):
				if (offBit):
					serialFromArduino.write(b'0,4,4,3,6\n')
					aircomp.fadeout(2000)
					offBit = 0
				else:
					serialFromArduino.write(b'1,4,4,3,6\n')
					aircomp.play()
					suitCompPushed = suitCompPushed + 1
					sleep(0.1)
					if (suitCompPushed % 5) == 0:
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,2,5\n')
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,1,7\n')
						cwsC.play(cws)
			elif (digit == 33):
				if (offBit):
					offbit = 0
					serialFromArduino.write(b'0,4,4,2,6\n')
				else:
					serialFromArduino.write(b'1,4,4,2,6\n')
			elif (digit == 34):
				if (offBit):
					offbit = 0
					serialFromArduino.write(b'1,4,4,6,6\n')
					sceAux = True
					if lightningStruck:
						sceRestored = True
				else:
					serialFromArduino.write(b'0,4,4,6,6\n')
					sceAux = False
			elif (digit == 35):
				if (offBit):
					serialFromArduino.write(b'1,4,4,5,6\n')
					offbit = 0
					dieselpump.play()
					glycolPushed = glycolPushed + 1
					if glycolPushed == 6:
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,3,3\n')
						sleep(0.1)
						serialFromArduino.write(b'1,4,4,1,7\n')
						cwsC.play(cws)
				else:
					serialFromArduino.write(b'0,4,4,5,6\n')
					dieselpump.fadeout(1000)
			elif (digit == 36):
				if (offBit):
					serialFromArduino.write(b'0,4,4,4,6\n')
					retract.stop()
					offBit = 0
				else:
					serialFromArduino.write(b'1,4,4,4,6\n')
					retract.play()
			elif (digit == 37):
				if (offBit):
					serialFromArduino.write(b'0,4,4,4,6\n')
					extend.stop()
					offBit = 0
				else:
					serialFromArduino.write(b'1,4,4,4,6\n')
					extend.play()
			elif (digit == 38):
				if (offBit):   
					serialFromArduino.write(b'0,4,4,0,6\n')
					cfan.fadeout(2000)
					offBit = 0
				else:
					serialFromArduino.write(b'1,4,4,0,6\n')
					cfan.play()
			elif (digit == 39):
				if (offBit):
					serialFromArduino.write(b'0,4,4,1,6\n')
					hflow.fadeout(500)
					offBit = 0
				else:
					serialFromArduino.write(b'1,4,4,1,6\n')
					hflow.play()

			sleep(0.01)
		except KeyboardInterrupt:
			exit()

#if __name__ == '__main__':


thread1 = threading.Thread(target = mainLoop)
thread2 = threading.Thread(target = lightningStrike)
thread3 = threading.Thread(target = sceRestore)
thread13 = threading.Thread(target = thirteenEvent)

# Start new Threads
thread1.start()
thread2.start()
thread3.start()
thread13.start()
print "Exiting Main Thread"

