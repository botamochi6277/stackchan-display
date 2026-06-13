#include "ToonEye1.h"

namespace stackchan::display
{

    void ToonEye1::computeEyelidBaseWaypoints(m5::Vector2i &medial,
                                              m5::Vector2i &center,
                                              m5::Vector2i &lateral,
                                              uint16_t eyelid_width, uint16_t eyelid_height,
                                              uint16_t eyelid_bottom)
    {
        // calculate base waypoints for eyelid and eyeliner drawing
        // the eyelid will be drawn as a arc from medial to lateral with center as the peak
        // ----|-----
        //     O

        // inner(medial)
        medial.x = this->is_left_ ? this->position_.x - 0.4f * eyelid_width
                                  : this->position_.x + 0.4f * eyelid_width;
        medial.y = eyelid_bottom;

        // outer(lateral)
        lateral.x = this->is_left_ ? this->position_.x + 0.6f * eyelid_width
                                   : this->position_.x - 0.6f * eyelid_width;
        lateral.y = eyelid_bottom;

        // peak
        center.x = this->position_.x;
        center.y = eyelid_bottom - eyelid_height;
    }

    void ToonEye1::computeEyelashBaseWaypoints(
        m5::Vector2i &tip, m5::Vector2i &bottom, m5::Vector2i &medial,
        uint16_t eye_lash_width, uint16_t eye_lash_height, uint16_t eyelid_lateral_x,
        uint16_t eyelid_bottom, uint16_t eyelid_width, uint16_t eyelid_height)
    {
        // calculate base waypoints for eyelash drawing
        // the eyelash will be drawn as a triangle from medial to lateral with tip as the peak

        float grad =
            static_cast<float>(eyelid_height) / static_cast<float>(eyelid_width / 2);

        // bottom (lateral)
        bottom.x = this->is_left_ ? eyelid_lateral_x - (eye_lash_width * 0.25f)
                                  : eyelid_lateral_x + (eye_lash_width * 0.25f);
        bottom.y = eyelid_bottom - grad * (eye_lash_width * 0.255f);

        // peak (edge)
        tip.x = this->is_left_ ? eyelid_lateral_x - (size_.width * 0.05f)
                               : eyelid_lateral_x + (size_.width * 0.05f);
        tip.y = bottom.y - eye_lash_height;

        //
        medial.x =
            this->is_left_ ? tip.x - (eye_lash_width) : tip.x + (eye_lash_width);
        medial.y = bottom.y - grad * (eye_lash_width * 1.05f);
    }

    void ToonEye1::drawIris(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette)
    {
        uint16_t iris_color = canvas.getColorDepth() == 1
                                  ? 1
                                  : palette.get(DrawingLocation::kIris1);
        uint16_t iris_bg_color = canvas.getColorDepth() == 1
                                     ? 1
                                     : palette.get(DrawingLocation::kIrisBackground);

        uint16_t thickness = 4;

        m5::Size2i iris_size = {static_cast<uint16_t>(0.9f * size_.height),
                                static_cast<uint16_t>(0.9f * size_.height)};

        // iris bg
        canvas.fillEllipse(iris_position_.x, iris_position_.y, iris_size.width / 2, iris_size.height / 2,
                           iris_bg_color);
        // iris1 color
        if (palette.contains(DrawingLocation::kIris1))
        {
            auto iris_color_1 = palette.get(DrawingLocation::kIris1);
            canvas.fillEllipse(iris_position_.x, iris_position_.y, iris_size.width / 2 - thickness,
                               iris_size.height / 2 - thickness, iris_color_1);
        }

        if (palette.contains(DrawingLocation::kIris2))
        {
            auto iris_color_2 = palette.get(DrawingLocation::kIris2);
            // center horizontal line
            canvas.fillRect(iris_position_.x - iris_size.width / 2 + thickness, iris_position_.y,
                            iris_size.width - 2 * thickness + 1, 1, iris_color_2);
            // lower half moon will be filled
            canvas.floodFill(iris_position_.x, iris_position_.y + 2, iris_color_2);
        }
        // pupil
        if (palette.contains(DrawingLocation::kPupil))
        {
            auto pupil_color = palette.get(DrawingLocation::kPupil);
            canvas.fillEllipse(iris_position_.x, iris_position_.y, iris_size.width / 6 + thickness, iris_size.height / 6 + thickness,
                               pupil_color);
        }

        // highlight
        if (palette.contains(DrawingLocation::kEyeHighlight))
        {
            auto highlight_color = palette.get(DrawingLocation::kEyeHighlight);
            canvas.fillCircle(iris_position_.x - iris_size.width / 4, iris_position_.y - iris_size.height / 4,
                              iris_size.min() / 8, highlight_color);
        }
    }

    void ToonEye1::drawEyelid(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette)
    {
        // required color paring
        uint16_t eyelash_color = palette.contains(DrawingLocation::kEyelash) ? palette.get(DrawingLocation::kEyelash) : TFT_MAGENTA;
        uint16_t skin_color = palette.contains(DrawingLocation::kSkin) ? palette.get(DrawingLocation::kSkin) : TFT_MAGENTA;
        // TODO: draw eyeshadow using eyelash color if eyelid color is not set
        // eyelid color is skin color by default, but can be overridden by eyelash color in palette, i.e eyeshadow color
        uint16_t eyelid_color = palette.contains(DrawingLocation::kSkin) ? palette.get(DrawingLocation::kSkin) : TFT_MAGENTA;

        // ** rotate waypoints
        float open_ratio = calculateOpenRatio(expression_weight);
        float tilt = 0.0f;
        float ref_tilt = open_ratio * 0.2;

        uint8_t weight;
        weight = expression_weight.get(Expression::kAngry);
        if (weight > 12)
        {
            tilt = (this->is_left_ ? -ref_tilt : ref_tilt) * (weight / 255.0f);
        }

        weight = expression_weight.get(Expression::kSad);
        if (weight > 12)
        {
            tilt = (this->is_left_ ? ref_tilt : -ref_tilt) * (weight / 255.0f);
        }

        weight = expression_weight.get(Expression::kGrin);
        // TODO:

        weight = expression_weight.get(Expression::kSurprised);
        if (weight > 12)
        {
            auto tmp = 1.0f + (weight / 255.0f) * 0.2f;
            open_ratio = open_ratio > 0.99f ? tmp : open_ratio;
        }

        uint8_t thickness = 4;
        // eyelid
        uint16_t eyelid_bottom_y = iris_position_.y - 0.65f * size_.height / 2 +
                                   (1.0f - open_ratio) * size_.height * 0.6;
        m5::Size2i eyelid_size = {size_.width, static_cast<uint16_t>(0.1f * size_.height * open_ratio) + 1};

        uint16_t eyelid_width = size_.width;
        uint16_t eyelid_height =
            0.1f * size_.height * open_ratio + 1; // this height must not be 0
        if (expression_weight.contains(Expression::kSmile) && expression_weight.get(Expression::kSmile) > 127)
        {
            eyelid_height += size_.height / 8;
        }

        // ## prepare eyelid  base waypoints
        m5::Vector2i eyelid_med, eyelid_c, eyelid_lat;
        this->computeEyelidBaseWaypoints(
            eyelid_med, eyelid_c, eyelid_lat, eyelid_width, eyelid_height, eyelid_bottom_y);

        // ## prepare eyelash base waypoints

        uint16_t eye_lash_width = 0.25 * size_.width;
        uint16_t eye_lash_height = eye_lash_width;
        m5::Vector2i eyelash_tip, eyelash_btm, eyelash_med;
        this->computeEyelashBaseWaypoints(
            eyelash_tip, eyelash_btm, eyelash_med, eye_lash_width, eye_lash_height, eyelid_lat.x,
            eyelid_bottom_y, eyelid_width, eyelid_height);

        auto rot_x = eyelid_c.x;
        auto rot_y = eyelid_bottom_y;
        m5::Vector2i rot_center = {rot_x, rot_y};

        m5::shearPointAround(eyelid_med, 0.0f, tilt, rot_center);
        m5::shearPointAround(eyelid_lat, 0.0f, tilt, rot_center);
        m5::shearPointAround(eyelid_c, 0.0f, tilt, rot_center);

        // draw eyelid

        // masking
        uint16_t mask_height = eye_lash_height;
        uint16_t mask_offset = eye_lash_height / 2;
        if ((eyelid_bottom_y - eye_lash_height) > (position_.y - size_.height / 2))
        {
            mask_height = (position_.y - size_.height / 2);
            mask_offset = mask_height / 2;
        }

        m5::fillArc(canvas, eyelid_med.x, eyelid_med.y, eyelid_lat.x, eyelid_lat.y,
                    eyelid_c.x, eyelid_c.y, mask_height, eyelid_color, mask_offset);
        // arc curve
        m5::fillArc(canvas, eyelid_med.x, eyelid_med.y, eyelid_lat.x, eyelid_lat.y,
                    eyelid_c.x, eyelid_c.y, thickness, eyelash_color);

        m5::rotatePointAround(eyelash_tip, tilt, rot_center);
        m5::rotatePointAround(eyelash_med, tilt, rot_center);
        m5::rotatePointAround(eyelash_btm, tilt, rot_center);

        canvas.fillTriangle(eyelash_tip.x, eyelash_tip.y, eyelash_med.x,
                            eyelash_med.y, eyelash_btm.x, eyelash_btm.y,
                            eyelash_color);
    }

    void ToonEye1::draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette)
    {
        // https://www.youtube.com/watch?v=Ig63OfuuiNA&list=LL&index=4
        // https://github.com/botamochi6277/m5stack-avatar/blob/b2c368c92a05d5a6b405e554c7450dba6db3247d/src/Eyes.cpp#L112
        // NOTE https://comic.smiles55.jp/guide/9879/

        milli_sec_ = M5.millis();
        // 0.2f is offset for natural eye opening. Usually, eyebrow overlap iris
        float open_ratio = calculateOpenRatio(expression_weight);
        auto wink_base_y = position_.y + (1.0f - open_ratio + 0.2f) * this->size_.height / 4;
        uint16_t thickness = 4;

        gaze_ = m5::Vector2i(0, 0);
        this->updateSaccade(expression_weight);
        iris_position_ = position_ + gaze_ + saccade_noise_;

        uint16_t iris_color = canvas.getColorDepth() == 1
                                  ? 1
                                  : palette.get(DrawingLocation::kIris1);
        uint16_t iris_bg_color = canvas.getColorDepth() == 1
                                     ? 1
                                     : palette.get(DrawingLocation::kIrisBackground);
        uint16_t eyelid_color = canvas.getColorDepth() == 1
                                    ? 0
                                    : palette.get(DrawingLocation::kSkin);

        if (open_ratio > 0.2f)
        {
            // iris
            this->drawIris(canvas, expression_weight, palette);
        }

        // eyelid
        this->drawEyelid(canvas, expression_weight, palette);

        // debug, drawing gizmo
        // canvas.fillCircle(position_.x, position_.y, 2, TFT_GREEN);
        // canvas.drawRect(is_left_ ? position_.x - size_.width * 0.4f : position_.x - size_.width * 0.6f,
        //                 position_.y - size_.height / 2, size_.width, size_.height, TFT_GREEN);
    }
}
