#pragma once

#include "FacialDrawable.h"
#include "../eyes/Eyes.h"
#include "../eyes/ToonEye1.h"
#include "../eyes/ToonEye2.h"
#include "../mouths/Mouth.h"
#include "../mouths/OmegaMouth.h"
#include "../mouths/ToonMouth1.h"

namespace stackchan::display
{

    /**
     * @brief Base face composed of two eyes and a mouth. Also the Stack-chan default face `[・_・]`.
     *
     * The face owns its parts through raw pointers so that they can be exchanged.
     * Subclasses only pass differently shaped parts and coordinates (laid out for a 320x240 canvas)
     * to the constructor. `updateState()` handles auto blink and breathing,
     * and `draw()` fills the canvas with the skin color then draws the eyes and the mouth.
     */
    class Face : public FacialDrawable
    {
    protected:
        // auto blink parameters
        bool is_auto_blink_ = true;
        uint32_t auto_blink_interval_ = 3000; // [msec] base interval between blinks (a random offset is added)
        uint32_t last_blink_time_ = 0;
        uint32_t blink_duration_ = 200; // 0.2 seconds, which is the duration of closed eye

        float breath_frequency_ = 0.25f; // in Hz,

        // child facial components
        // NOTE: use object pointers for exchanging components
        BaseEye *left_eye_;
        BaseEye *right_eye_;
        BaseMouth *mouth_;

    public:
        /// @brief Construct the default face
        Face();

        /**
         * @brief Construct a face from parts
         *
         * @param left_eye left eye (on the right side of the screen)
         * @param right_eye right eye (on the left side of the screen)
         * @param mouth mouth
         */
        Face(BaseEye *left_eye,
             BaseEye *right_eye,
             BaseMouth *mouth);

        BaseEye *getLeftEye()
        {
            return left_eye_;
        }

        BaseEye *getRightEye()
        {
            return right_eye_;
        }

        BaseMouth *getMouth()
        {
            return mouth_;
        }

        /// @brief Enable automatic blinking (enabled by default)
        void activateAutoBlink()
        {
            is_auto_blink_ = true;
        }
        /// @brief Disable automatic blinking
        void deactivateAutoBlink()
        {
            is_auto_blink_ = false;
        }
        /// @return true if automatic blinking is enabled
        bool getIsAutoBlink()
        {
            return is_auto_blink_;
        }

        /**
         * @brief Scale and move the parts to fit the screen size of `M5.Lcd`
         *
         * Parts are laid out for a 320x240 screen. Call this once after `M5.begin()`.
         */
        void autoScale();

        /**
         * @brief Update time-dependent state: smooth auto blink and breathing
         *
         * The blink is a triangle pulse written to `Expression::kBlink`,
         * and the breath is a sine wave written to `Expression::kBreath`.
         *
         * @param expression_weight weights to be updated
         */
        virtual void updateState(ExpressionWeight &expression_weight);

        /**
         * @brief Fill the canvas with the skin color and draw the eyes and the mouth
         *
         * @param canvas off-screen canvas to draw on
         * @param expression_weight expression weights used to blend the shapes
         * @param palette colors of each facial location
         */
        virtual void draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette) override;
        // virtual void update() override;
    };

    // custom faces

    /**
     * @brief `[O O]` face with tall ellipse eyes and no mouth
     */
    class EllFace : public Face
    {
    public:
        EllFace() : Face(
                        new EllipseEye(160 + 160 / 2, 240 / 2, 320 / 5, 320 / 5 * 3, true),
                        new EllipseEye(160 - 160 / 2, 240 / 2, 320 / 5, 320 / 5 * 3, false),
                        new BaseMouth(0, 0, 0, 0)) {};
    };

    /**
     * @brief `[OωO]` face with ellipse eyes and an omega-shaped mouth
     */
    class OmegaFace : public Face
    {
    public:
        OmegaFace() : Face(
                          new EllipseEye(84 + 154, 165, 36, 70, true),
                          new EllipseEye(84, 165, 36, 70, false),
                          new OmegaMouth(160, 225, 72, 30)) {};
    };

    /**
     * @brief `[▯ ▯]` face with round-rectangle eyes and no mouth
     */
    class RectFace : public Face
    {
    public:
        RectFace() : Face(
                         new RoundRectEye(160 + 160 / 2, 240 / 2, 320 / 5, 320 / 5 * 2, true),
                         new RoundRectEye(160 - 160 / 2, 240 / 2, 320 / 5, 320 / 5 * 2, false),
                         new BaseMouth(0, 0, 0, 0)) {};
    };

    /**
     * @brief Toon face with eyelid-and-eyelash eyes (ToonEye1) and ToonMouth1
     */
    class ToonFace1 : public Face
    {
    public:
        ToonFace1() : Face(
                          new ToonEye1(256, 163, 68, 60, true),
                          new ToonEye1(64, 163, 68, 60, false),
                          new ToonMouth1(160, 222, 48, 24)) {};
    };

    /**
     * @brief Toon face with quadrilateral-eyelid eyes (ToonEye2) and an omega mouth
     */
    class ToonFace2 : public Face
    {
    public:
        ToonFace2() : Face(
                          new ToonEye2(256, 163, 60, 84, true),
                          new ToonEye2(64, 163, 60, 84, false),
                          new OmegaMouth(160, 225, 72, 30)) {};
    };

} // namespace stackchan::display
