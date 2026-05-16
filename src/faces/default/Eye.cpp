#include "Eye.h"

namespace stackchan::avatar
{

    Eye::Eye(uint16_t r, bool is_left) : is_left_(is_left)
    {
        radius_ = r;
        size_ = m5::Size2i(r * 2, r * 2);
    }

    Eye::Eye(m5::Vector2i &position, uint16_t r, bool is_left) : Eye(r, is_left)
    {
        position_ = position;
    }

    Eye::Eye(int x, int y, uint16_t r, bool is_left) : Eye(r, is_left)
    {
        position_ = m5::Vector2i(x, y);
    }

    // Eye::Eye(m5::Vector2i &position, bool is_left) : is_left_(is_left)
    // {
    //     position_ = position;
    // }

    void Eye::draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette)
    {
        static unsigned long milli_sec = 0;
        static unsigned long last_saccade_millis = 0;
        static uint32_t saccade_interval = 1000;
        static auto saccade_noise = m5::Vector2i();

        unsigned int iris_color = palette.get(DrawingLocation::kIris1);
        unsigned int eyelid_color = palette.get(DrawingLocation::kSkin);

        milli_sec = M5.millis();
        gaze_ = m5::Vector2i(0, 0); // tmp

        iris_position_ = position_ + gaze_ + saccade_noise;

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
        if (milli_sec - last_saccade_millis > saccade_interval)
        {
            saccade_noise = m5::Vector2i(rand() % 10 - 5, rand() % 10 - 5);
            last_saccade_millis = milli_sec;
            saccade_interval = 1000 + rand() % 1000; // add random to avoid fixed interval
        }

        // draw iris
        canvas.fillCircle(iris_position_.x, iris_position_.y, radius_, iris_color);

        // draw eyelid (if any)

        if (expression_weight.contains(Expression::kAngry) && expression_weight.get(Expression::kAngry) > 0)
        {
            int w = expression_weight.get(Expression::kAngry);
            int x0, y0, x1, y1, x2, y2;
            x0 = iris_position_.x - radius_;
            y0 = iris_position_.y - radius_;
            x1 = x0 + radius_ * 2;
            y1 = y0;
            x2 = is_left_ ? x0 : x1;
            y2 = y0 + (w / 255.0f) * radius_;
            canvas.fillTriangle(x0, y0, x1, y1, x2, y2, eyelid_color);
        }
    }

} // namespace stackchan::avatar