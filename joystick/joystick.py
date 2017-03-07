#!/usr/bin/env python
import rospy
from sensor_msgs.msg import Joy
from communication.msg import target_positions_msg
from subprocess import call

number_of_robots = 3

def callback(data):
    k = 1
    msg = target_positions_msg()

    for robot in range(number_of_robots):
        msg.x[robot] = -data.axes[0]*k
        msg.y[robot] =  data.axes[1]*k

    msg.x[0] = 0
    msg.y[0] = 0

    msg.x[1] = 0
    msg.y[1] = 0


    pub.publish(msg)

def start():
        global pub
        pub = rospy.Publisher('relative_positions_topic',target_positions_msg, queue_size=10)
        rospy.Subscriber('joy', Joy, callback)
        rospy.init_node('joystick')
        rospy.spin()

if __name__ == '__main__':
    start()