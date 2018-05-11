#pragma once
#include "../IRender.h"
class ForwardRender : public IRender
{
	ForwardRender() {}
public:
	virtual void Render() override;
	static ForwardRender& Instance()
	{
		static ForwardRender render;
		return render;
	}
};