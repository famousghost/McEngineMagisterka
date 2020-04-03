#pragma once
#include <iostream>
#include <vector>
namespace McEngine
{
namespace Meshes
{
typedef float GLfloat;
struct VertexCoords
{
    union
    {
        GLfloat x;
        GLfloat y;
        GLfloat z;
    };
    union
    {
        GLfloat r;
        GLfloat g;
        GLfloat b;
    };
    union
    {
        GLfloat s;
        GLfloat t;
        GLfloat b;
    };

    bool operator==(const VertexCoords& p_verCoords)
    {
        return this->x == p_verCoords.x
               and this->y == p_verCoords.y
               and this->z == p_verCoords.z;
    }

    bool operator!=(const VertexCoords& p_vertCoords)
    {
        return not (*this == p_vertCoords);
    }

    friend std::ostream& operator<<(std::ostream& out, const VertexCoords& verticies)
    {
        out << "(" << verticies.x << ", " << verticies.y << ", " << verticies.z << ")" << std::endl;
        return out;
    }
};
struct TextureCoords
{
    union
    {
        GLfloat x;
        GLfloat y;
    };
    union
    {
        GLfloat r;
        GLfloat g;
    };
    union
    {
        GLfloat s;
        GLfloat t;
    };

    bool operator==(const TextureCoords& p_verCoords)
    {
        return this->x == p_verCoords.x
            and this->y == p_verCoords.y;
    }

    bool operator!=(const TextureCoords& p_vertCoords)
    {
        return not (*this == p_vertCoords);
    }

    friend std::ostream& operator<<(std::ostream& out, const TextureCoords& textureCoords)
    {
        out << "(" << textureCoords.x << ", " << textureCoords.y << ")" << std::endl;
        return out;
    }
};
struct NormalCoords
{
    union
    {
        GLfloat x;
        GLfloat y;
        GLfloat z;
    };
    union
    {
        GLfloat r;
        GLfloat g;
        GLfloat b;
    };
    union
    {
        GLfloat s;
        GLfloat t;
        GLfloat b;
    };

    bool operator==(const NormalCoords& p_verCoords)
    {
        return this->x == p_verCoords.x
            and this->y == p_verCoords.y
            and this->z == p_verCoords.z;
    }

    bool operator!=(const NormalCoords& p_vertCoords)
    {
        return not (*this == p_vertCoords);
    }

    friend std::ostream& operator<<(std::ostream& out, const NormalCoords& normal)
    {
        out << "(" << normal.x << ", " << normal.y << ", " << normal.z << ")" << std::endl;
        return out;
    }
};
struct ColorValues
{
    union
    {
        GLfloat x;
        GLfloat y;
        GLfloat z;
    };
    union
    {
        GLfloat r;
        GLfloat g;
        GLfloat b;
    };
    union
    {
        GLfloat s;
        GLfloat t;
        GLfloat b;
    };

    bool operator==(const ColorValues& p_verCoords)
    {
        return this->x == p_verCoords.x
            and this->y == p_verCoords.y
            and this->z == p_verCoords.z;
    }

    bool operator!=(const ColorValues& p_vertCoords)
    {
        return not (*this == p_vertCoords);
    }

    friend std::ostream& operator<<(std::ostream& out, const ColorValues& color)
    {
        out << "("<< color.x << ", " << color.y << ", " << color.z << ")" << std::endl;
        return out;
    }
};

struct Model
{
    std::vector<VertexCoords> m_vertexCoords;
    std::vector<ColorValues> m_colorValues;
    std::vector<NormalCoords> m_normalCoords;
    std::vector<TextureCoords> m_textureCoords;
    std::vector<uint32_t> m_indicies;
};

}//Meshes
}//GameEngine