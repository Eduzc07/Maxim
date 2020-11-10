# How install flask + python

Flask + python to develop a local server and run os.system() functions.

Using .wsgi + flask, the server is running, but os.system does not work.

```
sudo systemctl enable flask.service
sudo systemctl disable flask.service
sudo systemctl start flask.service
sudo systemctl stop flask.service
systemctl status flask.service

sudo service flask start
sudo service flask status
```

## How to Deploy Flask app as Ubuntu service

This is not using Apache or Nginx.
#### Step 1

Create the Flask app ``odroidApp.py``

#### Step 2

We have to write the following service configuration in edit ` /etc/init/cloudapp.conf``

```
description "cloudapp"
start on stopped rc RUNLEVEL=[2345]
respawn
exec /home/odroid/Workspace/cloudApp/odroidApp.py
```

#### Step 3
Run
``sudo service cloudapp start``

If you receive any error, you may have to create an additional file ``/lib/systemd/system/cloudapp.service``

``sudo vim /lib/systemd/system/cloudapp.service``

Note: **User=odroid** is the username

```
[Unit]
Description=Flask web server

[Install]
WantedBy=multi-user.target

[Service]
User=odroid
PermissionsStartOnly=true
ExecStart=/home/odroid/Workspace/cloudApp/odroidApp.py
TimeoutSec=600
Restart=on-failure
RuntimeDirectoryMode=755
```
#### Step 4
Change the permission to server.py( Note: odroid is username)
```
chown odroid odroidApp.py
chmod +x odroidApp.py
```
#### Step 5
Run the service and check the status
```
sudo service cloudapp start
sudo service cloudapp stop
sudo service cloudapp status
sudo systemctl enable cloudapp.service
sudo systemctl disable cloudapp.service
```

### Sources
- [Deploy Flask as service](https://medium.com/@manivannan_data/how-to-deploy-the-flask-app-as-ubuntu-service-399c0adf3606)
- [Active service](https://better-coding.com/autostart-how-to-run-a-service-on-linux-boot-time-using-systemd/)
