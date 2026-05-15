#include "Eye.h"

namespace stackchan::avatar
{

    Eye::Eye(uint16_t r, bool is_left) : is_left_(is_left)
    {
        radius_ = r;
        size_ = m5::Size2i(r * 4, r * 4);
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
        // canvas.createSprite(size_.width, size_.height);
        // canvas.fillSprite(TFT_TRANSPARENT);

        gaze_ = m5::Vector2i(0, 0); // tmp
        unsigned int iris_color = palette.get(DrawingLocation::kIris1);
        unsigned int eyelid_color = palette.get(DrawingLocation::kSkin);
        iris_position_ = position_ + gaze_;

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