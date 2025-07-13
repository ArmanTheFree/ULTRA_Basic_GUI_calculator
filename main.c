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

//struct nk_colorf bg;
char input_buffer[10] = {0};
char history_buffer[256] = {0};
long long unsigned n_input =0 ;


void createKeys(struct nk_context *,float);
void handelInput(char);
void changeCurrentSign();
void calculateResults();


int main(void){
    //bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
    if(!glfwInit()){
        fprintf(stderr, "glfw init failed");
        exit(1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    GLFWwindow *wind = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CAL", NULL, NULL);
    glfwMakeContextCurrent(wind);

    if(glewInit() != GLEW_OK){
        fprintf(stderr, "glew init fialed \n");
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
    ctx->style.button.hover  = nk_style_item_color(nk_rgb(255/3, 255/3, 255/3)); 

    int beginY = WINDOW_HEIGHT- WINDOW_HEIGHT/3.0f;

    while(!glfwWindowShouldClose(wind)){
        glfwPollEvents();
        nk_glfw3_new_frame(&glfw);
        
        createKeys(ctx,beginY);
        nk_end(ctx);
        glClear(GL_COLOR_BUFFER_BIT);
        nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        glfwSwapBuffers(wind);
        static int frame = 0;
        //printf("frame: %d\n", ++frame);
    }
    nk_glfw3_shutdown(&glfw);
    glfwTerminate();

    return 0;
}


void createKeys(struct nk_context* ctx, float beginY){

    if(nk_begin(ctx, "CAL", nk_rect(0.0f,beginY ,600.0f, WINDOW_HEIGHT/3.0f),NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BACKGROUND | NK_WINDOW_BORDER)){

        nk_layout_row_dynamic(ctx,  (WINDOW_HEIGHT/3.0f)/4.33f , 4);
        if(nk_button_label(ctx, "C")) handelInput('C');
        if(nk_button_label(ctx, "x²"))handelInput("x²");
        if(nk_button_label(ctx, "sqrt"))handelInput("sqrt");//since √ is not included(can be fixed byt adding a separate font but sqrt gets the job done I guess)
        if(nk_button_label(ctx, "="))calculateResults ('=');

        nk_layout_row_dynamic(ctx,  (WINDOW_HEIGHT/3.0f)/4.33f , 6);
        if(nk_button_label(ctx, "1"))handelInput('1');
        if(nk_button_label(ctx, "2"))handelInput('2');
        if(nk_button_label(ctx, "3"))handelInput('3');
        if(nk_button_label(ctx, "+"))handelInput('+');
        if(nk_button_label(ctx, "x"))handelInput('x');
        if(nk_button_label(ctx, "%"))handelInput('%');

        if(nk_button_label(ctx, "4"))handelInput('4');
        if(nk_button_label(ctx, "5"))handelInput('5');
        if(nk_button_label(ctx, "6"))handelInput('6');
        if(nk_button_label(ctx, "-"))handelInput('-');
        if(nk_button_label(ctx, "÷"))handelInput("÷");
        if(nk_button_label(ctx, "(-)"))changeCurrentSign();

        if(nk_button_label(ctx, "7"))handelInput('7');
        if(nk_button_label(ctx, "8"))handelInput('8');
        if(nk_button_label(ctx, "9"))handelInput('9');
        if(nk_button_label(ctx, "0"))handelInput('0');
        if(nk_button_label(ctx, "("))handelInput('(');
        if(nk_button_label(ctx, ")"))handelInput(')');

    }
}

void handelInput(char input){
    if(n_input <=256) {printf("buffer is full\n"); return;} // on console-less apps this wont be of any use
    input_buffer[n_input] = input;
    n_input ++;

}

void changeCurrentSign(){}
void calculateResults(){}