#pragma once

#include <M5Unified.h>
#include "Drawable.h"
#include "Expression.h"
#include "ColorPalette.h"

namespace stackchan::display
{
    class FacialDrawable : public Drawable
    {
    public:
        using Drawable::Drawable;
        virtual void draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette) {};
    };
}
