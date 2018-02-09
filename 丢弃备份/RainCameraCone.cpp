struct VertexB
{
	XMFLOAT3 position;
	XMFLOAT2 uv;
};
const float PI_Eight = 3.1415926/8;
void GenRainCameraCone()
{
	std::vector<VertexB> vertices;
	std::vector<int> indices;
	//x -0.75 ~ 0.75  y -0.75 ~ 0.75  z -1 ~ 1
	float z_steps[] = { 1.0,0.75,0.50,0.25,-0.25,-0.5,-0.75,-1.0 };
	for (int i = 0; i < sizeof(z_steps) / sizeof(float); ++i)
	{
		for (int j = 0; j <= 16; ++j)
		{
			XMFLOAT3 Pos;
			float Pos.z = z_steps[i];
			float radius = (1.0 - Pos.);
			float Pos.x = sin(j * PI_Eight) * radius;
			float Pos.y = cos(j * PI_Eight) * radius;
			XMFLOAT2 UV;
			UV.x = 0;
			UV.y = 0;
			VertexB VB;
			VB.position = Pos;
			VB.uv = UV;

			vertices.push_back(VB);
		}
	}
	for (int i = 0; i < sizeof(z_steps) / sizeof(float) - 1; ++i)
	{
		int firstPosIndex = 17 * i;
		for (int j = 0; j < 16; ++j)
		{
			
			indices.push_back(firstPosIndex + j);
			indices.push_back(firstPosIndex + j + 1);
			indices.push_back(firstPosIndex + j + 17);

			indices.push_back(firstPosIndex + j + 1);
			indices.push_back(firstPosIndex + j + 1 + 17);
			indices.push_back(firstPosIndex + j + 17);
		}
	}

}