# Robot with wireless joystick control

Project in the second year at the computer engineering programme. A two-wheel-drive out-of-the-box robot with an Atmega 328P microcontroller was programmed to follow instructions received wirelessly from a, by the group designed, remote control via ZigBee communication. The robot automatically avoids obstacles in its forward direction using an HC-SR04 ultrasonic distance sensor and detects collisions using a MPU6050 accelerometer.

If an obstacle is detected by the robot, it disables forward motion and sends a message to the joystick which displays a short collision warning message on the remote control OLED display. Now, the remote control operator must reverse the robot to get out of the obstacle zone.

If a collision is detected by the robot, the robot disables all motion and sends another message to the remote control. The remote displays info about this event and how to reactivate the robot. Reactivation is done by pressing and holding the joystick (button), which sends an "collision confirmed" message to robot.

All code is written in C and runs bare-metal on the Atmega 328P microcontrollers on both the remote and the robot.
