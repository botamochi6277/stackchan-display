
#include "Display.h"
#include "ColorPalette.h"
#include "Expression.h"
#include "SpeechBalloon.h"
#include <M5Unified.h>

stackchan::display::Display stackchan_display;

unsigned int count = 0;
unsigned char emotion = 0;

stackchan::display::Face *faces[6];
const uint8_t num_faces = sizeof(faces) / sizeof(
                                              stackchan::display::Face *);
uint8_t face_idx = 0; // face index

stackchan::display::ColorPalette *color_palettes[2];
const int num_palettes = sizeof(color_palettes) / sizeof(stackchan::display::ColorPalette *);
int palette_idx = 0;

void setSpeechText(stackchan::display::Display &display);

void setup()
{
  M5.begin();
  M5.Lcd.setBrightness(150);
  M5.Lcd.clear();

  faces[0] = stackchan_display.getFace(); // native face
  faces[1] = new stackchan::display::ToonFace1();
  faces[2] = new stackchan::display::ToonFace2();
  faces[3] = new stackchan::display::EllFace();
  faces[4] = new stackchan::display::OmegaFace();
  faces[5] = new stackchan::display::RectFace();

  color_palettes[0] = new stackchan::display::ColorPalette();
  color_palettes[1] = new stackchan::display::ColorPalette();
  color_palettes[1]->set(stackchan::display::DrawingLocation::kSkin, M5.Lcd.color24to16(0xfac2a8));           // light skin tone
  color_palettes[1]->set(stackchan::display::DrawingLocation::kIris1, M5.Lcd.color565(65, 105, 225));         // cornflower blue
  color_palettes[1]->set(stackchan::display::DrawingLocation::kIris2, M5.Lcd.color565(100, 149, 237));        // royal blue
  color_palettes[1]->set(stackchan::display::DrawingLocation::kPupil, M5.Lcd.color565(0, 0, 0));              // black
  color_palettes[1]->set(stackchan::display::DrawingLocation::kEyeHighlight, M5.Lcd.color565(255, 255, 255)); // white
  color_palettes[1]->set(stackchan::display::DrawingLocation::kSclera, M5.Lcd.color565(255, 255, 255));       // white
  // color_palettes[1]->set(stackchan::display::DrawingLocation::kEyelid, M5.Lcd.color565(200, 200, 200));            // light gray
  color_palettes[1]->set(stackchan::display::DrawingLocation::kEyelash, TFT_BROWN);
  color_palettes[1]->set(stackchan::display::DrawingLocation::kMouthBackground, M5.Lcd.color565(0, 0, 0));         // black
  color_palettes[1]->set(stackchan::display::DrawingLocation::kTongue, M5.Lcd.color565(255, 105, 180));            // hot pink
  color_palettes[1]->set(stackchan::display::DrawingLocation::kInnerMouth, M5.Lcd.color565(255, 182, 193));        // light pink
  color_palettes[1]->set(stackchan::display::DrawingLocation::kBalloonForeground, M5.Lcd.color565(255, 255, 224)); // light yellow
  color_palettes[1]->set(stackchan::display::DrawingLocation::kCheek1, M5.Lcd.color565(255, 182, 193));            // light pink
  stackchan_display.getFace()->autoScale();

  stackchan_display.getCanvas().setColorDepth(8);
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

  if (M5.BtnC.wasPressed())
  {
    palette_idx = (palette_idx + 1) % num_palettes; // loop index
    stackchan_display.setColorPalette(color_palettes[palette_idx]);
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

  if (weight.get(Expression::kNeutral) > 127)
  {
    display.getSpeechBalloon().setText("neutral");
  }

  if (weight.get(Expression::kAngry) > 127)
  {
    display.getSpeechBalloon().setText("angry");
  }

  if (weight.get(Expression::kSad) > 127)
  {
    display.getSpeechBalloon().setText("sad");
  }

  if (weight.get(Expression::kDoubt) > 127)
  {
    display.getSpeechBalloon().setText("doubt");
  }

  if (weight.get(Expression::kSleepy) > 127)
  {
    display.getSpeechBalloon().setText("sleepy");
  }

  if (weight.get(Expression::kSmile) > 127)
  {
    display.getSpeechBalloon().setText("smile");
  }

  if (weight.get(Expression::kGrin) > 127)
  {
    display.getSpeechBalloon().setText("grin");
  }

  if (weight.get(Expression::kSurprised) > 127)
  {
    display.getSpeechBalloon().setText("!!(surprised)");
  }

  if (weight.get(Expression::kUpset) > 127)
  {
    display.getSpeechBalloon().setText("upset");
  }

  if (weight.get(Expression::kRelax) > 127)
  {
    display.getSpeechBalloon().setText("relaxed");
  }
}
