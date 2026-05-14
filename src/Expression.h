#pragma once
namespace stackchan::avatar
{
  // facial expression keys
  // NOTE what is difference between expression & emotion?
  // NOTE: https://vrm.dev/en/univrm1/vrm1_tutorial/expression/
  enum class Expression : unsigned char
  {
    kNeutral = 0,
    kHappy,
    kAngry,
    kSad,
    kDoubt,
    kSleepy,
    kSmile,
    kLaugh,
    kSurprised,
    kRelax,
  };
} // namespace stackchan::avatar
