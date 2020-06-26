#pragma once

#include "glew/glew.h"

class   FrameBufferObject
{
public:
    unsigned    _width;
    unsigned    _height;
    unsigned    _FBOID;
    unsigned    _RBOID;
public:
    /**
    *   ½¨Á¢
    */
    bool    setup(int w, int h)
    {
        _width  =   w;
        _height =   h;
        glGenFramebuffers(1, &_FBOID);
        glBindFramebuffer(GL_FRAMEBUFFER, _FBOID);

        glGenRenderbuffers(1, &_RBOID);
        glBindRenderbuffer(GL_RENDERBUFFER, _RBOID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, _width, _height);

        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _RBOID);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return  true;
    }

    void    begin(GLuint textureId)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _FBOID);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
        glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _RBOID );

    }

    void    end()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    /**
    *   Ïú»Ù
    */
    void    destroy()
    {
        glDeleteFramebuffers(1, &_FBOID);
        glDeleteRenderbuffers(1,&_RBOID);
        _RBOID  =   0;
        _FBOID  =   0;
    }
};