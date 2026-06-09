#include "ToonEye2.h"

namespace stackchan::display
{
    void ToonEye2::drawEyelid(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette)
    {
        // required color paring
        uint16_t eyelash_color = palette.contains(DrawingLocation::kEyelash) ? palette.get(DrawingLocation::kEyelash) : TFT_MAGENTA;
        uint16_t skin_color = palette.contains(DrawingLocation::kSkin) ? palette.get(DrawingLocation::kSkin) : TFT_MAGENTA;
        // TODO: draw eyeshadow using eyelash color if eyelid color is not set
        // eyelid color is skin color by default, but can be overridden by eyelash color in palette, i.e eyeshadow color
        uint16_t eyelid_color = palette.contains(DrawingLocation::kSkin) ? palette.get(DrawingLocation::kSkin) : TFT_MAGENTA;

        // update state
        auto open_ratio = this->calculateOpenRatio(expression_weight);

        // rect eyelid
        auto upper_eyelid_y =
            this->iris_position_.y - 0.8f * size_.height / 2 + (1.0f - open_ratio) * size_.height * 0.6;

        uint16_t eyelash_base_thickness = 4;
        // eyelid mask convexes
        // right    left
        // p1--p4   p4--p1
        // |   |    |   |
        // p2--p3   p3--p2
        m5::Vector2i p1, p2, p3, p4;
        updateQuadrilateralEyelidPoints(this->iris_position_, this->size_, open_ratio, this->is_left_, expression_weight, p1, p2, p3, p4, 0.25f);

        // right    left
        // p1--p4       tip
        // | \ |      / |
        // p2--p3   med--lat
        m5::Vector2i eyelash_tip, eyelash_lat, eyelash_med; // tip, lateral, medial points for eyelash
        eyelash_tip.x = this->is_left_ ? this->iris_position_.x + (22 / 60.0f) * size_.width : this->iris_position_.x - (22 / 60.0f) * size_.width;
        eyelash_tip.y = p3.y - (27 / 84.0f) * size_.height;

        eyelash_lat.x = this->is_left_ ? this->iris_position_.x + (26 / 60.0f) * size_.width : this->iris_position_.x - (26 / 60.0f) * size_.width;
        eyelash_lat.y = p3.y;

        eyelash_med.x = this->is_left_ ? this->iris_position_.x - (10 / 60.0f) * size_.width : this->iris_position_.x + (10 / 60.0f) * size_.width;
        eyelash_med.y = p3.y;

        float grad = p2.x - p3.x != 0 ? (float)(p2.y - p3.y) / (p2.x - p3.x) : 0;

        m5::Vector2i ref_p = {p3.x, p2.y < p3.y ? p2.y : p3.y};
        m5::shearPointAround(eyelash_tip, 0.0f, grad, p3);
        m5::shearPointAround(eyelash_lat, 0.0f, grad, p3);
        m5::shearPointAround(eyelash_med, 0.0f, grad, p3);

        if ((open_ratio >= 0.9f) && (abs(grad) < 0.1f))
        {
            uint16_t iris_w = size_.width;
            uint16_t iris_h = size_.height;
            m5::Size2i iris_r = {iris_w / 2, iris_h / 2};

            float pi = M_PI;
            m5::computePointOnEllipse(eyelash_med, iris_position_, iris_r, this->is_left_ ? -pi / 4.0f : -pi + pi / 4.0f);
            m5::computePointOnEllipse(eyelash_lat, iris_position_, iris_r, this->is_left_ ? -pi / 4.0f - 0.2f : -pi + pi / 4.0f + 0.2f);

            eyelash_tip.x = is_left_ ? iris_position_.x + iris_w / 2 : iris_position_.x - iris_w / 2;
            eyelash_tip.y = iris_position_.y - iris_h / 2;

            // debug
            // canvas.fillCircle(p1.x, p1.y, 2, M5.Lcd.color565(0, 255, 0));
            // canvas.fillCircle(p2.x, p2.y, 2, M5.Lcd.color565(0, 255, 0));
            // canvas.fillCircle(p3.x, p3.y, 2, M5.Lcd.color565(0, 255, 0));
            // canvas.fillCircle(p4.x, p4.y, 2, M5.Lcd.color565(0, 255, 0));
        }
        else
        {
            float mask_top_left_x = this->iris_position_.x - (this->size_.width / 2);
            float mask_top_left_y = this->iris_position_.y - 0.75f * this->size_.height;
            float mask_bottom_right_x = this->iris_position_.x + (this->size_.width / 2);
            float mask_bottom_right_y = upper_eyelid_y - 1;

            // debug
            // canvas.fillCircle(p1.x, p1.y, 2, M5.Lcd.color565(255, 0, 0));
            // canvas.fillCircle(p2.x, p2.y, 2, M5.Lcd.color565(255, 0, 0));
            // canvas.fillCircle(p3.x, p3.y, 2, M5.Lcd.color565(255, 0, 0));
            // canvas.fillCircle(p4.x, p4.y, 2, M5.Lcd.color565(255, 0, 0));

            canvas.fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, skin_color); // draw eyelid
            canvas.fillTriangle(p3.x, p3.y, p4.x, p4.y, p1.x, p1.y, skin_color);

            // eyelash base
            canvas.fillTriangle(p2.x, p2.y - eyelash_base_thickness, p2.x, p2.y, p3.x, p3.y, eyelash_color);
            canvas.fillTriangle(p3.x, p3.y, p3.x, p3.y - eyelash_base_thickness, p2.x, p2.y, eyelash_color);
        }

        // eyelash
        canvas.fillTriangle(eyelash_tip.x, eyelash_tip.y, eyelash_lat.x, eyelash_lat.y,
                            eyelash_med.x, eyelash_med.y, eyelash_color);
    }

    void ToonEye2::draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette)
    {
        milli_sec_ = M5.millis();
        auto open_ratio = this->calculateOpenRatio(expression_weight);
        this->updateSaccade(expression_weight);
        this->updateGaze(expression_weight, this->size_.height / 8);
        iris_position_ = position_ + gaze_ + saccade_noise_;
        auto wink_base_y = iris_position_.y + (1.0f - open_ratio + 0.2f) * this->size_.height / 4;

        uint16_t iris_w = size_.width;
        uint16_t iris_h = size_.height;
        uint32_t thickness = 2;

        // main eye
        if (open_ratio > 0.1f)
        {
            // iris bg
            uint16_t iris_bg_color = palette.get(DrawingLocation::kIrisBackground);
            canvas.fillEllipse(iris_position_.x, iris_position_.y, iris_w / 2, iris_h / 2,
                               iris_bg_color);

            if (palette.contains(DrawingLocation::kIris1))
            {
                auto iris_color_1 = palette.get(DrawingLocation::kIris1);
                canvas.fillEllipse(iris_position_.x, iris_position_.y, iris_w / 2 - thickness,
                                   iris_h / 2 - thickness, iris_color_1);
            }

            if (palette.contains(DrawingLocation::kIris2))
            {
                auto iris_color_2 = palette.get(DrawingLocation::kIris2);
                // center arc line
                m5::fillArc(canvas, // canvas
                            iris_position_.x - iris_w / 2 + thickness + 2, iris_position_.y + iris_h / 4,
                            iris_position_.x + iris_w / 2 - thickness - 2, iris_position_.y + iris_h / 4,
                            iris_position_.x + thickness, iris_position_.y + iris_h / 8,
                            1, // thickness
                            iris_color_2);

                // lower half moon will be filled
                canvas.floodFill(iris_position_.x, iris_position_.y + iris_h / 8 + 2, iris_color_2);
            }
        }

        this->drawEyelid(canvas, expression_weight, palette);
    }

}
