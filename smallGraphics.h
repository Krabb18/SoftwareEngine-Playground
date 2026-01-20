#ifndef SMALLGRAPHICS_H
#define SMALLGRAPHICS_H

#include <iostream>
#include "stb_image.h"
#include "smallMath.h"

using namespace std;


class KrabbTex
{
private:
    unsigned char* pixels = nullptr;
    int windowWidth = 0;
    int windowHeight = 0;

public:
    int width = 0;
    int height = 0;
    int channels = 0;

    void LoadTexture(string path, int w, int h)
    {
        windowWidth = w;
        windowHeight = h;

        pixels = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if(pixels == nullptr)
        {
            std::cout << "Error loading image: " << stbi_failure_reason() << std::endl;
        }
    }

    vec3d GetPixelColor(float x, float y)
    {
        vec3d rgb;
        rgb.x = 0;
        rgb.y = 0;
        rgb.z = 0;

        int sx = x;
        int sy = y;
        if (sx >= 0 && sy >= 0 && sx < windowWidth && sy < windowHeight)
        {
            unsigned bytePerPixel = channels;
            unsigned char* pixelOffset = pixels + (sy * width + sx) * bytePerPixel;
            unsigned char r = pixelOffset[0];
            unsigned char g = pixelOffset[1];
            unsigned char b = pixelOffset[2];

            
            rgb.x = static_cast<int>(r);
            rgb.y = static_cast<int>(g);
            rgb.z = static_cast<int>(b);

            /*
            cout << rgb.x << endl;
            cout << rgb.y << endl;
            cout << rgb.z << endl;
            */
            
           
        }
        return rgb;
    }

    void Cleartexture()
    {
        stbi_image_free(pixels);
    }

};

uint32_t rgb_value(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return ((uint32_t)a << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

void TexturedTriangle(int x1, int y1, float u1, float v1, float w1,
    int x2, int y2, float u2, float v2, float w2,
    int x3, int y3, float u3, float v3, float w3,
    fenster& f,
    KrabbTex &tex
    )
{
    if(y2 < y1)
    {
        swap(y2, y1);
        swap(x2, x1);
        swap(u2, u1);
        swap(v2, v1);
        swap(w2, w1);
    }

    if(y3 < y2)
    {
        swap(y2, y3);
        swap(x2, x3);
        swap(u2, u3);
        swap(v2, v3);
        swap(w2, w3);
    }

    if(y3 < y1)
    {
        swap(y3, y1);
        swap(x3, x1);
        swap(u3, u1);
        swap(v3, v1);
        swap(w3, w1);
    }

    //Steigung der linie A
    int dy1 = y2 - y1;
    int dx1 = x2 - x1;
    float dv1 = v2 - v1;
    float du1 = u2 - u1;
    float dw1 = w2 - w1;


    //steigung der linie B
    int dy2 = y3 - y1;
    int dx2 = x3 - x1;
    float dv2 = v3 - v1;
    float du2 = u3 - u1;
    float dw2 = w3 - w1;

    float steps_aSide = 0, steps_bSide = 0, du1_step = 0, dv1_step = 0, du2_step = 0, dv2_step = 0;
    float tex_u, tex_v, tex_w;

    if (dy1) steps_aSide = dx1 / (float)abs(dy1);
    if (dy2) steps_bSide = dx2 / (float)abs(dy2);

    if (dy1) du1_step = du1 / (float)abs(dy1);
    if (dy1) dv1_step = dv1 / (float)abs(dy1);

    if (dy2) du2_step = du2 / (float)abs(dy2);
    if (dy2) dv2_step = dv2 / (float)abs(dy2);

    if(dy1)
    {
        //Gehe das dreieck runter
        for(int i = y1; i<=y2; i++)
        {
            //Berehcne von wo bis wo es auf der x ahsce geht
            int ax = x1 + (float)(i - y1) * steps_aSide;
            int bx = x1 + (float)(i - y1) * steps_bSide;
            
            //Acuh für die texture koordinaten
            float tex_su = u1 + (float)(i - y1) * du1_step;
            float tex_sv = v1 + (float)(i - y1) * dv1_step;

            float tex_eu = u1 + (float)(i - y1) * du2_step;
            float tex_ev = v1 + (float)(i - y1) * dv2_step;

            if (ax > bx)
            {
                swap(ax, bx);
                swap(tex_su, tex_eu);
                swap(tex_sv, tex_ev);
                //swap(tex_sw, tex_ew);
            }

            tex_u = tex_su;
            tex_v = tex_sv;

            float tstep = 1.0f / ((float)(bx - ax));
            float t = 0.0f;

            for(int j = ax; j<=bx; j++)
            {
                //Lineare Interpolation entlang der textur
                tex_u = (1.0f - t) * tex_su + t * tex_eu;
                tex_v = (1.0f - t) * tex_sv + t * tex_ev;

                vec3d rgb = tex.GetPixelColor(tex_u, tex_v);

                //Draw(j, i, tex->SampleGlyph(tex_u, tex_v), tex->SampleColour(tex_u, tex_v));
                fenster_pixel(&f, (int)j, (int)i) = rgb_value(rgb.x, rgb.y, rgb.z, 255);
                t += tstep;
            }
        }
    }


    //ZWEITE SEITE VOM DREIECK. AB DA WO DR GRADIENT SICH ÄNDERT
    dy1 = y3 - y2;
    dx1 = x3 - x2;
    dv1 = v3 - v2;
    du1 = u3 - u2;
    //dw1 = w3 - w2;

    if (dy1) steps_aSide = dx1 / (float)abs(dy1);
    if (dy2) steps_bSide = dx2 / (float)abs(dy2);

    du1_step = 0, dv1_step = 0;
    if (dy1) du1_step = du1 / (float)abs(dy1);
    if (dy1) dv1_step = dv1 / (float)abs(dy1);

    if(dy1)
    {
        for(int i = y2; i<= y3; i++)
        {
            int ax = x2 + (float)(i - y2) * steps_aSide;
            int bx = x1 + (float)(i - y1) * steps_bSide;

            float tex_su = u2 + (float)(i - y2) * du1_step;
            float tex_sv = v2 + (float)(i - y2) * dv1_step;
            //float tex_sw = w2 + (float)(i - y2) * dw1_step;

            float tex_eu = u1 + (float)(i - y1) * du2_step;
            float tex_ev = v1 + (float)(i - y1) * dv2_step;

            if(ax > bx)
            {
                swap(ax, bx);
                swap(tex_su, tex_eu);
                swap(tex_sv, tex_ev);
            }

            tex_u = tex_su;
            tex_v = tex_sv;

            float tstep = 1.0f / ((float)(bx - ax));
            float t = 0.0f;

            for(int j = ax; j<bx; j++)
            {
                tex_u = (1.0f - t) * tex_su + t * tex_eu;
                tex_v = (1.0f - t) * tex_sv + t * tex_ev;

                vec3d rgb = tex.GetPixelColor(tex_u, tex_v);

                //Draw(j, i, tex->SampleGlyph(tex_u, tex_v), tex->SampleColour(tex_u, tex_v));
                fenster_pixel(&f, (int)j, (int)i) = rgb_value(rgb.x, rgb.y, rgb.z, 255);
                t += tstep;
            }
        }
        
    }
}

#endif // !SMALLGRAPHICS_H
