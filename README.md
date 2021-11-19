# BLE-glove-Mouse-Controller
A Bluetooth Mouse Controller made with an ESP32 and an MPU6050 (gyroscope).
The project uses an ESP32, an MPU6050 (mainly for the gyroscope measures) and a strain gauge to simulate the click.



![Arduino_scheme](https://user-images.githubusercontent.com/77463115/142691077-3fadf3ad-f0fd-4bf1-ad5c-32688202ee20.png)

The project can be updated by using BLe functionnalities to improve the power effeciency. One can also use a different IMU instead of the MPU6050, in order to exploit the magnetometer to know when the user's hand palm is facing up or down, and change subsenquently the direction the mouses moves. This would let people to use the glove in both situation and to change seamlessy without having to change the code each time. Doing so, the microcontroller would automatically adapt the Mouse's movement to the hand gestures of the user, according to the hand's palm direction. 

Project made by Akash MAHTANI and Romain Perennou as per a project for OCENE at CentraleSupélec (Paris-Saclay University).
