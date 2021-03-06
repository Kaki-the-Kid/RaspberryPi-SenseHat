////////////////////////////////////////////////////////////////////////////
//
//  This file is part of RTIMULib
//
//  Copyright (c) 2014-2015, richards-tech, LLC
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//  this software and associated documentation files (the "Software"), to deal in
//  the Software without restriction, including without limitation the rights to use,
//  copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
//  Software, and to permit persons to whom the Software is furnished to do so,
//  subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
/*
file 		http://raspberrypi.pagesperso-orange.fr/dossiers/27-01.htm
file		https://github.com/RTIMULib/RTIMULib2/blob/master/Linux/RTIMULibDrive11/RTIMULibDrive11.cpp
Compilation 	g++ -o exemple exemple.cpp -l RTIMULib
Exécution 	./exemple




*/


#include "RTIMULib.h"

int main()
{
    int sampleCount = 0;
    int sampleRate = 0;
    uint64_t rateTimer;
    uint64_t displayTimer;
    uint64_t now;

    //  using RTIMULib here allows it to use the .ini file generated by RTIMULibDemo.

    RTIMUSettings *settings = new RTIMUSettings("RTIMULib");

    RTIMU *imu = RTIMU::createIMU(settings);
    RTPressure *pressure = RTPressure::createPressure(settings);
    RTHumidity *humidity = RTHumidity::createHumidity(settings);

    if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL)) {
        printf("No IMU found\n");
        exit(1);
    }

    //  This is an opportunity to manually override any settings before the call IMUInit

    //  set up IMU

    imu->IMUInit();

    //  this is a convenient place to change fusion parameters

    imu->setSlerpPower(0.02);
    imu->setGyroEnable(true);
    imu->setAccelEnable(true);
    imu->setCompassEnable(true);

    //  set up pressure sensor

    if (pressure != NULL)
        pressure->pressureInit();

    //  set up humidity sensor

    if (humidity != NULL)
        humidity->humidityInit();

    //  set up for rate timer

    rateTimer = displayTimer = RTMath::currentUSecsSinceEpoch();

    //  now just process data

    while (1) {
        //  poll at the rate recommended by the IMU

        usleep(imu->IMUGetPollInterval() * 1000);

        while (imu->IMURead()) {
            RTIMU_DATA imuData = imu->getIMUData();

            //  add the pressure data to the structure

            if (pressure != NULL)
                pressure->pressureRead(imuData);

            //  add the humidity data to the structure

            if (humidity != NULL)
                humidity->humidityRead(imuData);

            sampleCount++;

            now = RTMath::currentUSecsSinceEpoch();

            //  display 5 times per second

            if ((now - displayTimer) > 200000) {
				system("clear");
                printf("Sample rate %d\n", sampleRate);
                printf("Magnétique x: %f\n", imuData.compass.x());
                printf("Magnétique y: %f\n", imuData.compass.y());
                printf("Magnétique z: %f\n", imuData.compass.z());
                printf("Acceleration x(G): %f\n", imuData.accel.x());
                printf("Acceleration y(G): %f\n", imuData.accel.y());
                printf("Acceleration z(G): %f\n", imuData.accel.z());
                printf("Gyro (deg/s): %s\n", RTMath::displayDegrees("", imuData.gyro));
                printf("Gyro x (rad/s): %f\n", imuData.gyro.x());
                printf("Gyro y (rad/s): %f\n", imuData.gyro.y());
                printf("Gyro z (rad/s): %f\n", imuData.gyro.z());
                open_fbdev(const char *dev_name);

                if (pressure != NULL) {
                    printf("Pressure: %4.1f, height above sea level: %4.1f, temperature: %4.1f",
                           imuData.pressure, RTMath::convertPressureToHeight(imuData.pressure), imuData.temperature);
                }
                if (humidity != NULL) {
                    printf(", humidity: %4.1f",
                           imuData.humidity);
                }
                printf("\n");

                fflush(stdout);
                displayTimer = now;
            }

            //  update rate every second

            if ((now - rateTimer) > 1000000) {
                sampleRate = sampleCount;
                sampleCount = 0;
                rateTimer = now;
            }
        }
    }
}
