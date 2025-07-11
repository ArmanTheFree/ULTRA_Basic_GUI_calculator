#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_VARARGS


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600


#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION


#include "nuklear.h"
#include"nuklear_glfw_gl3.h"
#include<GLFW/glfw3.h>



int main(){
    if(!glfwInit()){
        fprintf(stderr, "glfw failed!\n");
        exit(1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow * wind = glfwCreateWindow(WIDTH, HEIGHT, "CAL", 0, 0);
    glfwMakeContextCurrent(wind);

    struct nk_context * nukContext;
    struct nk_glfw  glfwContext;
    nukContext = nk_glfw3_init(&glfwContext, wind, NK_GLFW3_INSTALL_CALLBACKS);

    struct nk_colorf backGrndColor;
    backGrndColor.r = 0.2f;
    backGrndColor.b = 0.5f;
    backGrndColor.g = 1.0f;
    backGrndColor.a = 1.0f;

    struct nk_font_atlas *fontAtlas;
    nk_glfw3_font_stash_begin(&glfwContext, &fontAtlas);
    nk_glfw3_font_stash_end(&glfwContext);

    while(!glfwWindowShouldClose(wind)){
        glfwPollEvents();
        nk_glfw3_new_frame(&glfwContext);
        if(nk_begin(nukContext, "CAL", nk_rect(50,50,100,500), NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
        NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)){
            nk_layout_row_static(nukContext, 30, 80, 1);
            if (nk_button_label(nukContext, "Click me")) {
                printf("Button pressed!\n");
            }
        }
        nk_end(nukContext);
    }



    return 0;
}