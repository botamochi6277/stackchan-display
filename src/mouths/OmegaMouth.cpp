#include "OmegaMouth.h"

namespace stackchan::display
{
    void OmegaMouth::draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette)
    {
        uint16_t bg_color = canvas.getColorDepth() == 1
                                ? 1
                                : palette.get(DrawingLocation::kMouthBackground);
        uint16_t inner_color = canvas.getColorDepth() == 1
                                   ? 1
                                   : palette.get(DrawingLocation::kInnerMouth);
        uint16_t skin_color = canvas.getColorDepth() == 1
                                  ? 0
                                  : palette.get(DrawingLocation::kSkin);
        int breath_offset = static_cast<int>(((expression_weight.get(Expression::kBreath) / 255.0f) - 0.5f) * 2.0f * breath_amplitude_);

        // float open_ratio = expression_weight.get(Expression::kMouthOpen) / 100.0f;
        float open_ratio = expression_weight.get(Expression::kAa) / 255.0f;

        uint8_t outline_thickness = 2;
        auto h = static_cast<int16_t>((size_.height / 2) * open_ratio);

        if (open_ratio > 0.01f)
        {
            // M5_LOGD("open ratio %0.2f", open_ratio);
            // inner mouth background

            canvas.fillEllipse(position_.x, position_.y - size_.width / 12 + breath_offset, size_.width / 4,
                               h, bg_color);
            if (palette.contains(DrawingLocation::kInnerMouth) && (h > outline_thickness * 2))
            {
                // i.e. (h-outline_thickness > 0)
                canvas.fillEllipse(position_.x, position_.y - size_.width / 12 + breath_offset,
                                   size_.width / 4 - 4, h - outline_thickness,
                                   inner_color);
            }
        }

        // omega
        uint16_t overlap = 4;
        m5::Vector2i offset = {size_.width / 4 - overlap, size_.height / 2};
        m5::Size2i s = {size_.width / 2 + overlap, size_.height};
        canvas.fillEllipse(position_.x - offset.x, position_.y - offset.y + breath_offset, s.width / 2, s.height / 2,
                           bg_color); // outer
        canvas.fillEllipse(position_.x + offset.x, position_.y - offset.y + breath_offset, s.width / 2, s.height / 2,
                           bg_color);

        canvas.fillEllipse(position_.x - offset.x, position_.y - offset.y + breath_offset, s.width / 2 - outline_thickness, s.height / 2 - outline_thickness,
                           skin_color); // inner
        canvas.fillEllipse(position_.x + offset.x, position_.y - offset.y + breath_offset, s.width / 2 - outline_thickness, s.height / 2 - outline_thickness,
                           skin_color);

        // rect mask for omega
        canvas.fillRect(position_.x - size_.width / 2, position_.y - offset.y + breath_offset - s.height / 2,
                        size_.width, size_.height / 2, skin_color);

        // // cheek
        // if (palette.contains(DrawingLocation::kCheek1))
        // {
        //     auto cheek_color = palette.get(DrawingLocation::kCheek1);
        //     canvas.fillEllipse(position_.x - 132, position_.y - 23, 24, 10, cheek_color);
        //     canvas.fillEllipse(position_.x + 132, position_.y - 23, 24, 10, cheek_color);
        // }
    }
}
