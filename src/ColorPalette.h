#pragma once
#include <M5Unified.h>

#include <map>
#include <string>

namespace stackchan::display
{

  /**
   * @brief enum for palette key to set location of drawing
   *
   * google guide for enum naming:
   * https://google.github.io/styleguide/cppguide.html#Enumerator_Names
   */
  enum class DrawingLocation : unsigned char
  {
    // Skin
    kSkin = 0, // required
               // Eye
    kPupil,
    kEyelash,
    kEyelid,
    kIris1,
    kIris2,
    kIrisBackground, // required
    kEyeHighlight,
    kSclera,
    kScleraBackground,
    // Eyebrow
    kEyeBrow,
    // Mouth
    kMouthBackground,
    kInnerMouth,
    kTongue,
    // Cheek
    kCheek1,
    kCheek2,
    // Chat Balloon
    kBalloonForeground,
    kBalloonBackground
  };

  /**
   * Color palette for drawing face
   */
  class ColorPalette
  {
  private:
    std::map<DrawingLocation, uint16_t> palette_;

  public:
    // TODO(meganetaaan): constructor with color settings
    ColorPalette();
    ~ColorPalette() = default;
    ColorPalette(const ColorPalette &other) = default;
    ColorPalette &operator=(const ColorPalette &other) = default;

    uint16_t get(DrawingLocation key) const;
    void set(DrawingLocation key, uint16_t value);
    bool contains(DrawingLocation key);
    void clear(void);
  };
} // namespace stackchan::display

// defines for backward compatibility
#define COLOR_PRIMARY stackchan::display::DrawingLocation::kIris1
#define COLOR_SECONDARY stackchan::display::DrawingLocation::kIrisBackground
#define COLOR_BACKGROUND stackchan::display::DrawingLocation::kSkin
#define COLOR_BALLOON_FOREGROUND stackchan::display::DrawingLocation::kBalloonForeground
#define COLOR_BALLOON_BACKGROUND stackchan::display::DrawingLocation::kBalloonBackground
