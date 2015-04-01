//////////////////////////////////////////////////////////////////////
/// Dragon Blaze Game Library
///
/// Copyright (c) 2015 by Jan Moeller
///
/// This software is provided "as-is" and does not claim to be
/// complete or free of bugs in any way. It should work, but
/// it might also begin to hurt your kittens.
//////////////////////////////////////////////////////////////////////

#include "../include/utility.h"

bool setupProperties(dbgl::Properties& prop, int argc, const char* argv[])
{
	if (!prop.read("properties.txt"))
	{
		std::cout << "Unable to load properties.txt" << std::endl;
		return false;
	}
	// Overwrite with application arguments if any
	prop.interpret(argc, argv);
	return true;
}

bool writeCharMaps(std::string const& file, CharMaps const& characterMaps)
{
	// Open file
	std::ofstream outFile;
	outFile.open(file, std::ios::out | std::ios::trunc);
	if (outFile.is_open())
	{
		// Write header
		outFile << g_fileHeader << std::endl;
		outFile << characterMaps.size() << std::endl;
		for (unsigned int i = 0; i < characterMaps.size(); i++)
		{
			// Write amount of elements
			outFile << "# " << characterMaps[i].size() << std::endl;
			// Write elements in form key=value
			for (auto it = characterMaps[i].begin(); it != characterMaps[i].end(); ++it)
				outFile << it->first << "=" << it->second << std::endl;
		}
		// Close file
		outFile.close();
		return true;
	}
	else
	{
		std::cout << "File " << file << " could not be written!" << std::endl;
		return false;
	}
}

bool readCharMaps(std::string const& file, CharMaps& characterMaps)
{
	// Open file
	std::ifstream inFile;
	inFile.open(file, std::ios::in);
	if (inFile.is_open())
	{
		// Seek to beginning
		inFile.seekg(0, std::ios::beg);
		// Check header
		if (inFile.good())
		{
			std::string line;
			std::getline(inFile, line);
			if (line != g_fileHeader)
			{
				std::cout << "File " << file << " is misformatted! Header invalid." << std::endl;
				inFile.close();
				return false;
			}
			line.clear();
			std::getline(inFile, line);
			unsigned int maxLength = std::stoi(line);
			// Setup character maps
			characterMaps.resize(maxLength);
		}
		// Scan whole file
		unsigned int elemsLeft = 0;
		int charMap = -1;
		while (inFile.good())
		{
			std::string line;
			std::getline(inFile, line);
			if (line.empty())
				continue;
			if (elemsLeft <= 0)
			{
				if (line.substr(0, 2) != "# ")
				{
					std::cout << "File " << file << " is misformatted! Missing count indicator." << std::endl;
					inFile.close();
					return false;
				}
				else
					elemsLeft = std::stoi(line.substr(2));
				charMap++;
			}
			else
			{
				// Split on '='
				auto start = 0U;
				auto end = line.find('=');
				std::vector<std::string> tokens;
				while (end != std::string::npos)
				{
					tokens.push_back(line.substr(start, end - start));
					start = end + 1;
					end = line.find('=', start);
				}
				tokens.push_back(line.substr(start, end));
				if (tokens.size() != 2)
				{
					std::cout << "File " << file << " is misformatted! No key=value pair." << std::endl;
					inFile.close();
					return false;
				}
				else
				{
					characterMaps[charMap][tokens[0]] = std::stod(tokens[1]);
					elemsLeft--;
				}
			}
		}
		inFile.close();
		return true;
	}
	else
	{
		std::cout << "File " << file << " could not be opened!" << std::endl;
		return false;
	}
}

std::string& encodeString(std::string& str)
{
#ifdef __WIN32
	for (unsigned int i = 0; i < str.length(); i++)
	{
		if (str[i] == 'ä')
			str[i] = '\x84';
		else if (str[i] == 'ö')
			str[i] = '\x94';
		else if (str[i] == 'ü')
			str[i] = '\x81';
		else if (str[i] == 'ß')
			str[i] = '\xE1';
		else if (str[i] == 'Ä')
			str[i] = '\x8E';
		else if (str[i] == 'Ö')
			str[i] = '\x99';
		else if (str[i] == 'Ü')
			str[i] = '\x9A';
	}
#endif
	return str;
}
