//
//  airHealth.h
//  
//
//  Created by Ansan on 6/3/18.
//
//

#ifndef airHealth_h
#define airHealth_h


#endif /* airHealth_h */

struct aqi {
    float bpH;
    float bpL;
    float iH;
    float iL;
};

struct chart {
    aqi good;
    aqi moderate;
    aqi unhealthyGroups;
    aqi unhealthy;
    aqi veryUnhealthy;
    aqi hazardous;
    aqi veryHazardous;
    aqi error;
};

static int initChartBitMaps();
static int initChartPM10();
static int initChartPM25();
int getIndex(float pm25);



