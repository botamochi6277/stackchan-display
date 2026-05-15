

#include "Face.h"

namespace stackchan::avatar
{
    Face::Face() : left_eye(165, 84 + 154, 16, true),
                   right_eye(165, 84, 16, false) {}

    void Face::draw(ExpressionWeight &expression_weight, ColorPalette &palette)
    {
        canvas_.createSprite(size_.width, size_.height);
        unsigned int skin_color = palette.get(DrawingLocation::kSkin);
        canvas_.fillSprite(skin_color);
        canvas_.pushSprite(position_.x - size_.width / 2, position_.y - size_.height / 2);
        canvas_.deleteSprite();

        this->left_eye.draw(expression_weight, palette);
        this->right_eye.draw(expression_weight, palette);
    }
}