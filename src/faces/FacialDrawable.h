#pragma once

#include <M5Unified.h>
#include "Drawable.h"
#include "Expression.h"
#include "ColorPalette.h"

namespace stackchan::avatar
{
    class FacialDrawable : public Drawable
    {
    public:
        virtual void draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette) {};
    };
}