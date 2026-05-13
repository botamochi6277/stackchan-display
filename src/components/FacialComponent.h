#pragma once
#include <M5Unified.h>

#include "Vector.h"

// reference:
// https://github.com/m5stack/StackChan/blob/main/firmware/main/stackchan/avatar/avatar/elements/element.h

namespace stackchan::avatar
{
    class FacialComponent
    {
    protected:
        m5::Vector2i _position{0, 0};
        int _rotation = 0; // in degrees, counter-clockwise
        bool _visible = true;
        bool _ignore_expression = false;

    public:
        virtual ~FacialComponent() = default;
        /**
         * @brief (-100 ~ 100, -100 ~ 100)
         *
         * @param position
         */
        virtual void setPosition(const m5::Vector2i &position)
        {
            _position = position;
            _position.clamp({-100, -100}, {100, 100});
        }
        virtual m5::Vector2i getPosition()
        {
            return _position;
        }

        /**
         * @brief 0~3600
         *
         * @param rotation
         */
        virtual void setRotation(int rotation)
        {
            _rotation = constrain(rotation, 0, 3600);
        }
        /**
         * @brief Get the Rotation in degrees, counter-clockwise
         *
         * @return int
         */
        virtual int getRotation()
        {
            return _rotation;
        }

        virtual void setExpression(int expression, int weight) {}
        virtual int getExpression(int &expression) { return 0; }

        virtual void setVisible(bool visible)
        {
            _visible = visible;
        }
        virtual bool getVisible()
        {
            return _visible;
        }
        virtual bool isVisible()
        {
            return _visible;
        }

        virtual void setIgnoreExpression(bool ignore)
        {
            _ignore_expression = ignore;
        }
        virtual bool getIgnoreExpression()
        {
            return _ignore_expression;
        }
        virtual bool isIgnoreExpression()
        {
            return _ignore_expression;
        }

        virtual void update() {}
    };

}