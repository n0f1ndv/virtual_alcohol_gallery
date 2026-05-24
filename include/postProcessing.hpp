#pragma once

class PostProcessing {
public:
    float width;
    float height;

    PostProcessing(float width, float height);
    ~PostProcessing();

    void Setup(float updatedWidth, float updatedHeight);
    void Bind();
    void BindDefault();
    void Draw();

private:
    unsigned int VAO, VBO, FBO, RBO;
    unsigned int texture;
};
