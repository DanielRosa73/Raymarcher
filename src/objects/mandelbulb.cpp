#include "mandelbulb.h"

Mandelbulb::Mandelbulb() 
    : m_center(Vector3(0.0f, 0.0f, 0.0f)), m_scale(1.0f), m_power(8) {
        this->m_color = Color(1,0,1);
        this->m_material = Material();
    }

Mandelbulb::Mandelbulb(const Vector3& center, float scale, int power, const Color& color, const Material& mat) 
    : m_center(center), m_scale(scale), m_power(power) {
        this->m_color = color;
        this->m_material = mat;
    }

float Mandelbulb::SDF(const Vector3& point) const {
        Vector3 z = (point - m_center) / m_scale;
        Vector3 c = z;
        float dr = 1.0;
        float r = 0.0;
        for (int i = 0; i < 1000 ; i++) {
            r = z.length();
            if (r>2) break;
        
            
            float theta = acos(z.z/r);
            float phi = atan2(z.y,z.x);
            dr =  pow( r, m_power -1.0)*m_power*dr + 1.0;
            
          
            float zr = pow( r,m_power);
            theta = theta*m_power;
            phi = phi*m_power;
            
            
            z = zr*Vector3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
            z+=c;
        }
        return 0.5*log(r)*r/dr;
}