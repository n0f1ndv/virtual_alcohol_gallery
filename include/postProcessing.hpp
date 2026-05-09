#pragma once

class PostProcessing {
public:
    PostProcessing(int width, int height);
    ~PostProcessing();

    void Bind();
    void BindDefault();
    void Draw();

private:
    unsigned int VAO, VBO, FBO, RBO;
    unsigned int texture;
    int width;
    int height;
};