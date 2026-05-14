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
    kRelax,
  };

  class ExpressionWeight
  {
  private:
    std::map<Expression, int> weight_;

  public:
    ExpressionWeight();
    void set(Expression expression, int weight);
    int get(Expression expression);
    bool contains(Expression expression) const;
    void clear(void);
  };

} // namespace stackchan::avatar
