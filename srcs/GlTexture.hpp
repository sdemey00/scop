#pragma once

#include "Gl.hpp"
#include <string>

class GlTexture {
	private:
		GLuint	_id = 0;
	public:
		GlTexture(const GlTexture&)				= delete;
        GlTexture& operator=(const GlTexture&)	= delete;
        GlTexture(GlTexture&& o) noexcept;
        GlTexture& operator=(GlTexture&& o) noexcept;

		explicit GlTexture(const std::string& path);

		void bind(unsigned int unit = 0) const;

		~GlTexture();
};
