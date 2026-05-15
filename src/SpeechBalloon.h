// Copyright (c) Shinya Ishikawa. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#pragma once
#include <M5Unified.h>

#include "Drawable.h"
#include "ColorPalette.h"

#ifndef ARDUINO
#include <string>
typedef std::string String;
#endif // ARDUINO

const int16_t TEXT_HEIGHT = 8;
const int16_t TEXT_SIZE = 2;
const int16_t MIN_WIDTH = 40;
const int cx = 240;
const int cy = 220;

namespace stackchan::avatar
{
  class SpeechBalloon final : public Drawable
  {
  protected:
    std::string text_;
    const lgfx::IFont *font_;

  public:
    // constructor
    SpeechBalloon() = default;
    ~SpeechBalloon() = default;
    SpeechBalloon(const SpeechBalloon &other) = default;
    SpeechBalloon &operator=(const SpeechBalloon &other) = default;

    void setText(const std::string &text)
    {
      text_ = text;
    }
    void draw() override
    {
      // This method is intentionally left empty. The actual drawing logic is implemented in the overloaded draw method that takes a ColorPalette parameter.
    }

    void draw(ColorPalette *cp)
    {

      if (text_.length() == 0)
      {
        return;
      }

      canvas_.createSprite(M5.Lcd.width(), M5.Lcd.height());

      uint16_t primaryColor = cp->get(COLOR_BALLOON_FOREGROUND);
      uint16_t backgroundColor = cp->get(COLOR_BALLOON_BACKGROUND);
      M5.Lcd.setTextSize(TEXT_SIZE);
      M5.Lcd.setTextDatum(MC_DATUM);
      canvas_.setTextSize(TEXT_SIZE);
      canvas_.setTextColor(primaryColor, backgroundColor);
      canvas_.setTextDatum(MC_DATUM);
      M5.Lcd.setFont(font_);
      int textWidth = M5.Lcd.textWidth(text_.c_str());
      int textHeight = TEXT_HEIGHT * TEXT_SIZE;
      canvas_.fillEllipse(cx - 20, cy, textWidth + 2, textHeight * 2 + 2,
                          primaryColor);
      canvas_.fillTriangle(cx - 62, cy - 42, cx - 8, cy - 10, cx - 41, cy - 8,
                           primaryColor);
      canvas_.fillEllipse(cx - 20, cy, textWidth, textHeight * 2,
                          backgroundColor);
      canvas_.fillTriangle(cx - 60, cy - 40, cx - 10, cy - 10, cx - 40, cy - 10,
                           backgroundColor);
      canvas_.drawString(text_.c_str(), cx - textWidth / 6 - 15, cy, font_); // Continue printing from new x position

      canvas_.pushSprite(0, 0, TFT_TRANSPARENT); // top-left corner
      canvas_.deleteSprite();
    }
  };

} // namespace stackchan::avatar
