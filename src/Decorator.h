#pragma once

#include "Drawable.h"

namespace stackchan::display
{
    void drawBubble(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t d,
                    uint16_t color, float offset)
    {
        uint32_t r = d / 2; // radius
        r = r + floor(r * 0.2 * offset);
        canvas.drawCircle(x, y, r, color);                         // outer circle
        canvas.drawCircle(x - (r / 4), y - (r / 4), r / 4, color); // inner circle
    }

    void drawBubble(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t d,
                    uint16_t color)
    {
        drawBubble(canvas, x, y, d, color, 0);
    }

    void drawWaterDrop(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t d,
                       uint16_t color, float offset)
    {
        uint32_t r = d / 2; // radius
        y = y + floor(5 * offset);
        r = r + floor(r * 0.2 * offset);
        canvas.fillCircle(x, y, r, color);
        uint32_t a = (sqrt(3) * r) / 2;
        canvas.fillTriangle(x, y - r * 2, x - a, y - r * 0.5, x + a, y - r * 0.5,
                            color);
    }

    void drawWaterDrop(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t d,
                       uint16_t color)
    {
        drawWaterDrop(canvas, x, y, d, color, 0);
    }

    void drawPaleMark(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t d,
                      uint16_t color, float offset)
    {
        uint32_t r = d / 2; // radius
        uint32_t h = r + abs(r * 0.2 * offset);
        canvas.fillRect(x - (r / 2), y, 3, h / 2, color);
        canvas.fillRect(x, y, 3, h * 3 / 4, color);
        canvas.fillRect(x + (r / 2), y, 3, h, color);
    }

    void drawPaleMark(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t d,
                      uint16_t color)
    {
        drawPaleMark(canvas, x, y, d, color, 0);
    }

    void drawAngerMark(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t d,
                       uint16_t color, float offset)
    {
        uint32_t r = d / 2; // radius
        r = r + abs(r * 0.4 * offset);

        // Four L-shaped brackets around an open cross, drawn with lines only.
        //  | |
        // _| |_
        //
        // ‾| |‾
        //  | |
        const int32_t g = r / 3; // half width of the open cross
        for (int sx = -1; sx <= 1; sx += 2)
        {
            for (int sy = -1; sy <= 1; sy += 2)
            {
                int32_t corner_x = x + sx * g;
                int32_t corner_y = y + sy * g;
                // 2 px stroke: the second line is shifted away from the center to keep the gap width
                for (int w = 0; w < 2; w++)
                {
                    canvas.drawLine(corner_x + sx * w, corner_y, corner_x + sx * w, y + sy * (int32_t)r, color); // vertical edge
                    canvas.drawLine(corner_x, corner_y + sy * w, x + sx * (int32_t)r, corner_y + sy * w, color); // horizontal edge
                }
            }
        }
    }

    void drawAngerMark(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t d,
                       uint16_t color)
    {
        drawAngerMark(canvas, x, y, d, color, 0);
    }

    /**
     * @brief Draw a cheek blush as three slanted lines `///`
     *
     * @param canvas canvas to draw on
     * @param x center x of the blush [px]
     * @param y center y of the blush [px]
     * @param d diameter of the blush [px]
     * @param color color of the lines
     * @param offset 0 is the normal size. Larger values make the blush bigger.
     */
    void drawBlush(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t d,
                   uint16_t color, float offset)
    {
        int32_t r = d / 2; // radius
        r = r + floor(r * 0.2 * offset);
        int32_t pitch = r / 2; // distance between the lines
        for (int i = -1; i <= 1; i++)
        {
            int32_t cx = x + i * pitch;
            // 2 px stroke, from bottom-left to top-right
            for (int w = 0; w < 2; w++)
            {
                canvas.drawLine(cx - r / 4 + w, y + r / 2, cx + r / 4 + w, y - r / 2, color);
            }
        }
    }

    void drawBlush(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t d,
                   uint16_t color)
    {
        drawBlush(canvas, x, y, d, color, 0);
    }

    void drawHeart(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t d,
                   uint16_t color, float offset)
    {
        uint32_t r = d / 2; // radius
        r = r + floor(r * 0.4 * offset);
        canvas.fillCircle(x - r / 2, y, r / 2, color);
        canvas.fillCircle(x + r / 2, y, r / 2, color);
        float a = (sqrt(2) * r) / 4.0;
        canvas.fillTriangle(x, y, x - r / 2 - a, y + a, x + r / 2 + a, y + a, color);
        canvas.fillTriangle(x, y + (r / 2) + 2 * a, x - r / 2 - a, y + a,
                            x + r / 2 + a, y + a, color);
    }

    void drawHeart(M5Canvas &canvas, uint32_t x, uint32_t y, uint32_t d,
                   uint16_t color)
    {
        drawHeart(canvas, x, y, d, color, 0);
    }

    // TODO: Implement simple cheek blush decorator, which can be drawn as a simple ellipse on the cheeks of the avatar. The color and size of the blush can be adjusted based on the expression weight and color palette.

    // TODO: Implement cheek blush decorator with 3 lines

    // TODO: Implement cheek blush decorator like m5stack/StackChan

}