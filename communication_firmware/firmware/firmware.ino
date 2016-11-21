#include <ros.h>
#include <std_msgs/Int8.h>

ros::NodeHandle nh;
int new_val = 0;
std_msgs::Int8 new_int;
ros::Publisher pubs("arduino_topic", &new_int);

void messageCb( const std_msgs::Int8& toggle_msg){
   new_val = toggle_msg.data;
   new_val /= 2;
   digitalWrite(13, HIGH-digitalRead(13));   // blink the led
}

ros::Subscriber<std_msgs::Int8> sub("dummy_topic", &messageCb );

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(pubs);
}

void loop() {
  // put your main code here, to run repeatedly:
  new_int.data = new_val;
  pubs.publish(&new_int);
  nh.spinOnce();
  delay(1);
}
