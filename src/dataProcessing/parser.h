#pragma once

#include <../main.h>
#include <objLoader.h>
namespace CourseLab 
{
	class Parser 
	{
	public:
		static GLfloat* GetVertexBuffer(const std::vector<objl::Vertex>& data) 
		{
			auto size = data.size();
			GLfloat* bufferData = new GLfloat[size * 8];

			for (int i = 0; i < size; i++)
			{
				bufferData[i*8+0] = data[i].Position.X;
				bufferData[i*8+1] = data[i].Position.Y;
				bufferData[i*8+2] = data[i].Position.Z;

				bufferData[i*8+3] = data[i].TextureCoordinate.X;
				bufferData[i*8+4] = data[i].TextureCoordinate.Y;
				
				bufferData[i*8+5] = data[i].Normal.X;
				bufferData[i*8+6] = data[i].Normal.Y;
				bufferData[i*8+7] = data[i].Normal.Z;
			}
			return bufferData;
		}

		static GLuint* GetIndexBuffer(const std::vector<unsigned int>& data) 
		{
			auto size = data.size();
			GLuint* bufferData = new GLuint[size];

			std::copy(data.begin(), data.end(), bufferData);

			return bufferData;
		}
	};
}