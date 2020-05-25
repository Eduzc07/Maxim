#!/usr/bin/python
import os
import time
from flask import Flask, render_template

app = Flask(__name__)

@app.route('/')
def index():
   name = 'Zarate Cloud'
   return render_template('index.html', title='zarateCloud', username=name)

@app.route('/poweroff', methods=['GET', 'POST'])
def poweroff():
   app.logger.info('Powering off')
#   os.system("echo --- Retirar Fabi HDD ---")
#   os.system("sudo umount /media/odroid/Fabi_HDD")
#   os.system("echo --- Shutdown now ---")
   time.sleep(2)
#   os.system("sudo shutdown -h now")
   return "Done!"

@app.route('/donepage', methods=['GET', 'POST'])
def done():
#   app.logger.info('Powering off')
#   os.system("echo --- Retirar Fabi HDD ---")
   #os.system("sudo umount /media/odroid/Fabi_HDD")
   os.system("sudo umount /home/fabi/hdd")
#   os.system("echo --- Shutdown now ---")
#   time.sleep(2)
#   os.system("sudo shutdown -hP -t 5")
   os.system("sudo shutdown -hP now")

   return render_template('done.html', title="Done")

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, debug=True)
