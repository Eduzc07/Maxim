#!/home/edu/Softwares/anaconda3/bin/python3
import datetime
#import numpy as np
from tkinter import *

#---- = [#  Nombre  Categoria Club Procedencia ]
comp1 = ["17", "Edu Zárate", "Saqra"]
comp2 = ["21", "Gabriel Quispe", "Quimur Racing"]
comp3 = ["23", "Yan Bedoya", "Army Bike"]
comp4 = ["42", "Siwar Monteagudo", "Apu Riders"]
comp5 = ["2 ", "Bill Vargas", "Abuela Bikes"]
comp6 = ["70", "Brsysn Chino", "Cusco"]
comp7 = ["25", "Irving Quispe", "Cusco"]
comp8 = ["31", "Elias Cruz", "Cusco"]
comp9 = ["12", "Agustin Forcade", "Apu Riders"]
comp10 = ["4", "Mario Condori", "Cusco"]
comp11 = ["43", "Mario Castillon", "Awkis"]

comp = {"comp1": comp1,
        "comp2": comp2,
        "comp3": comp3,
        "comp4": comp4,
        "comp5": comp5,
        "comp6": comp6,
        "comp7": comp7,
        "comp8": comp8,
        "comp9": comp9,
        "comp10": comp10,
        "comp11": comp11}

global count
count = 1

class App():
    
    def reset(self):
        global count
        count = 1
        self.t.set('00:00.000')
        
    def start(self):
        compResult = comp["comp" + str(self.competitorStartTime + 1)]
        start = self.time[self.competitorStartTime]
        print("Start  " + start + " --->  #" + compResult[0] + "  " + compResult[1])
        self.competitorStartTime += 1

        #Update only the first rider
        if (self.competitor == 0):
            compResult = comp["comp" + str(self.competitor + 1)]
            self.riderData.set("#" + compResult[0] + "  " + compResult[1])

        global count
        count = 0
        #self.start_timer()
    
    def start_timer(self):
        global count
        self.timer()

    def stop(self):
        #global count
        #count = 1
        self.getResult()

    def timer(self):
        timeG = datetime.datetime.now().time()
        s = int(timeG.second)
        ms = timeG.microsecond / 1000.0

        if (s == 0 and self.key):
            self.displayMinute += 1
            self.key = False

        m = "%02d" % (self.displayMinute)
        s = "%02d" % (s)
        ms = "%03d" % (ms)

        if (self.displayMinute != 0):
            self.d = m + ":" + s + "." + ms
        else:
            self.d = "   " + s + "." + ms

        self.t.set(self.d)
        self.dt.set("")
        self.fPosRider.set("(-)")
    
    def resfreshTime(self):
        self.globalTime()

    #Main Loop
    def globalTime(self):
        #Get current global time
        timeG = datetime.datetime.now().time()
        h = int(timeG.hour)
        m = int(timeG.minute)
        s = int(timeG.second)

        #Prepare next rider
        if (s == 50):
            self.key = True

        h = "%02d" % (h)
        m = "%02d" % (m)
 
        #Start next Rider
        if (s == 0 and self.key):
            startTime = h + ":" + m + ":00.000"
            self.time.append(startTime)
            self.startCompetitor += 1
            self.start()

        st = 60 - s
        s = "%02d" % (s)
        
        #Update Global Time
        self.dg = h + "hr " + m + "min " + s + "s"
        self.tg.set(self.dg)

        #Upload start Time
        if (st == 60):
            st = 0

        if( st <= 3 and st >= 0):
            self.lbStart.config(fg =("green"))

        if( st <= 10 and st > 3):
            self.lbStart.config(fg =("yellow"))

        if( st <= 30 and st > 10):
            self.lbStart.config(fg =("red"))

        if( st > 30):
            self.lbStart.config(fg =("black"))

        st = "%02d" % (st)
        startTime = "00:" + st
        self.rt.set(startTime)

        #Display current rider time
        global count
        if(count == 0):
            self.timer()

        #Update time position
        if (self.timeUpdateBool):
            time1 = datetime.datetime.now()
            elapsedTime = time1 - self.timeToUpdate
            timeSecond = divmod(elapsedTime.total_seconds(), 60)[1]
            if (timeSecond > 5):
                self.timeUpdateBool = False
                count = 0
                self.dt.set("")
                self.fPosRider.set("(-)")
                self.updatePosition()

        self.root.after(50, self.resfreshTime)

    def getFinalTime(self, time):
        time = str(time)

        h, m, seg = time.split(":")
        s, ms = map(int, seg.split("."))

        h = "%02d" % int(h)
        m = "%02d" % int(m)
        s = "%02d" % int(s)
        ms = "%03d" % (ms/1000)

        if (h != "00"):
            fTime = h + ":" + m + ":" + s + "." + ms
        else:
            fTime = m + ":" + s + "." + ms
        
        return fTime


    def getResult(self):
        timeG = datetime.datetime.now().time()
        h = int(timeG.hour)
        m = int(timeG.minute)
        s = int(timeG.second)
        ms = timeG.microsecond / 1000.0

        self.timeToUpdate = datetime.datetime.now()
        self.timeUpdateBool = True
        global count
        count = 1

        hf = "%02d" % (h)
        mf = "%02d" % (m)
        sf = "%02d" % (s)
        ms = "%03d" % (ms)
        
        #Update Result
        #Get start Time
        start = self.time[self.competitor]
        end = hf + ":" + mf + ":" + sf + "." + ms
        start_dt = datetime.datetime.strptime(start, '%H:%M:%S.%f')
        end_dt = datetime.datetime.strptime(end, '%H:%M:%S.%f')
        diff = end_dt - start_dt
        diff.seconds/60

        ctime = self.getFinalTime(diff)

        #result = str(self.competitor+1) + ". " + ctime + " (" + hf + ":" + mf + ":" + sf + ":" + ms + ")"

        self.competitor += 1
        compResult = comp["comp" + str(self.competitor)]
        result = "#" + compResult[0] + " " + compResult[1] + " " + compResult[2] + " " + ctime 

        #Save time
        self.list.append(str(diff))
        self.listResult.append(result)

        #Get current position
        sortedTime = sorted(self.list)
        pos = sortedTime.index(str(diff))

        #get difference 
        if (len(self.lastList) < 1 ):
            diff = "  +0:00.000"
        else:
            start = self.lastList[0] #Check th e last better time
            end = str(diff)          #Current time
            start_dt = datetime.datetime.strptime(start, '%H:%M:%S.%f')
            end_dt = datetime.datetime.strptime(end, '%H:%M:%S.%f')

            if (start_dt <= end_dt ):
                diffTime = end_dt - start_dt
                diff = "  +" + self.getFinalTime(diffTime)
                if (str(diffTime) != "0:00:00"):
                    self.lbdt.config(fg =("red"))
                else:
                    diff = "  +0:00.000"
            else:
                diffTime = start_dt - end_dt
                diff = "  -" + self.getFinalTime(diffTime)
                if (str(diffTime) != "0:00:00"):
                    self.lbdt.config(fg =("green"))
                else:
                    diff = "  +0:00.000"

        self.fPosRider.set("(" + str(pos + 1) +") ")
        self.dt.set(diff)

    def updatePosition(self):
        # Lower to greater
        sortedTime = sorted(self.list)
        self.lastList = sortedTime
        n = 0
        for i in sortedTime:
            pos = self.list.index(i)
            n += 1

            if (len(sortedTime) > 1):
                #get difference 
                start = sortedTime[0]
                end = self.list[pos]
                start_dt = datetime.datetime.strptime(start, '%H:%M:%S.%f')
                end_dt = datetime.datetime.strptime(end, '%H:%M:%S.%f')

                diffTime = end_dt - start_dt
                if (str(diffTime) != "0:00:00"):
                    diff = "  +" + self.getFinalTime(diffTime)

            if (n == 1):
                self.r1.set("(1) " + self.listResult[pos])
            elif (n == 2):
                self.r2.set("(2) " + self.listResult[pos] + diff)
            elif (n == 3):
                self.r3.set("(3) " + self.listResult[pos] + diff)
            elif (n == 4):
                self.r4.set("(4) " + self.listResult[pos] + diff)
            elif (n == 5):
                self.r5.set("(5) " + self.listResult[pos] + diff)

        #Restore Current timer for the next competitor
        intervalTime = 1 #Minute
        if (self.displayMinute - 1 < 0):
            self.displayMinute = -1
            global count
            count = 1
            self.d = "00:00.000"
            self.t.set(self.d)
        else:
            self.displayMinute -= 1

        #Update name next rider
        compResult = comp["comp" + str(self.competitor + 1)]
        self.riderData.set("#" + compResult[0] + "  " + compResult[1])

    def __init__(self):
        self.root = Tk()
        self.root.title("Stop Watch")
        self.root.geometry("600x600")
        self.root.resizable(False,False)

        #Global Time 
        self.tg = StringVar()
        self.tg.set("00:00:00")
        self.lbGlobal = Label(self.root,textvariable=self.tg)
        self.lbGlobal.config(font=("Courier 40 bold"))    

        #Rider
        self.riderData = StringVar()
        self.riderData.set("Rider ")
        self.lbRider = Label(self.root,textvariable=self.riderData)
        self.lbRider.config(font=("Courier 30 bold"))

        #Final position Rider
        self.fPosRider = StringVar()
        self.fPosRider.set("(-)")
        self.lbPosRider = Label(self.root,textvariable=self.fPosRider)
        self.lbPosRider.config(font=("Courier 30 bold"))

        #Time
        self.t = StringVar()
        self.t.set("00:00.000")
        self.lb = Label(self.root,textvariable=self.t)
        self.lb.config(font=("Courier 30 bold"))

        #Difference Time at result
        self.dt = StringVar()
        self.dt.set("+00:00.000")
        self.lbdt = Label(self.root,textvariable=self.dt)
        self.lbdt.config(font=("Courier 30 bold"))

        #Start time
        self.rt = StringVar()
        self.rt.set("00:00")
        self.lbStart = Label(self.root,textvariable=self.rt)
        self.lbStart.config(font=("Courier 40 bold"))

        #Result
        self.r1 = StringVar()
        self.r1.set("-. 00:00.000")
        self.lbr1 = Label(self.root, textvariable=self.r1)
        self.lbr1.config(font=("Courier 15 bold"))

        self.r2 = StringVar()
        self.r2.set("-. 00:00.000")
        self.lbr2 = Label(self.root, textvariable=self.r2)
        self.lbr2.config(font=("Courier 15 bold"))

        self.r3 = StringVar()
        self.r3.set("-. 00:00.000")
        self.lbr3 = Label(self.root, textvariable=self.r3)
        self.lbr3.config(font=("Courier 15 bold"))

        self.r4 = StringVar()
        self.r4.set("-. 00:00.000")
        self.lbr4 = Label(self.root, textvariable=self.r4)
        self.lbr4.config(font=("Courier 15 bold"))

        self.r5 = StringVar()
        self.r5.set("-. 00:00.000")
        self.lbr5 = Label(self.root, textvariable=self.r5)
        self.lbr5.config(font=("Courier 15 bold"))

        #self.bt1 = Button(self.root,text="Start",command=self.start,font=("Courier 12 bold"))
        self.bt2 = Button(self.root,text="Stop",command=self.stop,font=("Courier 12 bold"))
        self.bt3 = Button(self.root,text="Reset",command=self.reset,font=("Courier 12 bold"))

        #Place objects
        self.lbGlobal.place(x=100,y=10)
        self.lbRider.place(x=50,y=120)
        self.lbPosRider.place(x=50,y=180)
        self.lb.place(x=120,y=180)
        self.lbdt.place(x=320,y=180)
        #self.bt1.place(x=130,y=200)
        self.bt2.place(x=200,y=300)
        self.bt3.place(x=300,y=300)
        self.lbStart.place(x=200,y=340)
        self.lbr1.place(x=20,y=400)
        self.lbr2.place(x=20,y=440)
        self.lbr3.place(x=20,y=480)
        self.lbr4.place(x=20,y=520)
        self.lbr5.place(x=20,y=560)

        self.key = False
        self.displayMinute = -1
        self.competitor = 0 #Competitor arrived
        self.competitorStartTime = 0 #Competitor started
        self.startCompetitor = 1
        self.timeToUpdate = datetime.datetime.now()
        self.timeUpdateBool = False
        self.time = []
        self.list = []
        self.lastList = []
        self.listResult = []
        #while 1:
        #    self.globalTime()
        #    self.root.update()

        self.globalTime()
        self.root.mainloop()

app = App()