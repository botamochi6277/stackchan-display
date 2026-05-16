#pragma once

#include "../FacialDrawable.h"
#include "Eye.h"
#include "Mouth.h"

namespace stackchan::avatar
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

    public:
        Face();
        Eye left_eye;
        Eye right_eye;
        Mouth mouth;

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
} // namespace stackchan::avatar