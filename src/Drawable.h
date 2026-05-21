#pragma once
#include <M5GFX.h>
#include <M5Unified.h>

#include "Vector.h"

// reference:
// https://github.com/m5stack/StackChan/blob/main/firmware/main/stackchan/avatar/avatar/elements/element.h

namespace stackchan::display
{
    template <typename T>
    T clamp(T value, T min, T max)
    {
        if (value < min)
        {
            return min;
        }
        if (value > max)
        {
            return max;
        }
        return value;
    }

    class Drawable
    {
    protected:
        m5::Vector2i position_{0, 0}; // in pixels, with positive x to the right and positive y downwards
        int rotation_ = 0;            // in degrees, counter-clockwise
        m5::Size2i size_{100, 100};   // width and height, in pixels
        bool visible_ = true;
        bool ignore_expression_ = false;

    public:
        Drawable() = default;
        Drawable(int16_t x, int16_t y, int16_t width, int16_t height) : position_(x, y), size_(width, height) {};

        virtual ~Drawable() = default;

        /**
         * @brief set primary position
         *
         * @param position
         */
        virtual void setPosition(const m5::Vector2i &position)
        {
            position_ = position;
            position_.clamp({-100, -100}, {100, 100});
        }
        virtual m5::Vector2i &getPosition()
        {
            return position_;
        }

        /**
         * @brief
         *
         * @param rotation
         */
        virtual void setRotation(int rotation)
        {
            rotation_ = rotation;
        }
        /**
         * @brief Get the Rotation in degrees, counter-clockwise
         *
         * @return int
         */
        virtual int getRotation()
        {
            return rotation_;
        }

        virtual void setSize(const m5::Size2i &size)
        {
            size_ = size;
            size_.width = clamp(size_.width, 1, 200);
            size_.height = clamp(size_.height, 1, 200);
        }

        virtual m5::Size2i &getSize()
        {
            return size_;
        }

        virtual void setVisible(bool visible)
        {
            visible_ = visible;
        }
        virtual bool getVisible()
        {
            return visible_;
        }
        virtual bool isVisible()
        {
            return visible_;
        }

        virtual void setIgnoreExpression(bool ignore)
        {
            ignore_expression_ = ignore;
        }
        virtual bool getIgnoreExpression()
        {
            return ignore_expression_;
        }
        virtual bool isIgnoreExpression()
        {
            return ignore_expression_;
        }

        virtual void draw(M5Canvas &canvas) {}

        virtual void update() {}
    };

}
