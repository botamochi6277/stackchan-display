#include "ToonMouth1.h"

namespace stackchan::display
{

    void ToonMouth1::draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette)
    {
        // this->update(canvas, rect, ctx); // update drawing cache
        uint16_t neutral_w = 0.8f * size_.width;
        // uint16_t h = upper_lip_amp + (lower_lip_amp - upper_lip_amp) * open_ratio_;
        // uint16_t w = min_width_ + (neutral_w - min_width_) * (1.0f - open_ratio_);

        uint16_t upper_lip_amp = size_.height * 0.4;
        uint16_t lower_lip_amp = size_.height * 0.6f;
        auto lip_baseline_y = position_.y + size_.height * 0.1f;
        float lip_ratio = upper_lip_amp / lower_lip_amp; // upper_lip_h/lower_lip_height
        uint16_t thickness = 2;
        uint16_t upper_lip_y = lip_baseline_y;
        uint16_t lower_lip_y = lip_baseline_y;
        int breath_offset = static_cast<int>(((expression_weight.get(Expression::kBreath) / 255.0f) - 0.5f) * 2.0f * breath_amplitude_);

        uint16_t weight = 0;
        uint16_t w = neutral_w;
        if (expression_weight.get(Expression::kSmile) > 0)
        {
            // :)
            weight = expression_weight.get(Expression::kSmile);
            w = size_.width;
            lower_lip_y = lip_baseline_y + lower_lip_amp * 0.5f * (weight / 255.0f);
            upper_lip_y = lower_lip_y;
        }

        else if (expression_weight.get(Expression::kAngry) > 0)
        {
            // :(
            weight = expression_weight.get(Expression::kAngry);
            w = neutral_w;
            upper_lip_y = lip_baseline_y - upper_lip_amp * (weight / 255.0f);
            lower_lip_y = upper_lip_y;
        }
        else if (expression_weight.get(Expression::kSad) > 0)
        {
            // :o
            weight = expression_weight.get(Expression::kSad);
            w = neutral_w;
            upper_lip_y = lip_baseline_y - upper_lip_amp * (weight / 255.0f);
            lower_lip_y = lip_baseline_y + lower_lip_amp * (weight / 255.0f);
        }
        else if (expression_weight.get(Expression::kDoubt) > 0)
        {
            // :|
            weight = expression_weight.get(Expression::kDoubt);
            w = size_.width * 0.3f + (neutral_w) * 0.5f;
            upper_lip_y = lip_baseline_y - 0.5 * upper_lip_amp * (weight / 255.0f);
            lower_lip_y = upper_lip_y;
        }
        else if (expression_weight.get(Expression::kAa) > 0)
        {
            // :|
            weight = expression_weight.get(Expression::kAa);
            // w = size_.width * 0.3f + (neutral_w) * 0.5f;
            upper_lip_y = lip_baseline_y;
            lower_lip_y = lip_baseline_y + lower_lip_amp * (weight / 255.0f);
            ;
        }

        lower_lip_y = m5::clamp(lower_lip_y, static_cast<uint16_t>(0U), static_cast<uint16_t>(M5.Lcd.height() - breath_offset));
        // expressions
        // switch (expression_)
        // {
        // case Expression::kHappy:
        // case Expression::kSmile:
        //     w = max_width_;
        //     upper_lip_y = lip_baseline_y + lower_lip_amp;
        //     lower_lip_y = lip_baseline_y + lower_lip_amp;
        //     break;
        // case Expression::kAngry:
        //     upper_lip_y = lip_baseline_y - upper_lip_amp;
        //     lower_lip_y = lip_baseline_y - upper_lip_amp;
        //     break;
        // case Expression::kDoubt:
        //     // overwrite width
        //     // NOTE should we prepare extra_min_width?
        //     w = min_width_;
        //     upper_lip_y = lip_baseline_y - upper_lip_amp / 2;
        //     lower_lip_y = lip_baseline_y + upper_lip_amp / 2;
        //     break;
        // case Expression::kSad:
        //     w = min_width_ + (neutral_w - min_width_) * 0.5f;
        //     upper_lip_y = lip_baseline_y - upper_lip_amp / 2;
        //     lower_lip_y = lip_baseline_y + upper_lip_amp / (1.0f + lip_ratio);
        //     break;
        // default:
        //     // neutral
        //     // max_width x upper_lip_amp @0.0 open_ratio
        //     // min_width x lower_lip_amp @1.0 open_ratio

        //     // upper lib outline
        //     upper_lip_y = lip_baseline_y + upper_lip_amp;
        //     // lower lib outline
        //     lower_lip_y = lip_baseline_y + h;

        //     break;
        // }

        // color parsing
        uint16_t bg_color = palette.contains(DrawingLocation::kMouthBackground) ? palette.get(DrawingLocation::kMouthBackground) : TFT_MAGENTA;

        // draw lip outlines
        m5::fillArc(canvas, position_.x - w / 2, position_.y + breath_offset,
                    position_.x + w / 2, position_.y + breath_offset,
                    position_.x, upper_lip_y + breath_offset, thickness, bg_color);

        m5::fillArc(canvas, position_.x - w / 2, position_.y + breath_offset,
                    position_.x + w / 2, position_.y + breath_offset,
                    position_.x, lower_lip_y + breath_offset, thickness, bg_color);

        // M5_LOGD("lower y %d--%d", upper_lip_y, lower_lip_y);

        // bbox
        // canvas.drawRect(position_.x - w / 2, center_y_ - h / 2, w, h, TFT_BLUE);

        // fill inner
        if (palette.contains(DrawingLocation::kInnerMouth))
        {
            if (lower_lip_y - upper_lip_y > thickness + 2)
            {
                auto inner_color = palette.get(DrawingLocation::kInnerMouth);
                canvas.floodFill(position_.x, 0.5f * upper_lip_y + 0.5f * lower_lip_y + breath_offset,
                                 inner_color);
            }
        }

        // cheek
        // TODO: implement as decorator
        // if (palette.contains(DrawingLocation::kCheek1))
        // {
        //     auto cheek_color = palette.get(DrawingLocation::kCheek1);
        //     canvas.fillEllipse(position_.x - 132, position_.y - 23, 24, 10, cheek_color);
        //     canvas.fillEllipse(position_.x + 132, position_.y - 23, 24, 10, cheek_color);
        // }

        // gizmo
        // canvas.fillCircle(position_.x, position_.y, 2, TFT_GREEN);
        // canvas.fillCircle(position_.x, position_.y - upper_lip_amp, 2, TFT_GREEN);
        // canvas.fillCircle(position_.x, position_.y + lower_lip_amp, 2, TFT_GREEN);
        // canvas.fillCircle(position_.x, upper_lip_y, 4, TFT_CYAN);
        // canvas.fillCircle(position_.x, lower_lip_y, 4, TFT_CYAN);
        // // canvas.fillCircle(position_.x, lip_baseline_y, 2, TFT_GREEN);
        // m5::fillArc(canvas,
        //             position_.x - neutral_w / 2, position_.y,
        //             position_.x + neutral_w / 2, position_.y,
        //             position_.x, lip_baseline_y, 1, TFT_GREEN);
        // canvas.drawRect(position_.x - size_.width / 2, position_.y - upper_lip_amp, size_.width, size_.height, TFT_GREEN);
    }
}
