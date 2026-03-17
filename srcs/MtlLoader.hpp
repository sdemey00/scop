#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

struct Material {
    std::string	name;
	float	Ns;
	float	Ka[3];
	float	Kd[3];
	float	Ks[3];
	float	Ni;
	float	d;
	GLuint	illum;

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
			else if (token == "Ns") {}
			else if (token == "Ka") {}
			else if (token == "Kd") {}
			else if (token == "Ks") {}
			else if (token == "Ni") {}
			else if (token == "d") {}
			else if (token == "illum") {}
        }
        return (mat);
    }
};
