#pragma once

#include "../FacialDrawable.h"

namespace stackchan::display
{

  class Eye : public FacialDrawable
  {
  private:
    int radius_ = 16;
    bool is_left_ = true;
    m5::Vector2i gaze_{0, 0};
    m5::Vector2i iris_position_{0, 0};

  public:
    // constructor
    Eye() = delete;
    Eye(uint16_t r, bool isLeft);
    Eye(m5::Vector2i &position, uint16_t r, bool isLeft);
    Eye(int x, int y, uint16_t r, bool isLeft);

    virtual void draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette) override;
  };

} // namespace stackchan::display
