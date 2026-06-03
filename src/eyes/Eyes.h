// NOTE: migrated from https://github.com/botamochi6277/m5stack-avatar/blob/master/src/Eyes.hpp
#pragma once

#include "../faces/FacialDrawable.h"
#include "DrawingUtils.h"

namespace stackchan::display
{

  // pure drawing functions
  void drawStraightEyelid(M5Canvas &canvas, int16_t x, int16_t y, int16_t width,
                          int16_t height, uint16_t color);

  void updateTriangleEyelidPoints(m5::Vector2i &center, m5::Size2i &size, bool is_left,
                                  ExpressionWeight &expression_weight,
                                  m5::Vector2i &p1, m5::Vector2i &p2, m5::Vector2i &p3);

  /**
   * @brief Base class for Eye, draw nothing
   *
   */
  class BaseEye : public FacialDrawable
  {
  protected:
    // common state values for eyes
    bool is_left_;
    m5::Vector2i gaze_{0, 0};
    m5::Vector2i iris_position_{0, 0};

    unsigned long milli_sec_ = 0;
    unsigned long last_saccade_millis_ = 0;
    uint32_t saccade_interval_ = 1000;
    m5::Vector2i saccade_noise_ = m5::Vector2i();

    void updateGaze(ExpressionWeight &weight, uint16_t range);
    void updateSaccade(ExpressionWeight &expression_weight);
    float calculateOpenRatio(ExpressionWeight &expression_weight);

  public:
    using FacialDrawable::FacialDrawable;
    BaseEye(int16_t x, int16_t y, int16_t width, int16_t height, bool is_left);
  };

  class Eye : public BaseEye
  {
  private:
    int radius_ = 16;

  public:
    using BaseEye::BaseEye;

    virtual void draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette) override;
  };

  class EllipseEye : public BaseEye
  {
  public:
    using BaseEye::BaseEye;
    virtual void draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette) override;
  };

  class RoundRectEye : public BaseEye
  {
  public:
    using BaseEye::BaseEye;
    virtual void draw(M5Canvas &canvas, ExpressionWeight &expression_weight, ColorPalette &palette) override;
  };

} // namespace stackchan::display
