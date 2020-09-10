#include "OrangeFilterRender.h"
#include "stdio.h"
#include "pathutils.h"
#include <vector>
#include <mutex>


#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))

std::recursive_mutex g_uiMutex;

OrangeFilterRender::OrangeFilterRender() :
m_quadVao(0),
m_quadVbo(0),
m_quadIbo(0),
m_copyProgram(0),
m_rewriteAlphaProgram(0),
m_tex_in(0),
m_tex_out(0),
m_tex_of_out(0),
m_fbo(0)
{
}

OrangeFilterRender::~OrangeFilterRender()
{

}

void OrangeFilterRender::tearDown()
{
    if (m_tex_in)
    {
        glDeleteTextures(1, &m_tex_in);
        m_tex_in = 0;
    }

    if (m_tex_out)
    {
        glDeleteTextures(1, &m_tex_out);
        m_tex_out = 0;
    }

    if (m_tex_of_out)
    {
        glDeleteTextures(1, &m_tex_of_out);
        m_tex_of_out = 0;
    }

    if (m_quadVao)
    {
        glDeleteVertexArrays(1, &m_quadVao);
        m_quadVao = 0;
    }

    if (m_quadVbo)
    {
        glDeleteBuffers(1, &m_quadVbo);
        m_quadVbo = 0;
    }

    if (m_quadIbo)
    {
        glDeleteBuffers(1, &m_quadIbo);
        m_quadIbo = 0;
    }

    if (m_copyProgram)
    {
        glDeleteProgram(m_copyProgram);
        m_copyProgram = 0;
    }

    if (m_rewriteAlphaProgram)
    {
        glDeleteProgram(m_rewriteAlphaProgram);
        m_rewriteAlphaProgram = 0;
    }

    if (m_fbo)
    {
        glDeleteFramebuffers(1, &m_fbo);
        m_fbo = 0;
    }
}

void OrangeFilterRender::initQuad()
{
    float vertices[] = {
        -1, 1, 0, 0,
        -1, -1, 0, 1,
        1, -1, 1, 1,
        1, 1, 1, 0
    };

    glGenVertexArrays(1, &m_quadVao);
    glBindVertexArray(m_quadVao);

    glGenBuffers(1, &m_quadVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned short indices[] = {
        0, 1, 2, 0, 2, 3
    };
    glGenBuffers(1, &m_quadIbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glGenFramebuffers(1, &m_fbo);
    auto errorCode = glGetError();
    
}

void OrangeFilterRender::initCopyProgram()
{
    const char* v = R"(
#version 120
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
#version 120
#ifdef GL_ES
    precision mediump float;
#endif
uniform sampler2D uTexture;
varying vec2 vUV;
void main()
{
vec4 rgbColor = texture2D(uTexture, vUV);
gl_FragColor = rgbColor;
}
)";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &v, OF_NULL);
    glCompileShader(vs);

    int len = 0;
    glGetShaderInfoLog(vs, 0, &len, nullptr);
    if (len > 0)
    {
        std::string str;
        str.resize(len + 1);
        glGetShaderInfoLog(vs, str.size(), &len, &str[0]);
        printf("%s\n", str.c_str());
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &f, OF_NULL);
    glCompileShader(fs);

    len = 0;
    glGetShaderInfoLog(fs, 0, &len, nullptr);
    if (len > 0)
    {
        std::string str;
        str.resize(len + 1);
        glGetShaderInfoLog(fs, str.size(), &len, &str[0]);
        printf("%s\n", str.c_str());
    }

    m_copyProgram = glCreateProgram();
    glAttachShader(m_copyProgram, vs);
    glAttachShader(m_copyProgram, fs);
    glLinkProgram(m_copyProgram);
    auto errorCode = glGetError();
    glDeleteShader(vs);
    glDeleteShader(fs);
}


void OrangeFilterRender::initRewriteAlphaProgram()
{
    const char* v = R"(
#version 120
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
#version 120
#ifdef GL_ES
    precision mediump float;
#endif
uniform sampler2D uTexture;
varying vec2 vUV;
void main()
{
vec4 rgbColor = texture2D(uTexture, vUV);
gl_FragColor = vec4(rgbColor.rgb, 1.0);
}
)";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &v, OF_NULL);
    glCompileShader(vs);

    int len = 0;
    glGetShaderInfoLog(vs, 0, &len, nullptr);
    if (len > 0)
    {
        std::string str;
        str.resize(len + 1);
        glGetShaderInfoLog(vs, str.size(), &len, &str[0]);
        printf("%s\n", str.c_str());
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &f, OF_NULL);
    glCompileShader(fs);

    len = 0;
    glGetShaderInfoLog(fs, 0, &len, nullptr);
    if (len > 0)
    {
        std::string str;
        str.resize(len + 1);
        glGetShaderInfoLog(fs, str.size(), &len, &str[0]);
        printf("%s\n", str.c_str());
    }

    m_rewriteAlphaProgram = glCreateProgram();
    glAttachShader(m_rewriteAlphaProgram, vs);
    glAttachShader(m_rewriteAlphaProgram, fs);
    glLinkProgram(m_rewriteAlphaProgram);
    auto errorCode = glGetError();
    glDeleteShader(vs);
    glDeleteShader(fs);
}

void OrangeFilterRender::drawQuad(GLuint intex, GLuint outtex, OFUInt32 width, OFUInt32 height, float* transform, GLuint currentProgram)
{
    int err = glGetError();
    if (err != 0)
    {
        printf("DrawQuad start  GL error %d\n", err);
    }

    GLint oldFBO;
    GLint last_program;
    GLint last_texture;
    GLenum last_active_texture;
    bool ret = true;
    glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);
    
    glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outtex, 0);
    glViewport(0, 0, width, height);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_BLEND);
    glDisable(GL_SCISSOR_TEST);

    int loc;
    glUseProgram(currentProgram);
    GLboolean  ispro = glIsProgram(currentProgram);
    glActiveTexture(GL_TEXTURE0);
    loc = glGetUniformLocation(currentProgram, "uTexture");
    glUniform1i(loc, 0);
    glBindTexture(GL_TEXTURE_2D, intex);

    loc = glGetUniformLocation(currentProgram, "uMat");
    glUniformMatrix4fv(loc, 1, false, transform);

    glBindBuffer(GL_ARRAY_BUFFER, m_quadVbo);

    int locPos = glGetAttribLocation(currentProgram, "aPos");
    glEnableVertexAttribArray(locPos);
    glVertexAttribPointer(locPos, 2, GL_FLOAT, GL_FALSE, 4 *sizeof(float), (void*)0);
    
    int locUV = glGetAttribLocation(currentProgram, "aUV");
    glEnableVertexAttribArray(locUV);
    glVertexAttribPointer(locUV, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float) * 2));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIbo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(locPos);
    glDisableVertexAttribArray(locUV);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glActiveTexture(last_active_texture);
    glBindTexture(GL_TEXTURE_2D, (GLuint)last_texture);
    glUseProgram(last_program);
    glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
    

    glEnable(GL_BLEND);
    glEnable(GL_SCISSOR_TEST);
    err = glGetError();
    if (err != 0)
    {
        printf("DrawQuad glDraw error %d\n", err);
    }
}

void OrangeFilterRender::updateTexture(OFUInt8 *frame, OFUInt32 width, OFUInt32 height)
{
    GLint last_texture;
    auto errorCode = glGetError();
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);

    if (0 == m_tex_in || 0 == m_tex_out || 0 == m_tex_of_out || width != m_width || height != m_height)
    {
        if ( m_tex_in)
        {
            glDeleteTextures(1, &m_tex_in);
            m_tex_in = 0;
        }

        if (m_tex_out)
        {
            glDeleteTextures(1, &m_tex_out);
            m_tex_out = 0;
        }

        if (m_tex_of_out)
        {
            glDeleteTextures(1, &m_tex_of_out);
            m_tex_of_out = 0;
        }

		//创造纹理
        glGenTextures(1, &m_tex_in);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glBindTexture(GL_TEXTURE_2D, m_tex_in);  //绑定纹理
		//绑定之后设置该纹理对象的纹理环绕方式和过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//把数据
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame);

        glGenTextures(1, &m_tex_out);
        glBindTexture(GL_TEXTURE_2D, m_tex_out);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glGenTextures(1, &m_tex_of_out);
        glBindTexture(GL_TEXTURE_2D, m_tex_of_out);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        m_width = width;
        m_height = height;
    }
    else
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glBindTexture(GL_TEXTURE_2D, m_tex_in);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    };
    // Restore state
    glBindTexture(GL_TEXTURE_2D, last_texture);
    errorCode = glGetError();
}

bool OrangeFilterRender::copyTextureToMemory(unsigned char* data, OFUInt32 width, OFUInt32 height,OFUInt32 fbo, GLuint texID)
{
    bool ret = true;
    GLint last_texture;
    GLint oldFrameBuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFrameBuffer);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);

    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glBindTexture(GL_TEXTURE_2D, texID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glFinish();
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        printf("copyTexToMemry, glGetError() = %u", err);
        ret = false;
    }

    //Restore
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindFramebuffer(GL_FRAMEBUFFER, oldFrameBuffer);
    return ret;
}

bool OrangeFilterRender::init(const std::string& runDir)
{
    std::lock_guard<std::recursive_mutex> lock(g_uiMutex);
    initQuad();
    initCopyProgram();
    initRewriteAlphaProgram();
	return true;
}

bool OrangeFilterRender::unInit()
{
    std::lock_guard<std::recursive_mutex> lock(g_uiMutex);
    tearDown();
	OrangHelperManager::getOrangHelper()->destroyContext();
    return true;
}

bool OrangeFilterRender::applyFrame(OFUInt8 *srcframe, OFUInt8 *dstframe, OFUInt32 width, OFUInt32 height)
{
    std::lock_guard<std::recursive_mutex> lock(g_uiMutex);
    glBindVertexArray(m_quadVao);

	updateTexture(srcframe, width, height);

	OrangeHelper::GLTexture texIn, texOut;
	texIn.textureId = m_tex_in;
	texIn.width = width;
	texIn.height = height;
	texIn.format = GL_RGBA;
	texIn.target = GL_TEXTURE_2D;

	texOut.textureId = m_tex_of_out;
	texOut.width = width;
	texOut.height = height;
	texOut.format = GL_RGBA;
	texOut.target = GL_TEXTURE_2D;

	OrangeHelper::ImageInfo image;
	memset(&image, 0, sizeof(image));
	image.data = srcframe;
	image.orientation = 0;
	image.width = width;
	image.height = height;
	image.format = OF_PixelFormat_RGB32;
	image.frontCamera = 0;
	image.timestamp = 0;
	image.dir = 0;

	bool ret = OrangHelperManager::getOrangHelper()->updateFrameParams(texIn, texOut, image);
	if (ret)
	{
		//this code for m_rewriteAlphaProgram.need this code because imgui enable GL_BLEND, it some out put modify src alpha may be influenced
		// so this pass and draw call rewrite alpha
		float flipY[] =
		{
			1.0f,  0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 0.0f, 0.0f,
			0.0f,  0.0f, 1.0f, 0.0f,
			0.0f,  0.0f, 0.0f, 1.0f,
		};

		drawQuad(m_tex_of_out, m_tex_out, width, height, flipY, m_rewriteAlphaProgram);
		glFinish();
		auto errorCode = glGetError();
		errorCode = errorCode;
		//If you need output pixel buffer then call this function
		//copyTextureToMemory(dstframe, m_width, m_height, m_fbo, m_tex_out);
	}
	else
	{
		float identity[] =
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		float flipY[] =
		{
			1.0f,  0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 0.0f, 0.0f,
			0.0f,  0.0f, 1.0f, 0.0f,
			0.0f,  0.0f, 0.0f, 1.0f,
		};
		drawQuad(m_tex_in, m_tex_out, width, height, flipY, m_copyProgram);
		//If you need output pixel buffer then call this function
		//copyTextureToMemory(dstframe, m_width, m_height, m_fbo, m_tex_out);
	}

	glBindVertexArray(0);
	return true;
}

const GLuint OrangeFilterRender::getOutTextureID()
{
    std::lock_guard<std::recursive_mutex> lock(g_uiMutex);
    return m_tex_out;
}

