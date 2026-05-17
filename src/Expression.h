#pragma once

#include <map>
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
    kUpset,
    kRelax,
    // eye expressions
    kBlink,
    kLeftBlink,
    kRightBlink,
    // mouth expressions
    kAa,
    // misc expressions
    kBreath,
    // TODO: add more expressions
    kCount,
  };

  class ExpressionWeight
  {
  private:
    std::map<Expression, unsigned char> weight_;

  public:
    ExpressionWeight();
    void set(Expression expression, unsigned char weight);
    void setEmotionalExpression(Expression expression, unsigned char weight);
    unsigned char get(Expression expression);
    bool contains(Expression expression) const;
    void clear(void);
  };

} // namespace stackchan::avatar
