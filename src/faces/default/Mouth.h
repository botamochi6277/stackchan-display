#pragma once

#include "../FacialDrawable.h"
namespace stackchan::avatar
{

    // M5StackChan default face
    class Mouth : public FacialDrawable
    {
    protected:
        uint8_t breath_amplitude_ = 5; // in pixels
    public:
        Mouth();
        Mouth(int16_t x, int16_t y, int16_t width, int16_t height);

        virtual void draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette) override;
        // virtual void update() override;
    };
} // namespace stackchan::avatar