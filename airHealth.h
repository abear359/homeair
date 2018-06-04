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

static void initChartPM10();
static void initChartPM25();
static int getIndex(float pm25);



