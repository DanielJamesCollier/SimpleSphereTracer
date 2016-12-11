#include <cmath>
#include <fstream>

struct Vec3 {
    double x,y,z;
    Vec3(double xx = 0, double yy = 0, double zz = 0) : x(xx), y(yy), z(zz){}
 
    Vec3 operator + (Vec3 const & v) {return Vec3(x + v.x, y + v.y, z + v.z);}
    Vec3 operator - (Vec3 const & v) {return Vec3(x - v.x, y - v.y, z - v.z);}
    Vec3 operator / (Vec3 const & v) {return Vec3(x / v.x, y / v.y, z / v.z);}
};

   double dot(Vec3 const & v1, Vec3 const & v2) {return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));}

struct Ray {
    Ray(Vec3 o, Vec3 d) : origin(o), direction(d) {}
    Vec3 origin, direction;
};

struct Sphere {
    Vec3    center;
    double  radius;
    Sphere(Vec3 const & c = Vec3(0,0,0), double r = 1) : center(c), radius(r){}
    bool intersect(Ray ray, double & t) {
        Vec3    oc   = ray.origin - center;
        double  b    = 2 * dot(oc,ray.direction);
        double  c    = dot(oc,oc) - (radius * radius);
        double  disc = b*b - 4*c;
        if(disc < 0) { return false; }
        else {return true; }
    }
};

struct Colour {
    double r,g,b;
    Colour(double red = 0, double green = 0, double blue = 0) : r(red), g(green), b(blue){}
};

int main() {
    const int size = 500;
    std::ofstream out("/Users/danielcollier/Desktop/rayTracedCircle.ppm"); // change file path for own system
    out << "P3\n" << size << '\n' << size << '\n' << "255\n";
    Colour pixels[size][size];
    Sphere sphere(Vec3(size / 2,size / 2,50),50);
    for(int y = 0; y < size; ++y) {
        for(int x = 0; x < size; ++x) {
            Ray ray(Vec3(x,y,0),Vec3(0,0,1));
            pixels[y][x] = Colour(0,0,100);
            double t = 20000;
            if(sphere.intersect(ray,t)) {
                pixels[y][x] = Colour(0,255,0);
            }
            out << (int)pixels[y][x].r << '\n' << (int)pixels[y][x].g << '\n' <<(int)pixels[y][x].b << '\n';
        }
    }
    out.close();
    return 0;
};
