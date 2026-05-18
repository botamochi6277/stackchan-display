#include "Expression.h"

namespace stackchan::display
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

    void ExpressionWeight::setEmotionalExpression(Expression expression, unsigned char weight)
    {
        for (unsigned char i = 0; i < static_cast<unsigned char>(Expression::kRelax) + 1; i++)
        {
            if (static_cast<unsigned char>(expression) == i)
            {
                this->set(static_cast<Expression>(i), weight);
            }
            else
            {
                this->set(static_cast<Expression>(i), 0);
            }
        }
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