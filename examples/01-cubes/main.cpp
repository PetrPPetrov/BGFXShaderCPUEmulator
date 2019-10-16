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

#define main vertex_shader_main
#include "vs_cubes.sc"
#undef main

#define main fragment_shader_main
#include "fs_cubes.sc"
#undef main

#include "bgfx_cpu_emulation.h"

int main()
{
    CPURendering renderer(10, 10);

    renderer.addAttribute(&a_position);
    renderer.addAttribute(&a_color0);
    renderer.addOutputAttribute(&v_color0);

    struct vertex_data
    {
        vec3 position;
        vec4 color;
    };
    vertex_data vertex_data[] =
    {
        { { 0.0f, 0.0f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f } },
        { { 300.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { 0.0f, 140.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
    };
    uint16_t triangles[] = { 0, 1, 2 };

    renderer.setVertexBuffer(vertex_data, sizeof(vertex_data) / sizeof(vertex_data[0]));
    renderer.setIndexBuffer(triangles, sizeof(triangles) / sizeof(triangles[0]) / 3);
    renderer.render();

    renderer.saveToPPM("screen.ppm");

    return 0;
}
