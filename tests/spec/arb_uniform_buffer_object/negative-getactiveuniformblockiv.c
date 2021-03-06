/*
 * Copyright © 2012 Intel Corporation
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
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/** @file negative-getactiveuniformblockiv.c
 *
 * From the GL_ARB_uniform_buffer_object spec:
 *
 *     "<uniformBlockIndex> is an active uniform block index of
 *      <program>.  If <uniformBlockIndex> is greater than or equal to
 *      the value of ACTIVE_UNIFORM_BLOCKS, or is not the index of an
 *      active uniform block in <program>, the error INVALID_VALUE is
 *      generated.
 *
 *      If no error occurs, the uniform block parameter(s) specified
 *      by <pname> are returned in <params>. Otherwise, nothing will
 *      be written to <params>.
 *
 *      The error INVALID_VALUE is generated by GetUniformIndices,
 *      GetActiveUniformsiv, GetActiveUniformName,
 *      GetUniformBlockIndex, GetActiveUniformBlockiv,
 *      GetActiveUniformBlockName, and UniformBlockBinding if
 *      <program> is not a value generated by GL.
 *
 *      The error INVALID_ENUM is generated by GetActiveUniformsiv and
 *      GetActiveUniformBlockiv if <pname> is not one of the accepted
 *      values."
 */

#include "piglit-util-gl-common.h"
#include "uniform-types.h"

PIGLIT_GL_TEST_CONFIG_BEGIN

	config.supports_gl_compat_version = 10;

	config.window_width = 10;
	config.window_height = 10;
	config.window_visual = PIGLIT_GL_VISUAL_RGBA | PIGLIT_GL_VISUAL_DOUBLE;

PIGLIT_GL_TEST_CONFIG_END

void
piglit_init(int argc, char **argv)
{
	bool pass = true;
	const char *fs_source =
		"#extension GL_ARB_uniform_buffer_object : require\n"
		"uniform ubo {\n"
		"	float u;\n"
		"};\n"
		"\n"
		"void main() {\n"
		"	gl_FragColor = vec4(u);\n"
		"}\n";
	GLuint prog;
	GLint junk = 0xd0d0, unwritten_junk = junk;

	piglit_require_extension("GL_ARB_uniform_buffer_object");

	prog = piglit_build_simple_program(NULL, fs_source);

	/* Test a bad pname (it's one for glActiveUniformsiv). */
	glGetActiveUniformBlockiv(prog, 0, GL_UNIFORM_TYPE, &junk);
	if (!piglit_check_gl_error(GL_INVALID_ENUM) || junk != unwritten_junk)
		pass = false;

	/* Bad active uniform block index.  "The indices of the active
	 * uniform blocks of a program are assigned in consecutive
	 * order, beginning with zero."
	*/
	glGetActiveUniformBlockiv(prog, 1, GL_UNIFORM_BLOCK_BINDING, &junk);
	if (!piglit_check_gl_error(GL_INVALID_VALUE) || junk != unwritten_junk)
		pass = false;

	/* Test bad program name by deleting ours */
	glDeleteProgram(prog);
	glGetActiveUniformBlockiv(prog, 0, GL_UNIFORM_BLOCK_BINDING, &junk);
	if (!piglit_check_gl_error(GL_INVALID_VALUE) || junk != unwritten_junk)
		pass = false;

	piglit_report_result(pass ? PIGLIT_PASS : PIGLIT_FAIL);
}

enum piglit_result piglit_display(void)
{
	/* UNREACHED */
	return PIGLIT_FAIL;
}

