#pragma once
#include <M5Unified.h>

// NOTE: This is wheel-reinventing, but for simplicity and to avoid extra dependencies, we define our own simple Vector2i class here.

namespace m5
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

    template <typename T>
    T remap(T x, T in_min, T in_max, T out_min, T out_max, bool clamp = false)
    {
        if (clamp)
        {
            if (x < in_min)
            {
                return out_min;
            }
            if (in_max < x)
            {
                return out_max;
            }
        }

        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    template <typename T>
    T max(T a, T b)
    {
        return a > b ? a : b;
    };

    template <typename T>
    T max(T a, T b, T c)
    {
        return max(max(a, b), c);
    };

    template <typename T>
    T max(T a, T b, T c, T d)
    {
        return max(max(a, b), max(c, d));
    };

    class Vector2i
    {
    public:
        int x;
        int y;

        Vector2i() : x(0), y(0) {}
        Vector2i(int x, int y) : x(x), y(y) {}

        Vector2i operator+(const Vector2i &other) const
        {
            return Vector2i(x + other.x, y + other.y);
        }

        Vector2i operator-(const Vector2i &other) const
        {
            return Vector2i(x - other.x, y - other.y);
        }

        template <typename T>
        Vector2i operator*(T scalar) const
        {
            return Vector2i(x * scalar, y * scalar);
        }

        template <typename T>
        Vector2i operator/(T scalar) const
        {
            return Vector2i(x / scalar, y / scalar);
        }

        Vector2i &operator+=(const Vector2i &other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector2i &operator-=(const Vector2i &other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vector2i &operator*=(int scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        Vector2i &operator/=(int scalar)
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        bool operator==(const Vector2i &other) const
        {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Vector2i &other) const
        {
            return !(*this == other);
        }

        int dot(const Vector2i &other) const
        {
            return x * other.x + y * other.y;
        }

        void clamp(const Vector2i &min, const Vector2i &max)
        {
            if (x < min.x)
            {
                x = min.x;
            }
            else if (x > max.x)
            {
                x = max.x;
            }

            if (y < min.y)
            {
                y = min.y;
            }
            else if (y > max.y)
            {
                y = max.y;
            }
        }
    };

    class Size2i
    {
    public:
        int width;
        int height;

        Size2i() : width(0), height(0) {}
        Size2i(int width, int height) : width(width), height(height) {}

        void operator*=(int scalar)
        {
            width *= scalar;
            height *= scalar;
        }

        template <typename T>
        Size2i operator*(T scalar) const
        {
            return Size2i(width * scalar, height * scalar);
        }

        int area() const
        {
            return width * height;
        }

        int max() const
        {
            return width > height ? width : height;
        }

        int min() const
        {
            return width < height ? width : height;
        }
    };
}
