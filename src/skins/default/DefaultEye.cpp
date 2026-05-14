#include "DefaultAvatar.h"

namespace stackchan::avatar
{
    DefaultAvatar::DefaultAvatar() : left_eye(true), right_eye(false) {}

    void DefaultAvatar::draw(M5Canvas *canvas, ExpressionWeight &expression_weight, ColorPalette &palette)
    {
        this->left_eye.draw(canvas, expression_weight, palette);
        this->right_eye.draw(canvas, expression_weight, palette);
    }

    DefaultEye::DefaultEye(bool is_left) : is_left_(is_left) {}

    void DefaultEye::draw(M5Canvas *canvas, ExpressionWeight &expression_weight, ColorPalette &palette)
    {

        unsigned int iris_color = palette.get(DrawingLocation::kIrisBackground);
        unsigned int eyelid_color = palette.get(DrawingLocation::kSkin);
        iris_position_ = this->position_ + iris_offset_;

        radius_ = this->size_.min() / 2;

        // draw iris
        canvas->fillCircle(iris_position_.x, iris_position_.y, radius_, iris_color);

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
            canvas->fillTriangle(x0, y0, x1, y1, x2, y2, eyelid_color);
        }
    }

    void DefaultEye::update()
    {
        // Update the eye state (e.g., open/closed) based on some logic or expression
        // This is a placeholder implementation. You can customize it as needed.
        // For example, you could animate the eyelid offset to simulate blinking.
    }
} // namespace stackchan::avatar