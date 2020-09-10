package com.yy.orangefilter.test.utils;

import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.util.Log;

import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

public class QuadRenderer {
    private static final String TAG = "QuadRenderer";
    private boolean mOEStexture;
    private int mVBO;
    private int mIBO;
    private int mProgram;

    public QuadRenderer(boolean isOEStexture) {
        mOEStexture = isOEStexture;
        createVBO();
        createIBO();
        createProgram();
    }

    private void createVBO() {
        int[] vbos = new int[1];
        GLES20.glGenBuffers(1, vbos, 0);
        mVBO = vbos[0];

        float[] vertices = new float[]{
            -1.0f,  1.0f, 0.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 1.0f,
             1.0f, -1.0f, 1.0f, 1.0f,
             1.0f,  1.0f, 1.0f, 0.0f
        };

        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, mVBO);
        GLES20.glBufferData(
                GLES20.GL_ARRAY_BUFFER,
                vertices.length * 4,
                FloatBuffer.wrap(vertices),
                GLES20.GL_STATIC_DRAW
        );
        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
    }

    private void createIBO() {
        int[] ibos = new int[1];
        GLES20.glGenBuffers(1, ibos, 0);
        mIBO = ibos[0];

        short[] indices = new short[]{
                0, 1, 2,
                0, 2, 3
        };

        GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, mIBO);
        GLES20.glBufferData(
                GLES20.GL_ELEMENT_ARRAY_BUFFER,
                indices.length * 2,
                ShortBuffer.wrap(indices),
                GLES20.GL_STATIC_DRAW
        );
        GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    private void createProgram() {
        String vss =
            "uniform mat4 uMat;\n" +
            "attribute vec2 aPos;\n" +
            "attribute vec2 aUV;\n" +
            "varying vec2 vUV;\n" +
            "void main()" +
            "{" +
            "   gl_Position = uMat * vec4(aPos, 0.0, 1.0);" +
            "   vUV = aUV;" +
            "}";
        String fss;
        if (mOEStexture) {
            fss =
                "#extension GL_OES_EGL_image_external : require\n" +
                "precision mediump float;\n" +
                "uniform samplerExternalOES uTexture;\n" +
                "varying vec2 vUV;\n" +
                "void main()" +
                "{" +
                "   gl_FragColor = texture2D(uTexture, vUV);" +
                "}";
        } else {
            fss =
                "precision mediump float;\n" +
                "uniform sampler2D uTexture;\n" +
                "varying vec2 vUV;\n" +
                "void main()" +
                "{" +
                "   gl_FragColor = texture2D(uTexture, vUV);" +
                "}";
        }
        int vs = createShader(GLES20.GL_VERTEX_SHADER, vss);
        int fs = createShader(GLES20.GL_FRAGMENT_SHADER, fss);

        mProgram = GLES20.glCreateProgram();

        GLES20.glAttachShader(mProgram, vs);
        GLES20.glAttachShader(mProgram, fs);
        GLES20.glLinkProgram(mProgram);

        String log = GLES20.glGetProgramInfoLog(mProgram);
        if (log.length() > 0) {
            Log.e(TAG, "program log: " + log);
        }

        GLES20.glDeleteShader(vs);
        GLES20.glDeleteShader(fs);
    }

    private int createShader(int type, String source) {
        int shader = GLES20.glCreateShader(type);
        GLES20.glShaderSource(shader, source);
        GLES20.glCompileShader(shader);
        String log = GLES20.glGetShaderInfoLog(shader);
        if (log.length() > 0) {
            Log.e(TAG, "shader log: " + log);
        }
        return shader;
    }

    public void draw(float[] transform, int tex) {
        int loc;
        GLES20.glUseProgram(mProgram);

        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        int target = mOEStexture ? GLES11Ext.GL_TEXTURE_EXTERNAL_OES : GLES20.GL_TEXTURE_2D;
        GLES20.glBindTexture(target, tex);
        loc = GLES20.glGetUniformLocation(mProgram, "uTexture");
        GLES20.glUniform1i(loc, 0);

        loc = GLES20.glGetUniformLocation(mProgram, "uMat");
        GLES20.glUniformMatrix4fv(loc, 1, false, transform, 0);

        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, mVBO);

        loc = GLES20.glGetAttribLocation(mProgram, "aPos");
        GLES20.glEnableVertexAttribArray(loc);
        GLES20.glVertexAttribPointer(loc, 2, GLES20.GL_FLOAT, false, 4 * 4, 0);
        loc = GLES20.glGetAttribLocation(mProgram, "aUV");
        GLES20.glEnableVertexAttribArray(loc);
        GLES20.glVertexAttribPointer(loc, 2, GLES20.GL_FLOAT, false, 4 * 4, 4 * 2);

        GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, mIBO);
        GLES20.glDrawElements(GLES20.GL_TRIANGLES, 6, GLES20.GL_UNSIGNED_SHORT, 0);

        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
        GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    public void release() {
        GLES20.glDeleteBuffers(1, new int[]{mVBO}, 0);
        mVBO = 0;

        GLES20.glDeleteBuffers(1, new int[]{mIBO}, 0);
        mIBO = 0;

        GLES20.glDeleteProgram(mProgram);
        mProgram = 0;
    }
}
