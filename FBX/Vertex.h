#pragma once
#include "MathHelper.h"
#include <vector>
#include <algorithm>


struct PNTVertex
{
	XMFLOAT3 mPosition;
	XMFLOAT3 mNormal;
	XMFLOAT2 mUV;

	bool operator==(const PNTVertex& rhs) const
	{
		uint32_t position;
		uint32_t normal;
		uint32_t uv;

		XMVectorEqualR(&position, XMLoadFloat3(&(this->mPosition)), XMLoadFloat3(&rhs.mPosition));
		XMVectorEqualR(&normal, XMLoadFloat3(&(this->mNormal)), XMLoadFloat3(&rhs.mNormal));
		XMVectorEqualR(&uv, XMLoadFloat2(&(this->mUV)), XMLoadFloat2(&rhs.mUV));

		return XMComparisonAllTrue(position) && XMComparisonAllTrue(normal) && XMComparisonAllTrue(uv);
	}
};

struct VertexBlendingInfo
{
	unsigned int mBlendingIndex;
	double mBlendingWeight;

	VertexBlendingInfo():
		mBlendingIndex(0),
		mBlendingWeight(0.0)
	{}

	bool operator < (const VertexBlendingInfo& rhs)
	{
		return (mBlendingWeight > rhs.mBlendingWeight);
	}
};

struct PNTIWVertex
{
	XMFLOAT3 mPosition;
	XMFLOAT3 mNormal;
	XMFLOAT3 mTangent;
	XMFLOAT3 mBinormal;
	XMFLOAT2 mUV;
	std::vector<VertexBlendingInfo> mVertexBlendingInfos;

	void SortBlendingInfoByWeight()
	{
		std::sort(mVertexBlendingInfos.begin(), mVertexBlendingInfos.end());
	}
	bool operator <(const PNTIWVertex& rhs) const
	{
		if (mPosition.x < rhs.mPosition.x) return true;
		else if (mPosition.x > rhs.mPosition.x) return false;

		if (mPosition.y < rhs.mPosition.y) return true;
		else if (mPosition.y > rhs.mPosition.y) return false;

		if (mPosition.z < rhs.mPosition.z) return true;
		else if (mPosition.z > rhs.mPosition.z) return false;



		if (mNormal.x < rhs.mNormal.x) return true;
		else if (mNormal.x > rhs.mNormal.x) return false;

		if (mNormal.y < rhs.mNormal.y) return true;
		else if (mNormal.y > rhs.mNormal.y) return false;

		if (mNormal.z < rhs.mNormal.z) return true;
		else if (mNormal.z > rhs.mNormal.z) return false;



		if (mUV.x < rhs.mUV.x) return true;
		else if (mUV.x > rhs.mUV.x) return false;

		if (mUV.y < rhs.mUV.y) return true;
		else if (mUV.y > rhs.mUV.y) return false;

		return false;
	}
	bool operator==(const PNTIWVertex& rhs) const
	{
		bool sameBlendingInfo = true;

		// We only compare the blending info when there is blending info
		if(!(mVertexBlendingInfos.empty() && rhs.mVertexBlendingInfos.empty()))
		{
			// Each vertex should only have 4 index-weight blending info pairs
			for (unsigned int i = 0; i < 4; ++i)
			{
				if (mVertexBlendingInfos[i].mBlendingIndex != rhs.mVertexBlendingInfos[i].mBlendingIndex ||
					abs(mVertexBlendingInfos[i].mBlendingWeight - rhs.mVertexBlendingInfos[i].mBlendingWeight) > 0.001)
				{
					sameBlendingInfo = false;
					break;
				}
			}
		}
		
		bool result1 = MathHelper::CompareVector3WithEpsilon(mPosition, rhs.mPosition);
		bool result2 = MathHelper::CompareVector3WithEpsilon(mNormal, rhs.mNormal);
		bool result3 = MathHelper::CompareVector2WithEpsilon(mUV, rhs.mUV);

		return result1 && result2 && result3 && sameBlendingInfo;
	}
};
