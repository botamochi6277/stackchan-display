#include "Expression.h"

namespace stackchan::avatar
{
    ExpressionWeight::ExpressionWeight()
    {
        // initialize all expression weights to 0
        for (unsigned char i = static_cast<unsigned char>(Expression::kNeutral); i < static_cast<unsigned char>(Expression::kCount); ++i)
        {
            weight_[static_cast<Expression>(i)] = 0;
        }
    }

    void ExpressionWeight::set(Expression expression, unsigned char weight)
    {
        auto itr = weight_.find(expression);
        if (itr != weight_.end())
        {
            itr->second = weight;
            return;
        }

        weight_.insert(std::make_pair(expression, weight));
    }

    unsigned char ExpressionWeight::get(Expression expression)
    {
        auto itr = weight_.find(expression);
        if (itr != weight_.end())
        {
            return itr->second;
        }
        return 0;
    }

    bool ExpressionWeight::contains(Expression expression) const
    {
        return weight_.find(expression) != weight_.end();
    }

    void ExpressionWeight::clear(void)
    {
        weight_.clear();
    }
}