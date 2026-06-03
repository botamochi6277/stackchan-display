#pragma once

#include "Eyes.h"

namespace stackchan::display
{
    class ToonEye1 : public BaseEye
    {
    protected:
        void computeEyelidBaseWaypoints(m5::Vector2i &medial,
                                        m5::Vector2i &center,
                                        m5::Vector2i &lateral,
                                        uint16_t eyelid_width, uint16_t eyelid_height,
                                        uint16_t eyelid_bottom);
        void computeEyelashBaseWaypoints(
            m5::Vector2i &tip, m5::Vector2i &bottom, m5::Vector2i &medial,
            uint16_t eye_lash_width, uint16_t eye_lash_height, uint16_t eyelid_lateral_x,
            uint16_t eyelid_bottom, uint16_t eyelid_width, uint16_t eyelid_height);

    public:
        using BaseEye::BaseEye;

        void drawIris(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette);
        void drawEyelid(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette);
        void drawEyeLash(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette);
        virtual void draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette) override;
    };
}
