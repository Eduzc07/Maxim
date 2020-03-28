'''
======================================
 Program to send data to MySql
======================================
'''
import MySQLdb
import time
from datetime import datetime

def getDate():
    now = datetime.now()
    date_string = now.strftime("%Y-%m-%d %H:%M:%S")
    #time_string = now.strftime("%H:%M:%S")
    val = [date_string, 1]
    return val

def senddata():
    # Get Data
    values = getDate()

    #Open MySQL
    db = MySQLdb.connect(host = 'www.dairel.com',
                         user = 'daireu1b_zarate',
                         passwd = 'Cwj0bV]q!5',
                         db = 'daireu1b_nodered',
                         charset = 'utf8mb4')
    cursor = db.cursor()
    sql = "INSERT INTO db_count (ID, FECHA, VALOR)\
           VALUES (NULL, '%s','%i')" % \
           (values[0], values[1])

    cursor.execute(sql)
    db.commit()
    db.close()
    print('%s, %i'%(values[0], values[1]))
    print('Data Sent')
    return

if __name__ == "__main__":
    print(__doc__)
    print('Starting!...')
    senddata()
