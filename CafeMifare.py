import serial
from serial.tools import list_ports


#looking for a COM-ports
comlist = list()

for listitem in list(list_ports.comports()):
    if listitem[0][:3] == 'COM':
        comlist.append(listitem[0])
if not comlist:
    print 'No ports available. Insert a CafeMifare Reader'
else:
    
    for port in comlist:
        ser = serial.Serial(port, 115200)
        print 'start handshaking'
        #handshaking
        line = ser.readline()
        print line
        if line[0:17] == 'CafeMifare Reader':
            print 'Reader is found on ' + port
            ser.write('2')
        else:
            ser.close()
            print 'close port ' + port
    

    c = 1
    while True:
        print ser.readline()
        c=+1
        if c == 10:
            ser.close()
    
    
    
