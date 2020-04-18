#!/usr/bin/env python

## Read ADC value and calculate battery voltage
## See http://odroid.com/dokuwiki/doku.php?id=en:c0_read_battery_voltage

import rospy
from std_msgs.msg import Float32

def batmon_node():
    pub = rospy.Publisher('Odroid_Voltage', Float32, queue_size=1)
    rospy.init_node('batmon', anonymous=True)
    rate = rospy.Rate(1) # 10hz
    while not rospy.is_shutdown():
	voltage=-1
	f=open("/sys/class/saradc/saradc_ch0")
	try:
		raw_value=int(f.readline().strip())        
	finally:
		f.close()

        voltage=0.004106*raw_value

	if voltage<3.7:
		rospy.logwarn('Battery voltage low!')
        pub.publish(voltage)
        rate.sleep()

if __name__ == '__main__':
    try:
        batmon_node()
    except rospy.ROSInterruptException:
        pass
