#if !defined(PH_DFROBOT_SENSOR_H)
#define PH_DFROBOT_SENSOR_H

#include "../base_sensor.h"
#include "ads_sensor.h"

class PHDFRobotSensor : public BaseSensor
{
private:
    uint8_t _channelADS = 255;
    uint8_t _pinAnalog = 255;

    static const uint8_t _windowSize = 40;
    uint16_t _rawBuffer[_windowSize];
    uint8_t _arrayIndex = 0;
    uint64_t _prevSampling = 0;

    const float _offset = 0;
    const float _vref = 3.3;
    const uint16_t _adcResolution = 4095;
    ADS1115Module *_ads = nullptr;

    double _avergearray(uint16_t *arr, uint8_t number)
    {
        uint8_t i;
        uint16_t max, min;
        double avg;
        long amount = 0;

        if (number < 5) // less than 5, calculated directly statistics
        {
            for (i = 0; i < number; i++)
                amount += arr[i];
            avg = amount / number;
            return avg;
        }
        else
        {
            if (arr[0] < arr[1])
            {
                min = arr[0];
                max = arr[1];
            }
            else
            {
                min = arr[1];
                max = arr[0];
            }
            for (i = 2; i < number; i++)
            {
                if (arr[i] < min)
                {
                    amount += min; // arr<min
                    min = arr[i];
                }
                else
                {
                    if (arr[i] > max)
                    {
                        amount += max; // arr>max
                        max = arr[i];
                    }
                    else
                    {
                        amount += arr[i]; // min<=arr<=max
                    }
                } // if
            } // for
            avg = (double)amount / (number - 2);
        } // if
        return avg;
    }

public:
    PHDFRobotSensor(
        unsigned char id, const char *name,
        uint8_t channelADS, ADS1115Module *ads)
        : BaseSensor(id, name), _channelADS(channelADS), _ads(ads) {}

    PHDFRobotSensor(
        unsigned char id, const char *name,
        uint8_t pinAnalog)
        : BaseSensor(id, name), _pinAnalog(pinAnalog) {}

    ~PHDFRobotSensor() override = default;

    void begin()
    {
        if (_ads == nullptr)
            pinMode(_pinAnalog, INPUT);
    }

    void update()
    {
        if (millis() - _prevSampling > 40U)
        {
            _prevSampling = millis();
            _rawBuffer[_arrayIndex] = _ads != nullptr ? _ads->read(_channelADS) : analogRead(_pinAnalog);
            _arrayIndex++;
            if (_arrayIndex == _windowSize)
                _arrayIndex = 0;
        }
    }

    float read() override
    {
        uint16_t analogBufferTemp[_windowSize];
        for (uint8_t copyIndex = 0; copyIndex < _windowSize; copyIndex++)
            analogBufferTemp[copyIndex] = _rawBuffer[copyIndex];
        float averageVoltage = _avergearray(analogBufferTemp, _windowSize) * (float)_vref / _adcResolution;

        float pHValue = 3.5 * averageVoltage + _offset;
        return pHValue;
    }
};

class MockPHDFRobotSensor : public BaseSensor
{
private:
    uint8_t _channelADS = 255;
    uint8_t _pinAnalog = 255;

    static const uint8_t _windowSize = 40;
    uint16_t _rawBuffer[_windowSize];
    uint8_t _arrayIndex = 0;
    uint64_t _prevSampling = 0;

    const float _offset = 0;
    const float _vref = 3.3;
    const uint16_t _adcResolution = 4095;
    ADS1115Module *_ads = nullptr;

    double _avergearray(uint16_t *arr, uint8_t number)
    {
        uint8_t i;
        uint16_t max, min;
        double avg;
        long amount = 0;

        if (number < 5) // less than 5, calculated directly statistics
        {
            for (i = 0; i < number; i++)
                amount += arr[i];
            avg = amount / number;
            return avg;
        }
        else
        {
            if (arr[0] < arr[1])
            {
                min = arr[0];
                max = arr[1];
            }
            else
            {
                min = arr[1];
                max = arr[0];
            }
            for (i = 2; i < number; i++)
            {
                if (arr[i] < min)
                {
                    amount += min; // arr<min
                    min = arr[i];
                }
                else
                {
                    if (arr[i] > max)
                    {
                        amount += max; // arr>max
                        max = arr[i];
                    }
                    else
                    {
                        amount += arr[i]; // min<=arr<=max
                    }
                } // if
            } // for
            avg = (double)amount / (number - 2);
        } // if
        return avg;
    }

public:
    MockPHDFRobotSensor(
        unsigned char id, const char *name,
        uint8_t channelADS, ADS1115Module *ads)
        : BaseSensor(id, name), _channelADS(channelADS), _ads(ads) {}

    MockPHDFRobotSensor(
        unsigned char id, const char *name,
        uint8_t pinAnalog)
        : BaseSensor(id, name), _pinAnalog(pinAnalog) {}

    ~MockPHDFRobotSensor() override = default;

    void begin()
    {
        return;
    }

    void update()
    {
        if (millis() - _prevSampling > 40U)
        {
            _prevSampling = millis();
            _rawBuffer[_arrayIndex] = _ads != nullptr ? random(4095) : random(4095);
            _arrayIndex++;
            if (_arrayIndex == _windowSize)
                _arrayIndex = 0;
        }
    }

    float read() override
    {
        uint16_t analogBufferTemp[_windowSize];
        for (uint8_t copyIndex = 0; copyIndex < _windowSize; copyIndex++)
            analogBufferTemp[copyIndex] = _rawBuffer[copyIndex];
        float averageVoltage = _avergearray(analogBufferTemp, _windowSize) * (float)_vref / _adcResolution;

        float pHValue = 3.5 * averageVoltage + _offset;
        return pHValue;
    }
};

#endif // PH_DFROBOT_SENSOR_H
