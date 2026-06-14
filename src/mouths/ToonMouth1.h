#pragma once

#include "Mouth.h"
#include "../DrawingUtils.h"
namespace stackchan::display
{
    class ToonMouth1 : public BaseMouth
    {
    public:
        using BaseMouth::BaseMouth;
        virtual void draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette) override;
    };
}
