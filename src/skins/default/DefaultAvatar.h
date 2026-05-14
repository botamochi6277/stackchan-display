#pragma once
#include "../../Avatar.h"

namespace stackchan::avatar
{
    // M5StackChan default face
    class DefaultAvatar : public Avatar
    {
    public:
        DefaultAvatar();
        virtual void update() override;
    };

    class DefaultEye : public Drawable
    {
    public:
        DefaultEye();
        virtual void update() override;
    };
} // namespace stackchan::avatar