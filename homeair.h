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

#pragma once

#include <Arduino.h>
#ifndef HOMEAIR
#define HOMEAIR
#endif

#ifndef ERROR
#define ERROR -1
#endif


namespace HomeAir {
	///////////////////////////////////////
	//////////// Program Default /////////
	
	struct Data {
		float temp;
		float pm10;
		float pm25;
		int humid;
		int pm10aqi;
		int pm25aqi;
		int aqi;
	};

	struct Settings {
		unsigned long lastTempUpdate;
		unsigned long LastPMUpdate;
		bool airSensorError;
		unsigned long tempUpdateInterval;
		unsigned long pmUpdateInterval;
	};
}

namespace AirView {
	////////////////////////////////
	////     AirView  //////////////
	
	enum class views {
		aqiTop,
		temperature,
		rawPM25,
		rawPM10,
		helloWorld,
		loading,
		errorTempSensor,
		errorAirSensor
	};

	views& operator++(views&);

	void show(HomeAir::Data*, views, int=30);

	void start();	void showAqiTop(HomeAir::Data*);

} //namespace AirView




namespace AqiMath {
	//////////////////////////////////////
	///////// AqiMath ///////////////////

	enum pollutantType {
		PM25,
		PM10
	};

	int airQualityIndex(float, enum pollutantType);
	
	
	struct breakPoints {
		float bpH;
		float bpL;
		float iH;
		float iL;
	};

	struct chart {
		breakPoints good;
		breakPoints moderate;
		breakPoints unhealthyGroups;
		breakPoints unhealthy;
		breakPoints veryUnhealthy;
		breakPoints hazardous;
		breakPoints veryHazardous;
		breakPoints error;
	};

	void initChartPM10(chart *);
	void initChartPM25(chart *);

} // namespace AqiMath



namespace SendHomeAirData {
	//////////////////////////////////////////
	///////   Send Home Air Data ////////////

	void send(HomeAir::Data*, Stream* serial);

}//namespace SendHomeAirData
