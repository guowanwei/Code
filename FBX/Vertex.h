#pragma once
#include "MathHelper.h"
#include <vector>
#include <algorithm>
#include "../BaseUtilities/AS3DVector.h"

struct PNTVertex
{
	AS3DVECTOR3 mPosition;
	AS3DVECTOR3 mNormal;
	AS3DVECTOR2 mUV;

	bool operator==(const PNTVertex& rhs) const
	{
		return mPosition == rhs.mPosition
			&& mNormal == rhs.mNormal
			&& mUV == rhs.mUV;
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
	AS3DVECTOR3 mPosition;
	AS3DVECTOR3 mNormal;
	AS3DVECTOR3 mTangent;
	AS3DVECTOR3 mBinormal;
	AS3DVECTOR2 mUV;
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
		return mPosition == rhs.mPosition
			&& mNormal == rhs.mNormal
			&& mUV == rhs.mUV && sameBlendingInfo;
	}
};
