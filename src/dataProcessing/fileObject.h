#pragma once

#include <../main.h>

namespace CourseLab 
{
	class File 
	{
	private:
		std::string m_path;
	public:
		File(const std::string& path) 
		{
			m_path = path;
		};
		~File() {};

		std::string ReadRaw() 
		{
			std::string temp;
			std::string result = "";
			std::ifstream stream(m_path);

			while (std::getline(stream, temp)) 
				result += temp;

			stream.close();
			return result;
		};


	};
}