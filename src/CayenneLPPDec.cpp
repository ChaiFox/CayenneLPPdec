// 
// 
// 

// CayenneLPP Decoder
//
// Decodes a CayenneLPP binary buffer to JSON format
//
// https://os.mbed.com/teams/myDevicesIoT/code/Cayenne-LPP
// https://github.com/open-source-parsers/jsoncpp
//
// Copyright (c) 2018 Robbert E. Peters. All rights reserved.  
// Licensed under the MIT License. See LICENSE file in the project root for full license information.  
//
// 29 April 2018 - Initial Version
// 21 February 2019 - 0.1.0 - Ported to Arduino platform by Germán Martín
//
// Based on https://github.com/gizmocuz/CayenneLPP-Decoder

#include "CayenneLPPDec.h"
#include <CayenneLPP.h>

/*
Example Usage:
CayenneLPP lpp(200);
lpp.addTemperature(1, 10.4f);
lpp.addTemperature(2, -23.5f);
lpp.addDigitalInput(1, 0);
lpp.addDigitalOutput(1, 1);
lpp.addAnalogInput(1, 1.23f);
lpp.addAnalogOutput(1, 3.45f);
lpp.addLuminosity(1, 20304);
lpp.addPresence(1, 1);
lpp.addTemperature(3, 26.5f);
lpp.addRelativeHumidity(1, 86.6f);
lpp.addAccelerometer(1, 1.234f, -1.234f, 0.567f);
lpp.addBarometricPressure(1, 1023.4f);
lpp.addGyrometer(1, -12.34f, 45.56f, 89.01f);
lpp.addGPS(1, 54.184668f, 7.886778f , -6.3f);
//Decode buffer to JSON
Json::Value root;
if (CayenneLPPDec::ParseLPP(lpp.getBuffer(), lpp.getSize(), root))
{
    std::cout << "Found " << root.size() << " entries:\n\n";
    std::cout << root.toStyledString();
}
*/


bool CayenneLPPDec::ParseLPP (const uint8_t *pBuffer, size_t Len, JsonArray root) {

    while (Len > 2) {
        uint8_t channel = pBuffer[0];
        uint8_t lpp_type = pBuffer[1];

        if (lpp_type == LPP_DIGITAL_INPUT) {
            if (Len < LPP_DIGITAL_INPUT_SIZE+2)
                return false;

            JsonObject data = root.createNestedObject();
            data["channel"] = channel;
            data["type"] = "digital_input";
            data["value"] = pBuffer[2];

            pBuffer += (LPP_DIGITAL_INPUT_SIZE+2);
            Len -= (LPP_DIGITAL_INPUT_SIZE+2);
        } else if (lpp_type == LPP_DIGITAL_OUTPUT) {
            if (Len < LPP_DIGITAL_OUTPUT_SIZE+2)
                return false;

            JsonObject data = root.createNestedObject();
            data["channel"] = channel;
            data["type"] = "digital_output";
            data["value"] = pBuffer[2];

            pBuffer += (LPP_DIGITAL_OUTPUT_SIZE+2);
            Len -= (LPP_DIGITAL_OUTPUT_SIZE+2);
        } else if (lpp_type == LPP_ANALOG_INPUT) {
            if (Len < LPP_ANALOG_INPUT_SIZE+2)
                return false;

            JsonObject data = root.createNestedObject();
            data["channel"] = channel;
            data["type"] = "analog_input";

            float value = float ((pBuffer[2] << 8) | pBuffer[3]) / 100.0f;
            data["value"] = value;

            pBuffer += (LPP_ANALOG_INPUT_SIZE+2);
            Len -= (LPP_ANALOG_INPUT_SIZE+2);
        } else if (lpp_type == LPP_ANALOG_OUTPUT) {
            if (Len < LPP_ANALOG_OUTPUT_SIZE+2)
                return false;

            JsonObject data = root.createNestedObject();
            data["channel"] = channel;
            data["type"] = "analog_output";

            float value = float ((pBuffer[2] << 8) | pBuffer[3]) / 100.0f;
            data["value"] = value;

            pBuffer += (LPP_ANALOG_OUTPUT_SIZE+2);
            Len -= (LPP_ANALOG_OUTPUT_SIZE+2);
        } else if (lpp_type == LPP_LUMINOSITY) {
            if (Len < LPP_LUMINOSITY_SIZE+2)
                return false;

            JsonObject data = root.createNestedObject();
            data["channel"] = channel;
            data["type"] = "luminosity";

            uint16_t value = (pBuffer[2] << 8) | pBuffer[3];
            data["value"] = value;

            pBuffer += (LPP_LUMINOSITY_SIZE;
            Len -= (LPP_LUMINOSITY_SIZE;
        } else if (lpp_type == LPP_PRESENCE) {
            if (Len < LPP_PRESENCE_SIZE+2)
                return false;

            JsonObject data = root.createNestedObject();
            data["channel"] = channel;
            data["type"] = "presence";

            data["value"] = pBuffer[2];

            pBuffer += (LPP_PRESENCE_SIZE+2);
            Len -= (LPP_PRESENCE_SIZE+2);
        } else if (lpp_type == LPP_TEMPERATURE) {
            if (Len < LPP_TEMPERATURE_SIZE+2)
                return false;

            JsonObject data = root.createNestedObject();
            data["channel"] = channel;
            data["type"] = "temp";

            float value = float (int16_t ((pBuffer[2] << 8) | pBuffer[3])) / 10.0f;
            data["value"] = value;

            pBuffer += (LPP_TEMPERATURE_SIZE+2);
            Len -= (LPP_TEMPERATURE_SIZE+2);
        } else if (lpp_type == LPP_RELATIVE_HUMIDITY) {
            if (Len < LPP_RELATIVE_HUMIDITY_SIZE+2)
                return false;
            
            JsonObject data = root.createNestedObject();
            data["channel"] = channel;
            data["type"] = "humidity";

            float value = float (pBuffer[2]) / 2.0f;
            data["value"] = value;

            pBuffer += (LPP_RELATIVE_HUMIDITY_SIZE+2);
            Len -= (LPP_RELATIVE_HUMIDITY_SIZE+2);
        } else if (lpp_type == LPP_ACCELEROMETER) {
            if (Len < LPP_ACCELEROMETER_SIZE+2)
                return false;

            JsonObject data = root.createNestedObject();
            data["channel"] = channel;
            data["type"] = "accel";

            int16_t tvalue = (pBuffer[2] << 8) | pBuffer[3];
            float value = float (tvalue) / 1000.0f;
            data["X"] = value;
            tvalue = (pBuffer[4] << 8) | pBuffer[5];
            value = float (tvalue) / 1000.0f;
            data["Y"] = value;
            tvalue = (pBuffer[6] << 8) | pBuffer[7];
            value = float (tvalue) / 1000.0f;
            data["Z"] = value;

            pBuffer += (LPP_ACCELEROMETER_SIZE+2);
            Len -= (LPP_ACCELEROMETER_SIZE+2);
        } else if (lpp_type == LPP_BAROMETRIC_PRESSURE) {
            if (Len < LPP_BAROMETRIC_PRESSURE_SIZE+2)
                return false;

            JsonObject data = root.createNestedObject();
            data["channel"] = channel;
            data["type"] = "baro";

            float value = float (int16_t ((pBuffer[2] << 8) | pBuffer[3])) / 10.0f;
            data["value"] = value;

            pBuffer += (LPP_BAROMETRIC_PRESSURE_SIZE+2);
            Len -= (LPP_BAROMETRIC_PRESSURE_SIZE+2);
        } else if (lpp_type == LPP_UNIXTIME) {
            if (Len < LPP_UNIXTIME_SIZE+2)
                return false;

            JsonObject data = root.createNestedObject();
            data["channel"] = channel;
            data["type"] = "unixtime";

            uint32_t value = (pBuffer[2] << 24) | (pBuffer[3] << 16) | (pBuffer[4] << 8) | pBuffer[5];
            data["value"] = value;

            pBuffer += (LPP_UNIXTIME_SIZE+2);
            Len -= (LPP_UNIXTIME_SIZE+2);
        } else if (lpp_type == LPP_GYROMETER) {
            if (Len < LPP_GYROMETER_SIZE+2)
                return false;

            JsonObject data = root.createNestedObject();
            data["channel"] = channel;
            data["type"] = "gyro";

            int16_t tvalue = (pBuffer[2] << 8) | pBuffer[3];
            float value = float (tvalue) / 100.0f;
            data["X"] = value;
            tvalue = (pBuffer[4] << 8) | pBuffer[5];
            value = float (tvalue) / 100.0f;
            data["Y"] = value;
            tvalue = (pBuffer[6] << 8) | pBuffer[7];
            value = float (tvalue) / 100.0f;
            data["Z"] = value;

            pBuffer += (LPP_GYROMETER_SIZE+2);
            Len -= (LPP_GYROMETER_SIZE+2);
        } else if (lpp_type == LPP_GPS) {
            if (Len < LPP_GPS_SIZE+2)
                return false;

            JsonObject data = root.createNestedObject();
            data["channel"] = channel;
            data["type"] = "gps";

            int32_t tvalue = (int32_t)(pBuffer[2] << 16) | (pBuffer[3] << 8) | pBuffer[4];
            if ((pBuffer[2] & 0xF0) == 0xF0)
                tvalue |= 0xFF000000;
            float value = float (tvalue) / 10000.0f;
            data["lat"] = value;
            tvalue = (pBuffer[5] << 16) | (pBuffer[6] << 8) | pBuffer[7];
            if ((pBuffer[5] & 0xF0) == 0xF0)
                tvalue |= 0xFF000000;
            value = float (tvalue) / 10000.0f;
            data["lon"] = value;
            tvalue = (int32_t)((pBuffer[8] << 16) | (pBuffer[9] << 8) | pBuffer[10]);
            if ((pBuffer[8] & 0xF0) == 0xF0)
                tvalue |= 0xFF000000;
            value = float (tvalue) / 100.0f;
            data["alt"] = value;

            pBuffer += (LPP_GPS_SIZE+2);
            Len -= (LPP_GPS_SIZE+2);
        } else {
            return false;
        }
    }
    
    return (Len == 0);
}
