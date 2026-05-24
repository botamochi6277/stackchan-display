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

    class OmegaMouth : public BaseMouth
    {
        // https://github.com/botamochi6277/m5stack-avatar/blob/b2c368c92a05d5a6b405e554c7450dba6db3247d/src/Mouths.cpp#L40

    public:
        using BaseMouth::BaseMouth;
        virtual void draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette) override;
    };
} // namespace stackchan::display
