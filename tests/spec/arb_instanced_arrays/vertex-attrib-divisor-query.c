/**
 * Copyright © 2013 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/**
 * \file
 * Test that VERTEX_ATTRIB_ARRAY_DIVISOR may be passed to GetVertexAttrib*
 * functions.
 *
 * GL 3.3 core spec, section 6.1.11 (Shader and Program Queries) says:
 * "The commands
 *  	void GetVertexAttribdv( uint index, enum pname, double *params );
 *  	void GetVertexAttribfv( uint index, enum pname, float *params );
 *  	void GetVertexAttribiv( uint index, enum pname, int *params );
 *  	void GetVertexAttribIiv( uint index, enum pname, int *params );
 *  	void GetVertexAttribIuiv( uint index, enum pname, uint *params );
 *  obtain the vertex attribute state named by pname for the generic vertex
 *  attribute numbered index and places the information in the array params.
 *  pname must be one of VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
 *  VERTEX_ATTRIB_ARRAY_ENABLED, VERTEX_ATTRIB_ARRAY_SIZE,
 *  VERTEX_ATTRIB_ARRAY_STRIDE, VERTEX_ATTRIB_ARRAY_TYPE,
 *  VERTEX_ATTRIB_ARRAY_NORMALIZED, VERTEX_ATTRIB_ARRAY_INTEGER,
 *  VERTEX_ATTRIB_ARRAY_DIVISOR, or CURRENT_VERTEX_ATTRIB."
 */

#include "piglit-util-gl-common.h"

PIGLIT_GL_TEST_CONFIG_BEGIN

	config.supports_gl_core_version = 33;
	config.supports_gl_compat_version = 33;

PIGLIT_GL_TEST_CONFIG_END

void
piglit_init(int argc, char **argv)
{
	bool pass = true;
	GLdouble dParam[] = {1.0};
	GLfloat fParam[] = {1.0f};
	GLint iParam[] = {-1};
	GLuint uParam[] = {1};

	glGetVertexAttribdv(0, GL_VERTEX_ATTRIB_ARRAY_DIVISOR, dParam);
	pass = piglit_check_gl_error(GL_NO_ERROR) && pass;

	glGetVertexAttribfv(0, GL_VERTEX_ATTRIB_ARRAY_DIVISOR, fParam);
	pass = piglit_check_gl_error(GL_NO_ERROR) && pass;

	glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_DIVISOR, iParam);
	pass = piglit_check_gl_error(GL_NO_ERROR) && pass;

	glGetVertexAttribIiv(0, GL_VERTEX_ATTRIB_ARRAY_DIVISOR, iParam);
	pass = piglit_check_gl_error(GL_NO_ERROR) && pass;

	glGetVertexAttribIuiv(0, GL_VERTEX_ATTRIB_ARRAY_DIVISOR, uParam);
	pass = piglit_check_gl_error(GL_NO_ERROR) && pass;

	piglit_report_result(pass ? PIGLIT_PASS : PIGLIT_FAIL);
}

enum piglit_result
piglit_display(void)
{
	/* UNREACHED */
	return PIGLIT_FAIL;
}
