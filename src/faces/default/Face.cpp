#include "Face.h"

// 226,86

namespace stackchan::avatar
{
    Face::Face() : left_eye(235, 95, 9, true),
                   right_eye(99, 95, 9, false) {}

    void Face::draw(
        M5Canvas &canvas,
        ExpressionWeight &expression_weight,
        ColorPalette &palette)
    {
        unsigned int skin_color = palette.get(DrawingLocation::kSkin);
        canvas.fillSprite(skin_color);
        this->left_eye.draw(canvas, expression_weight, palette);
        this->right_eye.draw(canvas, expression_weight, palette);
    }
}