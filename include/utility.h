//////////////////////////////////////////////////////////////////////
/// Dragon Blaze Game Library
///
/// Copyright (c) 2015 by Jan Moeller
///
/// This software is provided "as-is" and does not claim to be
/// complete or free of bugs in any way. It should work, but
/// it might also begin to hurt your kittens.
//////////////////////////////////////////////////////////////////////

#ifndef INCLUDE_UTILITY_H_
#define INCLUDE_UTILITY_H_

#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include "DBGL/Core/Parsers/Properties.h"

using CharMap = std::map<std::string, double>;
using CharMaps = std::vector<CharMap>;

const std::string g_fileHeader = "# NameGen character map";

/**
 * @brief Reads in properties.txt and inserts arguments
 * @param[in] prop Properties object to write to
 * @param argc Amount of arguments
 * @param argv Arguments
 * @returns True if everything went right, otherwise false
 */
bool setupProperties(dbgl::Properties& prop, int argc, const char* argv[]);
/**
 * @brief Writes character maps to file
 * @param file File to write to
 * @param characterMaps Character map to write
 * @return True in case everything went right, otherwise false
 */
bool writeCharMaps(std::string const& file, CharMaps const& characterMaps);
/**
 * @brief Reads in character maps from file
 * @param file File to read from
 * @param[in] characterMaps Character maps to write to
 * @return True in case everything went right, otherwise false
 */
bool readCharMaps(std::string const& file, CharMaps& characterMaps);

#endif /* INCLUDE_UTILITY_H_ */
