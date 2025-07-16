#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#include <stack>
#include<iostream>

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

using namespace std;

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 800

string inputBuffer;
string historyBuffer;

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

double apply(double a, double b, char op) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') return a / b;
    return 0;
}




void createKeys(struct nk_context *,float);
void handelInput(string);
void createOutputFrame(struct nk_context*, float);
void evaluate();



int main(void){
    int viewPortH, viewPortW;
    int beginY = WINDOW_HEIGHT- WINDOW_HEIGHT/3.0f;
    const char * windowName = "Basic GUI Calcualtor";
    GLFWwindow *wind;

    struct nk_context * ctx;
    struct nk_font_atlas *atlas;
    struct nk_glfw glfw = {0};

    if(!glfwInit()){
        fprintf(stderr, "glfw init failed");
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);//for static sized window, comment out for resizabality, no functionality is tested with resizable windows
    
    wind = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, windowName, NULL, NULL);
    glfwMakeContextCurrent(wind);

    if(glewInit() != GLEW_OK){
        fprintf(stderr, "glew init fialed \n");
        exit(1);
    }
    glfwGetWindowSize(wind, &viewPortW, &viewPortH);
    glViewport(0, 0, viewPortW, viewPortH);
    
    ctx = nk_glfw3_init(&glfw, wind, NK_GLFW3_INSTALL_CALLBACKS);

    glClearColor(0.10f, 0.18f, 0.24f, 1.0f);
    

    nk_glfw3_font_stash_begin(&glfw, &atlas);
    nk_glfw3_font_stash_end(&glfw);
    ctx->style.button.hover  = nk_style_item_color(nk_rgb(255/3, 255/3, 255/3)); 

    

    while(!glfwWindowShouldClose(wind)){
        glfwPollEvents();
        nk_glfw3_new_frame(&glfw);
        createKeys(ctx,beginY);
        createOutputFrame(ctx, WINDOW_HEIGHT/3.0f*2.0f);
        
        glClear(GL_COLOR_BUFFER_BIT);
        nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        glfwSwapBuffers(wind);

    }

    
    nk_glfw3_shutdown(&glfw);
    nk_glfw3_device_destroy(&glfw);
    glfwDestroyWindow(wind);
    glfwTerminate();

    return 0;
}


void createKeys(struct nk_context* ctx, float beginY){

    if(nk_begin(ctx, "Keys", nk_rect(0.0f,beginY-100 ,600.0f, WINDOW_HEIGHT/3.0f+100),NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BACKGROUND | NK_WINDOW_BORDER)){

        nk_layout_row_dynamic(ctx,  (WINDOW_HEIGHT/3.0f)/3.95f , 1);
        if(nk_button_label(ctx, "CLEAR")) inputBuffer.clear();



        nk_layout_row_dynamic(ctx,  (WINDOW_HEIGHT/3.0f)/3.95f , 4);
        if(nk_button_label(ctx, "<-")) inputBuffer.pop_back();
        if(nk_button_label(ctx, "="))evaluate();
        if(nk_button_label(ctx, "-"))inputBuffer.append("-");
        if(nk_button_label(ctx, "+"))inputBuffer.append("+");
        
        if(nk_button_label(ctx, "1"))inputBuffer.append("1");
        if(nk_button_label(ctx, "2"))inputBuffer.append("2");
        if(nk_button_label(ctx, "3"))inputBuffer.append("3");
        if(nk_button_label(ctx, "*"))inputBuffer.append("*");

        if(nk_button_label(ctx, "4"))inputBuffer.append("4");
        if(nk_button_label(ctx, "5"))inputBuffer.append("5");
        if(nk_button_label(ctx, "6"))inputBuffer.append("6");
        if(nk_button_label(ctx, "/"))inputBuffer.append("/");
        
        if(nk_button_label(ctx, "7"))inputBuffer.append("7");
        if(nk_button_label(ctx, "8"))inputBuffer.append("8");
        if(nk_button_label(ctx, "9"))inputBuffer.append("9");
        if(nk_button_label(ctx, "0"))inputBuffer.append("0");


    }
    nk_end(ctx);

}

void createOutputFrame(struct nk_context* ctx, float inputH){

    if(nk_begin(ctx, "input section", nk_rect(0.0f, 0.0f,WINDOW_WIDTH, inputH-100), NK_WINDOW_BORDER| NK_WINDOW_BACKGROUND|NK_WINDOW_NO_SCROLLBAR)){

        nk_layout_row_dynamic(ctx, inputH*(1.0f/3.0f)/2.0f, 1);
        nk_label(ctx, historyBuffer.c_str(), NK_TEXT_RIGHT);


        nk_layout_row_dynamic(ctx, inputH*(2.0f/3.0f), 1);
        nk_label(ctx, inputBuffer.c_str(), NK_TEXT_LEFT);
    }
    nk_end(ctx);
}




void evaluate() {
    if (inputBuffer.empty()) return;

    stack<double> values;
    stack<char> ops;
    size_t i = 0;

    while (i < inputBuffer.size()) {
        if (isspace(inputBuffer[i])) {
            i++;
            continue;
        }

        if (isdigit(inputBuffer[i])) {
            double val = 0;
            while (i < inputBuffer.size() && isdigit(inputBuffer[i])) {
                val = val * 10 + (inputBuffer[i] - '0');
                i++;
            }
            values.push(val);
        }
        else {
            char currOp = inputBuffer[i];
            while (!ops.empty() && precedence(ops.top()) >= precedence(currOp)) {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(apply(a, b, op));
            }
            ops.push(currOp);
            i++;
        }
    }

    while (!ops.empty()) {
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(apply(a, b, op));
    }

    double result = values.top();
    historyBuffer = inputBuffer + " = " + to_string(result);
    inputBuffer = "";
}