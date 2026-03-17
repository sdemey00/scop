#pragma once

#include <unordered_map>
#include "GlTexture.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

class MtlLoader {
	public:
		std::unordered_map<std::string, GlTexture> load(const std::string& path) {
			// load and parse .mtl file
		}
}