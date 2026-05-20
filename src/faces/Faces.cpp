#include "Faces.h"

#ifndef rand_r
#define init_rand() srand(seed)
#define _rand() rand()
#else
#define init_rand() ;
#define _rand() rand_r(&seed)
#endif

namespace stackchan::display
{
    Face::Face() : left_eye(160 + 70, 95, 9, true),
                   right_eye(160 - 70, 95, 9, false),
                   mouth(160, 148, 90, 60) {}

    void Face::updateState(ExpressionWeight &expression_weight)
    {
        static unsigned long mill_sec = 0;
        static unsigned long last_saccade_millis = 0;
        static unsigned long last_blink_millis = 0;
        static uint32_t saccade_interval = 1000; // [msec]
        static uint32_t blink_interval = 1000;

        mill_sec = M5.millis();

        // blink
        if (is_auto_blink_ && (mill_sec - last_blink_millis > blink_interval))
        {
            if (expression_weight.get(Expression::kBlink) > 127)
            {
                expression_weight.set(Expression::kBlink, 0);        // weight to open eye
                blink_interval = auto_blink_interval_ + rand() % 20; // add random to avoid fixed interval
            }
            else
            {
                expression_weight.set(Expression::kBlink, 255); // weight to close eye
                blink_interval = blink_duration_;
            }
            last_blink_millis = mill_sec;
        }

        // breath
        float breath = sinf(breath_frequency_ * 2.0f * M_PI * (mill_sec / 1000.0f));
        expression_weight.set(Expression::kBreath, static_cast<unsigned char>((breath + 1.0f) / 2.0f * 255.0f));
    }

    void Face::draw(
        M5Canvas &canvas,
        ExpressionWeight &expression_weight,
        ColorPalette &palette)
    {
        unsigned int skin_color = palette.get(DrawingLocation::kSkin);
        canvas.fillSprite(skin_color);
        this->left_eye.draw(canvas, expression_weight, palette);
        this->right_eye.draw(canvas, expression_weight, palette);
        this->mouth.draw(canvas, expression_weight, palette);
    }
}