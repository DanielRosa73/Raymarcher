#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H


#include <string>
#include "stb_image.h"
#include "color.h"

class ImageTexture {
public:
    ImageTexture(const std::string& filename) {
        int width, height, channels;
        unsigned char *img = stbi_load(filename.c_str(), &width, &height, &channels, 0);
        if (img) {
            m_width = width;
            m_height = height;
            m_channels = channels;
            m_data = img;
        }
        else {
            throw std::runtime_error("Failed to load image");
        }
    }

    ~ImageTexture() {
        stbi_image_free(m_data);
    }

    Color getColor(float u, float v) const  {
        if (u < 0) u = 0;
        if (v < 0) v = 0;
        if (u > 1) u = 1;
        if (v > 1) v = 1;

        int x = u * (m_width - 1);
        int y = (1 - v) * (m_height - 1);  // Flip v so 0 is at the bottom

        int index = (y * m_width + x) * m_channels;

        float r = m_data[index] / 255.0f;
        float g = m_data[index + 1] / 255.0f;
        float b = m_data[index + 2] / 255.0f;

        return Color(r, g, b);
    }

private:
    int m_width, m_height, m_channels;
    unsigned char* m_data;
};

#endif // IMAGE_TEXTURE_H
