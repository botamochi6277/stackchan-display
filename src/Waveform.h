#pragma once
#include <cmath>
namespace stackchan::waveform
{
    /**
     * @brief Computes the hard sigmoid function
     *
     * @param x
     * @return float
     */
    float hardSigmoid(float x)
    {
        if (x < -0.5f)
        {
            return 0.0f;
        }
        else if (x < 0.5f)
        {
            return x + 0.5f;
        }
        else
        {
            return 1.0f;
        }
    }

    float hardSigmoidInOut(float x, float duration = 1.0f, float scale = 1.0f)
    {
        return scale * hardSigmoid((x - 0.5f) / (duration + 1e-9f));
    }

    float trianglePulse(float x, float duration = 1.0f, float scale = 1.0f)
    {
        return scale * (hardSigmoid((2.0f * (x + 0.5f)) / (duration + 1.0e-9f)) -
                        hardSigmoid((2.0f * (x - 0.5f)) / (duration + 1.0e-9f)));
    }

    float quadInOut(float x, float duration = 1.0f)
    {
        float xx = x / (duration + 1e-9f);
        if (x < 0.0f)
        {
            return 0.0f;
        }
        else if (x < 0.5f * duration)
        {
            return 2.0f * xx * xx;
        }
        else if (xx < duration)
        {
            return (1.0f - std::pow(-2.0f * xx + 2.0f, 2.0f) / 2.0f);
        }
        else
        {
            return 1.0f;
        }
    }

    float quadPulse(float x, float duration = 1.0f, float scale = 1.0f)
    {
        return scale * (quadInOut(x, duration / 2.0f) -
                        quadInOut(x - duration / 2.0f, duration / 2.0f));
    }
}
