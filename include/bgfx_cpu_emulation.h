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

#include <cassert>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include "bgfx_shader.h"

vec4 gl_Position;
vec4 gl_FragColor;

mat4 u_view;
mat4 u_invView;
mat4 u_proj;
mat4 u_invProj;
mat4 u_viewProj;
mat4 u_invViewProj;
mat4 u_modelView;
mat4 u_modelViewProj;

class CPURendering
{
    enum class AttributeType : unsigned char
    {
        AttributeFloat,
        AttributeVec2,
        AttributeVec3,
        AttributeVec4,
        AttributeMat4
    };

    struct Attribute
    {
        AttributeType attribute_type;
        void* attribute_varying_data;

        size_t getAttributeSize() const
        {
            switch (attribute_type)
            {
            case AttributeType::AttributeFloat:
                return sizeof(float);
            case AttributeType::AttributeVec2:
                return sizeof(vec2);
            case AttributeType::AttributeVec3:
                return sizeof(vec3);
            case AttributeType::AttributeVec4:
                return sizeof(vec4);
            case AttributeType::AttributeMat4:
                return sizeof(mat4);
            }
            std::cerr << "Out of AttributeType bounds " << static_cast<int>(attribute_type) << std::endl;
            return 0;
        }
        void loadVaryingData(void* attribute_data) const
        {
            switch (attribute_type)
            {
            case AttributeType::AttributeFloat:
                *static_cast<float*>(attribute_varying_data) = *static_cast<float*>(attribute_data);
                break;
            case AttributeType::AttributeVec2:
                *static_cast<vec2*>(attribute_varying_data) = *static_cast<vec2*>(attribute_data);
                break;
            case AttributeType::AttributeVec3:
                *static_cast<vec3*>(attribute_varying_data) = *static_cast<vec3*>(attribute_data);
                break;
            case AttributeType::AttributeVec4:
                *static_cast<vec4*>(attribute_varying_data) = *static_cast<vec4*>(attribute_data);
                break;
            case AttributeType::AttributeMat4:
                *static_cast<mat4*>(attribute_varying_data) = *static_cast<mat4*>(attribute_data);
                break;
            default:
                std::cerr << "Out of AttributeType bounds " << static_cast<int>(attribute_type) << std::endl;
            }
        }
        void saveVaryingData(void* attribute_data) const
        {
            switch (attribute_type)
            {
            case AttributeType::AttributeFloat:
                *static_cast<float*>(attribute_data) = *static_cast<float*>(attribute_varying_data);
                break;
            case AttributeType::AttributeVec2:
                *static_cast<vec2*>(attribute_data) = *static_cast<vec2*>(attribute_varying_data);
                break;
            case AttributeType::AttributeVec3:
                *static_cast<vec3*>(attribute_data) = *static_cast<vec3*>(attribute_varying_data);
                break;
            case AttributeType::AttributeVec4:
                *static_cast<vec4*>(attribute_data) = *static_cast<vec4*>(attribute_varying_data);
                break;
            case AttributeType::AttributeMat4:
                *static_cast<mat4*>(attribute_data) = *static_cast<mat4*>(attribute_varying_data);
                break;
            default:
                std::cerr << "Out of AttributeType bounds " << static_cast<int>(attribute_type) << std::endl;
            }
        }
        void interpolate(void* attribute_data1, void* attribute_data2, void* result_data, float a)
        {
            switch (attribute_type)
            {
            case AttributeType::AttributeFloat:
                *static_cast<float*>(result_data) = mix(*static_cast<float*>(attribute_data1), * static_cast<float*>(attribute_data1), a);
                break;
            case AttributeType::AttributeVec2:
                *static_cast<vec2*>(result_data) = mix(*static_cast<vec2*>(attribute_data1), *static_cast<vec2*>(attribute_data1), a);
                break;
            case AttributeType::AttributeVec3:
                *static_cast<vec3*>(result_data) = mix(*static_cast<vec3*>(attribute_data1), *static_cast<vec3*>(attribute_data1), a);
                break;
            case AttributeType::AttributeVec4:
                *static_cast<vec4*>(result_data) = mix(*static_cast<vec4*>(attribute_data1), *static_cast<vec4*>(attribute_data1), a);
                break;
            case AttributeType::AttributeMat4:
                //*static_cast<mat4*>(result_data) = mix(*static_cast<mat4*>(attribute_data1), *static_cast<mat4*>(attribute_data1), a);
                //break;
            default:
                std::cerr << "Out of AttributeType bounds " << static_cast<int>(attribute_type) << std::endl;
            }
        }
    };
    std::vector<Attribute> attributes;
    std::vector<Attribute> output_attributes;
    void* vertex_buffer = nullptr;
    size_t vertex_count = 0;
    uint16_t* index_buffer = nullptr;
    size_t triangle_count = 0;

    std::vector<unsigned char> rgba_buffer;
    std::vector<float> z_buffer;

    const unsigned width;
    const unsigned height;
    const unsigned size;

    size_t getVertexSize() const
    {
        size_t result = 0;
        for (size_t i = 0; i < attributes.size(); ++i)
        {
            result += attributes[i].getAttributeSize();
        }
        return result;
    }

    size_t getOutputVertexSize() const
    {
        size_t result = 0;
        for (size_t i = 0; i < output_attributes.size(); ++i)
        {
            result += output_attributes[i].getAttributeSize();
        }
        return result;
    }

    void loadVaryingData(void* attribute_data) const
    {
        for (size_t i = 0; i < attributes.size(); ++i)
        {
            attributes[i].loadVaryingData(attribute_data);
            attribute_data = static_cast<unsigned char*>(attribute_data) + attributes[i].getAttributeSize();
        }
    }

    void saveVaryingData(void* attribute_data) const
    {
        for (size_t i = 0; i < output_attributes.size(); ++i)
        {
            output_attributes[i].saveVaryingData(attribute_data);
            attribute_data = static_cast<unsigned char*>(attribute_data) + output_attributes[i].getAttributeSize();
        }
    }

    void loadOutputVaryingData(void* attribute_data) const
    {
        for (size_t i = 0; i < output_attributes.size(); ++i)
        {
            output_attributes[i].loadVaryingData(attribute_data);
            attribute_data = static_cast<unsigned char*>(attribute_data) + output_attributes[i].getAttributeSize();
        }
    }

    void interpolateAttributes(void* attribute_data1, void* attribute_data2, void* result_data, float a)
    {
        for (size_t i = 0; i < output_attributes.size(); ++i)
        {
            output_attributes[i].interpolate(attribute_data1, attribute_data2, result_data, a);
            attribute_data1 = static_cast<unsigned char*>(attribute_data1) + output_attributes[i].getAttributeSize();
            attribute_data2 = static_cast<unsigned char*>(attribute_data2) + output_attributes[i].getAttributeSize();
            result_data = static_cast<unsigned char*>(result_data) + output_attributes[i].getAttributeSize();
        }
    }

    static float sign(vec2 p1, vec2 p2, vec2 p3)
    {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    }

    static bool pointInTriangle(vec2 pt, vec2 v1, vec2 v2, vec2 v3)
    {
        float d1, d2, d3;
        bool has_neg, has_pos;

        d1 = sign(pt, v1, v2);
        d2 = sign(pt, v2, v3);
        d3 = sign(pt, v3, v1);

        has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

        return !(has_neg && has_pos);
    }

    int xToScreen(int x) const
    {
        return x + width / 2;
    }

    int yToScreen(int y) const
    {
        return y + height / 2;
    }

    bool insideScreen(int x, int y) const
    {
        return x >= 0 && static_cast<unsigned>(x) < width && y >= 0 && static_cast<unsigned>(y) < height;
    }

    unsigned char rBuffer(int x, int y) const
    {
        return rgba_buffer[(y * width + x) * 4 + 0];
    }

    unsigned char gBuffer(int x, int y) const
    {
        return rgba_buffer[(y * width + x) * 4 + 1];
    }

    unsigned char bBuffer(int x, int y) const
    {
        return rgba_buffer[(y * width + x) * 4 + 2];
    }

    unsigned char aBuffer(int x, int y) const
    {
        return rgba_buffer[(y * width + x) * 4 + 3];
    }

    unsigned char& rBuffer(int x, int y)
    {
        return rgba_buffer[(y * width + x) * 4 + 0];
    }

    unsigned char& gBuffer(int x, int y)
    {
        return rgba_buffer[(y * width + x) * 4 + 1];
    }

    unsigned char& bBuffer(int x, int y)
    {
        return rgba_buffer[(y * width + x) * 4 + 2];
    }

    unsigned char& aBuffer(int x, int y)
    {
        return rgba_buffer[(y * width + x) * 4 + 3];
    }

    float& zBuffer(int x, int y)
    {
        return z_buffer[y * width + x];
    }

public:
    CPURendering(unsigned width_, unsigned height_) : width(width_), height(height_), size(width_ * height_)
    {
        rgba_buffer.resize(static_cast<size_t>(size) * 4, 0);
        z_buffer.resize(size, 4194304.0f);
    }

    CPURendering& addAttribute(float* varying_data)
    {
        Attribute new_attribute;
        new_attribute.attribute_type = AttributeType::AttributeFloat;
        new_attribute.attribute_varying_data = varying_data;
        attributes.push_back(new_attribute);
        return *this;
    }

    CPURendering& addOutputAttribute(float* varying_data)
    {
        Attribute new_attribute;
        new_attribute.attribute_type = AttributeType::AttributeFloat;
        new_attribute.attribute_varying_data = varying_data;
        output_attributes.push_back(new_attribute);
        return *this;
    }

    CPURendering& addAttribute(vec2* varying_data)
    {
        Attribute new_attribute;
        new_attribute.attribute_type = AttributeType::AttributeVec2;
        new_attribute.attribute_varying_data = varying_data;
        attributes.push_back(new_attribute);
        return *this;
    }

    CPURendering& addOutputAttribute(vec2* varying_data)
    {
        Attribute new_attribute;
        new_attribute.attribute_type = AttributeType::AttributeVec2;
        new_attribute.attribute_varying_data = varying_data;
        output_attributes.push_back(new_attribute);
        return *this;
    }

    CPURendering& addAttribute(vec3* varying_data)
    {
        Attribute new_attribute;
        new_attribute.attribute_type = AttributeType::AttributeVec3;
        new_attribute.attribute_varying_data = varying_data;
        attributes.push_back(new_attribute);
        return *this;
    }

    CPURendering& addOutputAttribute(vec3* varying_data)
    {
        Attribute new_attribute;
        new_attribute.attribute_type = AttributeType::AttributeVec3;
        new_attribute.attribute_varying_data = varying_data;
        output_attributes.push_back(new_attribute);
        return *this;
    }

    CPURendering& addAttribute(vec4* varying_data)
    {
        Attribute new_attribute;
        new_attribute.attribute_type = AttributeType::AttributeVec4;
        new_attribute.attribute_varying_data = varying_data;
        attributes.push_back(new_attribute);
        return *this;
    }

    CPURendering& addOutputAttribute(vec4* varying_data)
    {
        Attribute new_attribute;
        new_attribute.attribute_type = AttributeType::AttributeVec4;
        new_attribute.attribute_varying_data = varying_data;
        output_attributes.push_back(new_attribute);
        return *this;
    }

    CPURendering& addAttribute(mat4* varying_data)
    {
        Attribute new_attribute;
        new_attribute.attribute_type = AttributeType::AttributeMat4;
        new_attribute.attribute_varying_data = varying_data;
        attributes.push_back(new_attribute);
        return *this;
    }

    CPURendering& addOutputAttribute(mat4* varying_data)
    {
        Attribute new_attribute;
        new_attribute.attribute_type = AttributeType::AttributeMat4;
        new_attribute.attribute_varying_data = varying_data;
        output_attributes.push_back(new_attribute);
        return *this;
    }

    void setVertexBuffer(void* vertex_buffer_, size_t vertex_count_)
    {
        vertex_buffer = vertex_buffer_;
        vertex_count = vertex_count_;
    }

    void setIndexBuffer(uint16_t* index_buffer_, size_t triangle_count_)
    {
        index_buffer = index_buffer_;
        triangle_count = triangle_count_;
    }

    void render()
    {
        if (!index_buffer || !triangle_count)
        {
            std::cerr << "Index buffer is not specified or triangle count is zero" << std::endl;
            return;
        }

        if (!vertex_buffer || !vertex_count)
        {
            std::cerr << "Vertex buffer is not specified or vertex count is zero" << std::endl;
            return;
        }

        const size_t vertex_size = getVertexSize();
        const size_t output_vertex_size = getOutputVertexSize();
        for (size_t triangle_index = 0; triangle_index < triangle_count; ++triangle_index)
        {
            uint16_t first_triangle_index = index_buffer[triangle_index * 3 + 0];
            uint16_t second_triangle_index = index_buffer[triangle_index * 3 + 1];
            uint16_t third_triangle_index = index_buffer[triangle_index * 3 + 2];

            if (first_triangle_index >= vertex_count)
            {
                std::cerr << "Out of vertex index " << first_triangle_index << std::endl;
                return;
            }
            void* first_vertex = static_cast<unsigned char*>(vertex_buffer) + vertex_size * first_triangle_index;
            loadVaryingData(first_vertex);
            vertex_shader_main();
            vec4 first_gl_position = gl_Position;
            std::vector<unsigned> first_vertex_output_data(output_vertex_size);
            saveVaryingData(&first_vertex_output_data[0]);

            if (second_triangle_index >= vertex_count)
            {
                std::cerr << "Out of vertex index " << second_triangle_index << std::endl;
                return;
            }
            void* second_vertex = static_cast<unsigned char*>(vertex_buffer) + vertex_size * second_triangle_index;
            loadVaryingData(second_vertex);
            vertex_shader_main();
            vec4 second_gl_position = gl_Position;
            std::vector<unsigned> second_vertex_output_data(output_vertex_size);
            saveVaryingData(&second_vertex_output_data[0]);

            if (third_triangle_index >= vertex_count)
            {
                std::cerr << "Out of vertex index " << third_triangle_index << std::endl;
                return;
            }
            void* third_vertex = static_cast<unsigned char*>(vertex_buffer) + vertex_size * third_triangle_index;
            loadVaryingData(third_vertex);
            vertex_shader_main();
            vec4 third_gl_position = gl_Position;
            std::vector<unsigned> third_vertex_output_data(output_vertex_size);
            saveVaryingData(&third_vertex_output_data[0]);

            vec2 v0(first_gl_position.x, first_gl_position.y);
            vec2 v1(second_gl_position.x, second_gl_position.y);
            vec2 v2(third_gl_position.x, third_gl_position.y);
            vec2 v0v1 = normalize(v1 - v0);
            vec2 v0v2 = normalize(v2 - v0);
            vec3 v0v1_3d(v0v1.x, v0v1.y, 0.0f);
            vec3 v0v2_3d(v0v2.x, v0v2.y, 0.0f);
            vec3 n = cross(v0v1_3d, v0v2_3d);
            vec3 new_v0v2_3d = normalize(cross(n, v0v1_3d));
            v0v2.x = new_v0v2_3d.x;
            v0v2.y = new_v0v2_3d.y;
            float v0v1_length = length(v1 - v0);
            float v0v2_length = dot((v2 - v0), v0v2);
            assert(v0v1_length > 0.0f);
            assert(v0v2_length > 0.0f);

            vec2 bbox_min, bbox_max;
            bbox_min.x = std::min<float>(std::min<float>(first_gl_position.x, second_gl_position.x), third_gl_position.x);
            bbox_min.y = std::min<float>(std::min<float>(first_gl_position.y, second_gl_position.y), third_gl_position.y);
            bbox_max.x = std::max<float>(std::max<float>(first_gl_position.x, second_gl_position.x), third_gl_position.x);
            bbox_max.y = std::max<float>(std::max<float>(first_gl_position.y, second_gl_position.y), third_gl_position.y);

            int min_x = static_cast<int>(bbox_min.x);
            int min_y = static_cast<int>(bbox_min.y);
            int max_x = static_cast<int>(bbox_max.x);
            int max_y = static_cast<int>(bbox_max.y);
            for (int x = min_x; x <= max_x; ++x)
            {
                for (int y = min_y; y <= max_y; ++y)
                {
                    int screen_x = xToScreen(x);
                    int screen_y = yToScreen(y);
                    if (insideScreen(screen_x, screen_y))
                    {
                        vec2 cur_point(static_cast<float>(x), static_cast<float>(y));
                        if (pointInTriangle(cur_point, v0, v1, v2))
                        {
                            vec2 v = cur_point - v0;
                            float rx = dot(v, v0v1);
                            float ry = dot(v, v0v2);
                            assert(rx >= 0.0f);
                            assert(ry >= 0.0f);
                            float nx = rx / v0v1_length;
                            float ny = ry / v0v2_length;
                            float interim_z = mix(first_gl_position.z, second_gl_position.z, nx);
                            float result_z = mix(interim_z, third_gl_position.z, ny);
                            if (result_z < zBuffer(screen_x, screen_y))
                            {
                                zBuffer(screen_x, screen_y) = result_z;

                                std::vector<unsigned> interim_vertex_output_data(output_vertex_size);
                                interpolateAttributes(&first_vertex_output_data[0], &second_vertex_output_data[0], &interim_vertex_output_data[0], nx);

                                std::vector<unsigned> result_vertex_output_data(output_vertex_size);
                                interpolateAttributes(&interim_vertex_output_data[0], &third_vertex_output_data[0], &result_vertex_output_data[0], ny);
                                loadOutputVaryingData(&result_vertex_output_data);
                                fragment_shader_main();

                                rBuffer(screen_x, screen_y) = static_cast<unsigned char>(gl_FragColor.r * 255.0f);
                                gBuffer(screen_x, screen_y) = static_cast<unsigned char>(gl_FragColor.g * 255.0f);
                                bBuffer(screen_x, screen_y) = static_cast<unsigned char>(gl_FragColor.b * 255.0f);
                                aBuffer(screen_x, screen_y) = static_cast<unsigned char>(gl_FragColor.a * 255.0f);
                            }
                        }
                    }
                }
            }
        }
    }

    void saveToPPM(const std::string& file_name) const
    {
        std::ofstream out_file(file_name);
        out_file << "P3\n";
        out_file << width << " " << height << "\n";
        out_file << "255\n";
        for (int y = 0; static_cast<unsigned>(y) < height; ++y)
        {
            for (int x = 0; static_cast<unsigned>(x) < width; ++x)
            {
                out_file << static_cast<int>(rBuffer(x, y)) << " " << static_cast<int>(gBuffer(x, y)) << " " << static_cast<int>(bBuffer(x, y)) << " ";
            }
            out_file << "\n";
        }
    }
};
