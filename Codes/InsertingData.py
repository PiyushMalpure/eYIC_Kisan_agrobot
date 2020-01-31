            #Project Name :Kisaan Agricultural Robot
            #Author list :Abhishek Gore
            #File name:InsertingData
            #Functions:None
            #Global Variables:None
            #Installation Required : Apache server, myphpadmin, mysql 
import serial #importing serial to read serial data coming from Arduino Mega
import time #importing time to use function like delay
import MySQLdb as mdb #importing MySQl to push data into the database
ser = serial.Serial(
 port='/dev/ttyUSB0', #selecting serial port from which data is to read
 baudrate = 9600,
 parity=serial.PARITY_NONE,
 stopbits=serial.STOPBITS_ONE,
 bytesize=serial.EIGHTBITS,
 timeout=1
)
counter=0
data=ser.readline() #reading data from serial port
time.sleep(1)
pieces=data.plit("\t") #splitting data 
tempreture = pieces[0]
humidity = pieces[1]
soilmoisture = pieces[2]
#making coneection to the MyPHP Server database
#Database credentials are removed and replaced by '*' for security purpose 
con=mdb.connect('localhost','****','*******','kisanSensorValues');
#pushing values into the database
with con:
    cursor=con.cursor()
    cursor.execute("""INSERT INTO TempDB VALUES('',%s,%s,%s)""",
    (tempreture,humidity,soilmoisture)) #Using execute command to run the SQL Query
    con.commit()
    cursor.close() #Closing connection