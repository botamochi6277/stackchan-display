#include "MonoEye.h"

namespace stackchan::display
{
    void MonoEye::drawSmile(M5Canvas &canvas, uint16_t radius, uint16_t color)
    {
        // ^ shaped arc
        float x = iris_position_.x;
        float y = iris_position_.y;
        uint8_t thickness = m5::max<int>(2, radius / 6);
        m5::fillArc(canvas,
                    x - radius * 0.7f, y + radius * 0.3f,
                    x + radius * 0.7f, y + radius * 0.3f,
                    x, y - radius * 0.3f,
                    thickness, color);
    }

    void MonoEye::draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette)
    {
        bool mono = canvas.getColorDepth() == 1;
        uint16_t light_color = mono ? 1 : palette.get(DrawingLocation::kIris1);
        uint16_t skin_color = mono ? 0 : palette.get(DrawingLocation::kSkin);
        uint16_t dark_color = (!mono && palette.contains(DrawingLocation::kPupil))
                                  ? palette.get(DrawingLocation::kPupil)
                                  : skin_color;

        milli_sec_ = M5.millis();
        this->updateSaccade(expression_weight);
        this->updateGaze(expression_weight, size_.width / 8);
        iris_position_ = position_ + gaze_ + saccade_noise_;

        int16_t cx = iris_position_.x;
        int16_t cy = iris_position_.y;
        int16_t radius = size_.width / 2;

        if (expression_weight.get(Expression::kSmile) > 127)
        {
            drawSmile(canvas, radius, light_color);
            return;
        }

        float open_ratio = calculateOpenRatio(expression_weight);
        if (expression_weight.get(Expression::kSleepy) > 127)
        {
            open_ratio = 0.1f;
        }

        if (open_ratio <= 0.1f)
        {
            // closed: a thin line
            canvas.fillRect(cx - radius, cy - 2, radius * 2, 4, light_color);
            return;
        }

        bool surprised = expression_weight.get(Expression::kSurprised) > 127;
        if (surprised)
        {
            radius = radius * 115 / 100;
        }

        // visible band, symmetric around the center line
        int16_t band_top = cy - radius * open_ratio;
        int16_t band_bottom = cy + radius * open_ratio;

        // grin raises the lower edge
        float grin = expression_weight.get(Expression::kGrin) / 255.0f;
        band_bottom -= static_cast<int16_t>(grin * 0.4f * radius);

        canvas.setClipRect(cx - radius - 1, band_top, radius * 2 + 3, band_bottom - band_top + 1);

        // rings
        canvas.fillCircle(cx, cy, radius, light_color);
        canvas.fillCircle(cx, cy, radius * 78 / 100, dark_color);
        canvas.fillCircle(cx, cy, radius * 56 / 100, light_color);
        canvas.fillCircle(cx, cy, radius * (surprised ? 18 : 30) / 100, dark_color);
        if (!mono && palette.contains(DrawingLocation::kEyeHighlight))
        {
            canvas.fillCircle(cx - radius / 12, cy - radius / 12, m5::max<int>(1, radius / 14),
                              palette.get(DrawingLocation::kEyeHighlight));
        }

        canvas.clearClipRect();

        // upper edge of the lid, symmetric about the vertical center line:
        // angry lowers the center (V shape), sad lowers both ends (inverted V shape)
        float angry = expression_weight.get(Expression::kAngry) / 255.0f;
        float sad = expression_weight.get(Expression::kSad) / 255.0f;
        if (angry > 0.0f || sad > 0.0f)
        {
            int16_t left = cx - radius - 1;
            int16_t right = cx + radius + 1;
            int16_t top = cy - radius - 2;
            int16_t drop_center = static_cast<int16_t>(angry * 0.7f * radius);
            int16_t drop_ends = static_cast<int16_t>(sad * 0.7f * radius);
            // the edge runs through (left, edge_end), (cx, edge_center) and (right, edge_end)
            int16_t edge_end = top + 2 + drop_ends;
            int16_t edge_center = top + 2 + drop_center;
            // fill the area above the edge with the skin color: one quadrangle (two triangles) for each half
            canvas.fillTriangle(left, top, cx, top, left, edge_end, skin_color);
            canvas.fillTriangle(cx, top, cx, edge_center, left, edge_end, skin_color);
            canvas.fillTriangle(cx, top, right, top, right, edge_end, skin_color);
            canvas.fillTriangle(cx, top, cx, edge_center, right, edge_end, skin_color);
        }
    }
}
