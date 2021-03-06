// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#ifndef CHECK_ERROR_GL
extern void CHECK_ERROR_GL();
#endif

struct renderbuffer;
struct texture;


struct framebuffer
{
	GLuint id;

	framebuffer();
	~framebuffer();

	void attach_color(renderbuffer* value);
	void attach_color(texture* value);

	void attach_depth(renderbuffer* value);
	void attach_depth(texture* value);

	void attach_stencil(renderbuffer* value);

private:
	framebuffer(const framebuffer&) {}
	framebuffer& operator = (const framebuffer&) {return *this;}
};


class bind_framebuffer
{
	GLint _old;

public:
	bind_framebuffer(const framebuffer& fb)
	{
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_old);
		glBindFramebuffer(GL_FRAMEBUFFER, fb.id);
		CHECK_ERROR_GL();
	}

	~bind_framebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _old);
	}
};


#endif
