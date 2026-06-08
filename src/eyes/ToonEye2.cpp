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
        if (palette.contains(DrawingLocation::kEyelid))
        {
            eyelash_color = palette.get(DrawingLocation::kEyelid);
        }
        // uint16_t skin_color = palette.contains(DrawingLocation::kSkin) ? palette.get(DrawingLocation::kSkin) : TFT_GREEN;

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
        updateQuadrilateralEyelidPoints(this->iris_position_, this->size_, this->is_left_, expression_weight, p1, p2, p3, p4, 0.25f);

        float eyelash_x0, eyelash_y0, eyelash_x1, eyelash_y1, eyelash_x2, eyelash_y2;
        eyelash_x0 = this->is_left_ ? this->iris_position_.x + 22 : this->iris_position_.x - 22;
        eyelash_y0 = upper_eyelid_y - 27;
        eyelash_x1 = this->is_left_ ? this->iris_position_.x + 26 : this->iris_position_.x - 26;
        eyelash_y1 = upper_eyelid_y;
        eyelash_x2 = this->is_left_ ? this->iris_position_.x - 10 : this->iris_position_.x + 10;
        eyelash_y2 = upper_eyelid_y;

        float tilt = 0.0f;
        float ref_tilt = open_ratio * M_PI / 12.0f;
        float bias;
        if (expression_weight.get(Expression::kAngry) > 12)
        {
            tilt = this->is_left_ ? -ref_tilt : ref_tilt;
        }
        else if (expression_weight.get(Expression::kSad) > 12)
        {
            tilt = this->is_left_ ? ref_tilt : -ref_tilt;
        }

        bias = 0.1f * size_.width * tilt / (M_PI / 6.0f);

        if ((open_ratio < 0.99f) || (abs(tilt) > 0.1f))
        {
            float mask_top_left_x = this->iris_position_.x - (this->size_.width / 2);
            float mask_top_left_y = this->iris_position_.y - 0.75f * this->size_.height;
            float mask_bottom_right_x = this->iris_position_.x + (this->size_.width / 2);
            float mask_bottom_right_y = upper_eyelid_y - 1;

            // m5::fillRectRotatedAround(canvas, mask_top_left_x, mask_top_left_y,
            //                           mask_bottom_right_x, mask_bottom_right_y, tilt,
            //                           iris_position_.x, upper_eyelid_y, skin_color);

            // eyelid
            float eyelid_top_left_x = iris_position_.x - (this->size_.width / 2) + bias;
            float eyelid_top_left_y = upper_eyelid_y - 4;
            float eyelid_bottom_right_x = iris_position_.x + (this->size_.width / 2) + bias;
            float eyelid_bottom_right_y = upper_eyelid_y;

            // debug
            canvas.fillCircle(p1.x, p1.y, 2, M5.Lcd.color565(255, 0, 0));
            canvas.fillCircle(p2.x, p2.y, 2, M5.Lcd.color565(255, 0, 0));
            canvas.fillCircle(p3.x, p3.y, 2, M5.Lcd.color565(255, 0, 0));
            canvas.fillCircle(p4.x, p4.y, 2, M5.Lcd.color565(255, 0, 0));

            // m5::fillRectRotatedAround(canvas, eyelid_top_left_x, eyelid_top_left_y,
            //                           eyelid_bottom_right_x, eyelid_bottom_right_y, tilt,
            //                           iris_position_.x, upper_eyelid_y, eyelid_color);

            canvas.fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, skin_color); // draw eyelid
            canvas.fillTriangle(p3.x, p3.y, p4.x, p4.y, p1.x, p1.y, skin_color);

            // eyelash base
            canvas.fillTriangle(p2.x, p2.y - eyelash_base_thickness, p2.x, p2.y, p3.x, p3.y, eyelash_color);
            canvas.fillTriangle(p3.x, p3.y, p3.x, p3.y - eyelash_base_thickness, p2.x, p2.y, eyelash_color);

            // eyelash
            eyelash_x0 += bias;
            eyelash_x1 += bias;
            eyelash_x2 += bias;
        }

        // eyelash

        m5::rotatePointAround(eyelash_x0, eyelash_y0, tilt, iris_position_.x, upper_eyelid_y);
        m5::rotatePointAround(eyelash_x1, eyelash_y1, tilt, iris_position_.x, upper_eyelid_y);
        m5::rotatePointAround(eyelash_x2, eyelash_y2, tilt, iris_position_.x, upper_eyelid_y);
        canvas.fillTriangle(eyelash_x0, eyelash_y0, eyelash_x1, eyelash_y1,
                            eyelash_x2, eyelash_y2, eyelash_color);
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

        // FIXME: move this logic to drawEyelid and drawEyelash
        if ((open_ratio > 0.9f))
        {
            // draw only eyelash

            auto eyelash_color = palette.get(DrawingLocation::kEyelash);
            auto eyelash_tip_x = is_left_ ? iris_position_.x + iris_w / 2 : iris_position_.x - iris_w / 2;
            auto eyelash_tip_y = iris_position_.y - iris_h / 2;
            auto eyelash_med_x = is_left_ ? iris_position_.x : iris_position_.x;
            auto eyelash_med_y = iris_position_.y - iris_h / 4;
            auto eyelash_btm_x = is_left_ ? iris_position_.x + iris_w / 3 : iris_position_.x - iris_w / 3;
            auto eyelash_btm_y = iris_position_.y - iris_h / 4;

            canvas.fillTriangle(eyelash_tip_x, eyelash_tip_y, eyelash_med_x,
                                eyelash_med_y, eyelash_btm_x, eyelash_btm_y,
                                eyelash_color);
        }

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
