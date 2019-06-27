#!/usr/bin/env python
# license removed for brevity
import rospy
from std_msgs.msg import String
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import Image
import numpy as np

bridge = CvBridge()
def talker():
    #pub = rospy.Publisher('chatter', String, queue_size=10)
    img_pub = rospy.Publisher("image", Image, queue_size=1)
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(1000) # 10hz
    while not rospy.is_shutdown():
        #hello_str = "hello world %s" % rospy.get_time()
        #rospy.loginfo(hello_str)
        #pub.publish(hello_str)
        img = np.zeros((480, 640, 4))
        imgmsg = bridge.cv2_to_imgmsg(img)
        img_pub.publish(imgmsg)
        rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
