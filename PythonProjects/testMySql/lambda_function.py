'''
=============================================
 Program to send data to MySql in lambda AWS
=============================================
'''
import json
#import logging
import rds_config
import pymysql

#rds settings
rds_host  = "e2idb.c0dc6ib7q6dp.us-west-2.rds.amazonaws.com"
name = rds_config.db_username
password = rds_config.db_password
db_name = rds_config.db_name

#logger = logging.getLogger()
#logger.setLevel(logging.INFO)

#Set key names
fechaKey ="Tiempo2"
equipoKey = "Equipo"
piscPressKey = "PiscPress"
piscTempKey = "PiscTemp"
n3temp0Key = "N3temp0"
n3temp1Key = "N3temp1"
n3temp2Key = "N3temp2"
n3temp3Key = "N3temp3"
n3temp8Key = "N3temp8"

try:
    conn = pymysql.connect(rds_host, user=name, passwd=password, db=db_name, connect_timeout=5)
except pymysql.MySQLError as e:
    #logger.error("ERROR: Unexpected error: Could not connect to MySQL instance.")
    #logger.error(e)
    sys.exit()

#logger.info("SUCCESS: Connection to RDS MySQL instance succeeded")

def changeDate(date):
    """
    This function changes yo mysql dattime format
    IMPORTANT: Always in UTC format
    """
    DD, MM, val = date.split("/", 3)
    YY, hour = val.split(" ", 2)
    hh, mm, ss = hour.split(":", 3)
    #Chante UTC format America/Lima -05:00
    hh = int(hh) + 5
    DD = int(DD)
    if (hh >= 24):
        DD = int(DD) + 1
        hh = hh - 24
    datetime = ("%s-%s-%02d %02d:%s:%s-05:00"%(YY, MM, DD, hh, mm, ss))
    return datetime

def isEmpty(value):
    out = False
    if (value == "" or value == None):
        out = True

    return out

def lambda_handler(event = None, context = None):
    """
    This function fetches content from MySQL RDS instance
    """
    if (len(event) == 4):
        # Presion y temperatura en la Piscina
        fecha = changeDate(event[fechaKey])
        equipo = event[equipoKey]
        presion = event[piscPressKey]
        temperatura = event[piscTempKey]
        
        #Check if the values are not empty
        if isEmpty(presion):
            return 'Empty preassure.'
        
        if isEmpty(temperatura):
            return 'Empty Temperature.'
        
        print("INSERT INTO db_hve.db_piscina VALUES (NULL,%s,%s,%s,%s)",(fecha, equipo, presion, temperatura))
        # Insert to MySql
        #with conn.cursor() as cur:
            # cur.execute("INSERT INTO db_hve.db_piscina VALUES (NULL,%s,%s,%s,%s)",(fecha, equipo, presion, temperatura));
        
        #conn.commit()
        out = "Item in 'Piscina' from RDS MySQL table has been added"
    else:
        # Temperaturas
        fecha = changeDate(event[fechaKey])
        equipo = event[equipoKey]
        n3Temp0 = event[n3temp0Key]
        n3Temp1 = event[n3temp1Key]
        n3Temp2 = event[n3temp2Key]
        n3Temp3 = event[n3temp3Key]
        n3Temp8 = event[n3temp8Key]
        
        #Check if the values are not empty
        if isEmpty(n3Temp0):
            return 'Empty n3Temp0.'
        
        if isEmpty(n3Temp1):
            return 'Empty n3Temp1.'
        
        if isEmpty(n3Temp2):
            return 'Empty n3Temp2.'
        
        if isEmpty(n3Temp3):
            return 'Empty n3Temp3.'
        
        if isEmpty(n3Temp8):
            return 'Empty n3Temp8.'
        
        print("INSERT INTO db_hve.db_temperatura VALUES (NULL,%s,%s,%s,%s,%s,%s,%s)",(fecha, equipo, n3Temp0, n3Temp1, n3Temp2, n3Temp3, n3Temp8))

        # Insert to MySql
        #with conn.cursor() as cur:
            #cur.execute("INSERT INTO db_hve.db_temperatura VALUES (NULL,%s,%s,%s,%s,%s,%s,%s)",(fecha, equipo, n3Temp0, n3Temp1, n3Temp2, n3Temp3, n3Temp8));
            
        #conn.commit()
        out = "Item in 'Temperaturas' from RDS MySQL table has been added"
    return out

# ## Only for test    
if __name__ == "__main__":
    print(__doc__)
    print('Starting!...')
    
    #event = "{\"Tiempo2\":\"28/05/2020 10:27:08\",\"Equipo\":\"HVE\",\"N3temp0\":34,\"N3temp1\":33,\"N3temp2\":36,\"N3temp3\":35,\"N3temp8\":51}\"
    ini_string = {"Tiempo2":"28/05/2020 16:25:53","Equipo":"HVE","N3temp0":33,"N3temp1":36,"N3temp2":36,"N3temp3":38,"N3temp8":43}
    #ini_string = {"Tiempo2":"28/05/2020 16:25:53","Equipo":"HVE","N3temp0":33,"N3temp1":36,"N3temp2":None,"N3temp3":38,"N3temp8":43}
    #ini_string = {'Tiempo2':'28/05/2020 10:30:12','Equipo':'HVE','PiscPress': None,'PiscTemp': None}
    #ini_string = {'Tiempo2':'28/05/2020 10:30:12','Equipo':'HVE','PiscPress': '','PiscTemp': ''}
    #ini_string = {'Tiempo2':'28/05/2020 10:30:12','Equipo':'HVE','PiscPress':59,'PiscTemp':29}
    #event = "{"Tiempo2":"28/05/2020 10:30:12","Equipo":"HVE","PiscPress":59,"PiscTemp":29}"
    
    # printing initial json 
    ini_string = json.dumps(ini_string) 
    #print ("initial 1st dictionary", ini_string) 
    #print ("type of ini_object", type(ini_string)) 

    # converting string to json 
    event = json.loads(ini_string) 

    # printing final result 
    #print ("final dictionary", str(event)) 
    #print ("type of final_dictionary", type(event)) 

    #print(changeDate("28/05/2020 16:22:53"))
    print(lambda_handler(event = event))
    