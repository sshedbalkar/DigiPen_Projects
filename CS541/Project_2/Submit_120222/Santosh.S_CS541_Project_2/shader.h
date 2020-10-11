///////////////////////////////////////////////////////////////////////
// $Id$
//
// A small class for building a shader program, attaching shaders, and
// linking.  All steps are carfully error checked.
//
// Gary Herron
//
// Copyright © 2009 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

class ShaderProgram
{
public:
    int program;
    
    void CreateProgram();
    void CreateShader(const char* fileName, const int type);
    void LinkProgram();
    void Use();
    void Unuse();
};
