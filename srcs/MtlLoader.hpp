#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

struct Material {
    std::string	name;
    std::string	mapKd;
	float	Ns;
	float	Ka[3];
	float	Kd[3];
	float	Ks[3];
	float	Ni;
	float	d;
    GLuint	illum = 0;

};

class MtlLoader {
public:
    static Material load(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open())
            throw std::runtime_error("MtlLoader: cannot open: " + path);

        Material	mat;
        std::string	line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#')
				continue ;
            std::istringstream	ss(line);
            std::string			token;
            ss >> token;
            if (token == "newmtl")
				ss >> mat.name;
			else if (token == "map_Kd") {
                std::string value;
                while (ss >> value)
                    mat.mapKd = value;
			}
        }
        return (mat);
    }

    static std::unordered_map<std::string, std::string>
    loadDiffuseMapByMaterial(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open())
            throw std::runtime_error("MtlLoader: cannot open: " + path);

        std::unordered_map<std::string, std::string> out;
        std::string currentMaterial;
        std::string line;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#')
                continue;

            std::istringstream ss(line);
            std::string token;
            ss >> token;

            if (token == "newmtl") {
                ss >> currentMaterial;
                if (!currentMaterial.empty())
                    out[currentMaterial];
            } else if (token == "map_Kd" && !currentMaterial.empty()) {
                std::string value;
                while (ss >> value)
                    out[currentMaterial] = value;
            }
        }
        return out;
    }
};
