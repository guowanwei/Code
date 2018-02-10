#pragma once
#include "Resource.h"
class Texture : public Resource
{
public:
	Texture()
		:Resource(Resource::RT_TEXTURE)
	{

	}
};