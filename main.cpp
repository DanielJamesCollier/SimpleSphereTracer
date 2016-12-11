#include <cmath>
#include <fstream>

struct Vec3 {
    double x,y,z;
    Vec3(double xx = 0, double yy = 0, double zz = 0) : x(xx), y(yy), z(zz){}
    Vec3 operator + (Vec3 const & v) {return Vec3(x + v.x, y + v.y, z + v.z);}
    Vec3 operator - (Vec3 const & v) {return Vec3(x - v.x, y - v.y, z - v.z);}
};
double dot(Vec3 const & v1, Vec3 const & v2) {return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));}

struct Ray {
    Vec3 o, d;
    Ray(Vec3 origin, Vec3 direction) : o(origin), d(direction) {}
};

struct Sphere {
    Vec3    center;
    double  radius;
    Sphere(Vec3 const & c = Vec3(0,0,0), double r = 1) : center(c), radius(r){}
    bool intersect(Ray ray) {
        Vec3 orms = ray.o - center;
        double disc = (2 * dot(orms,ray.d)) * (2 * dot(orms,ray.d)) - (4 * dot(orms,orms) - (radius * radius));
        if(disc < 0)  return false; else return true;
    }
};

int main() {
    const int size = 500;
    std::ofstream out("/Users/danielcollier/Desktop/rayTracedCircle.ppm"); // change file path for own system
    out << "P3\n" << size << '\n' << size << '\n' << "255\n";
    Vec3 pixels[size][size];
    Sphere sphere(Vec3(size / 2,size / 2,50),50);
    for(int y = 0; y < size; ++y) {
        for(int x = 0; x < size; ++x) {
            Ray ray(Vec3(x,y,0),Vec3(0,0,1));
            pixels[y][x] = Vec3(0,0,100);
            if(sphere.intersect(ray)) pixels[y][x] = Vec3(0,255,0);
            out << (int)pixels[y][x].x << '\n' << (int)pixels[y][x].y << '\n' <<(int)pixels[y][x].z << '\n';
        }
    }
    out.close();
    return 0;
};
