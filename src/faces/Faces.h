#pragma once

#include "FacialDrawable.h"
#include "../eyes/Eyes.h"
#include "../eyes/ToonEye1.h"
#include "../eyes/ToonEye2.h"
#include "Mouthes.h"

namespace stackchan::display
{

    // Stack-chan default face
    // [・_・]
    class Face : public FacialDrawable
    {
    protected:
        // auto blink parameters
        bool is_auto_blink_ = true;
        uint32_t auto_blink_interval_ = 3000; // 3 second
        uint32_t last_blink_time_ = 0;
        uint32_t blink_duration_ = 200; // 0.2 seconds, which is the duration of closed eye

        float breath_frequency_ = 0.25f; // in Hz,

        // child facial components
        // NOTE: use object pointers for exchanging components
        BaseEye *left_eye_;
        BaseEye *right_eye_;
        BaseMouth *mouth_;

    public:
        Face();
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

        void activateAutoBlink()
        {
            is_auto_blink_ = true;
        }
        void deactivateAutoBlink()
        {
            is_auto_blink_ = false;
        }
        bool getIsAutoBlink()
        {
            return is_auto_blink_;
        }

        void autoScale();

        virtual void updateState(ExpressionWeight &expression_weight);
        virtual void draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette) override;
        // virtual void update() override;
    };

    // custom faces

    // [O O]
    class EllFace : public Face
    {
    public:
        EllFace() : Face(
                        new EllipseEye(160 + 160 / 2, 240 / 2, 320 / 5, 320 / 5 * 3, true),
                        new EllipseEye(160 - 160 / 2, 240 / 2, 320 / 5, 320 / 5 * 3, false),
                        new BaseMouth(0, 0, 0, 0)) {};
    };

    // [OωO]
    class OmegaFace : public Face
    {
    public:
        OmegaFace() : Face(
                          new EllipseEye(84 + 154, 165, 36, 70, true),
                          new EllipseEye(84, 165, 36, 70, false),
                          new OmegaMouth(160, 225, 72, 30)) {};
    };

    // [▯ ▯]
    class RectFace : public Face
    {
    public:
        RectFace() : Face(
                         new RoundRectEye(160 + 160 / 2, 240 / 2, 320 / 5, 320 / 5 * 2, true),
                         new RoundRectEye(160 - 160 / 2, 240 / 2, 320 / 5, 320 / 5 * 2, false),
                         new BaseMouth(0, 0, 0, 0)) {};
    };

    class ToonFace1 : public Face
    {
    public:
        ToonFace1() : Face(
                          new ToonEye1(256, 163, 68, 60, true),
                          new ToonEye1(64, 163, 68, 60, false),
                          new OmegaMouth(160, 225, 72, 30)) {};
    };

    class ToonFace2 : public Face
    {
    public:
        ToonFace2() : Face(
                          new ToonEye2(256, 163, 60, 84, true),
                          new ToonEye2(64, 163, 60, 84, false),
                          new OmegaMouth(160, 225, 72, 30)) {};
    };

} // namespace stackchan::display
