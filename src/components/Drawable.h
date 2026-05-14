#pragma once
#include <M5GFX.h>
#include <M5Unified.h>

#include "Vector.h"

// reference:
// https://github.com/m5stack/StackChan/blob/main/firmware/main/stackchan/avatar/avatar/elements/element.h

namespace stackchan::avatar
{

    class Drawable
    {
    protected:
        m5::Vector2i _position{0, 0};
        int _rotation = 0;          // in degrees, counter-clockwise
        m5::Size2i _size{100, 100}; // width and height, in pixels
        bool _visible = true;
        bool _ignore_expression = false;

    public:
        virtual ~Drawable() = default;
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

        virtual void setSize(const m5::Size2i &size)
        {
            _size = size;
            _size.width = constrain(_size.width, 1, 200);
            _size.height = constrain(_size.height, 1, 200);
        }

        virtual m5::Size2i getSize()
        {
            return _size;
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

        virtual void draw(M5Canvas &canvas) {}

        virtual void update() {}
    };

}