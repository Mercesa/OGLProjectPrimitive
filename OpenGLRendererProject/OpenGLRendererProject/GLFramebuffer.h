#pragma once

namespace Primitive
{

class GLFramebuffer
{
public:
	GLFramebuffer();
	virtual ~GLFramebuffer();

	bool CheckStatus();
};
};
