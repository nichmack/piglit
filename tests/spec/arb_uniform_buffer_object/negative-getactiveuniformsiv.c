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

/** @file negative-getactiveuniformsiv.c
 *
 * From the GL_ARB_uniform_buffer_object spec:
 *
 *     "If any index is greater than or equal to the value of
 *      ACTIVE_UNIFORMS, the error INVALID_VALUE is generated.
 *
 *      For GetActiveUniformsiv, <uniformCount> indicates both the
 *      number of elements in the array of indices <uniformIndices>
 *      and the number of parameters written to <params> upon
 *      successful return.  <pname> identifies a property of each
 *      uniform in <uniformIndices> that should be written into the
 *      corresponding element of <params>.  If an error occurs,
 *      nothing will be written to <params>.
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

#define _GNU_SOURCE
#include "piglit-util-gl-common.h"
#include "uniform-types.h"

PIGLIT_GL_TEST_MAIN(
    10 /*window_width*/,
    10 /*window_height*/,
    GLUT_RGB | GLUT_DOUBLE | GLUT_ALPHA)

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
	GLuint fs, prog;
	GLint junk = 0xd0d0;
	GLint unwritten_junk = junk;
	GLuint good_index = 0, bad_index = 1;

	piglit_require_extension("GL_ARB_uniform_buffer_object");

	fs = piglit_compile_shader_text(GL_FRAGMENT_SHADER, fs_source);
	prog = piglit_link_simple_program(0, fs);
	if (!fs || !prog) {
		fprintf(stderr, "Failed to compile shader:\n%s", fs_source);
		piglit_report_result(PIGLIT_FAIL);
	}

	/* Test a bad pname (it's one for glActiveUniformBlockiv). */
	glGetActiveUniformsiv(prog, 1, &good_index,
			      GL_UNIFORM_BLOCK_BINDING, &junk);
	if (!piglit_check_gl_error(GL_INVALID_ENUM) || junk != unwritten_junk)
		pass = false;

	/* Bad active uniform index.  "Each active uniform, whether in
	 * a named uniform block or in the default block, is assigned
	 * an index when a program is linked.  These indices are
	 * assigned in consecutive order, beginning with zero.  The
	 * indices assigned to a set of uniforms in a program may be
	 * queried by calling".  We only have one active uniform.
	*/
	glGetActiveUniformsiv(prog, 1, &bad_index, GL_UNIFORM_TYPE, &junk);
	if (!piglit_check_gl_error(GL_INVALID_VALUE) || junk != unwritten_junk)
		pass = false;

	/* Test bad program name by deleting ours */
	glDeleteProgram(prog);
	glGetActiveUniformsiv(prog, 1, &good_index, GL_UNIFORM_TYPE, &junk);
	if (!piglit_check_gl_error(GL_INVALID_VALUE) || junk != unwritten_junk)
		pass = false;

	piglit_report_result(pass ? PIGLIT_PASS : PIGLIT_FAIL);
}

enum piglit_result piglit_display(void)
{
	/* UNREACHED */
	return PIGLIT_FAIL;
}
