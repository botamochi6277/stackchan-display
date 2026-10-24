#pragma once

#include <M5Unified.h>
#include "Drawable.h"
#include "Expression.h"
#include "ColorPalette.h"
#include "Decorator.h"
#include "SpeechBalloon.h"
#include "faces/Faces.h"

// https://github.com/m5stack/StackChan/blob/main/firmware/main/stackchan/avatar/avatar/avatar.h

namespace stackchan::display
{
    class Display
    {
    protected:
        // bool _is_modify_locked = false;

        M5Canvas canvas_;
        Face *face_;
        ExpressionWeight expression_weight_;
        ColorPalette *color_palette_;
        SpeechBalloon speech_balloon_;

        /**
         * @brief Draw the mark of the current emotion (bubble, anger mark, water drop, pale mark, heart)
         *
         * @param expression_weight weights to choose the marks
         * @param color_palette colors of the marks
         * @param x center x of the mark in the canvas [px]
         * @param y center y of the mark in the canvas [px]
         */
        void drawEmotionalDecorator(ExpressionWeight &expression_weight, ColorPalette &color_palette,
                                    int x = 320 - 40, int y = 40);

        /**
         * @brief Draw the cheek blush on both cheeks while `Expression::kBlush` or `Expression::kSmile` is strong
         *
         * The cheeks are placed between the eyes and the mouth of the current face, under each eye.
         * The color is `DrawingLocation::kCheek1` of the palette (pink if not set).
         *
         * @param expression_weight weights to decide whether to draw
         * @param color_palette colors of the blush
         */
        void drawCheekDecorator(ExpressionWeight &expression_weight, ColorPalette &color_palette)
        {
            // the blush appears automatically with a smile, or on demand with kBlush
            if (expression_weight.get(Expression::kBlush) <= 128 && expression_weight.get(Expression::kSmile) <= 128)
            {
                return;
            }

            uint16_t color = canvas_.getColorDepth() == 1
                                 ? 1
                                 : (color_palette.contains(DrawingLocation::kCheek1) ? color_palette.get(DrawingLocation::kCheek1) : TFT_PINK);

            BaseEye *eyes[2] = {face_->getLeftEye(), face_->getRightEye()};
            int mouth_y = face_->getMouth()->getPosition().y;
            for (auto eye : eyes)
            {
                int eye_bottom = eye->getPosition().y + eye->getSize().height / 2;
                int y = (eye_bottom + mouth_y) / 2;
                drawBlush(canvas_, eye->getPosition().x, y, eye->getSize().width * 0.6f, color);
            }
        }

    public:
        Display() : canvas_(&M5.Lcd), expression_weight_(), speech_balloon_()
        {
            face_ = new Face();
            color_palette_ = new ColorPalette();
        };
        Face *getFace()
        {
            return face_;
        }
        void setFace(Face *face)
        {
            face_ = face;
        }

        ExpressionWeight &getExpressionWeight()
        {
            return expression_weight_;
        }

        SpeechBalloon &getSpeechBalloon()
        {
            return speech_balloon_;
        }

        ColorPalette *getColorPalette()
        {
            return color_palette_;
        }
        void setColorPalette(ColorPalette *color_palette)
        {
            color_palette_ = color_palette;
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
            face_->draw(canvas_, expression_weight_, *color_palette_);
            drawEmotionalDecorator(expression_weight_, *color_palette_,
                                   canvas_.width() - 40, 40 + face_->getHairShadowHeight()); // draw the mark of the current emotion at the top-right corner
            drawCheekDecorator(expression_weight_, *color_palette_);
            speech_balloon_.draw(canvas_, *color_palette_);
        };
        /**
         * @brief Update avatar, trigger all elements, decorators and modifiers to update
         *
         */
        virtual void update()
        {
            face_->updateState(expression_weight_);
            this->draw(expression_weight_, *color_palette_);
            canvas_.pushSprite(0, 0);
            canvas_.deleteSprite();
        };
    };

    void Display::drawEmotionalDecorator(ExpressionWeight &expression_weight, ColorPalette &color_palette, int x, int y)
    {
        // FIXME: this is a temporary implementation for testing, should be refactored to use Decorator pattern and support more decorators and emotions
        uint16_t color = canvas_.getColorDepth() == 1
                             ? 1
                             : color_palette_->get(DrawingLocation::kBalloonBackground);

        // canvas_.fillCircle(x, y, 40, TFT_GREEN);
        if (expression_weight.get(Expression::kSleepy) > 128)
        {
            drawBubble(canvas_, x, y, 40, color);
        }
        if (expression_weight.get(Expression::kAngry) > 128)
        {
            drawAngerMark(canvas_, x, y, 40, color);
        }
        if (expression_weight.get(Expression::kUpset) > 128)
        {
            drawWaterDrop(canvas_, x, y, 20, color);
        }
        if (expression_weight.get(Expression::kSad) > 128)
        {
            drawPaleMark(canvas_, x, y, 40, color);
        }
        if (expression_weight.get(Expression::kSmile) > 128)
        {
            drawHeart(canvas_, x, y, 40, color);
        }
    }
}
