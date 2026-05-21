#include "Mouthes.h"

namespace stackchan::display
{

    void Mouth::draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette)
    {
        uint16_t mouth_color = canvas.getColorDepth() == 1
                                   ? 1
                                   : palette.get(DrawingLocation::kMouthBackground);

        int breath_offset = static_cast<int>(((expression_weight.get(Expression::kBreath) / 255.0f) - 0.5f) * 2.0f * breath_amplitude_);

        // float open_rate = expression_weight.get(Expression::kMouthOpen) / 100.0f;
        float open_rate = expression_weight.get(Expression::kAa) / 255.0f;

        int16_t max_width = size_.width;
        int16_t max_height = size_.height;
        int16_t min_height = max_height / 15;
        int16_t min_width = max_width / 2;

        int16_t height = min_height + (max_height - min_height) * open_rate;
        int16_t width = min_width + (max_width - min_width) * (1.0f - open_rate);

        m5::Vector2i top_left = position_ - m5::Vector2i(width / 2, height / 2 + breath_offset);
        canvas.fillRoundRect(top_left.x, top_left.y, width, height, 2, mouth_color);
    }
}
