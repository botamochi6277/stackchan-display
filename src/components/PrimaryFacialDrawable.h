#pragma once

#include "Drawable.h"
#include <functional>
#include <memory>

namespace stackchan::avatar
{
    // For shishikawa classc face, the primary facial components include left eye, right eye and mouth.
    struct PrimaryFacialDrawables_t
    {
        std::unique_ptr<Drawable> left_eye;
        std::unique_ptr<Drawable> right_eye;
        std::unique_ptr<Drawable> mouth;

        // TODO: add eyebrows, speech bubble etc.

        virtual void forEachComponent(const std::function<void(Drawable &component)> &func)
        {
            if (left_eye)
                func(*left_eye);
            if (right_eye)
                func(*right_eye);
            if (mouth)
                func(*mouth);
        }
    };

}