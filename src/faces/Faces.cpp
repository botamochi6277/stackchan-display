#include "Faces.h"
#include "Waveform.h"

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
        static uint32_t blink_close_duration = 200; // [msec]
        static unsigned int next_blink_millis = blink_interval;
        static float blink_signal;

        mill_sec = M5.millis();

        blink_signal = waveform::trianglePulse((next_blink_millis / 1000.0f) - (mill_sec / 1000.0f),
                                               blink_close_duration / 1000.0f);

        // blink
        if (is_auto_blink_)
        {
            expression_weight.set(Expression::kBlink, static_cast<unsigned char>(blink_signal * 255.0f));
        }

        if (mill_sec > next_blink_millis + blink_interval)
        {
            blink_interval = auto_blink_interval_ + _rand() % 200; // add random to avoid fixed interval
            last_blink_millis = mill_sec;
            next_blink_millis = mill_sec + blink_interval;
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
        this->drawBackground(canvas, expression_weight, palette);
        left_eye_->draw(canvas, expression_weight, palette);
        right_eye_->draw(canvas, expression_weight, palette);
        mouth_->draw(canvas, expression_weight, palette);

        // bar plot for debugging the blink weight
        // auto weight = expression_weight.get(stackchan::display::Expression::kBlink);
        // canvas.fillRect(0, 40, weight + 8, 10, TFT_GREEN);
    }

    void Face::drawBackground(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette)
    {
        if (hair_shadow_height_ == 0 || canvas.getColorDepth() == 1)
        {
            return; // no shadow, or no gradient on a monochrome canvas
        }

        uint16_t skin_color = palette.get(DrawingLocation::kSkin);
        uint16_t shadow_color;
        if (palette.contains(DrawingLocation::kHairShadow))
        {
            shadow_color = palette.get(DrawingLocation::kHairShadow);
        }
        else
        {
            // darken the skin color for the shadow
            uint8_t r = (skin_color >> 11) & 0x1F;
            uint8_t g = (skin_color >> 5) & 0x3F;
            uint8_t b = skin_color & 0x1F;

            r = static_cast<uint8_t>(r * 0.2f);
            g = static_cast<uint8_t>(g * 0.2f);
            b = static_cast<uint8_t>(b * 0.2f);

            shadow_color = (r << 11) | (g << 5) | b;
        }

        // fillGradientRect is too rough for the hair shadow. maybe  it is 5 steps
        // so implement the shadow with a simple rectangle
        canvas.fillRect(0, 0, canvas.width(), hair_shadow_height_, shadow_color);

        // canvas.fillGradientRect(
        //     0, 0, canvas.width(), hair_shadow_height_, shadow_color, skin_color,
        //     lgfx::fill_style_t::vertical_linear);
    }

    void Face::autoScale()
    {
        position_ = {M5.Lcd.width() / 2, M5.Lcd.height() / 2};
        size_ = {M5.Lcd.width(), M5.Lcd.height()};

        m5::Vector2i original_center = {160, 120}; // center position of M5 Core Controllers

        int16_t base_length = size_.min();
        float scale = base_length / 240.0f;
        m5::Vector2i offset = {0, 0};
        hair_shadow_height_ = hair_shadow_height_ * scale;

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
