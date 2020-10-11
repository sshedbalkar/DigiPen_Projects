///////////////////////////////////////////////////////////////////////
// Provides the creation of a Frame Buffer Object
//
// Gary Herron
//
// Copyright © 2011 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

class FBO {
public:
	int width, height;
	unsigned int fbo;
	unsigned int texture;

	void CreateFBO(const int w, const int h);
	void Bind();
	void Unbind();
};
