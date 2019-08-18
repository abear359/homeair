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

#include "homeair.h"
#include <Arduino.h>

// Over a serial connection...
// Send the following information in a packaged format:
// temp, pm10, pm25, humidity, pm25AQI, pm10AQI, AQI, fanStatus
// They data is packaged in a small form factor just because.
// It was fun getting intimate with bytes and bits.
//

namespace SendHomeAirData {


void send(HomeAir::Data* d, Stream* outStream){
    
    uint8_t dArray[20];
    
    uint16_t tempINT =  uint16_t(d->temp * 10);
    uint16_t pm10INT = uint16_t(d->pm10 * 10);
    uint16_t pm25INT = uint16_t(d->pm25 * 10);
    
    dArray[0] = 0xAA;
    dArray[1] = 0xB1;
    
    // 2 - 3 // TEMP
    dArray[2] = tempINT >> 8;
    dArray[3] = tempINT;
    
    // 4 // Humidity
    dArray[4] = d->humid;
    
    // 5 - 6 // pm25
    dArray[5] = pm25INT >> 8;
    dArray[6] = pm25INT;
    
    // 7-8 // pm25 AQI
    dArray[7] = d->pm25aqi >> 8;
    dArray[8] = d->pm25aqi;
    
    // 9-10 // pm10
    dArray[9] = pm10INT >> 8;
    dArray[10] = pm10INT;
    
    // 11-12 // pm 10 AQI
    dArray[11] = d->pm10aqi >> 8;
    dArray[12] = d->pm10aqi;
    
    // 13-14 // AQI
    dArray[13] = d->aqi >> 8;
    dArray[14] = d->aqi;
    
    dArray[15] = 0;
    
    // 15 // Checksum
    dArray[16] = (dArray[2] + dArray[3] + dArray[4] + dArray[5] + dArray[6]);
    dArray[16] += (dArray[7] + dArray[8] + dArray[9]);
    dArray[16] += (dArray[10] + dArray[11] + dArray[12] + dArray[13] + dArray[14] + dArray[15]);
    dArray[16] &= 0xFF;
    
    // 16 // ending
    dArray[17] = 0xAB;
    
    outStream->write(&dArray[0], 18);
    outStream->flush();
    
}

}//end namespace

