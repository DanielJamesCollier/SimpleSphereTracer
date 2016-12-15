#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <chrono>
#include <string>

struct Vec3 {
    double x,y,z;
    
    Vec3(double _x = 0, double _y = 0, double _z = 0)
    :
     x(_x),
     y(_y),
     z(_z){}

    Vec3 operator - (Vec3 const & v) {return Vec3(x - v.x, y - v.y, z - v.z);}
};

double dot(Vec3 const & v1, Vec3 const & v2) {
    return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
}

struct Colour {
    int r,g,b;
    
    Colour(int _r = 0, int _g = 0, int _b = 0)
    :
     r(_r),
     g(_g),
     b(_b) {}
};

struct Ray {
    Vec3 origin, direction;
    
    Ray(Vec3 _origin)
    :
      origin(_origin)
    , direction(0,0,1){}
};

struct Sphere {
    Vec3    center;
    Colour  colour;
    double  radiusSquared; // precompute so don't have to calc per ray
    
    Sphere(Vec3 const & _center = Vec3(0,0,0),Colour _colour = Colour(0,0,0), double _radius = 1 )
    :
      center(_center)
    , colour(_colour)
    , radiusSquared(_radius * _radius){}
    
    bool intersect(Ray ray) {
        Vec3 orms = ray.origin - center;
        double d = 2 * dot(orms,ray.direction);
        double disc = d * d - (4 * dot(orms,orms) - radiusSquared);
        if(0 < disc) {
            return true;
        }
        return false;
    }
};

int main() {
    std::cout.sync_with_stdio(false);
    const auto imgSize     = 600;
    const auto numSpheres = 1'0000;
    Colour     pixels[imgSize][imgSize];
    
    std::vector<Sphere> sphereList;
    sphereList.reserve(numSpheres);
    
    // generate random spheres
    for(auto i = 0; i < numSpheres; ++i) {
        auto radius = 10 + (rand() % (50 - 10 + 1));
        auto x  = (radius / 2) + (rand() % (int)((imgSize - (radius / 2)) - (radius / 2) + 1));
        auto y  = (radius / 2) + (rand() % (int)((imgSize - (radius / 2)) - (radius / 2) + 1));
        int  r  = 1 + (rand() % (255 - 1 + 1));
        int  g  = 1 + (rand() % (255 - 1 + 1));
        int  b  = 1 + (rand() % (255 - 1 + 1));
        sphereList.emplace_back(Vec3(x, y,0), Colour(r,g,b), radius);
    }
    
    std::string memoryBuffer;
    memoryBuffer.reserve(imgSize * imgSize * 3);
    auto percentRendered = 0;
    auto startRender     = std::chrono::system_clock::now();
    
    // ray trace all the spheres ever!
    std::cout << "All of the rays, all of the time.\nSphere Count: " << numSpheres << std::endl;
    for(auto y = 0; y < imgSize; ++y) {
        for(auto x = 0; x < imgSize; ++x) {
            
            Ray ray(Vec3(x,y,0));
            
            for(auto & s : sphereList) {
                if(s.intersect(ray)) {
                    pixels[y][x] = s.colour;
                }
            }
            memoryBuffer += std::to_string(pixels[y][x].r) + '\n';
            memoryBuffer += std::to_string(pixels[y][x].g) + '\n';
            memoryBuffer += std::to_string(pixels[y][x].b) + '\n';
        }
        percentRendered++;
        
        if(y % (imgSize / 10) == 0) {
            std::cout << "(" << (percentRendered / (imgSize / 100)) << "%)"<< std::endl;
        }
    }
    auto endRender = std::chrono::system_clock::now();
    std::cout << "render time: " <<  std::chrono::duration_cast<std::chrono::milliseconds>(endRender - startRender).count() << "ms"<< std::endl;
    
    // open file and dump memory buffer
    std::ofstream out("/Users/danielcollier/Desktop/rayTracedCircle.ppm"); // change file path for own system
    out << "P3\n" << imgSize << '\n' << imgSize << '\n' << "255\n";
    out << memoryBuffer << std::endl;
    out.close();
    
    return 0;
}
