#include "homeair.h"

//u8g2 requirements
#include <Arduino.h>
#include <U8x8lib.h>
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


// files filled with defined data for images
#include "img/GREAT.h"
#include "img/GOOD.h"
#include "img/MEDIUM.h"
#include "img/BAD.h"
#include "img/VERY_BAD.h"
#include "img/VERY_VERY_BAD.h"
#include "img/loading.h"
//#include "img/cat.h"

#ifndef SLC
#define SCL 2
#endif

#ifndef SDA
#define SDA 3
#endif

namespace AirView
{
	U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE);
	//U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0,SCL,SDA,U8X8_PIN_NONE);

	views& operator++(views& t)
		// prefix increment: ++
	{
		switch (t) {
			case views::aqiTop:    return t = views::temperature;
			case views::temperature:   return t = views::rawPM25;
			case views::rawPM25: return t = views::rawPM10;
			case views::rawPM10: return t = views::aqiTop;
			default: return t = views::aqiTop;
		}
	}

	///////////////////////////////////////////////
	//////////   DISPLAY FUNCTIONS ///////////////

	void drawIcon(int aqi){
		u8g2.setBitmapMode(0);
		if(aqi <= 50){
			// GREAT
			u8g2.drawXBMP(128-(greatIconW), 16 - (greatIconH/2), greatIconW, greatIconH, greatIcon);
		}else if(aqi <= 100){
			// GOOD
			u8g2.drawXBMP(128-(goodIconW), 16 - (goodIconH/2), goodIconW, goodIconH, goodIcon);
		}else if(aqi <= 150){
			// MEDIUM
			u8g2.drawXBMP(128-(mediumIconW), 16 - (mediumIconH/2), mediumIconW, mediumIconH, mediumIcon);
		}else if(aqi <= 200){
			// BAD
			u8g2.drawXBMP(128-(badIconW), 16 - (badIconH/2), badIconW, badIconH, badIcon);
		}else if(aqi <= 250){
			// VERY BAD
			u8g2.drawXBMP(128-(veryBadIconW), 16 - (veryBadIconH/2), veryBadIconW, veryBadIconH, veryBadIcon);
		}else if(aqi >= 250){
			// VERY VERY BAD
			u8g2.drawXBMP(128-(veryVeryBadIconW), 16 - (veryVeryBadIconH/2), veryVeryBadIconW, veryVeryBadIconH, veryVeryBadIcon);
		}

	}

	void drawRotatingLoadingIcon(int progress){
		int rotation = progress % 2;

		switch(rotation){
			case 0:
				u8g2.setDisplayRotation(U8G2_R0);
				u8g2.drawXBMP(128-(loadingW), 16 - (loadingH/2), loadingW, loadingH, loadingIcon);
				break;
			case 1:
				u8g2.setDisplayRotation(U8G2_R1);
				u8g2.drawXBMP(0, 0, loadingW, loadingH, loadingIcon);
				break;
			default:
				u8g2.setDisplayRotation(U8G2_R0);
				u8g2.drawXBMP(128-(loadingW), 16 - (loadingH/2), loadingW, loadingH, loadingIcon);
				break;
		}
		u8g2.setDisplayRotation(U8G2_R0);
	}


	void showAqiTop(HomeAir::Data* d) {
		u8g2.clearBuffer();

		u8g2.setFontPosTop();
		u8g2.setFont(u8g2_font_repress_mel_tr);
		u8g2.setCursor(0, 0);
		u8g2.print(F("AQI"));

		u8g2.setFont(u8g2_font_ncenB14_tr);
		u8g2.setFontPosBottom();
		u8g2.setCursor(0,32);
		u8g2.print(d->aqi);

		drawIcon(d->aqi);

		u8g2.sendBuffer();
	}


	void showTemperature(HomeAir::Data* d) {
		u8g2.clearBuffer();
		u8g2.setFont(u8g2_font_amstrad_cpc_extended_8f);

		u8g2.setFontPosTop();
		u8g2.setCursor(0, 0);
		u8g2.print(F("TEMP"));


		u8g2.setFont(u8g2_font_ncenB14_tr);
		u8g2.setFontPosBottom();
		u8g2.setCursor(0,32);
		u8g2.print(d->temp,0);
		u8g2.print(F(" C"));

		u8g2.setFont(u8g2_font_amstrad_cpc_extended_8f);
		u8g2.setFontPosTop();
		u8g2.setCursor(128 - u8g2.getUTF8Width("HUMID"), 0); // 40 pixels
		u8g2.print(F("HUMID"));

		u8g2.setFont(u8g2_font_ncenB14_tr);
		u8g2.setFontPosBottom();
		u8g2.setCursor(127 - u8g2.getUTF8Width("%") - u8g2.getUTF8Width(u8x8_u8toa(d->humid,2)), 32);
		u8g2.print(d->humid);
		u8g2.setCursor(128 - u8g2.getUTF8Width("%"),32);
		u8g2.print(F("%"));
		u8g2.sendBuffer();

	}


	void showRawPM25(HomeAir::Data* d) {
		u8g2.clearBuffer();
		u8g2.setFontPosTop();
		u8g2.setFont(u8g2_font_repress_mel_tr);
		u8g2.setCursor(0, 0);
		u8g2.print(F("PM 2.5"));

		u8g2.setFont(u8g2_font_ncenB14_tr);
		u8g2.setFontPosBottom();
		u8g2.setCursor(0,32);
		u8g2.print(d->pm25,1);

		drawIcon(d->aqi);

		u8g2.sendBuffer();

	}

	void showRawPM10(HomeAir::Data* d) {
		u8g2.clearBuffer();
		u8g2.setFontPosTop();
		u8g2.setFont(u8g2_font_repress_mel_tr);
		u8g2.setCursor(0, 0);
		u8g2.print(F("PM 10"));

		u8g2.setFont(u8g2_font_ncenB14_tr);
		u8g2.setFontPosBottom();
		u8g2.setCursor(0,32);
		u8g2.print(d->pm10,1);

		drawIcon(d->aqi);

		u8g2.sendBuffer();

	}

	void showErrorTempSensor(){
		u8g2.clearBuffer();
		u8g2.setFontPosTop();
		u8g2.setFont(u8g2_font_ncenB14_tr);
		u8g2.setCursor(0,0);
		u8g2.print(F("error: TempSensor"));
		u8g2.sendBuffer();
	}

	void showErrorAirSensor(){
		u8g2.clearBuffer();
		u8g2.setFontPosTop();
		u8g2.setFont(u8g2_font_ncenB14_tr);
		u8g2.setCursor(0,0);
		u8g2.print(F("error: AirSensor"));
		u8g2.sendBuffer();
	}


	void showHelloWorld() {
		u8g2.clearBuffer();
		u8g2.setFontPosTop();
		u8g2.setFont(u8g2_font_ncenB14_tr);
		u8g2.setCursor(0,0);
		u8g2.print(F("`|"));
		u8g2.print(123456789);
		u8g2.sendBuffer();
	}

	void showLoading(int s) {
		int i = 0;
		while(++i<s){
			u8g2.clearBuffer();
			u8g2.setFont(u8g2_font_ncenB14_tr);
			u8g2.setFontPosTop();
			u8g2.setCursor(0,0);
			u8g2.print(F("Loading"));
			drawRotatingLoadingIcon(i);
			u8g2.sendBuffer();
			delay(1000);
		}
	}

	//	public:
	void show(HomeAir::Data* d, views t, int seconds) {
		switch (t) {
			case views::aqiTop:
				showAqiTop(d);
				break;
			case views::temperature:
				showTemperature(d);
				break;
			case views::rawPM25:
				showRawPM25(d);
				break;
			case views::rawPM10:
				showRawPM10(d);
				break;
			case views::loading:
				showLoading(seconds);
				break;
			case views::errorTempSensor:
				showErrorTempSensor();
				break;
			case views::errorAirSensor:
				showErrorAirSensor();
				break;
			default: showHelloWorld();
		}
	}


	void start(){
		u8g2.begin();
		u8g2.enableUTF8Print();
	}

}//end class
