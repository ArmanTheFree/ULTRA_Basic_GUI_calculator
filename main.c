#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_glfw_gl3.h"


#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 800

struct nk_colorf bg;



int main(void){
    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
    if(!glfwInit()){
        fprintf(stderr, "glfw init failed\n");
        exit(1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow *wind = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CAL", NULL, NULL);
    glfwMakeContextCurrent(wind);

    if(glewInit() != GLEW_OK){
        printf(stderr, "glew init fialed \n");
        exit(1);
    }
    int wh,ww;
    glfwGetWindowSize(wind, &ww, &wh);
    glViewport(0, 0, ww, wh);
    struct nk_glfw glfw = {0};
    struct nk_context * ctx = nk_glfw3_init(&glfw, wind, NK_GLFW3_INSTALL_CALLBACKS);
    glClearColor(0.10f, 0.18f, 0.24f, 1.0f);


    struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&glfw, &atlas);
    nk_glfw3_font_stash_end(&glfw);




    while(!glfwWindowShouldClose(wind)){
        glfwPollEvents();
        nk_glfw3_new_frame(&glfw);

        if(nk_begin(ctx, "CAL", nk_rect(0,600,600,200),NK_WINDOW_BORDER)){

            nk_layout_row_static(ctx, 30, 600/6-7, 6);

            nk_button_label(ctx, "1");
            nk_button_label(ctx, "2");
            nk_button_label(ctx, "3");
            nk_button_label(ctx, "+");
            nk_button_label(ctx, "x");
            nk_button_label(ctx, "%");

        }
        nk_end(ctx);
        glClear(GL_COLOR_BUFFER_BIT);
        nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, 1000000, 100000);
        glfwSwapBuffers(wind);
        static int frame = 0;
        //printf("frame: %d\n", ++frame);
    }
    nk_glfw3_shutdown(&glfw);
    glfwTerminate();

    return 0;
}
