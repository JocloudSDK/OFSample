#include "EffectRender.h"
#include <assert.h>
#include <string.h>
#include <string>
#include <vector>
#import <AVFoundation/AVFoundation.h>
#import <OpenGLES/ES3/glext.h>
#import <UIKit/UIImage.h>

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))

static const float MatIdentity[] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};
static const float MatFlipY[] = {
    1.0f,  0.0f, 0.0f, 0.0f,
    0.0f, -1.0f, 0.0f, 0.0f,
    0.0f,  0.0f, 1.0f, 0.0f,
    0.0f,  0.0f, 0.0f, 1.0f,
};

EffectRender::EffectRender(EAGLContext* context, int width, int height, const char* dataPath)
{
    CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, (CVEAGLContext)context, NULL, &_textureCache);
    
    // screen size
    _width = width;
    _height = height;
    _dataPath = dataPath;
    
    glGenFramebuffers(1, &_fbo);
    
    this->InitQuad();
    this->InitCopyProgram();

    _time = [NSDate date].timeIntervalSince1970;
}


EffectRender::~EffectRender()
{
    if (_context != OF_INVALID_HANDLE)
    {
        if (_beautyEffect != OF_INVALID_HANDLE)
        {
            OF_DestroyEffect(_context, _beautyEffect);
        }
        if (_filterEffect != OF_INVALID_HANDLE)
        {
            OF_DestroyEffect(_context, _filterEffect);
        }
        if (_stickerEffect != OF_INVALID_HANDLE)
        {
            OF_DestroyEffect(_context, _stickerEffect);
        }
        OF_DestroyContext(_context);
    }

    if (_inTex != 0)
    {
        glDeleteTextures(1, &_inTex);
    }
    if (_outTex != 0)
    {
        glDeleteTextures(1, &_outTex);
    }
    
    glDeleteProgram(_copyProgram);
    glDeleteBuffers(1, &_quadVbo);
    glDeleteBuffers(1, &_quadIbo);
    glDeleteFramebuffers(1, &_fbo);
    
    if (_inputImageData)
    {
        free(_inputImageData);
        _inputImageData = nullptr;
    }
    if (_outputImageData)
    {
        free(_outputImageData);
        _outputImageData = nullptr;
    }
    CFRelease(_textureCache);
}

void EffectRender::ApplyFrame(OF_Texture* inTex, OF_Texture* outTex, OF_FrameData* frameData)
{
    OrangeHelper::GLTexture texIn, texOut;
    texIn.textureId = inTex->textureID;
    texIn.width = inTex->width;
    texIn.height = inTex->height;
    texIn.format = inTex->format;
    texIn.target = inTex->target;
    
    texOut.textureId = outTex->textureID;
    texOut.width = outTex->width;
    texOut.height = outTex->height;
    texOut.format = outTex->format;
    texOut.target = outTex->target;
    
    OrangeHelper::ImageInfo image;
    memset(&image, 0, sizeof(image));
    image.data = frameData->imageData;
    image.orientation = frameData->orientation;
    image.width = frameData->width;
    image.height = frameData->height;
    image.format = frameData->format;
    image.frontCamera = frameData->frontCamera;
    image.timestamp = frameData->timestamp;
    image.dir = frameData->imageDir;
    
    bool ret = _ofHelper->updateFrameParams(texIn, texOut, image);
    if (ret == false)
    {
        // copy
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outTex->textureID, 0);
        glViewport(0, 0, outTex->width, outTex->height);
        
        this->DrawQuad( inTex->textureID , MatFlipY);
    }
}

CVPixelBufferRef data2PiexlBuffer(uint8_t* data, int width, int height)
{
    CVPixelBufferRef outPixebuffer = nullptr;
    
    //
    // data to UIImage
    //
    size_t bitsPerComponent = 8;
//    size_t bitsPerPixel = 32;
    size_t bytesPerRow = static_cast<size_t>(4 * width);
    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    
    // set the alpha mode RGBA
    CGBitmapInfo bitmapInfo = kCGImageAlphaPremultipliedLast;
    
    ////
    // This method is much simple and without coordinate flip
    // You can use this method either
    // but the UIGraphicsBeginImageContext() is much more modern.
    ////
    
    CGContextRef cgBitmapCtx = CGBitmapContextCreate(data,
                                                     static_cast<size_t>(width),
                                                     static_cast<size_t>(height),
                                                     bitsPerComponent,
                                                     bytesPerRow,
                                                     colorSpaceRef,
                                                     bitmapInfo);
    CGImageRef cgImg = CGBitmapContextCreateImage(cgBitmapCtx);
    
    UIImage *uiImage = [UIImage imageWithCGImage:cgImg];
    CGContextRelease(cgBitmapCtx);
    CGColorSpaceRelease(colorSpaceRef);
    CGImageRelease(cgImg);
    
    //
    // UIImage to CVPixelBufferRef
    //
    CGSize frameSize = CGSizeMake(CGImageGetWidth(uiImage.CGImage),CGImageGetHeight(uiImage.CGImage));
    NSDictionary *options =
    [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:YES],kCVPixelBufferCGImageCompatibilityKey,[NSNumber numberWithBool:YES],kCVPixelBufferCGBitmapContextCompatibilityKey,nil];
    CVReturn status =
    CVPixelBufferCreate(kCFAllocatorDefault, frameSize.width, frameSize.height,kCVPixelFormatType_32BGRA, (__bridge CFDictionaryRef)options, &outPixebuffer);
    assert(status == kCVReturnSuccess && outPixebuffer != NULL);
    CVPixelBufferLockBaseAddress(outPixebuffer, 0);
    void *pxdata = CVPixelBufferGetBaseAddress(outPixebuffer);
    CGColorSpaceRef rgbColorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context_img = CGBitmapContextCreate(pxdata, frameSize.width, frameSize.height,8, CVPixelBufferGetBytesPerRow(outPixebuffer),rgbColorSpace,(CGBitmapInfo)kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst);
    CGContextDrawImage(context_img, CGRectMake(0, 0, CGImageGetWidth(uiImage.CGImage),CGImageGetHeight(uiImage.CGImage)), uiImage.CGImage);
    CGColorSpaceRelease(rgbColorSpace);
    CGContextRelease(context_img);
    CVPixelBufferUnlockBaseAddress(outPixebuffer, 0);
    
    return outPixebuffer;
}


CVPixelBufferRef EffectRender::Render(CVPixelBufferRef inPixelBuffer, int dir)
{
    CVPixelBufferRef outPixelBuffer = nullptr;
    if(!inPixelBuffer)
    {
        return outPixelBuffer;
    }
    
    CVPixelBufferLockBaseAddress(inPixelBuffer, 0);
    CVOpenGLESTextureRef inputTexture = nullptr;
    int width  = (int) CVPixelBufferGetWidth(inPixelBuffer);
    int height = (int) CVPixelBufferGetHeight(inPixelBuffer);
    uint8_t* baseAddress = (uint8_t*)inPixelBuffer;
    int cameraImageDataPitch = (int) CVPixelBufferGetBytesPerRow(inPixelBuffer);
    CVReturn err = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault, _textureCache, inPixelBuffer, NULL, GL_TEXTURE_2D, GL_RGBA, width, height, GL_BGRA, GL_UNSIGNED_BYTE, 0, &inputTexture);
    assert(kCVReturnSuccess == err);
    GLenum target = CVOpenGLESTextureGetTarget(inputTexture);
    assert(GL_TEXTURE_2D == target);
    _inTex = CVOpenGLESTextureGetName(inputTexture);
    glBindTexture(GL_TEXTURE_2D, _inTex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    CVPixelBufferUnlockBaseAddress(inPixelBuffer, 0);
    
    OF_FrameData frameData;
    memset(&frameData, 0, sizeof(frameData));
    frameData.width     = width;
    frameData.height    = height;
    frameData.format    = OF_PixelFormat_BGR32;
    frameData.imageData = baseAddress;
    frameData.widthStep = cameraImageDataPitch;
    frameData.timestamp = (float) ([NSDate date].timeIntervalSince1970 - _time);
    frameData.isUseCustomHarsLib = OF_FALSE;
    frameData.imageDir  = dir;
    
    if (inputTexture)
    {
        if (0 == _outTex)
        {
            glGenTextures(1, &_outTex);
            glBindTexture(GL_TEXTURE_2D, _outTex);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, OF_NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        
        assert(glGetError() == 0);
        
        GLint oldFbo;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFbo);
        
        // apply frame
        OF_Texture inTex;
        inTex.target    = GL_TEXTURE_2D;
        inTex.width     = width;
        inTex.height    = height;
        inTex.format    = GL_RGBA;
        inTex.textureID = _inTex;
        
        OF_Texture outTex;
        outTex.target    = GL_TEXTURE_2D;
        outTex.width     = width;
        outTex.height    = height;
        outTex.format    = GL_RGBA;
        outTex.textureID = _outTex;
        
        // clear out first
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _outTex, 0);
        glViewport(0, 0, width, height);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        
        this->ApplyFrame(&inTex, &outTex, &frameData);
        
        assert(glGetError() == 0);
        
//        // read pixels to data
//        if (nullptr == _outputImageData || _outputImageDataSize < width * height * 4)
//        {
//            _outputImageDataSize = width * height * 4;
//            _outputImageData = (uint8_t*) realloc(_outputImageData, width * height * 4);
//        }
//        /*
//        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
//        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _outTex, 0);
//        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, _outputImageData);
//         */
//        OF_ReadPixels(_context, &outTex, _outputImageData);
//        assert(glGetError() == 0);
//
//        outPixelBuffer = data2PiexlBuffer(_outputImageData, width, height);
        CFRelease(inputTexture);
        
        //
        // output to view
        //
        glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);
        glViewport(0, 0, _width, _height);
        glClear(GL_COLOR_BUFFER_BIT);
        int x, y, w, h;
#if 1
        // clip
        if (_height / (float) _width > height / (float) width)
        {
            h = _height;
            w = h * width / height;
        }
        else
        {
            w = _width;
            h = w * height / width;
        }
#else
        // center
        if (_height / (float) _width > height / (float) width)
        {
            w = _width;
            h = w * height / width;
        }
        else
        {
            h = _height;
            w = h * width / height;
        }
#endif
        x = 0.5 * (_width - w);
        y = 0.5 * (_height - h);
        glViewport(x, y, w, h);
        this->DrawQuad(_outTex, MatIdentity);
        assert(glGetError() == 0);
    }
    return outPixelBuffer;
}

void EffectRender::InitQuad()
{
    float vertices[] = {
        -1.0f,  1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 1.0f,
         1.0f,  1.0f, 1.0f, 0.0f
    };
    glGenBuffers(1, &_quadVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _quadVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    unsigned short indices[] = {
        0, 1, 2, 0, 2, 3
    };
    glGenBuffers(1, &_quadIbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quadIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void EffectRender::InitCopyProgram()
{
    const char* v = R"(
        uniform mat4 uMat;
        attribute vec2 aPos;
        attribute vec2 aUV;
        varying vec2 vUV;
        void main()
        {
            gl_Position = uMat * vec4(aPos, 0.0, 1.0);
            vUV = aUV;
        }
    )";
    const char* f = R"(
        precision mediump float;
        uniform sampler2D uTexture;
        varying vec2 vUV;
        void main()
        {
            gl_FragColor = texture2D(uTexture, vUV);
        }
    )";
    
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &v, OF_NULL);
    glCompileShader(vs);
    
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &f, OF_NULL);
    glCompileShader(fs);
    
    _copyProgram = glCreateProgram();
    glAttachShader(_copyProgram, vs);
    glAttachShader(_copyProgram, fs);
    glLinkProgram(_copyProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);
}

void EffectRender::DrawQuad(GLuint tex, const float* transform)
{
    int loc;
    glUseProgram(_copyProgram);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    loc = glGetUniformLocation(_copyProgram, "uTexture");
    glUniform1i(loc, 0);
    
    loc = glGetUniformLocation(_copyProgram, "uMat");
    glUniformMatrix4fv(loc, 1, false, transform);
    
    glBindBuffer(GL_ARRAY_BUFFER, _quadVbo);
    
    loc = glGetAttribLocation(_copyProgram, "aPos");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, false, 4 * 4, 0);
    loc = glGetAttribLocation(_copyProgram, "aUV");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, false, 4 * 4, (const void*) (4 * 2));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quadIbo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}
