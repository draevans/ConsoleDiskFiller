// ConsoleDiskFiller.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Windows.h"
#include <filesystem>
#include <iostream>
#include <sstream>
#include <iomanip>    
#include <fstream>

namespace fs = std::experimental::filesystem;

fs::path CreateFileName( const std::string & dir )
{
	time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::stringstream strm;
	strm << std::put_time(localtime(&now), "%F_%H%M%S") << ".txt" << std::ends;
	fs::path pth = dir + "/" + strm.str();
	std::cout << pth << std::endl;
	return pth;
}

int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		std::string drive = argv[1];
		if (fs::exists(drive))
		{
			fs::space_info drv = fs::space(drive);
			std::cout << drv.free << std::endl;
			std::string dirFiller = drive + "Filler";
			if (!fs::exists(dirFiller))
			{
				if (fs::create_directory(dirFiller))
				{
					std::cout << "Created directory" << std::endl;
				}
				else
				{
					std::cout << "Cannot create directory" << std::endl;
				}
			}
			fs::path filename = CreateFileName(dirFiller);
			if (drv.free > 100000000)
			{
				std::ofstream out(filename);
				if (out.is_open())
				{
					for (auto i = 0; i < 10000000; i++)
					{
						out << i % 2 ? 0x00 : 0xFF;
					}
					out.close();
					Sleep(500);
				}			
			}
			while (drv.free > 100000000) 
			{
				fs::path newFile = CreateFileName(dirFiller);
				try
				{
					fs::copy(filename, newFile);
					Sleep(500);
					filename = newFile;
				}
				catch (...)
				{
				}
				drv = fs::space(drive);		
			}			
		}
	}
}

