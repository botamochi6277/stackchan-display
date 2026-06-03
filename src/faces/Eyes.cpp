#include "Eyes.h"

namespace stackchan::display
{

    void updateQuadrilateralEyelidPoints(m5::Vector2i &center, m5::Size2i &size, bool is_left,
                                         ExpressionWeight &expression_weight,
                                         m5::Vector2i &p1, m5::Vector2i &p2, m5::Vector2i &p3, m5::Vector2i &p4)
    {

        // right    left
        // p1--p2   p2--p1
        // |   |    |   |
        // p4--p3   p3--p4

        Expression eye_expression;
        unsigned char weight;

        int lateral_x = is_left ? center.x + size.width / 2 + 1 : center.x - size.width / 2 - 1;
        int medial_x = is_left ? center.x - size.width / 2 - 1 : center.x + size.width / 2 + 1;

        p1 = {0, 0};
        p2 = {0, 0};
        p3 = {0, 0};
        p4 = {0, 0};

        eye_expression = Expression::kAngry;
        if (expression_weight.contains(eye_expression) && expression_weight.get(eye_expression) > 0)
        {
            weight = expression_weight.get(eye_expression);
            p1.x = lateral_x;
            p1.y = center.y - size.height / 2 - 2;

            p2.x = medial_x;
            p2.y = p1.y;

            p3.x = medial_x;
            p3.y = center.y + (weight / 255.0f) * (size.height / 4);

            p4.x = lateral_x;
            p4.y = p1.y + 1;
        }

        eye_expression = Expression::kSad;
        if (expression_weight.contains(eye_expression) && expression_weight.get(eye_expression) > 0)
        {
            weight = expression_weight.get(eye_expression);
            p1.x = lateral_x;
            p1.y = center.y - size.height / 2 - 2;
            p2.x = medial_x;
            p2.y = p1.y;
            p3.x = medial_x;
            p3.y = p2.y + 1;
            p4.x = lateral_x;
            p4.y = center.y + (weight / 255.0f) * (size.height / 4);
            // canvas.fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, eyelid_color);
        }

        eye_expression = Expression::kDoubt;
        if (expression_weight.contains(eye_expression) && expression_weight.get(eye_expression) > 0)
        {
            weight = expression_weight.get(eye_expression);
            p1.x = lateral_x;
            p1.y = center.y - size.height / 2;
            p2.x = medial_x;
            p2.y = p1.y;
            p3.x = medial_x;
            p3.y = center.y - (weight / 255.0f) * (size.height / 4);
            p4.x = lateral_x;
            p4.y = p3.y;
        }

        eye_expression = Expression::kGrin;
        if (expression_weight.contains(eye_expression) && expression_weight.get(eye_expression) > 0)
        {
            weight = expression_weight.get(eye_expression);
            p1.x = lateral_x;
            p1.y = center.y + size.height / 2;
            p2.x = medial_x;
            p2.y = center.y + size.height / 2 - (weight / 255.0f) * (size.height / 2);
            p3.x = medial_x;
            p3.y = p1.y + 4;
            p4.x = lateral_x;
            p4.y = p3.y;
        }

        eye_expression = Expression::kSmile;
        if (expression_weight.contains(eye_expression) && expression_weight.get(eye_expression) > 0)
        {
            weight = expression_weight.get(eye_expression);
            p1.x = lateral_x;
            p1.y = center.y + size.height / 2 - (weight / 255.0f) * (size.height / 2);
            p2.x = medial_x;
            p2.y = p1.y;

            p3.x = p2.x;
            p3.y = center.y + size.height / 2 + 2;
            p4.x = p1.x;
            p4.y = p3.y;

            // int w = size.width;
            // int h = p3.y - p1.y;
            // canvas.fillRect(p1.x, p1.y + 1, w + 1, h + 1, eyelid_color);
            // canvas.fillCircle(p3.x, p3.y, radius_ - 2, eyelid_color); // draw lower eyelid for smile expression
        }
    }

    BaseEye::BaseEye(int16_t x, int16_t y, int16_t width, int16_t height, bool is_left) : BaseEye::BaseEye(x, y, width, height)
    {
        is_left_ = is_left;
    }

    void BaseEye::updateGaze(ExpressionWeight &weight, uint16_t range)
    {
        using Exp = Expression;
        uint16_t w;
        gaze_ = {0, 0}; // init

        w = weight.get(Exp::kLookUp);
        gaze_.y -= range * (w / 255.0f);

        w = weight.get(Exp::kLookDown);
        gaze_.y += range * (w / 255.0f);

        w = weight.get(Exp::kLookLeft);
        gaze_.x += range * (w / 255.0f);

        w = weight.get(Exp::kLookRight);
        gaze_.x -= range * (w / 255.0f);
    }

    void BaseEye::updateSaccade(ExpressionWeight &expression_weight)
    {
        if (milli_sec_ - last_saccade_millis_ > saccade_interval_)
        {
            saccade_noise_ = m5::Vector2i(rand() % 4 - 2, rand() % 4 - 2);
            last_saccade_millis_ = milli_sec_;
            saccade_interval_ = 1000 + rand() % 1000; // add random to avoid fixed interval
            if (expression_weight.contains(Expression::kUpset) && expression_weight.get(Expression::kUpset) > 0)
            {
                saccade_interval_ = 800 * (1.0f - (expression_weight.get(Expression::kUpset) / 255.0f)) + 200; // make saccade more frequent when upset
            }
        }
    }

    float BaseEye::calculateOpenRatio(ExpressionWeight &expression_weight)
    {
        float open_ratio = 1.0f;

        if (expression_weight.contains(Expression::kBlink))
        {
            open_ratio = 1.0f - (expression_weight.get(Expression::kBlink) / 255.0f);
        }

        if (is_left_)
        {
            if (expression_weight.contains(Expression::kLeftBlink))
            {
                open_ratio = 1.0f - (expression_weight.get(Expression::kLeftBlink) / 255.0f); // left blink expression fully closes the left eye
            }
        }
        else
        {
            if (expression_weight.contains(Expression::kRightBlink))
            {
                open_ratio = 1.0f - (expression_weight.get(Expression::kRightBlink) / 255.0f); // right blink expression fully closes the right eye
            }
        }

        return clamp(open_ratio, 0.0f, 1.0f);
    }

    void Eye::draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette)
    {
        radius_ = size_.min() / 2;

        uint16_t iris_color = canvas.getColorDepth() == 1
                                  ? 1
                                  : palette.get(DrawingLocation::kIris1);
        uint16_t eyelid_color = canvas.getColorDepth() == 1
                                    ? 0
                                    : palette.get(DrawingLocation::kSkin);

        // temporary variables for eyelid drawing
        m5::Vector2i p1, p2, p3, p4;
        Expression eye_expression = Expression::kNeutral;
        unsigned char weight;

        milli_sec_ = M5.millis();
        gaze_ = m5::Vector2i(0, 0); // tmp

        iris_position_ = position_ + gaze_ + saccade_noise_;

        eye_expression = Expression::kRelax;
        if (expression_weight.contains(eye_expression) && expression_weight.get(eye_expression) > 0)
        {
            weight = expression_weight.get(eye_expression);
            p1.x = iris_position_.x - radius_;
            p1.y = iris_position_.y - radius_;
            p2.x = iris_position_.x + radius_;
            p2.y = iris_position_.y - radius_;

            p3.x = iris_position_.x;
            p3.y = iris_position_.y - radius_ + (weight / 255.0f) * (radius_);

            int w = 2 * radius_;
            int h = p3.y - p1.y;
            canvas.fillCircle(iris_position_.x, iris_position_.y, radius_, iris_color);
            canvas.fillRect(p1.x, p1.y, w + 1, h + 1, eyelid_color);
            canvas.fillCircle(p3.x, p3.y, radius_ - 2, eyelid_color); // draw lower eyelid for smile expression
            return;
        }

        float open_ratio = calculateOpenRatio(expression_weight);
        bool is_fully_closed = open_ratio <= 0.1f ||
                               (expression_weight.contains(Expression::kSleepy) && expression_weight.get(Expression::kSleepy) > 127);
        if (is_fully_closed)
        {

            canvas.fillRect(iris_position_.x - (size_.width / 2),
                            iris_position_.y - 2 + size_.height / 4, size_.width, 4,
                            iris_color);
            return;
        }

        // add saccade noise
        this->updateSaccade(expression_weight);

        // draw iris
        canvas.fillCircle(iris_position_.x, iris_position_.y, radius_, iris_color);

        // draw eyelid (if any)

        updateQuadrilateralEyelidPoints(
            iris_position_, size_, is_left_, expression_weight, p1, p2, p3, p4);
        canvas.fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, eyelid_color); // draw eyelid
        canvas.fillTriangle(p3.x, p3.y, p4.x, p4.y, p1.x, p1.y, eyelid_color);

        // draw special eyelid
        // smile
        // ^ ^
        eye_expression = Expression::kSmile;
        if (expression_weight.contains(eye_expression) && expression_weight.get(eye_expression) > 0)
        {
            int w = 2 * radius_;
            int h = p3.y - p1.y;
            canvas.fillCircle(iris_position_.x, p1.y, radius_ - 2, eyelid_color); // draw lower eyelid for smile expression
        }

        // extra
        eye_expression = Expression::kSurprised;
        if (expression_weight.contains(eye_expression) && expression_weight.get(eye_expression) > 0)
        {
            weight = expression_weight.get(eye_expression);
            int r = radius_ + (weight / 255.0f) * (radius_ / 2);
            canvas.fillCircle(iris_position_.x, iris_position_.y, r, iris_color); // draw surprised eye
        }
    }

    void EllipseEye::draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette)
    {
        uint16_t iris_color = canvas.getColorDepth() == 1
                                  ? 1
                                  : palette.get(DrawingLocation::kIris1);
        uint16_t eyelid_color = canvas.getColorDepth() == 1
                                    ? 0
                                    : palette.get(DrawingLocation::kSkin);
        // temporary variables for eyelid drawing
        m5::Vector2i p1, p2, p3, p4;
        Expression eye_expression = Expression::kNeutral;
        unsigned char weight;

        milli_sec_ = M5.millis();
        gaze_ = m5::Vector2i(0, 0); // tmp

        iris_position_ = position_ + gaze_ + saccade_noise_;

        float open_ratio = calculateOpenRatio(expression_weight);
        bool is_fully_closed = open_ratio <= 0.1f ||
                               (expression_weight.contains(Expression::kSleepy) && expression_weight.get(Expression::kSleepy) > 127);
        if (is_fully_closed)
        {

            canvas.fillRect(iris_position_.x - (size_.width / 2),
                            iris_position_.y - 2 + size_.height / 4, size_.width, 4,
                            iris_color);
            return;
        }

        // add saccade noise
        this->updateSaccade(expression_weight);

        // draw iris
        if (expression_weight.get(Expression::kSurprised) < 127)
        {
            canvas.fillEllipse(iris_position_.x, iris_position_.y, size_.width / 2, size_.height / 2, iris_color);
        }

        updateQuadrilateralEyelidPoints(
            iris_position_, size_, is_left_, expression_weight, p1, p2, p3, p4);
        canvas.fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, eyelid_color); // draw eyelid
        canvas.fillTriangle(p3.x, p3.y, p4.x, p4.y, p1.x, p1.y, eyelid_color);

        // draw special eyelid
        // smile
        // ^ ^
        eye_expression = Expression::kSmile;
        if (expression_weight.contains(eye_expression) && expression_weight.get(eye_expression) > 0)
        {
            canvas.fillEllipse(iris_position_.x, p1.y, size_.width / 2 - 2, size_.height / 2 - 2, eyelid_color); // draw lower eyelid for smile expression
        }
        // extra
        eye_expression = Expression::kSurprised;
        if (expression_weight.contains(eye_expression) && expression_weight.get(eye_expression) > 0)
        {
            weight = expression_weight.get(eye_expression);
            // int ry = size_.width / 2 + (weight / 255.0f) * (size_.width / 4);
            int ry = remap(static_cast<int>(weight), 0, 255, size_.height / 2, size_.width / 2);
            canvas.fillEllipse(iris_position_.x, iris_position_.y, size_.width / 2, ry, iris_color); // draw surprised eye
        }
    }

    void RoundRectEye::draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette)
    {
        uint16_t iris_color = canvas.getColorDepth() == 1
                                  ? 1
                                  : palette.get(DrawingLocation::kIris1);
        uint16_t eyelid_color = canvas.getColorDepth() == 1
                                    ? 0
                                    : palette.get(DrawingLocation::kSkin);
        // temporary variables for eyelid drawing
        m5::Vector2i p1, p2, p3, p4;
        Expression eye_expression = Expression::kNeutral;
        unsigned char weight;
        uint16_t radius = size_.min() / 8;

        milli_sec_ = M5.millis();
        gaze_ = m5::Vector2i(0, 0); // tmp

        iris_position_ = position_ + gaze_ + saccade_noise_;

        float open_ratio = calculateOpenRatio(expression_weight);
        bool is_fully_closed = open_ratio <= 0.1f ||
                               (expression_weight.contains(Expression::kSleepy) && expression_weight.get(Expression::kSleepy) > 127);
        if (is_fully_closed)
        {

            canvas.fillRect(iris_position_.x - (size_.width / 2),
                            iris_position_.y - 2 + size_.height / 4, size_.width, 4,
                            iris_color);
            return;
        }

        // add saccade noise
        this->updateSaccade(expression_weight);

        // draw iris
        if (expression_weight.get(Expression::kSurprised) < 127)
        {
            canvas.fillRoundRect(
                iris_position_.x - (size_.width / 2),
                iris_position_.y - (size_.height / 2),
                size_.width, size_.height, radius, iris_color);
        }

        updateQuadrilateralEyelidPoints(
            iris_position_, size_, is_left_, expression_weight, p1, p2, p3, p4);
        canvas.fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, eyelid_color); // draw eyelid
        canvas.fillTriangle(p3.x, p3.y, p4.x, p4.y, p1.x, p1.y, eyelid_color);

        // draw special eyelid
        // smile
        // ^ ^
        eye_expression = Expression::kSmile;
        if (expression_weight.contains(eye_expression) && expression_weight.get(eye_expression) > 0)
        {
            canvas.fillEllipse(iris_position_.x, p1.y, size_.width / 2 - 2, size_.height / 2 - 2, eyelid_color); // draw lower eyelid for smile expression
        }
        // extra
        eye_expression = Expression::kSurprised;
        if (expression_weight.contains(eye_expression) && expression_weight.get(eye_expression) > 0)
        {
            weight = expression_weight.get(eye_expression);
            // int ry = size_.width / 2 + (weight / 255.0f) * (size_.width / 4);
            int ry = remap(static_cast<int>(weight), 0, 255, size_.height / 2, size_.width / 2);
            canvas.fillEllipse(iris_position_.x, iris_position_.y, size_.width / 2, ry, iris_color); // draw surprised eye
        }
    }

    void ToonEye1::computeEyelidBaseWaypoints(m5::Vector2i &medial,
                                              m5::Vector2i &center,
                                              m5::Vector2i &lateral,
                                              uint16_t eyelid_width, uint16_t eyelid_height,
                                              uint16_t eyelid_bottom)
    {
        // calculate base waypoints for eyelid drawing
        // the eyelid will be drawn as a arc from medial to lateral with center as the peak

        // inner(medial)
        medial.x = this->is_left_ ? this->position_.x - eyelid_width / 2
                                  : this->position_.x + eyelid_width / 2;
        medial.y = eyelid_bottom;

        // outer(lateral)
        lateral.x = this->is_left_ ? this->position_.x + eyelid_width / 2
                                   : this->position_.x - eyelid_width / 2;
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
        // iris bg
        canvas.fillEllipse(iris_position_.x, iris_position_.y, size_.width / 2, size_.height / 2,
                           iris_bg_color);
        // iris1 color
        if (palette.contains(DrawingLocation::kIris1))
        {
            auto iris_color_1 = palette.get(DrawingLocation::kIris1);
            canvas.fillEllipse(iris_position_.x, iris_position_.y, size_.width / 2 - thickness,
                               size_.height / 2 - thickness, iris_color_1);
        }

        if (palette.contains(DrawingLocation::kIris2))
        {
            auto iris_color_2 = palette.get(DrawingLocation::kIris2);
            // center horizontal line
            canvas.fillRect(iris_position_.x - size_.width / 2 + thickness, iris_position_.y,
                            size_.width - 2 * thickness + 1, 1, iris_color_2);
            // lower half moon will be filled
            canvas.floodFill(iris_position_.x, iris_position_.y + 2, iris_color_2);
        }
        // pupil
        if (palette.contains(DrawingLocation::kPupil))
        {
            auto pupil_color = palette.get(DrawingLocation::kPupil);
            canvas.fillEllipse(iris_position_.x, iris_position_.y, size_.width / 6 + thickness, size_.height / 6 + thickness,
                               pupil_color);
        }

        // highlight
        if (palette.contains(DrawingLocation::kEyeHighlight))
        {
            auto highlight_color = palette.get(DrawingLocation::kEyeHighlight);
            canvas.fillCircle(iris_position_.x - size_.width / 4, iris_position_.y - size_.height / 4,
                              size_.min() / 8, highlight_color);
        }
    }

    void ToonEye1::drawEyelid(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette)
    {
        uint16_t eyelid_color = canvas.getColorDepth() == 1
                                    ? 0
                                    : palette.get(DrawingLocation::kEyelid);
        uint16_t skin_color = canvas.getColorDepth() == 1
                                  ? 0
                                  : palette.get(DrawingLocation::kSkin);
        // if (palette.contains(DrawingLocation::kEyelid))
        // {
        //     {
        //         eyelid_color = palette.get(DrawingLocation::kEyelid);
        //     }
        //     // temporary variables for eyelid drawing
        //     m5::Vector2i p1, p2, p3, p4;

        //     updateQuadrilateralEyelidPoints(
        //         iris_position_, size_, is_left_, expression_weight, p1, p2, p3, p4);
        //     canvas.fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, eyelid_color); // draw eyelid
        //     canvas.fillTriangle(p3.x, p3.y, p4.x, p4.y, p1.x, p1.y, eyelid_color);
        // }

        // ** rotate waypoints
        float open_ratio = calculateOpenRatio(expression_weight);
        float tilt = 0.0f;
        float ref_tilt = open_ratio * M_PI / 12.0f;
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

        weight = expression_weight.get(Expression::kDoubt);
        if (weight > 12)
        {
            open_ratio *= 0.6 * (weight / 255.0f);
            // eyelid_bottom_y = iris_position_.y - 0.65f * size_.height / 2 +
            //                   (1.0f - open_ratio) * size_.height * 0.6;
        }
        weight = expression_weight.get(Expression::kGrin);
        // TODO:

        weight = expression_weight.get(Expression::kSurprised);
        if (weight > 12)
        {
            open_ratio = 1.0f + (weight / 255.0f) * 0.2f;
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

        m5::rotatePointAround(eyelid_med, tilt, rot_center);
        m5::rotatePointAround(eyelid_lat, tilt, rot_center);
        m5::rotatePointAround(eyelid_c, tilt, rot_center);

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
                    eyelid_c.x, eyelid_c.y, mask_height, skin_color, mask_offset);
        // arc curve
        m5::fillArc(canvas, eyelid_med.x, eyelid_med.y, eyelid_lat.x, eyelid_lat.y,
                    eyelid_c.x, eyelid_c.y, thickness, eyelid_color);

        // eyelash
        if (!palette.contains(DrawingLocation::kEyelash))
        {
            return;
        }

        auto eyelash_color = palette.get(DrawingLocation::kEyelash);

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
    }

} // namespace stackchan::display
