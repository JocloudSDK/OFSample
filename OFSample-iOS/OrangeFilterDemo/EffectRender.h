#pragma once

#include "of_effect/orangefilter.h"
#include <OpenGLES/ES3/gl.h>
#import <AVFoundation/AVFoundation.h>
#include <string>
#include "utils/OrangeHelper.h"

class EffectRender
{
public:
    enum MeiyanType {
        EP_baseMeiyan = 0,
        EP_SeniorMeiyan
    };

public:
    EffectRender(EAGLContext* context, int width, int height, const char* dataPath);
    ~EffectRender();
    CVPixelBufferRef Render(CVPixelBufferRef inPixelBuffer, int dir);
    OFHandle GetContext() const { return _context; }
    void setOrangeHelper(OrangeHelper* helper) { _ofHelper = helper; }
private:
    void InitQuad();
    void InitCopyProgram();
    void DrawQuad(GLuint tex, const float* transform);
    void ApplyFrame(OF_Texture* inTex, OF_Texture* outTex, OF_FrameData* frameData);
    int getFilerIdByFilterName(const char *name, MeiyanType type);
private:
    CVOpenGLESTextureCacheRef _textureCache;
    int _inputImageDataSize = 0;
    uint8_t* _inputImageData = nullptr;
    int _outputImageDataSize = 0;
    uint8_t* _outputImageData = nullptr;
    
    int _width = 0;
    int _height = 0;
    GLuint _quadVbo = 0;
    GLuint _quadIbo = 0;
    GLuint _copyProgram = 0;
    GLuint _inTex = 0;
    GLuint _outTex = 0;
    GLuint _fbo = 0;
    OFHandle _context = 0;
    std::string _dataPath;
    OFHandle _beautyEffect = 0;
    OFHandle _beautyTypeEffect = 0;
    OFHandle _filterEffect = 0;
    OFHandle _stickerEffect = 0;
    OFHandle _gestureEffect = 0;
    NSTimeInterval _time;
    OrangeHelper* _ofHelper = nullptr;
};
