#pragma once

#include "../FacialDrawable.h"
#include "Eye.h"
namespace stackchan::avatar
{

    // M5StackChan default face
    class Face : public FacialDrawable
    {
    protected:
    public:
        Face();
        Eye left_eye;
        Eye right_eye;

        virtual void draw(ExpressionWeight &expression_weight, ColorPalette &palette) override;
        // virtual void update() override;
    };
} // namespace stackchan::avatar