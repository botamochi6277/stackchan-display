#include "Eyes.h"

namespace stackchan::display
{

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

    void Eye::draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette)
    {
        radius_ = size_.min();

        unsigned int iris_color = canvas.getColorDepth() == 1
                                      ? 1
                                      : palette.get(DrawingLocation::kIris1);
        unsigned int eyelid_color = canvas.getColorDepth() == 1
                                        ? 0
                                        : palette.get(DrawingLocation::kSkin);

        // temporary variables for eyelid drawing
        m5::Vector2i p1, p2, p3;
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

        bool is_fully_closed = (expression_weight.contains(Expression::kSleepy) && expression_weight.get(Expression::kBlink) > 127) ||
                               (is_left_ ? (expression_weight.contains(Expression::kLeftBlink) && expression_weight.get(Expression::kLeftBlink) > 127)
                                         : (expression_weight.contains(Expression::kRightBlink) && expression_weight.get(Expression::kRightBlink) > 127)) ||
                               (expression_weight.contains(Expression::kSleepy) && expression_weight.get(Expression::kSleepy) > 127);
        if (is_fully_closed)
        {

            canvas.fillRect(iris_position_.x - (size_.width / 2),
                            iris_position_.y - 2 + size_.height / 4, size_.width, 4,
                            iris_color);
            return;
        }

        // add saccade noise
        // if (milli_sec_ - last_saccade_millis_ > saccade_interval_)
        // {
        //     saccade_noise_ = m5::Vector2i(rand() % 10 - 5, rand() % 10 - 5);
        //     last_saccade_millis_ = milli_sec_;
        //     saccade_interval_ = 1000 + rand() % 1000; // add random to avoid fixed interval
        //     if (expression_weight.contains(Expression::kUpset) && expression_weight.get(Expression::kUpset) > 0)
        //     {
        //         saccade_interval_ = 800 * (1.0f - (expression_weight.get(Expression::kUpset) / 255.0f)) + 200; // make saccade more frequent when upset
        //     }
        // }
        this->updateSaccade(expression_weight);

        // draw iris
        canvas.fillCircle(iris_position_.x, iris_position_.y, radius_, iris_color);

        // draw eyelid (if any)

        // TODO: reimplement with function object
        eye_expression = Expression::kAngry;
        if (expression_weight.contains(eye_expression) && expression_weight.get(eye_expression) > 0)
        {
            weight = expression_weight.get(eye_expression);
            p1.x = iris_position_.x - radius_;
            p1.y = iris_position_.y - radius_;
            p2.x = iris_position_.x + radius_;
            p2.y = iris_position_.y - radius_;
            // p3 is the position of inner corner of the eye
            p3.x = is_left_ ? iris_position_.x - radius_ : iris_position_.x + radius_;
            p3.y = iris_position_.y + (weight / 255.0f) * (radius_ / 2);
            canvas.fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, eyelid_color); // draw angry eyelid
        }

        eye_expression = Expression::kSad;
        if (expression_weight.contains(eye_expression) && expression_weight.get(eye_expression) > 0)
        {
            weight = expression_weight.get(eye_expression);
            p1.x = iris_position_.x - radius_;
            p1.y = iris_position_.y - radius_;
            p2.x = iris_position_.x + radius_;
            p2.y = iris_position_.y - radius_;
            // p3 is the position of outer corner of the eye
            p3.x = is_left_ ? iris_position_.x + radius_ : iris_position_.x - radius_;
            p3.y = iris_position_.y + (weight / 255.0f) * (radius_ / 2);
            canvas.fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, eyelid_color);
        }

        eye_expression = Expression::kDoubt;
        if (expression_weight.contains(eye_expression) && expression_weight.get(eye_expression) > 0)
        {
            weight = expression_weight.get(eye_expression);
            p1.x = iris_position_.x - radius_;
            p1.y = iris_position_.y - radius_;
            p2.x = iris_position_.x + radius_;
            p2.y = iris_position_.y - radius_;
            // p3 is the position of inner corner of the eye
            p3.x = is_left_ ? iris_position_.x - radius_ : iris_position_.x + radius_;
            p3.y = iris_position_.y - (weight / 255.0f) * (radius_ / 2);

            int w = 2 * radius_;
            int h = p3.y - p1.y;
            canvas.fillRect(p1.x, p1.y, w, h, eyelid_color); // draw doubt eyelid
        }

        eye_expression = Expression::kLaugh;
        if (expression_weight.contains(eye_expression) && expression_weight.get(eye_expression) > 0)
        {
            weight = expression_weight.get(eye_expression);
            p1.x = iris_position_.x - radius_;
            p1.y = iris_position_.y + radius_;
            p2.x = iris_position_.x + radius_;
            p2.y = iris_position_.y + radius_;
            // p3 is the position of inner corner of the eye
            p3.x = is_left_ ? iris_position_.x - radius_ : iris_position_.x + radius_;
            p3.y = iris_position_.y + radius_ - (weight / 255.0f) * (radius_);

            canvas.fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, eyelid_color); // draw lower eyelid for smile expression
        }

        eye_expression = Expression::kSmile;
        if (expression_weight.contains(eye_expression) && expression_weight.get(eye_expression) > 0)
        {
            weight = expression_weight.get(eye_expression);
            p1.x = iris_position_.x - radius_;
            p1.y = iris_position_.y + radius_;
            p2.x = iris_position_.x + radius_;
            p2.y = iris_position_.y + radius_;

            p3.x = iris_position_.x;
            p3.y = iris_position_.y + radius_ - (weight / 255.0f) * (radius_);

            int w = 2 * radius_;
            int h = p3.y - p1.y;
            canvas.fillRect(p1.x, p1.y + 1, w + 1, h + 1, eyelid_color);
            canvas.fillCircle(p3.x, p3.y, radius_ - 2, eyelid_color); // draw lower eyelid for smile expression
        }

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
        unsigned int iris_color = canvas.getColorDepth() == 1
                                      ? 1
                                      : palette.get(DrawingLocation::kIris1);
        unsigned int eyelid_color = canvas.getColorDepth() == 1
                                        ? 0
                                        : palette.get(DrawingLocation::kSkin);
        // temporary variables for eyelid drawing
        m5::Vector2i p1, p2, p3;
        Expression eye_expression = Expression::kNeutral;
        unsigned char weight;

        milli_sec_ = M5.millis();
        gaze_ = m5::Vector2i(0, 0); // tmp

        iris_position_ = position_ + gaze_ + saccade_noise_;

        bool is_fully_closed = (expression_weight.contains(Expression::kSleepy) && expression_weight.get(Expression::kBlink) > 127) ||
                               (is_left_ ? (expression_weight.contains(Expression::kLeftBlink) && expression_weight.get(Expression::kLeftBlink) > 127)
                                         : (expression_weight.contains(Expression::kRightBlink) && expression_weight.get(Expression::kRightBlink) > 127)) ||
                               (expression_weight.contains(Expression::kSleepy) && expression_weight.get(Expression::kSleepy) > 127);
        if (is_fully_closed)
        {

            canvas.fillRect(iris_position_.x - (size_.width / 2),
                            iris_position_.y - 2 + size_.height / 4, size_.width, 4,
                            iris_color);
            return;
        }

        // draw iris
        canvas.fillEllipse(iris_position_.x, iris_position_.y, size_.width / 2, size_.height / 2);
    }

} // namespace stackchan::display
