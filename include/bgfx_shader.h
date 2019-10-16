// Copyright (c) 2019 Petr Petrov
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <cmath>
#include <iostream>
#include <sstream>

#ifdef VERTEX_SHADER
#define main vertex_shader_main
#endif

#ifdef FRAGMENT_SHADER
#define main fragment_shader_main
#endif

// http://www.opengl.org/registry/doc/GLSLangSpec.Full.1.20.8.pdf (pages 49-50)
const int gl_MaxLights = 8;                    // GL 1.0
const int gl_MaxClipPlanes = 6;                // GL 1.0
const int gl_MaxTextureUnits = 2;              // GL 1.3
const int gl_MaxTextureCoords = 2;             // ARB_fragment_program
const int gl_MaxVertexAttribs = 16;            // ARB_vertex_shader
const int gl_MaxVertexUniformComponents = 512; // ARB_vertex_shader
const int gl_MaxVaryingFloats = 32;            // ARB_vertex_shader
const int gl_MaxVertexTextureImageUnits = 0;   // ARB_vertex_shader
const int gl_MaxCombinedTextureImageUnits = 2; // ARB_vertex_shader
const int gl_MaxTextureImageUnits = 2;         // ARB_fragment_shader
const int gl_MaxFragmentUniformComponents = 64;// ARB_fragment_shader
const int gl_MaxDrawBuffers = 1;               // proposed ARB_draw_buffers

const float pi = 3.141592653589793f;

struct vec3;
struct vec4;

struct vec2
{
    union
    {
        float x;
        float u;
    };
    union
    {
        float y;
        float v;
    };

    vec2()
    {
        // components are not initialized intentionally
    }

    vec2(float x_, float y_)
    {
        x = x_; y = y_;
    }

    vec2(const vec2& v)
    {
        x = v.x; y = v.y;
    }

    float operator[](int i) const
    {
        switch (i)
        {
        case 0: return x;
        case 1: return y;
        }
        std::cerr << "Out of bounds index " << i << " on vec2" << std::endl;
        return (*this)[i % 2];
    }

    float& operator[](int i)
    {
        switch (i)
        {
        case 0: return x;
        case 1: return y;
        }
        std::cerr << "Out of bounds index " << i << " on vec2" << std::endl;
        return (*this)[i % 2];
    }

    bool operator==(vec2 v) const
    {
        return v.x == x && v.y == y;
    }

    bool operator!=(vec2 v) const
    {
        return v.x != x || v.y != y;
    }

    vec2 operator-() const
    {
        return vec2(-x, -y);
    }

    std::string to_s() const
    {
        std::stringstream s;
        s << "(" << x << "," << y << ")";
        return s.str();
    }

    inline vec2(const vec3&);
    inline vec2(const vec4&);
};

struct vec3
{
    union
    {
        float x;
        float r;
    };
    union
    {
        float y;
        float g;
    };
    union
    {
        float z;
        float b;
    };

    vec3()
    {
        // components are not initialized intentionally
    }

    vec3(float x_, float y_, float z_)
    {
        x = x_; y = y_; z = z_;
    }

    vec3(const vec3& v)
    {
        x = v.x; y = v.y; z = v.z;
    }

    float operator[](int i) const
    {
        switch (i)
        {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        }
        std::cerr << "Out of bounds index " << i << " on vec3" << std::endl;
        return (*this)[i % 3];
    }

    float& operator[](int i)
    {
        switch (i)
        {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        }
        std::cerr << "Out of bounds index " << i << " on vec3" << std::endl;
        return (*this)[i % 3];
    }

    bool operator==(vec3 v) const
    {
        return v.x == x && v.y == y && v.z == z;
    }

    bool operator!=(vec3 v) const
    {
        return v.x != x || v.y != y || v.z != z;
    }

    vec3 operator-() const
    {
        return vec3(-x, -y, -z);
    }

    std::string to_s() const
    {
        std::stringstream s;
        s << "(" << x << "," << y << "," << z << ")";
        return s.str();
    }

    inline vec3(const vec4&);
};

struct vec4
{
    union
    {
        struct
        {
            union
            {
                float x;
                float r;
            };
            union
            {
                float y;
                float g;
            };
            union
            {
                float z;
                float b;
            };
        };
        vec3 rgb;
        vec3 xyz;
    };
    union
    {
        float w;
        float a;
    };

    vec4()
    {
        // components are not initialized intentionally
    }

    vec4(float x_, float y_, float z_, float w_)
    {
        x = x_; y = y_; z = z_; w = w_;
    }

    vec4(const vec4& v)
    {
        x = v.x; y = v.y; z = v.z; w = v.w;
    }

    vec4(const vec3& v, float f)
    {
        x = v.x; y = v.y; z = v.z; w = f;
    }

    float operator[](int i) const
    {
        switch (i)
        {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
        }
        std::cerr << "Out of bounds index " << i << " on vec4" << std::endl;
        return (*this)[i % 4];
    }

    float& operator[](int i)
    {
        switch (i)
        {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
        }
        std::cerr << "Out of bounds index " << i << " on vec4" << std::endl;
        return (*this)[i % 4];
    }

    bool operator==(vec4 v) const
    {
        return v.x == x && v.y == y && v.z == z && v.w == w;
    }

    bool operator!=(vec4 v) const
    {
        return v.x != x || v.y != y || v.z != z || v.w != w;
    }

    vec4 operator-() const
    {
        return vec4(-x, -y, -z, -w);
    }

    std::string to_s() const
    {
        std::stringstream s;
        s << "(" << x << "," << y << "," << z << "," << w << ")";
        return s.str();
    }
};

#define op_vec2(OPN,OP) \
    inline vec2 OPN(const vec2 v1, const vec2 v2) { \
        return vec2(v1.x OP v2.x, v1.y OP v2.y); \
    } \
    inline vec2 OPN(const vec2 v, float f) { \
        return vec2(v.x OP f, v.y OP f); \
    } \
    inline vec2 OPN(float f, const vec2 v) { \
        return vec2(v.x OP f, v.y OP f); \
    }

#define op_vec3(OPN,OP) \
    inline vec3 OPN(const vec3 v1, const vec3 v2) { \
        return vec3(v1.x OP v2.x, v1.y OP v2.y, v1.z OP v2.z); \
    } \
    inline vec3 OPN(const vec3 v, float f) { \
        return vec3(v.x OP f, v.y OP f, v.z OP f); \
    } \
    inline vec3 OPN(float f, const vec3 v) { \
        return vec3(v.x OP f, v.y OP f, v.z OP f); \
    }

#define op_vec4(OPN,OP) \
    inline vec4 OPN(const vec4 v1, const vec4 v2) { \
        return vec4(v1.x OP v2.x, v1.y OP v2.y, v1.z OP v2.z, v1.w OP v2.w); \
    } \
    inline vec4 OPN(const vec4 v, float f) { \
        return vec4(v.x OP f, v.y OP f, v.z OP f, v.w OP f); \
    } \
    inline vec4 OPN(float f, const vec4 v) { \
        return vec4(v.x OP f, v.y OP f, v.z OP f, v.w OP f); \
    }

#define op_vec(OPN,OP) \
    op_vec2(OPN,OP) \
    op_vec3(OPN,OP) \
    op_vec4(OPN,OP)

op_vec(operator+, +);
op_vec(operator-, -);
op_vec(operator*, *);
op_vec(operator/, / );

#undef op_vec2
#undef op_vec3
#undef op_vec4
#undef op_vec

inline vec2::vec2(const vec3& v)
{
    x = v.x; y = v.y;
}

inline vec2::vec2(const vec4& v)
{
    x = v.x; y = v.y;
}

inline vec3::vec3(const vec4& v)
{
    x = v.x; y = v.y; z = v.z;
}

inline std::ostream& operator<<(std::ostream& os, vec2& v) 
{
    return os << v.to_s();
}

inline std::ostream& operator<<(std::ostream& os, vec3& v)
{
    return os << v.to_s();
}

inline std::ostream& operator<<(std::ostream& os, vec4& v)
{
    return os << v.to_s();
}

inline std::ostream& operator<<(std::ostream& os, const vec2& v)
{
    return os << vec2(v).to_s();
}

inline std::ostream& operator<<(std::ostream& os, const vec3& v)
{
    return os << vec3(v).to_s();
}

inline std::ostream& operator<<(std::ostream& os, const vec4& v)
{
    return os << vec4(v).to_s();
}

inline std::istream& operator>>(std::istream& is, vec2& v)
{
    std::string tok;
    is >> tok;
    float x, y;
    sscanf(tok.c_str(), "(%f,%f)", &x, &y);
    v.x = x; v.y = y;
    return is;
}

inline std::istream& operator>>(std::istream& is, vec3& v)
{
    std::string tok;
    is >> tok;
    float x, y, z;
    sscanf(tok.c_str(), "(%f,%f,%f)", &x, &y, &z);
    v.x = x; v.y = y; v.z = z;
    return is;
}

inline std::istream& operator>>(std::istream& is, vec4& v)
{
    std::string tok;
    is >> tok;
    float x, y, z, w;
    scanf(tok.c_str(), "(%f,%f,%f,%f)", &x, &y, &z, &w);
    v.x = x; v.y = y; v.z = z; v.w = w;
    return is;
}

inline float dot(float a, float b)
{
    return a * b;
}

inline float dot(vec2 a, vec2 b)
{
    return a.x * b.x + a.y * b.y;
}

inline float dot(vec3 a, vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float dot(vec4 a, vec4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline vec3 cross(vec3 a, vec3 b)
{
    return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

inline float length(float x)
{
    return fabs(x);
}

inline float length(vec2 v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

inline float length(vec3 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline float length(vec4 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

struct mat4
{
    vec4 cols[4];

    mat4()
    {
        cols[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);
        cols[1] = vec4(0.0f, 1.0f, 0.0f, 0.0f);
        cols[2] = vec4(0.0f, 0.0f, 1.0f, 0.0f);
        cols[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    // create matrix by columns
    mat4(vec4 a, vec4 b, vec4 c, vec4 d)
    {
        cols[0] = a;
        cols[1] = b;
        cols[2] = c;
        cols[3] = d;
    }

    // return column i
    vec4 operator[](int i) const
    {
        return cols[i];
    }

    vec4& operator[](int i)
    {
        return cols[i];
    }

    vec4 column(int i) const
    {
        return cols[i];
    }

    vec4 row(int i) const
    {
        return vec4(cols[0][i], cols[1][i], cols[2][i], cols[3][i]);
    }

    mat4 operator+(const mat4& m)
    {
        mat4 z;
        for (int i = 0; i < 4; i++)
            z[i] = cols[i] + m[i];
        return z;
    }

    mat4 operator*(const mat4& m)
    {
        mat4 z;
        for (int c = 0; c < 4; c++)
        {
            vec4 col;
            for (int r = 0; r < 4; r++)
            {
                col[r] = dot(m[c], vec4(row(r)));
            }
            z[c] = col;
        }
        return z;
    }
};

inline vec4 mul(const mat4& m, const vec4& v)
{
    vec4 z;
    for (int c = 0; c < 4; c++)
    {
        z[c] = dot(v, m[c]);
    }
    return z;
}

// component-wise operations on one vector
#define app_v(f) \
    inline vec2 f(vec2 v) { \
        return vec2(f(v.x),f(v.y)); \
    } \
    inline vec3 f(vec3 v) { \
        return vec3(f(v.x),f(v.y),f(v.z)); \
    } \
    inline vec4 f(vec4 v) { \
        return vec4(f(v.x),f(v.y),f(v.z),f(v.w)); \
    }

// component-wise operations on one vector and one float
#define app_vf(f) \
    inline vec2 f(vec2 v, float x) { \
        return vec2(f(v.x,x),f(v.y,x)); \
    } \
    inline vec3 f(vec3 v, float x) { \
        return vec3(f(v.x,x),f(v.y,x),f(v.z,x)); \
    } \
    inline vec4 f(vec4 v, float x) { \
        return vec4(f(v.x,x),f(v.y,x),f(v.z,x),f(v.w,x)); \
    }

// component-wise operations on one float and one vector
#define app_fv(f) \
    inline vec2 f(float x, vec2 v) { \
        return vec2(f(v.x,x),f(v.y,x)); \
    } \
    inline vec3 f(float x, vec3 v) { \
        return vec3(f(v.x,x),f(v.y,x),f(v.z,x)); \
    } \
    inline vec4 f(float x, vec4 v) { \
        return vec4(f(v.x,x),f(v.y,x),f(v.z,x),f(v.w,x)); \
    }

// component-wise operations on one vector and two floats
#define app_vf2(f) \
    inline vec2 f(vec2 v, float x, float y) { \
        return vec2(f(v.x,x,y),f(v.y,x,y)); \
    } \
    inline vec3 f(vec3 v, float x, float y) { \
        return vec3(f(v.x,x,y),f(v.y,x,y),f(v.z,x,y)); \
    } \
    inline vec4 f(vec4 v, float x, float y) { \
        return vec4(f(v.x,x,y),f(v.y,x,y),f(v.z,x,y),f(v.w,x,y)); \
    }

// component-wise operations on two floats and one vector
#define app_f2v(f) \
    inline vec2 f(float x, float y, vec2 v) { \
        return vec2(f(v.x,x,y),f(v.y,x,y)); \
    } \
    inline vec3 f(float x, float y, vec3 v) { \
        return vec3(f(v.x,x,y),f(v.y,x,y),f(v.z,x,y)); \
    } \
    inline vec4 f(float x, float y, vec4 v) { \
        return vec4(f(v.x,x,y),f(v.y,x,y),f(v.z,x,y),f(v.w,x,y)); \
    }

// component-wise operations on two vectors
#define app_v2(f) \
    inline vec2 f(vec2 a, vec2 b) { \
        return vec2(f(a.x,b.x),f(a.y,b.y)); \
    } \
    inline vec3 f(vec3 a, vec3 b) { \
        return vec3( \
            f(a.x,b.x), \
            f(a.y,b.y), \
            f(a.z,b.z) \
        ); \
    } \
    inline vec4 f(vec4 a, vec4 b) { \
        return vec4( \
            f(a.x,b.x), \
            f(a.y,b.y), \
            f(a.z,b.z), \
            f(a.w,b.w) \
        ); \
    } \

// component-wise operations on two vectors and one float
#define app_v2f(f) \
    inline vec2 f(vec2 a, vec2 b, float x) { \
        return vec2(f(a.x,b.x,x),f(a.y,b.y,x)); \
    } \
    inline vec3 f(vec3 a, vec3 b, float x) { \
        return vec3( \
            f(a.x,b.x,x), \
            f(a.y,b.y,x), \
            f(a.z,b.z,x) \
        ); \
    } \
    inline vec4 f(vec4 a, vec4 b, float x) { \
        return vec4( \
            f(a.x,b.x,x), \
            f(a.y,b.y,x), \
            f(a.z,b.z,x), \
            f(a.w,b.w,x) \
        ); \
    } \

// component-wise operations on three vectors
#define app_v3(f) \
    inline vec2 f(vec2 a, vec2 b, vec2 c) { \
        return vec2( \
            f(a.x,b.x,c.x), \
            f(a.y,b.y,c.y) \
        ); \
    } \
    inline vec3 f(vec3 a, vec3 b, vec3 c) { \
        return vec3( \
            f(a.x,b.x,c.x), \
            f(a.y,b.y,c.y), \
            f(a.z,b.z,c.z) \
        ); \
    } \
    inline vec4 f(vec4 a, vec4 b, vec4 c) { \
        return vec4( \
            f(a.x,b.x,c.x), \
            f(a.y,b.y,c.y), \
            f(a.z,b.z,c.z), \
            f(a.w,b.w,c.w) \
        ); \
    } \

#define def_v1(f,a,expr) \
    inline float f(float a) { expr; } \
    inline vec2 f(vec2 a) { expr; } \
    inline vec3 f(vec3 a) { expr; } \
    inline vec4 f(vec4 a) { expr; } \

#define def_v2(f,a,b,expr) \
    inline f(float a, float b) { expr; } \
    inline f(vec2 a, vec2 b) { expr; } \
    inline f(vec3 a, vec3 b) { expr; } \
    inline f(vec4 a, vec4 b) { expr; } \

#define defT_v1(f,a,expr) \
    inline float f(float a, float b) { expr; } \
    inline vec2 f(vec2 a) { expr; } \
    inline vec3 f(vec3 a) { expr; } \
    inline vec4 f(vec4 a) { expr; } \

#define defT_v2(f,a,b,expr) \
    inline float f(float a, float b) { expr; } \
    inline vec2 f(vec2 a, vec2 b) { expr; } \
    inline vec3 f(vec3 a, vec3 b) { expr; } \
    inline vec4 f(vec4 a, vec4 b) { expr; } \

#define defT_v2f(f,a,b,c,expr) \
    inline float f(float a, float b, float c) { expr; } \
    inline vec2 f(vec2 a, vec2 b, float c) { expr; } \
    inline vec3 f(vec3 a, vec3 b, float c) { expr; } \
    inline vec4 f(vec4 a, vec4 b, float c) { expr; } \

#define defT_v3(f,a,b,c,expr) \
    inline float f(float a, float b, float c) { expr; } \
    inline vec2 f(vec2 a, vec2 b, vec2 c) { expr; } \
    inline vec3 f(vec3 a, vec3 b, vec2 c) { expr; } \
    inline vec4 f(vec4 a, vec4 b, vec2 c) { expr; } \

// angle and trigonometry functions

inline float radians(float d)
{
    return pi / 180.0f * d;
}
app_v(radians)

inline float degrees(float r)
{
    return 180.0f / pi * r;
}
app_v(degrees)

app_v(sin)
app_v(cos)
app_v(tan)
app_v(asin)
app_v(acos)
app_v(atan)

inline float atan(float x, float y)
{
    return atan2(x, y);
}
app_v2(atan)

// exponential functions
app_v2(pow)
app_v(exp)
app_v(log)
app_v(exp2)
app_v(log2)
app_v(sqrt)
inline float inversesqrt(float x)
{
    return 1.0f / sqrt(x);
}
app_v(inversesqrt)

// common functions
#ifndef _WIN32
inline float abs(float x)
{
    return fabs(x);
}
#endif
app_v(abs)

inline float sign(float x)
{
    return x > 0.0f ? 1.0f : 0.0f;
}
app_v(sign)

app_v(floor)
app_v(ceil)

inline float fract(float x)
{
    return x - floor(x);
}
app_v(fract)

inline float mod(float x, float y)
{
    return x - y * floor(x / y);
}
app_v2(mod)
app_vf(mod)

inline float min(float x, float y)
{
    return fmin(x, y);
}
app_v2(min)
app_vf(min)

inline float max(float x, float y)
{
    return fmax(x, y);
}
app_v2(max)
app_vf(max)

inline float clamp(float x, float min_v, float max_v)
{
    return min(max(x, min_v), max_v);
}
app_v3(clamp)
app_vf2(clamp)

inline float mix(float x, float y, float a)
{
    return x * (1.0f - a) + y * a;
}
app_v3(mix)
app_v2f(mix)

inline float step(float edge, float x)
{
    return x < edge ? 0.0f : 1.0f;
}
app_v2(step)
app_fv(step)

inline float smoothstep(float edge0, float edge1, float x)
{
    float t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return t * t * (3 - 2 * t);
}
app_f2v(smoothstep)
app_v3(smoothstep)

def_v2(float distance, a, b,
    return length(a - b)
);

defT_v1(normalize, x, return x / length(x));

// ignoring ftransform for now

defT_v3(faceforward, N, I, Nref,
    return dot(Nref, I) < 0.0f ? N : -N
);

defT_v2(reflect, I, N,
    return I - 2.0f * dot(N, I) * N
);

defT_v2f(refract, I, N, eta,
    float k = 1.0f - eta * eta * (1.0f - dot(N, I) * dot(N, I));
    return k < 0.0f ? I * 0.0f : eta * I - (eta * dot(N, I) + sqrt(k)) * N
);

#undef app_v
#undef app_vf
#undef app_fv
#undef app_vf2
#undef app_f2v
#undef app_v2
#undef app_v2f
#undef app_v3
#undef def_v1
#undef def_v2
#undef defT_v1
#undef defT_v2
#undef defT_v2f
#undef defT_v3

extern vec4 gl_Position;
extern vec4 gl_FragColor;

extern mat4 u_view;
extern mat4 u_invView;
extern mat4 u_proj;
extern mat4 u_invProj;
extern mat4 u_viewProj;
extern mat4 u_invViewProj;
extern mat4 u_modelView;
extern mat4 u_modelViewProj;

#include "varying.def.sc"
