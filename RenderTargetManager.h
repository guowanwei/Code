#pragma once
#include "RenderTarget.h"
#include <vector>
class RenderTargetManager
{
private:
	RenderTargetManager() {}
	std::vector<RenderTarget*> mUnusedPool;
	typedef std::vector<RenderTarget*> DataVector;
public:
	static RenderTargetManager& Instance() 
	{
		static RenderTargetManager manager;
		return manager;
	}
public:
	//delete all rendertarget
	~RenderTargetManager()
	{
		for (int i = 0; i < mUnusedPool.size(); ++i)
		{
			delete mUnusedPool[i];
		}
	}

	RenderTarget* PopRenderTarget(UINT width, UINT height, DXGI_FORMAT RenderTargetFormat)
	{
		RenderTarget* Result = NULL;
		for (DataVector::iterator iter = mUnusedPool.begin(); iter != mUnusedPool.end(); ++iter)
		{
			if ((*iter)->mFormat == RenderTargetFormat && (*iter)->mHeight == height && (*iter)->mWidth == width)
			{
				Result = *iter;
				mUnusedPool.erase(iter);
				return Result;
			}
		}
		
		return new RenderTarget(width,height,RenderTargetFormat);
	}
	bool PushRenderTarget(RenderTarget* rendertarget)
	{
		mUnusedPool.push_back(rendertarget);
		return true;
	}
};