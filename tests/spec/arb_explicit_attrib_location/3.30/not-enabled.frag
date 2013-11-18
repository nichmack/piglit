// [config]
// expect_result: pass
// gl_version: 3.3
// glsl_version: 3.30
// [end config]
//
// Try to use layout(location) without enabling the extension.

#version 330

layout(location = 0) out vec4 c;

void main()
{
	c = vec4(0);
}
