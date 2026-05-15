#include "Mouth.h"

namespace stackchan::avatar
{

    Mouth::Mouth()
    {
        position_ = {160, 148};
        size_ = {100, 100};
    }
    Mouth::Mouth(int16_t x, int16_t y, int16_t width, int16_t height)
    {
        position_ = {x, y};
        size_ = {width, height};
    }

    void Mouth::draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette)
    {
        uint16_t mouth_color = palette.get(DrawingLocation::kMouthBackground);
        // float open_rate = expression_weight.get(Expression::kMouthOpen) / 100.0f;
        float open_rate = 0.0f; // TODO: use actual expression weight

        int16_t max_width = size_.width;
        int16_t max_height = size_.height;
        int16_t min_height = max_height / 8;
        int16_t min_width = max_width / 4;

        int16_t height = min_height + (max_height - min_height) * open_rate;
        int16_t width = min_width + (max_width - min_width) * (1.0f - open_rate);

        m5::Vector2i top_left = position_ - m5::Vector2i(width / 2, height / 2);
        canvas.fillRect(top_left.x, top_left.y, width, height, mouth_color);
    }
}