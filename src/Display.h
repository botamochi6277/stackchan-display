#pragma once

#include <M5Unified.h>
#include "Drawable.h"
#include "Expression.h"
#include "ColorPalette.h"
#include "Decorator.h"
#include "SpeechBalloon.h"
#include "faces/default/Face.h"

// https://github.com/m5stack/StackChan/blob/main/firmware/main/stackchan/avatar/avatar/avatar.h

namespace stackchan::display
{
    class Display
    {
    protected:
        // bool _is_modify_locked = false;

        M5Canvas canvas_;
        Face face_;
        ExpressionWeight expression_weight_;
        ColorPalette color_palette_;
        SpeechBalloon speech_balloon_;

        void drawEmotionalDecorator(ExpressionWeight &expression_weight, ColorPalette &color_palette);

    public:
        Display() : canvas_(&M5.Lcd), face_(), expression_weight_(), color_palette_(), speech_balloon_() {};
        Face &getFace()
        {
            return face_;
        }

        ExpressionWeight &getExpressionWeight()
        {
            return expression_weight_;
        }

        SpeechBalloon &getSpeechBalloon()
        {
            return speech_balloon_;
        }

        ColorPalette &getColorPalette()
        {
            return color_palette_;
        }

        M5Canvas &getCanvas()
        {
            return canvas_;
        }

        // void setCanvas(M5Canvas &canvas)
        // {
        //     canvas_ = canvas;
        // }

        virtual void draw(ExpressionWeight &expression_weight, ColorPalette &palette)
        {
            canvas_.createSprite(320, 240); // canvas_.width() and canvas_.height() can be used if the canvas size is not fixed`
            face_.draw(canvas_, expression_weight_, color_palette_);
            // drawEmotionalDecorator(expression_weight_, color_palette_);
            // speech_balloon_.draw(canvas_, color_palette_);
        };
        /**
         * @brief Update avatar, trigger all elements, decorators and modifiers to update
         *
         */
        virtual void update()
        {
            face_.updateState(expression_weight_);
            this->draw(expression_weight_, color_palette_);
            canvas_.pushSprite(0, 0);
            canvas_.deleteSprite();
        };
    };

    void Display::drawEmotionalDecorator(ExpressionWeight &expression_weight, ColorPalette &color_palette)
    {
        if (expression_weight.get(Expression::kSleepy) > 128)
        {
            drawBubble(canvas_, 120, 120, 20, color_palette.get(DrawingLocation::kBalloonForeground));
        }
        if (expression_weight.get(Expression::kAngry) > 128)
        {
            drawAngerMark(canvas_, 120, 120, 20, color_palette.get(DrawingLocation::kBalloonForeground), color_palette.get(DrawingLocation::kBalloonBackground));
        }
        if (expression_weight.get(Expression::kUpset) > 128)
        {
            drawWaterDrop(canvas_, 120, 120, 20, color_palette.get(DrawingLocation::kBalloonForeground));
        }
        if (expression_weight.get(Expression::kSad) > 128)
        {
            drawPaleMark(canvas_, 120, 120, 20, color_palette.get(DrawingLocation::kBalloonForeground));
        }
        if (expression_weight.get(Expression::kHappy) > 128)
        {
            drawHeart(canvas_, 120, 120, 20, color_palette.get(DrawingLocation::kBalloonForeground));
        }
    }
}
