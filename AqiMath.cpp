#include "homeair.h"

namespace AqiMath
{

void initChartPM10(chart *aqi){
    /*Set the values for the PM 10 breakpoints
     and the corresponding index level */
    aqi->good.bpL = 0.0;
    aqi->good.bpH = 54.0;
    aqi->good.iL = 0.0;
    aqi->good.iH = 50.0;

    aqi->moderate.bpL = 55.0;
    aqi->moderate.bpH = 154.0;
    aqi->moderate.iL = 51.0;
    aqi->moderate.iH = 100.0;

    aqi->unhealthyGroups.bpL = 155.0;
    aqi->unhealthyGroups.bpH = 254.0;
    aqi->unhealthyGroups.iL = 101.0;
    aqi->unhealthyGroups.iH = 150.0;

    aqi->unhealthy.bpL = 255.0;
    aqi->unhealthy.bpH = 354.0;
    aqi->unhealthy.iL = 151.0;
    aqi->unhealthy.iH = 200.0;

    aqi->veryUnhealthy.bpL = 355.0;
    aqi->veryUnhealthy.bpH = 424.0;
    aqi->veryUnhealthy.iL = 201.0;
    aqi->veryUnhealthy.iH = 300.0;

    aqi->hazardous.bpL = 425.0;
    aqi->hazardous.bpH = 504.0;
    aqi->hazardous.iL = 301.0;
    aqi->hazardous.iH = 400.0;

    aqi->veryHazardous.bpL = 505.0;
    aqi->veryHazardous.bpH = 604.0;
    aqi->veryHazardous.iL = 401.0;
    aqi->veryHazardous.iH = 500.0;

    aqi->error.bpL = 1.0;
    aqi->error.bpH = -2.0;
    aqi->error.iL = 1.0;
    aqi->error.iH = -2.0;

}

void initChartPM25(chart *aqi){
    /*Set the values for the PM 25 breakpoints
     and the corresponding index level */

    aqi->good.bpL = 0.0;
    aqi->good.bpH = 12.0;
    aqi->good.iL = 0.0;
    aqi->good.iH = 50.0;

    aqi->moderate.bpL = 12.1;
    aqi->moderate.bpH = 35.4;
    aqi->moderate.iL = 51.0;
    aqi->moderate.iH = 100.0;

    aqi->unhealthyGroups.bpL = 35.5;
    aqi->unhealthyGroups.bpH = 55.4;
    aqi->unhealthyGroups.iL = 101.0;
    aqi->unhealthyGroups.iH = 150.0;

    aqi->unhealthy.bpL = 55.5;
    aqi->unhealthy.bpH = 150.4;
    aqi->unhealthy.iL = 151.0;
    aqi->unhealthy.iH = 200.0;

    aqi->veryUnhealthy.bpL = 150.5;
    aqi->veryUnhealthy.bpH = 250.4;
    aqi->veryUnhealthy.iL = 201.0;
    aqi->veryUnhealthy.iH = 300.0;

    aqi->hazardous.bpL = 250.5;
    aqi->hazardous.bpH = 350.4;
    aqi->hazardous.iL = 301.0;
    aqi->hazardous.iH = 400.0;

    aqi->veryHazardous.bpL = 350.5;
    aqi->veryHazardous.bpH = 500.4;
    aqi->veryHazardous.iL = 401.0;
    aqi->veryHazardous.iH = 500.0;

    aqi->error.bpL = 1.0;
    aqi->error.bpH = -2.0;
    aqi->error.iL = 1.0;
    aqi->error.iH = -2.0;

}

int airQualityIndex(float pm, pollutantType type){
    /* Calculate the Air Quality Index Value for
     *  a given concentration (milligrams per cubic microgram)
     */
    chart aqi;
    breakPoints *result;
    pm = int( pm * 10.0) / 10.0;

    if( type == PM25){
	    initChartPM25(&aqi);

    }else if( type == PM10 ){
	    initChartPM10(&aqi);

    }else{
	    return ERROR;
    }

    if( pm >= aqi.good.bpL && pm <= aqi.good.bpH ){
        result = &aqi.good;

    } else if ( pm >= aqi.moderate.bpL && pm <= aqi.moderate.bpH ){
        result = &aqi.moderate;

    } else if ( pm >= aqi.unhealthyGroups.bpL && pm <= aqi.unhealthyGroups.bpH ){
        result = &aqi.unhealthyGroups;

    } else if ( pm >= aqi.unhealthy.bpL && pm <= aqi.unhealthy.bpH ){
        result = &aqi.unhealthy;

    } else if ( pm >= aqi.veryUnhealthy.bpL && pm <= aqi.veryUnhealthy.bpH ){
        result = &aqi.veryUnhealthy;

    } else if ( pm >= aqi.hazardous.bpL && pm <= aqi.hazardous.bpH ){
        result = &aqi.hazardous;

    } else if ( pm >= aqi.veryHazardous.bpL && pm <= aqi.veryHazardous.bpH ){
        result = &aqi.veryHazardous;

    } else if ( pm > aqi.veryHazardous.bpH ){
	return 999.0;
    }else{
	return ERROR; }

    return int((result->iH - result->iL)/(result->bpH - result->bpL)*(pm-result->bpL) + result->iL);

}//end of function airQualityIndex

}//end of namespeace AqiMath
