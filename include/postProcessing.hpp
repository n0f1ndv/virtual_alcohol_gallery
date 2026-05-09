#pragma once

class PostProcessing {
public:
    PostProcessing(int width, int height);
    ~PostProcessing();

private:
    unsigned int FBO;
    unsigned int RBO;
    unsigned int texture;
    int width;
    int height;
};