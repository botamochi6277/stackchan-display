#pragma once
#include "../../Avatar.h"

namespace stackchan::avatar
{
    class DefaultEye : public Drawable
    {
    private:
        bool is_left_ = false;
        int eyelid_offset_y_ = 0;
        int radius_ = 16;
        m5::Vector2i iris_offset_{0, 0};
        m5::Vector2i iris_position_{0, 0};

    public:
        DefaultEye(bool is_left);

        virtual void draw(M5Canvas *canvas, ExpressionWeight &expression_weight, ColorPalette &palette);
        virtual void update() override;
    };

    // M5StackChan default face
    class DefaultAvatar : public Avatar
    {
    public:
        DefaultAvatar();

        DefaultEye left_eye;
        DefaultEye right_eye;

        virtual void draw(M5Canvas *canvas, ExpressionWeight &expression_weight, ColorPalette &palette) override;
        // virtual void update() override;
    };
} // namespace stackchan::avatar