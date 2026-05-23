#pragma once

#include "FacialDrawable.h"
#include "Eyes.h"
#include "Mouthes.h"

namespace stackchan::display
{

    // M5StackChan default face
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

        virtual void updateState(ExpressionWeight &expression_weight);
        virtual void draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette) override;
        // virtual void update() override;
    };

    // class EllFace : public Face
    // {
    // protected:
    //     EllipseEye left_eye_;
    //     EllipseEye right_eye_;

    // public:
    //     using Face::Face;
    // };
} // namespace stackchan::display
