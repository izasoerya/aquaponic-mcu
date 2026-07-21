#if !defined(MOVING_AVERAGE)
#define MOVING_AVERAGE

#include "base_filter.h"

class MovingAverageFilter : public BaseFilter
{
private:
    float *_buffer;
    uint8_t _windowSize;
    uint8_t _index = 0;

public:
    MovingAverageFilter(uint8_t windowSize = 10) : _windowSize(windowSize), _buffer(new float[_windowSize])
    {
        for (uint8_t i = 0; i < _windowSize; i++)
            _buffer[i] = 0;
    }
    ~MovingAverageFilter() override { delete[] _buffer; }

    void filter(float &raw) override
    {
        _buffer[_index] = raw;
        _index = (_index + 1) % _windowSize;

        float sum = 0.0f;
        for (uint8_t i = 0; i < _windowSize; i++)
            sum += _buffer[i];
        raw = sum / _windowSize;
    }
};

class TrimmedMovingAverage : public BaseFilter
{
private:
    float *_buffer;
    uint8_t _windowSize;
    uint8_t _trim;
    uint8_t _index = 0;

public:
    TrimmedMovingAverage(uint8_t windowSize = 20, uint8_t trim = 5)
        : _windowSize(windowSize), _trim(trim), _buffer(new float[_windowSize])
    {
        for (uint8_t i = 0; i < _windowSize; i++)
            _buffer[i] = 0;
    }
    ~TrimmedMovingAverage() override { delete[] _buffer; }

    void filter(float &raw) override
    {
        _buffer[_index] = raw;
        _index = (_index + 1) % _windowSize;
        for (int i = 0; i < _windowSize - 1; i++)
            for (int j = i + 1; j < _windowSize; j++)
                if (_buffer[i] > _buffer[j])
                {
                    int t = _buffer[i];
                    _buffer[i] = _buffer[j];
                    _buffer[j] = t;
                }

        double sum = 0;
        for (int i = _trim; i < _windowSize - _trim; i++)
            sum += _buffer[i];
        raw = sum / (_windowSize - 2 * _trim);
    }
};

#endif // MOVING_AVERAGE
