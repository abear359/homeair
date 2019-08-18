/*
 * This file is part of homeair.
 *
 * homeair is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * homeair is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with homeair.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <DHT.h>
//#include <SPI.h>
#include <Wire.h>
#include <SDS011.h>

#include "homeair.h"

#define DHT_PIN 2
#define DHT_TYPE DHT22

#define AIR_SENSOR_TX 11
#define AIR_SENSOR_RX 12
#define SERIAL_BAUD_RATE 9600

#define BUTTON_A 3
#define SWITCH_A 4

#define SLEEP false
#define AWAKE true

#ifndef ERROR
#define ERROR -1
#endif

#ifndef NO_ERROR
#define NO_ERROR 0
#endif

DHT tempSensor(DHT_PIN, DHT_TYPE);

////////// GLOBAL VARIABLES ////////////
HomeAir::Data *myData;
AirView::views viewType=AirView::views::aqiTop;

static SDS011 airSensor;

static unsigned long lastTempUpdate = 0;
static unsigned long lastPMUpdate = 0;

static const unsigned long tempUpdateInterval = 10000; // 10 seconds
static const unsigned long pmUpdateInterval = 240000; // 4 minutes

static bool airSensorStatus = AWAKE;

static bool buttonPressed = false;
static unsigned long buttonPressedTime = 0;
static const unsigned int buttonPressedTimeMin = 100;

int getFanStatus(HomeAir::Data* d){
	if(digitalRead(SWITCH_A) == HIGH && d->aqi >= 40){
		return 1;
	}else{
		return 0;
	}
}

static int readTempSensor(HomeAir::Data* d)
{
	d->humid = tempSensor.readHumidity();
	d->temp = tempSensor.readTemperature();

	if (isnan(d->humid) || isnan(d->temp)) {
		return ERROR;
	}else{
		return NO_ERROR;
	}
}

static int readAirSensor(HomeAir::Data* d){
	airSensor.read(&d->pm25, &d->pm10);
	if(d->pm25 < 0 || d->pm10 < 0){
		d->pm25aqi = -1;
		d->pm25aqi = -1;
		d->aqi = -1;
		return ERROR;
	}else{
		d->pm25aqi = airQualityIndex(d->pm25, AqiMath::pollutantType::PM25);
		d->pm10aqi = airQualityIndex(d->pm10, AqiMath::pollutantType::PM10);
		d->aqi = d->pm25aqi > d->pm10aqi ? d->pm25aqi : d->pm10aqi;
		return NO_ERROR;
	}
}


void setup() {

	airSensor.begin(AIR_SENSOR_RX, AIR_SENSOR_TX);
	airSensor.wakeup();

	Serial.begin(SERIAL_BAUD_RATE);

	tempSensor.begin();

	pinMode(BUTTON_A, INPUT);
	pinMode(SWITCH_A, INPUT);
	//    pinMode(13, OUTPUT);
	//   digitalWrite(13, LOW);

	viewType = AirView::views::aqiTop;
	myData = new HomeAir::Data {0.0, 0.0, 0.0, 0, 0, 0, 0};

	AirView::start();
	AirView::show(myData, AirView::views::loading);

	if(readTempSensor(myData) == ERROR){
		viewType = AirView::views::errorTempSensor;
	}
	if(readAirSensor(myData) == ERROR){
		viewType = AirView::views::errorAirSensor;
	}

	SendHomeAirData::send(myData, &Serial);

	airSensor.sleep();
	airSensorStatus = SLEEP;

	AirView::show(myData, viewType);

}

void loop() {

	unsigned long currentMillis = millis();

	if(digitalRead(BUTTON_A) == HIGH){
		if(buttonPressed == false){
			buttonPressedTime = currentMillis;
		}
		buttonPressed = true;
	}else{
		if(buttonPressed == true && currentMillis - buttonPressedTime > buttonPressedTimeMin){
			AirView::show(myData, ++viewType);
		}
		buttonPressed = false;
	}

	// Get the tempurature data
	if(currentMillis - lastTempUpdate > tempUpdateInterval){	
		if(readTempSensor(myData) == ERROR){
			viewType = AirView::views::errorTempSensor;
		}
		lastTempUpdate = currentMillis;
	}

	// After 4 minutes turn on the SDS011
	if(currentMillis - lastPMUpdate > pmUpdateInterval && airSensorStatus == SLEEP){
		airSensor.wakeup();
		airSensorStatus = AWAKE;
	}

	// After 5 minutes, get the air pollution levels
	if(currentMillis - lastPMUpdate > (pmUpdateInterval + 60000) && airSensorStatus == AWAKE){

		if(readAirSensor(myData) == ERROR){
			viewType = AirView::views::errorAirSensor;
		}
		AirView::show(myData, viewType);
		SendHomeAirData::send(myData, &Serial);
		lastPMUpdate = currentMillis;
		airSensor.sleep();
		airSensorStatus = SLEEP;
	}

}
