#pragma once

#include "Drawable.h"

namespace stackchan::avatar
{
    void drawBubble(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t r,
                    uint16_t color, float offset)
    {
        r = r + floor(r * 0.2 * offset);
        canvas.drawCircle(x, y, r, color);                         // outer circle
        canvas.drawCircle(x - (r / 4), y - (r / 4), r / 4, color); // inner circle
    }

    void drawBubble(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t r,
                    uint16_t color)
    {
        drawBubble(canvas, x, y, r, color, 0);
    }

    void drawWaterDrop(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t r,
                       uint16_t color, float offset)
    {
        y = y + floor(5 * offset);
        r = r + floor(r * 0.2 * offset);
        canvas.fillCircle(x, y, r, color);
        uint32_t a = (sqrt(3) * r) / 2;
        canvas.fillTriangle(x, y - r * 2, x - a, y - r * 0.5, x + a, y - r * 0.5,
                            color);
    }

    void drawWaterDrop(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t r,
                       uint16_t color)
    {
        drawWaterDrop(canvas, x, y, r, color, 0);
    }

    void drawPaleMark(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t r,
                      uint16_t color, float offset)
    {
        uint32_t h = r + abs(r * 0.2 * offset);
        canvas.fillRect(x - (r / 2), y, 3, h / 2, color);
        canvas.fillRect(x, y, 3, h * 3 / 4, color);
        canvas.fillRect(x + (r / 2), y, 3, h, color);
    }

    void drawPaleMark(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t r,
                      uint16_t color)
    {
        drawPaleMark(canvas, x, y, r, color, 0);
    }

    void drawAngerMark(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t r,
                       uint16_t color, uint16_t background_color, float offset)
    {
        r = r + abs(r * 0.4 * offset);
        canvas.fillRect(x - (r / 3), y - r, (r * 2) / 3, r * 2, color);
        canvas.fillRect(x - r, y - (r / 3), r * 2, (r * 2) / 3, color);
        canvas.fillRect(x - (r / 3) + 2, y - r, ((r * 2) / 3) - 4, r * 2, background_color);
        canvas.fillRect(x - r, y - (r / 3) + 2, r * 2, ((r * 2) / 3) - 4, background_color);
    }

    void drawAngerMark(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t r,
                       uint16_t color, uint16_t background_color)
    {
        drawAngerMark(canvas, x, y, r, color, background_color, 0);
    }

    void drawHeart(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t r,
                   uint16_t color, float offset)
    {
        r = r + floor(r * 0.4 * offset);
        canvas.fillCircle(x - r / 2, y, r / 2, color);
        canvas.fillCircle(x + r / 2, y, r / 2, color);
        float a = (sqrt(2) * r) / 4.0;
        canvas.fillTriangle(x, y, x - r / 2 - a, y + a, x + r / 2 + a, y + a, color);
        canvas.fillTriangle(x, y + (r / 2) + 2 * a, x - r / 2 - a, y + a,
                            x + r / 2 + a, y + a, color);
    }

    void drawHeart(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t r,
                   uint16_t color)
    {
        drawHeart(canvas, x, y, r, color, 0);
    }

}