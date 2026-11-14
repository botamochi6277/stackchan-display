#pragma once

#include "Eyes.h"

namespace stackchan::display
{
    /**
     * @brief A single round eye made of concentric rings (bullseye), for a face with one eye.
     *
     * Rings from the outside: iris color (`kIris1`), dark ring, iris color, then the pupil.
     * The dark parts use the pupil color (`kPupil`) if the palette has it, otherwise the skin color.
     * `kEyeHighlight` adds a small highlight on the pupil when the palette has it.
     *
     * The eye has no eyelids drawn in the skin color. The visible band is clipped
     * (like a slit of a visor), so it also works when a physical visor covers the screen.
     * - blink / sleepy / relax / doubt: the band gets thinner toward the center line
     * - surprised: the rings get bigger and the pupil gets smaller
     * - angry / sad: the upper edge of the band is slanted symmetrically about the center line.
     *   Angry lowers the center (V shape), and sad lowers both ends (inverted V shape).
     * - grin: the lower edge of the band rises
     * - smile: the eye is replaced with an arc `^`
     *
     * `width` is the diameter of the outer ring. `height` is ignored.
     */
    class MonoEye : public BaseEye
    {
    protected:
        void drawSmile(M5Canvas &canvas, uint16_t radius, uint16_t color);

    public:
        using BaseEye::BaseEye;

        virtual void draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette) override;
    };
}
