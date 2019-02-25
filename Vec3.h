#include <cfloat>
#include <cmath>

struct Vec3
{
public:
    union
    {
        struct 
        { 
            float x, y, z; 
        };
        float v[3];
    };

    Vec3();
    explicit Vec3(float a);
    Vec3(float _x, float _y, float _z);
    Vec3(float a[3]);
    Vec3(const Vec3& v);

    void Set(float _x, float _y, float _z);
    void SetZero();
    bool IsZero() const;
    float MagnitudeSquared() const;
    float Magnitude() const;
    void SetNegative();
    bool Normalize();
    void Absolute();

    Vec3& operator = (const Vec3& v);
    Vec3& operator += (const Vec3& v);
    Vec3& operator -= (const Vec3& v);
    Vec3& operator *= (const Vec3& v);
    Vec3& operator *= (float f);
    Vec3& operator /= (const Vec3& v);
    Vec3& operator /= (float f);
     
    friend bool operator == (const Vec3& v1, const Vec3& v2);
    friend bool operator != (const Vec3& v1, const Vec3& v2);
    friend float operator ~ (const Vec3& v);        
    friend Vec3 operator ! (const Vec3& v);         
    friend Vec3 operator + (const Vec3& v);        
    friend Vec3 operator + (const Vec3& v1, const Vec3& v2);     
    friend Vec3 operator - (const Vec3& v);
    friend Vec3 operator - (const Vec3& v1, const Vec3& v2);
    friend Vec3 operator * (const Vec3& v1, const Vec3& v2);
    friend Vec3 operator * (const Vec3& v1, float f);
    friend Vec3 operator * (float f, const Vec3& v1);
    friend Vec3 operator / (const Vec3& v1, const Vec3& v2);
    friend Vec3 operator / (const Vec3& v1, float f);
    friend Vec3 operator ^ (const Vec3& v1, const Vec3& v2);
    friend float operator | (const Vec3& v1, const Vec3& v2);
};

inline Vec3::Vec3() 
{
    x = y = z = 0.0f;
}

inline Vec3::Vec3(float a)    
{    
    x = y = z = a;    
}

inline Vec3::Vec3(float _x, float _y, float _z)    
{    
    x = _x;     y = _y;     z = _z;    
}

inline Vec3::Vec3(float a[3])    
{    
    x = a[0];   y = a[1];   z = a[2];    
}

inline Vec3::Vec3(const Vec3& v)    
{    
    x = v.x;    y = v.y;    z = v.z;
}

inline void Vec3::Set(float _x, float _y, float _z)
{
    x = _x;     y = _y;     z = _z; 
}

inline void Vec3::SetZero()
{
    x = y = z = 0.0f;
}

inline bool Vec3::IsZero() const
{
    return ((x < FLT_EPSILON) && (x > -FLT_EPSILON) && 
            (y < FLT_EPSILON) && (y > -FLT_EPSILON) && 
            (z < FLT_EPSILON) && (z > -FLT_EPSILON));
}

inline float Vec3::MagnitudeSquared() const
{
    return x*x + y*y + z*z;
}

inline float Vec3::Magnitude() const
{
    return sqrtf(x*x + y*y + z*z);
}

inline void Vec3::SetNegative()
{
    x = -x;
    y = -y;
    z = -z;
}

inline bool Vec3::Normalize()
{
    float lenSquared = MagnitudeSquared();
    if (lenSquared < FLT_EPSILON)
    {
        return false;
    }

    if ((lenSquared - 1.0f > FLT_EPSILON) || (lenSquared - 1.0f < -FLT_EPSILON))
    {
        float invlen = 1.0f / sqrtf(lenSquared);
        x *= invlen;
        y *= invlen;
        z *= invlen;
    }
    return true;
}

inline void Vec3::Absolute()
{
    x = fabsf(x);
    y = fabsf(y);
    z = fabsf(z);
}

inline Vec3& Vec3::operator =  (const Vec3& v)
{
    x = v.x;
	y = v.y;
	z = v.z;
    return *this;
}

inline Vec3& Vec3::operator += (const Vec3& v)
{
    x += v.x;
	y += v.y;
	z += v.z;
    return *this;
}

inline Vec3&  Vec3::operator -= (const Vec3& v)
{
    x -= v.x;
	y -= v.y;
	z -= v.z;
    return *this;
}

inline Vec3& Vec3::operator *= (const Vec3& v)
{
    x *= v.x;
	y *= v.y;
	z *= v.z;
    return *this;
}

inline Vec3& Vec3::operator *= (float f)
{
    x *= f;
	y *= f;
	z *= f;
    return *this;
}

inline Vec3&Vec3::operator /= (const Vec3& v)
{
    x /= v.x;
	y /= v.y;
	z /= v.z;
    return *this;
}

inline Vec3& Vec3::operator /= (float f)
{
    x /= f;
	y /= f;
	z /= f;
    return *this;
}

inline bool operator == (const Vec3& v1, const Vec3& v2)
{
    return (fabsf(v1.x - v2.x) < FLT_EPSILON && 
            fabsf(v1.y - v2.y) < FLT_EPSILON && 
            fabsf(v1.z - v2.z) < FLT_EPSILON);
}

inline bool operator != (const Vec3& v1, const Vec3& v2)
{
    return !(v1 == v2);
}

inline float operator ~ (const Vec3& v)
{
    return v.x*v.x + v.y*v.y + v.z*v.z;
}

inline Vec3 operator ! (const Vec3& v)
{
    float len = 1.0f / sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    return Vec3(v.x*len, v.y*len, v.z*len);
}

inline Vec3 operator + (const Vec3& v)
{
    return v;
}

inline Vec3 operator + (const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

inline Vec3 operator - (const Vec3& v)
{
    return Vec3(-v.x, -v.y, -v.z);    
}

inline Vec3 operator - (const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);    
}

inline Vec3 operator * (const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
}

inline Vec3 operator * (const Vec3& v1, float f)    
{
    return Vec3(v1.x*f, v1.y*f, v1.z*f);    
}

inline Vec3 operator * (float f, const Vec3& v1)    
{
    return Vec3(v1.x*f, v1.y*f, v1.z*f);    
}

inline Vec3 operator / (const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z);    
}

inline Vec3 operator / (const Vec3& v1, float f)
{
    float _f = 1.0f/f;    
    return Vec3(v1.x*_f, v1.y*_f, v1.z*_f);
}

inline Vec3 operator ^ (const Vec3& v1, const Vec3& v2)    
{
    return Vec3(v1.y * v2.z - v1.z * v2.y, 
                v1.z * v2.x - v1.x * v2.z, 
                v1.x * v2.y - v1.y * v2.x);    
}

inline float operator | (const Vec3& v1, const Vec3& v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}