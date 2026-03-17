#pragma once

#include "Mesh.hpp"
#include "MtlLoader.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <array>

// .obj faces use 3 separate index spaces (position / texcoord / normal).
// OpenGL uses one unified index. This key deduplicates the combination.
struct ObjKey {
    int v, vt, vn;
    bool operator==(const ObjKey& o) const {
        return (v == o.v && vt == o.vt && vn == o.vn);
    }
};

struct ObjKeyHash {
    size_t operator()(const ObjKey& k) const {
        // simple but effective hash combination
        size_t h = std::hash<int>{}(k.v);
        h ^= std::hash<int>{}(k.vt) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<int>{}(k.vn) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

class ObjLoader {
public:
    // Returns a Mesh ready to draw — throws std::runtime_error on any parse failure
    static Mesh load(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open())
            throw std::runtime_error("ObjLoader: cannot open file: " + path);

        // raw data parsed line by line
        std::vector<std::array<float, 3>> positions;   // v  x y z
        std::vector<std::array<float, 2>> texcoords;   // vt u v
        std::vector<std::array<float, 3>> normals;     // vn x y z

        // final unified buffers for the Mesh
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;

        // maps an ObjKey → index in the unified vertex buffer
        std::unordered_map<ObjKey, unsigned int, ObjKeyHash> cache;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#')
                continue;

            std::istringstream ss(line);
            std::string token;
            ss >> token;

            if (token == "v") {
                float x, y, z;
                ss >> x >> y >> z;
                positions.push_back({x, y, z});
            }
            else if (token == "vt") {
                float u, v;
                ss >> u >> v;
                texcoords.push_back({u, v});
            }
            else if (token == "vn") {
                float nx, ny, nz;
                ss >> nx >> ny >> nz;
                normals.push_back({nx, ny, nz});
            }
            else if (token == "f") {
                // collect all corner tokens on this face line
                std::vector<ObjKey> faceKeys;
                std::string corner;
                while (ss >> corner)
                    faceKeys.push_back(parseCorner(corner));

                // triangulate: fan from faceKeys[0]
                // triangle 0: [0,1,2], [0,2,3], [0,3,4] ...
                for (size_t i = 1; i + 1 < faceKeys.size(); ++i) {
                    for (ObjKey key : { faceKeys[0], faceKeys[i], faceKeys[i+1] }) {
                        auto it = cache.find(key);
                        if (it != cache.end()) {
                            indices.push_back(it->second);
                        } else {
                            Vertex vert = buildVertex(key, positions, texcoords, normals);
                            unsigned int idx = static_cast<unsigned int>(vertices.size());
                            vertices.push_back(vert);
                            cache[key] = idx;
                            indices.push_back(idx);
                        }
                    }
                }
            }
			else if (token == "mtllib") {
				// load texture
				Material m = MtlLoader::load(path);
			}
			else if (token == "usemtl") {
				// use material
			}
        }

        if (vertices.empty())
            throw std::runtime_error("ObjLoader: no geometry found in: " + path);

        return Mesh(vertices, indices);
    }

private:
    // parse "v/vt/vn", "v//vn", "v/vt", or "v" — all valid .obj face corners
    static ObjKey parseCorner(const std::string& s) {
        ObjKey key{-1, -1, -1};
        std::istringstream ss(s);
        std::string part;

        int slot = 0;
        while (std::getline(ss, part, '/')) {
            if (!part.empty()) {
                int idx = std::stoi(part);
                // .obj indices are 1-based; negative = relative to end (we ignore negative for now)
                if      (slot == 0) key.v  = idx - 1;
                else if (slot == 1) key.vt = idx - 1;
                else if (slot == 2) key.vn = idx - 1;
            }
            ++slot;
        }
        return key;
    }

    static Vertex buildVertex(
        const ObjKey& key,
        const std::vector<std::array<float, 3>>& positions,
        const std::vector<std::array<float, 2>>& texcoords,
        const std::vector<std::array<float, 3>>& normals)
    {
        Vertex v{};

        if (key.v >= 0 && key.v < (int)positions.size()) {
            v.x = positions[key.v][0];
            v.y = positions[key.v][1];
            v.z = positions[key.v][2];
        }
        if (key.vn >= 0 && key.vn < (int)normals.size()) {
            v.nx = normals[key.vn][0];
            v.ny = normals[key.vn][1];
            v.nz = normals[key.vn][2];
        }
        if (key.vt >= 0 && key.vt < (int)texcoords.size()) {
            v.u = texcoords[key.vt][0];
            v.v = texcoords[key.vt][1];
        }
        return v;
    }
};