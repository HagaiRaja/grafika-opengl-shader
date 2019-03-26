// background_gradients.c
// Author: Maciej Halber
// Compiled on macOS 10.12.4 with :
//          clang --std=c11 -lglfw3 -framework OpenGL background_gradients.c -o background_gradients

#include <stdio.h>
#include <stdlib.h>

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#elif defined(__linux__)
// Add your favourite extension loader
#elif defined(_WIN32) || defined(_WIN64)
// Add your favourite extension loader
#endif

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define SHADER_HEADER "#version 330 core\n"
#define SHADER_STR(x) #x
void mygl_GradientBackground( float top_r, float top_g, float top_b, float top_a,
                              float bot_r, float bot_g, float bot_b, float bot_a )
{
    glDisable(GL_DEPTH_TEST);

    static GLuint background_vao = 0;
    static GLuint background_shader = 0;

    if (background_vao == 0)
    {
        glGenVertexArrays(1, &background_vao);

        const char* vs_src = (const char*) SHADER_HEADER SHADER_STR
        (
                out vec2 v_uv;
                void main()
                {
                    uint idx = uint(gl_VertexID);
                    gl_Position = vec4( idx & 1U, idx >> 1U, 0.0, 0.5 ) * 4.0 - 1.0;
                    v_uv = vec2( gl_Position.xy * 0.5 + 0.5 );
                }
        );

        const char* fs_src = (const char*) SHADER_HEADER SHADER_STR
        (
                uniform vec4 top_color;
                uniform vec4 bot_color;
                in vec2 v_uv;
                out vec4 frag_color;

                void main()
                {
                    frag_color = bot_color * (1 - v_uv.y) + top_color * v_uv.y;
                }
        );
        GLuint vs_id, fs_id;
        vs_id = glCreateShader( GL_VERTEX_SHADER );
        fs_id = glCreateShader( GL_FRAGMENT_SHADER );
        glShaderSource(vs_id, 1, &vs_src, NULL);
        glShaderSource(fs_id, 1, &fs_src, NULL);
        glCompileShader(vs_id);
        glCompileShader(fs_id);
        background_shader = glCreateProgram();
        glAttachShader( background_shader, vs_id );
        glAttachShader( background_shader, fs_id );
        glLinkProgram(  background_shader );
        glDetachShader( background_shader, fs_id );
        glDetachShader( background_shader, vs_id );
        glDeleteShader( fs_id );
        glDeleteShader( vs_id );
        glUseProgram( background_shader );
    }

    glUseProgram( background_shader );
    GLuint top_color_loc = glGetUniformLocation( background_shader, "top_color" );
    GLuint bot_color_loc = glGetUniformLocation( background_shader, "bot_color" );
    glUniform4f( top_color_loc, top_r, top_g, top_b, top_a );
    glUniform4f( bot_color_loc, bot_r, bot_g, bot_b, bot_a );

    glBindVertexArray( background_vao );
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
}

int main( void )
{
    if( !glfwInit() ) { exit(EXIT_FAILURE); }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_RESIZABLE, GL_TRUE );

    struct GLFWwindow *window = glfwCreateWindow( 1024, 768,
                                                  "Background Gradients",
                                                  NULL, NULL );
    glfwMakeContextCurrent(window);
    while( !glfwWindowShouldClose(window) )
    {
        glClearColor(1.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        mygl_GradientBackground( 0.6, 0.8, 0.3, 1.0,
                                 0.3, 0.0, 0.6, 1.0 );

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}