#pragma once

#include <M5Unified.h>
#include "components/Drawable.h"
#include "components/PrimaryFacialDrawable.h"
#include "Expression.h"
#include "ColorPalette.h"
// #include "Face.h"

// https://github.com/m5stack/StackChan/blob/main/firmware/main/stackchan/avatar/avatar/avatar.h

namespace stackchan::avatar
{
    class Avatar
    {
    protected:
        Avatar() = default;
        // Expression _emotion = Expression::Neutral;
        PrimaryFacialDrawables_t primary_components_; // facial elements including eyes, eyebrows, and mouth
        // ObjectPool<Decorator> _decorator_pool;

        // bool _is_modify_locked = false;
        // ColorPalette palette;
        // Face face;

    public:
        virtual void draw(M5Canvas *canvas, ExpressionWeight &expression_weight, ColorPalette &palette) {};
        /**
         * @brief Update avatar, trigger all elements, decorators and modifiers to update
         *
         */
        virtual void update() {};
    };
}
