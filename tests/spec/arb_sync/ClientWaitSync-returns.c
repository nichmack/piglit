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
 * Test ClientWaitSync() returns correct values
 *
 *
 * Section 5.2.1(Waiting for Sync Objects) of OpenGL 3.2 Core says:
 * 	"ClientWaitSync returns one of four status values. A return value of
 * 	 ALREADY_SIGNALED indicates that sync was signaled at the time
 * 	 ClientWaitSync was called. ALREADY_SIGNALED will always be
 * 	 returned if sync was signaled, even if the value of timeout is
 * 	 zero. A return value of TIMEOUT_EXPIRED indicates that the
 * 	 specified timeout period expired before sync was signaled. A re-
 * 	 turn value of CONDITION_SATISFIED indicates that sync was signaled
 * 	 before the timeout expired. Finally, if an error occurs, in
 * 	 addition to generating a GL error as specified below,
 * 	 ClientWaitSync immediately returns WAIT_FAILED withoutblocking."
 *
 */

#include "piglit-util-gl-common.h"

PIGLIT_GL_TEST_CONFIG_BEGIN

	config.supports_gl_compat_version = 10;
	config.supports_gl_core_version = 31;

PIGLIT_GL_TEST_CONFIG_END

enum piglit_result
piglit_display(void)
{
	/* UNREACHED */
	return PIGLIT_FAIL;
}

/* One second in nanoseconds */
#define ONE_SECOND 1000000000

void
piglit_init(int argc, char **argv)
{
	bool pass = true;
	GLsync fence1,  fence2,  fence3;
	GLenum status1, status2, status3;

	double seconds_elapsed = -1;
	int64_t start = 0, end = 0;


	if (piglit_get_gl_version() < 32) {
		piglit_require_extension("GL_ARB_sync");
	}

	/* Test Case 1: Verify that fence times out correctly after set time */

	/* queue a draw command */
	piglit_draw_rect(-1, -1, 2, 2);

	/* create fence sync */
	fence1 = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

	/* get initial time */
	start = piglit_get_microseconds();
	if (start == -1) piglit_report_result(PIGLIT_SKIP);

	/* check fence status */
	status1 = glClientWaitSync(fence1, GL_SYNC_FLUSH_COMMANDS_BIT,
					ONE_SECOND);

	/* draw call should not be complete since glFlush() wasn't called */
	if (status1 != GL_TIMEOUT_EXPIRED) {
		printf("Expected: status1 == GL_TIMEOUT_EXPIRED\n"
			"Actual: status1 == %s\n",
			piglit_get_gl_enum_name(status1));
		pass = false;
	}

	/* check time elapsed */
	end = piglit_get_microseconds();
	if (end == -1) piglit_report_result(PIGLIT_SKIP);
	seconds_elapsed = (end - start) * .000001;
	if (seconds_elapsed < 1) { //TODO: check if significantly longer than 1 second
		printf("Expected: Test Case 1 wait >= 1 second\n"
			"Actual: Test Case 1 wait = %f seconds\n",
			seconds_elapsed);
		pass = false;
	}

	/* clean up */
	glFlush();
	glDeleteSync(fence1);

	/* Test Case 2: Verify that fence times out correctly with no timeout */

	/* queue a draw command */
	piglit_draw_rect(-1, -1, 2, 2);

	/* create fence sync */
	fence2 = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

	/* get initial time */
	start = piglit_get_microseconds();
	if (start == -1) piglit_report_result(PIGLIT_SKIP);

	/* check fence status */
	status2 = glClientWaitSync(fence2, GL_SYNC_FLUSH_COMMANDS_BIT,
					0);

	/* ClientWaitSync() should have returned immediately */
	if (status2 != GL_TIMEOUT_EXPIRED) {
		printf("Expected: status2 == GL_TIMEOUT_EXPIRED\n"
			"Actual: status2 == %s\n",
			piglit_get_gl_enum_name(status2));
		pass = false;
	}

	/* check time elapsed */
	end = piglit_get_microseconds();
	if (end == -1) piglit_report_result(PIGLIT_SKIP);
	seconds_elapsed = (end - start) * .000001;
	if (seconds_elapsed > .001) {
		printf("Expected: Test Case 2 wait < .001 second\n"
			"Actual: Test Case 2 wait = %f seconds\n",
			seconds_elapsed);
		pass = false;
	}

	/* clean up */
	glFlush();
	glDeleteSync(fence2);

	/* Test Case 3: Verify fence was already signaled after glFinish() */

	/* queue a draw command */
	piglit_draw_rect(-1, -1, 2, 2);

	/* create fence sync */
	fence3 = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

	/* flush the command stream */
	glFinish();

	/* check fence status */
	status3 = glClientWaitSync(fence2, GL_SYNC_FLUSH_COMMANDS_BIT,
					0);

	/* fence should have been already signaled */
	if (status3 != GL_ALREADY_SIGNALED) {
		printf("Expected: status3 == GL_ALREADY_SIGNALED\n"
			"Actual: status3 == %s\n",
			piglit_get_gl_enum_name(status3));
		pass = false;
	}

	/* clean up */
	glFlush();
	glDeleteSync(fence3);

	piglit_report_result(pass ? PIGLIT_PASS : PIGLIT_FAIL);
}
