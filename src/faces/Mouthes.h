#pragma once

#include "FacialDrawable.h"
namespace stackchan::display
{
    class BaseMouth : public FacialDrawable
    {
    protected:
        uint8_t breath_amplitude_ = 5; // in pixels

    public:
        using FacialDrawable::FacialDrawable;
    };

    // M5StackChan default face
    class Mouth : public BaseMouth
    {
    protected:
    public:
        using BaseMouth::BaseMouth;

        virtual void draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette) override;
        // virtual void update() override;
    };
} // namespace stackchan::display
