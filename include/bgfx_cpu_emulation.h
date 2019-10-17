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

namespace BGFXShaderCPUEmulator
{
    enum class AttributeType : unsigned char
    {
        AttributeFloat,
        AttributeVec2,
        AttributeVec3,
        AttributeVec4,
        AttributeMat4
    };

    class Attribute
    {
        AttributeType type;

        union
        {
            float* varying_float;
            vec2* varying_vec2;
            vec3* varying_vec3;
            vec4* varying_vec4;
            mat4* varying_mat4;
        };

        union
        {
            float saved_float;
            vec2 saved_vec2;
            vec3 saved_vec3;
            vec4 saved_vec4;
            mat4 saved_mat4;
        };

    public:
        Attribute(float* varying)
        {
            type = AttributeType::AttributeFloat;
            varying_float = varying;
            saved_float = 0.0f;
        }
        Attribute(vec2* varying)
        {
            type = AttributeType::AttributeVec2;
            varying_vec2 = varying;
            saved_vec2 = vec2(0.0f, 0.0f);
        }
        Attribute(vec3* varying)
        {
            type = AttributeType::AttributeVec3;
            varying_vec3 = varying;
            saved_vec3 = vec3(0.0f, 0.0f, 0.0f);
        }
        Attribute(vec4* varying)
        {
            type = AttributeType::AttributeVec4;
            varying_vec4 = varying;
            saved_vec4 = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        }
        Attribute(mat4* varying)
        {
            type = AttributeType::AttributeMat4;
            varying_mat4 = varying;
            saved_mat4 = mat4();
        }
        Attribute(const Attribute& other)
        {
            type = other.type;
            switch (type)
            {
            case AttributeType::AttributeFloat:
                varying_float = other.varying_float;
                saved_float = other.saved_float;
                break;
            case AttributeType::AttributeVec2:
                varying_vec2 = other.varying_vec2;
                saved_vec2 = other.saved_vec2;
                break;
            case AttributeType::AttributeVec3:
                varying_vec3 = other.varying_vec3;
                saved_vec3 = other.saved_vec3;
                break;
            case AttributeType::AttributeVec4:
                varying_vec4 = other.varying_vec4;
                saved_vec4 = other.saved_vec4;
                break;
            case AttributeType::AttributeMat4:
                varying_mat4 = other.varying_mat4;
                saved_mat4 = other.saved_mat4;
                break;
            default:
                assert(false);
            }
        }
        size_t getAttributeSize() const
        {
            switch (type)
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
            default:
                assert(false);
            }
            return 0;
        }
        void loadVaryingFromVertexBuffer(void* vertex_buffer) const
        {
            switch (type)
            {
            case AttributeType::AttributeFloat:
                *varying_float = *static_cast<float*>(vertex_buffer);
                break;
            case AttributeType::AttributeVec2:
                *varying_vec2 = *static_cast<vec2*>(vertex_buffer);
                break;
            case AttributeType::AttributeVec3:
                *varying_vec3 = *static_cast<vec3*>(vertex_buffer);
                break;
            case AttributeType::AttributeVec4:
                *varying_vec4 = *static_cast<vec4*>(vertex_buffer);
                break;
            case AttributeType::AttributeMat4:
                *varying_mat4 = *static_cast<mat4*>(vertex_buffer);
                break;
            default:
                assert(false);
            }
        }
        void saveVarying()
        {
            switch (type)
            {
            case AttributeType::AttributeFloat:
                saved_float = *varying_float;
                break;
            case AttributeType::AttributeVec2:
                saved_vec2 = *varying_vec2;
                break;
            case AttributeType::AttributeVec3:
                saved_vec3 = *varying_vec3;
                break;
            case AttributeType::AttributeVec4:
                saved_vec4 = *varying_vec4;
                break;
            case AttributeType::AttributeMat4:
                saved_mat4 = *varying_mat4;
                break;
            default:
                assert(false);
            }
        }
        void loadVarying() const
        {
            switch (type)
            {
            case AttributeType::AttributeFloat:
                *varying_float = saved_float;
                break;
            case AttributeType::AttributeVec2:
                *varying_vec2 = saved_vec2;
                break;
            case AttributeType::AttributeVec3:
                *varying_vec3 = saved_vec3;
                break;
            case AttributeType::AttributeVec4:
                *varying_vec4 = saved_vec4;
                break;
            case AttributeType::AttributeMat4:
                *varying_mat4 = saved_mat4;
                break;
            default:
                assert(false);
            }
        }
        Attribute interpolate(const Attribute& other, float a) const
        {
            if (type != other.type)
            {
                std::cerr << "Attributes have different types!" << std::endl;
            }
            assert(type == other.type);

            switch (type)
            {
            case AttributeType::AttributeFloat:
            {
                Attribute result_float(varying_float);
                result_float.saved_float = mix(saved_float, other.saved_float, a);
                return result_float;
            }
            case AttributeType::AttributeVec2:
            {
                Attribute result_vec2(varying_vec2);
                result_vec2.saved_vec2 = mix(saved_vec2, other.saved_vec2, a);
                return result_vec2;
            }
            case AttributeType::AttributeVec3:
            {
                Attribute result_vec3(varying_vec3);
                result_vec3.saved_vec3 = mix(saved_vec3, other.saved_vec3, a);
                return result_vec3;
            }
            case AttributeType::AttributeVec4:
            {
                Attribute result_vec4(varying_vec4);
                result_vec4.saved_vec4 = mix(saved_vec4, other.saved_vec4, a);
                return result_vec4;
            }
            case AttributeType::AttributeMat4:
            {
                //Attribute result_mat4(varying_mat4);
                //result_mat4.saved_mat4 = mix(saved_mat4, other.saved_mat4, a);
                //return result_mat4;
            }
            default:
                assert(false);
            }
            return Attribute(varying_float);
        }
    };

    class Attributes : public std::vector<Attribute>
    {
    public:
        size_t getAttributesSize() const
        {
            size_t result = 0;
            for (size_t i = 0; i < size(); ++i)
            {
                result += this->operator[](i).getAttributeSize();
            }
            return result;
        }
        void loadVaryingFromVertexBuffer(void* vertex_buffer) const
        {
            for (size_t i = 0; i < size(); ++i)
            {
                this->operator[](i).loadVaryingFromVertexBuffer(vertex_buffer);
                vertex_buffer = static_cast<unsigned char*>(vertex_buffer) + this->operator[](i).getAttributeSize();
            }
        }
        void saveVarying()
        {
            for (size_t i = 0; i < size(); ++i)
            {
                this->operator[](i).saveVarying();
            }
        }
        void loadVarying() const
        {
            for (size_t i = 0; i < size(); ++i)
            {
                this->operator[](i).loadVarying();
            }
        }
        Attributes interpolate(const Attributes& other, float a)
        {
            if (size() != other.size())
            {
                std::cerr << "Attribute array must have the same size!" << std::endl;
            }
            assert(size() == other.size());

            Attributes result;
            for (size_t i = 0; i < size(); ++i)
            {
                result.push_back(this->operator[](i).interpolate(other[i], a));
            }
            return result;
        }
    };

    class CPURendering
    {
        void* vertex_buffer;
        size_t vertex_count;
        uint16_t* index_buffer;
        size_t triangle_count;

        const size_t width;
        const size_t height;
        const size_t size;
        std::vector<unsigned char> rgba_buffer;
        std::vector<float> z_buffer;
        size_t vertex_size;

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
            return static_cast<int>(x + width / 2);
        }

        int yToScreen(int y) const
        {
            return static_cast<int>(y + height / 2);
        }

        bool insideScreen(int x, int y) const
        {
            return x >= 0 && static_cast<size_t>(x) < width&& y >= 0 && static_cast<size_t>(y) < height;
        }

        unsigned char rBuffer(int x, int y) const
        {
            return rgba_buffer[(width * y + x) * 4 + 0];
        }

        unsigned char gBuffer(int x, int y) const
        {
            return rgba_buffer[(width * y + x) * 4 + 1];
        }

        unsigned char bBuffer(int x, int y) const
        {
            return rgba_buffer[(width * y + x) * 4 + 2];
        }

        unsigned char aBuffer(int x, int y) const
        {
            return rgba_buffer[(width * y + x) * 4 + 3];
        }

        unsigned char& rBuffer(int x, int y)
        {
            return rgba_buffer[(width * y + x) * 4 + 0];
        }

        unsigned char& gBuffer(int x, int y)
        {
            return rgba_buffer[(width * y + x) * 4 + 1];
        }

        unsigned char& bBuffer(int x, int y)
        {
            return rgba_buffer[(width * y + x) * 4 + 2];
        }

        unsigned char& aBuffer(int x, int y)
        {
            return rgba_buffer[(width * y + x) * 4 + 3];
        }

        float zBuffer(int x, int y) const
        {
            return z_buffer[width * y + x];
        }

        float& zBuffer(int x, int y)
        {
            return z_buffer[width * y + x];
        }

        void processVertex(uint16_t index, Attributes& vertex_output_attributes, vec4& saved_gl_position)
        {
            if (index >= vertex_count)
            {
                std::cerr << "Out of vertex index " << index << std::endl;
                return;
            }
            void* vertex_buffer_attributes = static_cast<unsigned char*>(vertex_buffer) + vertex_size * index;
            input_attributes.loadVaryingFromVertexBuffer(vertex_buffer_attributes);
            vertex_shader_main(); // Call vertex shader for the triangle first vertex
            saved_gl_position = gl_Position; // Save output vertex
            output_attributes.saveVarying(); // Save vertex shader output variables
            vertex_output_attributes = output_attributes;
        }

    public:
        Attributes input_attributes;
        Attributes output_attributes;

        CPURendering(unsigned width_, unsigned height_) : width(width_), height(height_), size(width * height)
        {
            rgba_buffer.resize(size * 4, 0);
            z_buffer.resize(size, 4194304.0f);

            vertex_buffer = 0;
            vertex_count = 0;
            index_buffer = 0;
            triangle_count = 0;
            vertex_size = 0;
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
                assert(false);
                return;
            }

            if (!vertex_buffer || !vertex_count)
            {
                std::cerr << "Vertex buffer is not specified or vertex count is zero" << std::endl;
                assert(false);
                return;
            }

            vertex_size = input_attributes.getAttributesSize();
            if (vertex_size == 0)
            {
                std::cerr << "Input Vertex buffer attributes are empty!" << std::endl;
                assert(false);
                return;
            }

            for (size_t triangle_index = 0; triangle_index < triangle_count; ++triangle_index)
            {
                uint16_t triangle[3];
                triangle[0] = index_buffer[triangle_index * 3 + 0];
                triangle[1] = index_buffer[triangle_index * 3 + 1];
                triangle[2] = index_buffer[triangle_index * 3 + 2];

                Attributes first_vertex_output_data;
                vec4 first_gl_position;
                processVertex(triangle[0], first_vertex_output_data, first_gl_position);

                Attributes second_vertex_output_data;
                vec4 second_gl_position;
                processVertex(triangle[1], second_vertex_output_data, second_gl_position);

                Attributes third_vertex_output_data;
                vec4 third_gl_position;
                processVertex(triangle[2], third_vertex_output_data, third_gl_position);

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

                                    Attributes interim_vertex_output_data = first_vertex_output_data.interpolate(second_vertex_output_data, nx);
                                    Attributes result_vertex_output_data = interim_vertex_output_data.interpolate(third_vertex_output_data, ny);

                                    result_vertex_output_data.loadVarying(); // Set vertex shader outputs / fragment shader inputs
                                    fragment_shader_main(); // Call fragment shader for the current pixel with interpolated attribute values

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
            for (int y = static_cast<int>(height) - 1; y >= 0; --y)
            {
                for (int x = 0; static_cast<unsigned>(x) < width; ++x)
                {
                    out_file << static_cast<int>(rBuffer(x, y)) << " " << static_cast<int>(gBuffer(x, y)) << " " << static_cast<int>(bBuffer(x, y)) << " ";
                }
                out_file << "\n";
            }
        }
    };
}
