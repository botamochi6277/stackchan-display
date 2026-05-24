
#include "Display.h"
#include "ColorPalette.h"
#include "Expression.h"
#include "SpeechBalloon.h"
#include <M5Unified.h>

stackchan::display::Display stackchan_display;

unsigned int count = 0;
unsigned char emotion = 0;

stackchan::display::Face *faces[3];
const uint8_t num_faces = sizeof(faces) / sizeof(stackchan::display::Face *);
uint8_t face_idx = 0; // face index

void setSpeechText(stackchan::display::Display &display);

void setup()
{
  M5.begin();
  M5.Lcd.setBrightness(150);
  M5.Lcd.clear();

  faces[0] = stackchan_display.getFace(); // native face
  faces[1] = new stackchan::display::EllFace();
  faces[2] = new stackchan::display::OmegaFace();

  stackchan_display.getCanvas().setColorDepth(1);
  stackchan_display.getSpeechBalloon().setText("Hello, StackChan!");
}

void loop()
{
  M5.update();
  // M5Stack Core's button layout:
  // -----------
  // |         |
  // |         |
  // -----------
  // [A] [B] [C]

  if (M5.BtnA.wasPressed())
  {
    emotion++;
    stackchan_display.getExpressionWeight().setEmotionalExpression(
        static_cast<stackchan::display::Expression>(emotion % (static_cast<int>(stackchan::display::Expression::kRelax) + 1)), 255);

    if (face_idx == 0)
    {
      setSpeechText(stackchan_display);
    }
  }

  if (M5.BtnB.wasPressed())
  {
    face_idx = (face_idx + 1) % num_faces; // loop index
    stackchan_display.setFace(faces[face_idx]);
    if (face_idx != 0)
    {
      stackchan_display.getSpeechBalloon().setText("");
    }
  }

  stackchan_display.getExpressionWeight().set(stackchan::display::Expression::kAa, (sin(count / 10.0f) + 1.0f) / 2.0f * 255.0f);
  stackchan_display.update();

  count++;
  M5.delay(33); // approx. 30 FPS
}

void setSpeechText(stackchan::display::Display &display)
{
  using stackchan::display::Expression;
  auto weight = display.getExpressionWeight();

  if (weight.get(Expression::kAngry) > 127)
  {
    display.getSpeechBalloon().setText("...");
  }

  if (weight.get(Expression::kSad) > 127)
  {
    display.getSpeechBalloon().setText("Oh...");
  }

  if (weight.get(Expression::kDoubt) > 127)
  {
    display.getSpeechBalloon().setText("???");
  }

  if (weight.get(Expression::kSleepy) > 127)
  {
    display.getSpeechBalloon().setText("");
  }

  if (weight.get(Expression::kSmile) > 127)
  {
    display.getSpeechBalloon().setText("~~~");
  }

  if (weight.get(Expression::kGrin) > 127)
  {
    display.getSpeechBalloon().setText("");
  }

  if (weight.get(Expression::kSurprised) > 127)
  {
    display.getSpeechBalloon().setText("!!");
  }

  if (weight.get(Expression::kUpset) > 127)
  {
    display.getSpeechBalloon().setText("!?");
  }

  if (weight.get(Expression::kRelax) > 127)
  {
    display.getSpeechBalloon().setText("");
  }
}
