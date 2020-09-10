#pragma once
#include "GL/glew.h"
#include <string>
#include "OrangHelperManager.h"

typedef struct _OF_BeautyData
{
    float beautyIntensity;
    float whiteningIntensity;
} OF_BeautyData;

typedef struct _OF_FaceLiftingData
{
	float basicTypeIntensity;
    float thinFaceIntensity;
    float smallFaceIntensity;
    float squashedFaceIntensity;
    float foreheadLiftingIntensity;
    float wideForeheadLiftingIntensity;
    float bigSmallEyeIntensity;
    float eyeOffestIntensity;
    float eyesRotationIntensity;
    float thinNoseIntensity;
    float longNoseIntensity;
    float thinNoseBridgeIntensity;
    float thinMouthIntensity;
    float movemouthIntensity;
    float chinLiftingIntensity;
} OF_FaceLiftingData;

class OrangeFilterRender
{
public:
    OrangeFilterRender();
    ~OrangeFilterRender();
    bool init(const std::string& runDir);
    bool unInit();
    bool applyFrame(OFUInt8 *srcframe, OFUInt8 *dstframe, OFUInt32 width, OFUInt32 height);
    const GLuint getOutTextureID();


private:
    void initQuad();
    void initCopyProgram();
    void initRewriteAlphaProgram();
    void drawQuad(GLuint intex, GLuint outtex, OFUInt32 width, OFUInt32 height, float* transform, GLuint currentProgram);
    void updateTexture(OFUInt8 *frame, OFUInt32 width, OFUInt32 height);
    void tearDown();
    bool copyTextureToMemory(unsigned char* data, OFUInt32 width, OFUInt32 height, OFUInt32 fbo, GLuint texID);

    GLuint m_quadVbo;
    GLuint m_quadVao;
    GLuint m_quadIbo;
    GLuint m_copyProgram;
    GLuint m_rewriteAlphaProgram;
    GLuint m_fbo;
    GLuint m_tex_in;
    GLuint m_tex_out;
    GLuint m_tex_of_out;
    OFUInt32 m_width;
    OFUInt32 m_height;
	std::string m_currentStickerPath;
};
