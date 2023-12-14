#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <Windows.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <bits/stdc++.h>
#define uint unsigned int
#define ASSERT(x) if (!(x)) assert(false)
#define glCall(x) GLClearError();\
    x;\
    ASSERT(GLCheckError())
int logicArray[3][3];
int inputarr[9];
const float two3rds = (2.0f / 3.0f);
const float one3rd = 1.0f - two3rds;
bool gameover = 0;
//functions
void framebuffer_resize(GLFWwindow * w, int width, int height) {
    glViewport(0, 0, width, height);
}
void initArr() {
    int index = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            logicArray[i][j] = inputarr[index];
            index++;
        }
    }
}
void play(int x, int y) {
    gameover = 0;
    bool free = false;
    int rMove[4] = { -1,0,0,1 };
    int cMove[4] = { 0,-1,1,0 };
    for (int i = 0; i < 4; i++) {
        if (rMove[i] + x < 0 || rMove[i] + x>2 || cMove[i] + y < 0 || cMove[i] + y>2);
        else {
            if (logicArray[rMove[i] + x][cMove[i] + y] == 0) {
                int c = logicArray[x][y];
                logicArray[x][y] = logicArray[rMove[i] + x][cMove[i] + y];
                logicArray[rMove[i] + x][cMove[i] + y] = c;
                free = true;
            }
        }
    }
    if (!free)
        gameover = 1;
    //printArray();
    int count = 0;
    bool flag = false;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            count++;
            if (i == 2 && j == 2) {
                gameover = 1;
            }
            else if (logicArray[i][j] != count) {
                flag = true;
                break;
            }
            
        }
        if (flag) {
            break;
        }
    }
}
void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    int i=-1, j=-1;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        i =   (ypos >= 0.0f && ypos < 125.0f)
            + (ypos >= 125.0f && ypos < 250.0f)*2
            + (ypos >= 250.0f && ypos <= 375.0f)*3;
        j =   (xpos >= 0.0f && xpos < 400.0f * one3rd)
            + (xpos >= 400.0f * one3rd && xpos < 400.0f * two3rds)*2
            + (xpos >= 400.0f * two3rds && xpos <= 400.0f)*3;
        if (i > 0) {
            play(i - 1, j - 1);
            PlaySound(TEXT("C:/Users/LEGION/Desktop/open gl/second_lab/sounds/click.wav"), NULL, SND_SYNC);
        }
        else{
            gameover = false;
            initArr();
        }
    }
}

glm::vec3 translate(int number) {
    int index = 0;
    bool flag = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (number == logicArray[i][j]) {
                flag = 1;
                break;
            }
            index++;
        }
        if (flag)
            break;
    }
    switch (index) {
    case 0://do nothing
        return glm::vec3(0.0f, 0.0f, 0.0f);
    case 1://to the right
        return glm::vec3(two3rds, 0.0f, 0.0f);
    case 2://to the right twice
        return glm::vec3(2.0f * two3rds, 0.0f, 0.0f);
    case 3://down
        return glm::vec3(0.0f, -0.5f, 0.0f);
    case 4://right down
        return glm::vec3(two3rds, -0.5f, 0.0f);
    case 5://right twice then down
        return glm::vec3(2.0f * two3rds, -0.5f, 0.0f);
    case 6://down twice
        return glm::vec3(0.0f, -1.0f, 0.0f);
    case 7://down twice then right
        return glm::vec3(two3rds, -1.0f, 0.0f);
    case 8://down twice right twice
        return glm::vec3(2.0f * two3rds, -1.0f, 0.0f);
    }
}
//main
int main(int argc, char* argv[]) {
    //.\puzzle_game.exe 1 2 3 4 5 6 7 0 8
    if (argc != 10) {
        std::cout << "Invalid Input\n";
        return 1;  // Return an error code
    }
    std::set<int> values;
    for (int i = 1; i < argc; ++i) {
        int temp = std::atoi(argv[i]);
        values.insert(temp);
        std::cout << temp << std::endl;
        if (temp < 0 or temp >= 9) {
            std::cout << "Invalid Input\n";
            return 1;
        }
        inputarr[i - 1] = temp;
    }
    if (values.size() < 9) {
        std::cout << "Invalid Input\n";
        return 1;
    }
    initArr();
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(400, 500, "Sliding numbers", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    std::cout << "Using GL Version: " << glGetString(GL_VERSION) << std::endl;
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glfwSetFramebufferSizeCallback(window, framebuffer_resize);
    glfwSetMouseButtonCallback(window, mouse_callback);

    //-----------------------------------------------//
    Shader game = Shader("C:/Users/LEGION/Desktop/open gl/second_lab/vertexShader.vs", "C:/Users/LEGION/Desktop/open gl/second_lab/fragmentShader.fs");
    //our vertices
    float one[] = {
        //position            //texCoord
        -1.0f   ,  1.0f,    0.0f   ,  1.0f, //top left
        -1.0f   ,  0.5f,    0.0f , two3rds, //bottom left
        -one3rd  , 1.0f,    one3rd  , 1.0f, //top right

        -1.0f  ,   0.5f,    0.0f , two3rds, //bottom left
        -one3rd  , 1.0f,    one3rd  , 1.0f, //top right
        -one3rd,   0.5f,     one3rd,two3rds, //bottom right
    };
    float two[] = {
        //position            //texCoord
        -1.0f   ,  1.0f,    one3rd   ,  1.0f, //top left
        -1.0f   ,  0.5f,    one3rd , two3rds, //bottom left
        -one3rd  , 1.0f,    two3rds   , 1.0f, //top right

        -1.0f  ,   0.5f,    one3rd , two3rds, //bottom left
        -one3rd  , 1.0f,    two3rds   , 1.0f, //top right
        -one3rd,   0.5f,     two3rds ,two3rds, //bottom right
    };
    float three[] = {
        //position            //texCoord
        -1.0f   ,  1.0f,    two3rds  ,  1.0f, //top left
        -1.0f  ,   0.5f,    two3rds, two3rds, //bottom left
        -one3rd  , 1.0f,    1.0f   ,    1.0f, //top right

        -1.0f  ,   0.5f,    two3rds, two3rds, //bottom left
        -one3rd  , 1.0f,    1.0f   ,    1.0f, //top right
        -one3rd ,  0.5f,     1.0f  ,  two3rds,//bottom right
    };
    float four[] = {
        //position            //texCoord
        -1.0f   ,  1.0f,    0.0f  ,  two3rds, //top left
        -1.0f  ,   0.5f,    0.0f  ,   one3rd, //bottom left
        -one3rd  , 1.0f,    one3rd , two3rds, //top right

        -1.0f  ,   0.5f,    0.0f  ,   one3rd, //bottom left
        -one3rd  , 1.0f,    one3rd,  two3rds, //top right
        -one3rd,   0.5f,     one3rd,   one3rd, //bottom right
    };
    float five[] = {
        //position            //texCoord
        -1.0f   ,  1.0f,    one3rd , two3rds, //top left
        -1.0f  ,   0.5f,    one3rd  , one3rd, //bottom left
        -one3rd  , 1.0f,    two3rds, two3rds, //top right

        -1.0f  ,   0.5f,    one3rd  , one3rd, //bottom left
        -one3rd  , 1.0f,    two3rds, two3rds, //top right
        -one3rd,   0.5f,     two3rds , one3rd, //bottom right
    };
    float six[] = {
        //position            //texCoord
        -1.0f   ,  1.0f,    two3rds, two3rds, //top left
        -1.0f  ,   0.5f,    two3rds , one3rd, //bottom left
        -one3rd  , 1.0f,    1.0f   , two3rds, //top right

        -1.0f  ,   0.5f,    two3rds , one3rd, //bottom left
        -one3rd  , 1.0f,    1.0f   , two3rds, //top right
        -one3rd,   0.5f,     1.0f    , one3rd, //bottom right
    };
    float seven[] = {
        //position            //texCoord
        -1.0f   ,  1.0f,    0.0f,     one3rd, //top left
        -1.0f  ,   0.5f,    0.0f,       0.0f, //bottom left
        -one3rd  , 1.0f,    one3rd  , one3rd, //top right

        -1.0f  ,   0.5f,    0.0f,       0.0f, //bottom left
        -one3rd  , 1.0f,    one3rd  , one3rd, //top right
        -one3rd,   0.5f,     one3rd    , 0.0f, //bottom right
    };
    float eight[] = {
        //position            //texCoord
        -1.0f   ,  1.0f,    one3rd,   one3rd, //top left
        -1.0f  ,   0.5f,    one3rd    , 0.0f, //bottom left
        -one3rd  , 1.0f,    two3rds , one3rd, //top right

        -1.0f  ,   0.5f,    one3rd    , 0.0f, //bottom left
        -one3rd  , 1.0f,    two3rds , one3rd, //top right
        -one3rd,   0.5f,      two3rds   , 0.0f, //bottom right
    };
    float zero[] = {
        //position            //texCoord
        -1.0f   ,  1.0f,    two3rds,   one3rd, //top left
        -1.0f  ,   0.5f,    two3rds    , 0.0f, //bottom left
        -one3rd  , 1.0f,    1.0f  ,    one3rd, //top right

        -1.0f  ,   0.5f,    two3rds    , 0.0f, //bottom left
        -one3rd  , 1.0f,    1.0f  ,    one3rd, //top right
        -one3rd,   0.5f,     1.0f     ,   0.0f, //bottom right
    };
    float restart[] = {
        -1.0f ,0.5,          0.0f,1.0f,//top left
        -1.0f,0.0f,          0.0f,0.0f,//bottom left
         1.0f,0.5f,          1.0f,1.0f,//top right

        -1.0f,0.0f,          0.0f,0.0f,//bottom left
         1.0f  ,0.5,         1.0f,1.0f,//top right
         1.0f,0.0f,          1.0f,0.0f //top left
    };
    //our buffers
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //binding VAO first
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //assigning vertices from our array
    glBufferData(GL_ARRAY_BUFFER, sizeof(eight), eight, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //textures
    unsigned int grid;
    glGenTextures(1, &grid);
    glBindTexture(GL_TEXTURE_2D, grid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    auto data = stbi_load("C:/Users/LEGION/Desktop/open gl/second_lab/images/goku.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load image" << std::endl;
    }
    stbi_image_free(data);
    unsigned int reset;
    glGenTextures(1, &reset);
    glBindTexture(GL_TEXTURE_2D, reset);
    data = stbi_load("C:/Users/LEGION/Desktop/open gl/second_lab/images/restart.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load image" << std::endl;
    }
    unsigned int win;
    glGenTextures(1, &win);
    glBindTexture(GL_TEXTURE_2D, win);
    stbi_image_free(data);
    data= stbi_load("C:/Users/LEGION/Desktop/open gl/second_lab/images/win.jpg", &width, &height, &nrChannels, 0);
    if (!data)
        std::cout << "Failed to load image\n";
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    // render loop
    // -----------
    int frames = 0;
    int seconds = 12;
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        game.use();
        glBindVertexArray(VAO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniform1i(glGetUniformLocation(game.ID, "texture1"), 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, reset);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(restart), restart);
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.0f, -1.0f, 0.0f));
        int transLoc = glGetUniformLocation(game.ID, "transform");
        glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        if (gameover) {
            trans = glm::translate(trans, glm::vec3(0.0f, 1.0f, 0.0f));
            int transLoc = glGetUniformLocation(game.ID, "transform");
            glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));
            glUniform1i(glGetUniformLocation(game.ID, "texture1"), 2);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, win);
            glDrawArrays(GL_TRIANGLES, 0, 6);

        }
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                glUniform1i(glGetUniformLocation(game.ID, "texture1"), 0);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, grid);
                int number = logicArray[i][j];
                if (number == 1)
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(one), one);
                else if (number == 2)
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(two), two);
                else if (number == 3)
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(three), three);
                else if (number == 4)
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(four), four);
                else if (number == 5)
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(five), five);
                else if (number == 6)
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(six), six);
                else if (number == 7)
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(seven), seven);
                else if (number == 8)
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(eight), eight);
                else glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(zero), zero);;
                glm::mat4 trans = glm::mat4(1.0f);
                trans = glm::translate(trans, translate(number));
                int transLoc = glGetUniformLocation(game.ID, "transform");
                glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));

                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}