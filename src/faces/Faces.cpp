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
    Face::Face() : Face(new Eye(160 + 70, 95, 18, 18, true),
                        new Eye(160 - 70, 95, 18, 18, false),
                        new Mouth(160, 148, 90, 60)) {}

    Face::Face(BaseEye *left_eye,
               BaseEye *right_eye,
               BaseMouth *mouth) : left_eye_(left_eye), right_eye_(right_eye), mouth_(mouth) {}

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
        uint16_t skin_color = canvas.getColorDepth() == 1
                                  ? 0
                                  : palette.get(DrawingLocation::kSkin);
        canvas.fillSprite(skin_color);
        left_eye_->draw(canvas, expression_weight, palette);
        right_eye_->draw(canvas, expression_weight, palette);
        mouth_->draw(canvas, expression_weight, palette);
    }

    void Face::autoScale()
    {
        position_ = {M5.Lcd.width() / 2, M5.Lcd.height() / 2};
        size_ = {M5.Lcd.width(), M5.Lcd.height()};

        m5::Vector2i original_center = {160, 120}; // center position of M5 Core Controllers

        int16_t base_length = size_.min();
        float scale = base_length / 240.0f;
        m5::Vector2i offset = {0, 0};

        // left eye
        offset = left_eye_->getPosition() - original_center;
        left_eye_->setPosition(
            position_ + offset * scale);
        left_eye_->setSize(left_eye_->getSize() * scale);
        // right eye
        offset = right_eye_->getPosition() - original_center;
        right_eye_->setPosition(
            position_ + offset * scale);
        right_eye_->setSize(right_eye_->getSize() * scale);
        // mouth
        offset = mouth_->getPosition() - original_center;
        mouth_->setPosition(
            position_ + offset * scale);
        mouth_->setSize(mouth_->getSize() * scale);
    }
}
