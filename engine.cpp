#include <iostream>
#include <cmath>
#include <array>
#include <emscripten/bind.h>



// Basic 3D point structure
struct Point3D{
    double x,y,z;
    Point3D() : x(0), y(0), z(0) {}
    Point3D(double x, double y, double z) : x(x), y(y), z(z) {}
 
};




// Basic 2D point structure
struct Point2D{
    double x,y;
    Point2D() : x(0), y(0) {}
    Point2D(double x, double y) : x(x), y(y) {}
};




// 3x3 Matrix for 3D transformations
struct Matrix{
    double m[3][3];
    Matrix(double a00, double a01, double a02,
           double a10, double a11, double a12,
           double a20, double a21, double a22) {
        m[0][0] = a00; m[0][1] = a01; m[0][2] = a02;
        m[1][0] = a10; m[1][1] = a11; m[1][2] = a12;
        m[2][0] = a20; m[2][1] = a21; m[2][2] = a22;
    }
};




// Function to apply a transformation matrix to a 3D point
Point3D operations(Point3D p1,Matrix m){
    double x=m.m[0][0]*p1.x+m.m[0][1]*p1.y+m.m[0][2]*p1.z;
    double y=m.m[1][0]*p1.x+m.m[1][1]*p1.y+m.m[1][2]*p1.z;
    double z=m.m[2][0]*p1.x+m.m[2][1]*p1.y+m.m[2][2]*p1.z;
    return Point3D(x,y,z);
}




// Function to get the position matrix of a point after transformation
std::array<double,3>getPositionMatrix(Point3D p,Matrix m){
    Point3D transformed=operations(p,m);
    return {transformed.x, transformed.y, transformed.z};
} 




// Function to multiply two 3x3 matrices
Matrix multiplyMatrices(Matrix A, Matrix B) {
    Matrix result(0,0,0, 0,0,0, 0,0,0);
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            result.m[row][col] = A.m[row][0] * B.m[0][col] + 
                                 A.m[row][1] * B.m[1][col] + 
                                 A.m[row][2] * B.m[2][col];
        }
    }
    return result;
}




// Function to project a 3D point onto a 2D plane (simple perspective projection)
Point2D Projectionin2D(Point3D point){
    if(point.z>0){
        double x2d=point.x/(point.z+1);
        double y2d=point.y/(point.z+1);
        return Point2D(x2d,y2d);
    }
    else{double x2d=point.x/(1-point.z);
        double y2d=point.y/(1-point.z);
        return Point2D(x2d,y2d);
    } }



// Inversion matrix for 3D transformations
Matrix getInversionMatrix()
{return Matrix(-1,0,0, 0,-1,0, 0,0,-1);}


 // Implementation for n-fold rotation matrix
 Matrix getN_foldRotationMatrix(int n,char axis)
 {
    double angle=(360.0/n)*(std::acos(-1.0)/180.0);
    double c=std::cos(angle);
    double s=std::sin(angle);
    if (axis=='z')
    {
        return Matrix(c,-s,0, s,c,0, 0,0,1);
    }
    else if (axis=='y')
    {
        return Matrix(c,0,s, 0,1,0, -s,0,c);
    }
    else if (axis=='x')
    {
        return Matrix(1,0,0, 0,c,-s, 0,s,c);
    }return Matrix(1,0,0, 0,1,0, 0,0,1);}
    //Mirror transformation matrix across a plane defined by a normal vector
    Matrix getMirrorMatrix(char normal_axis)
    {
    // A mirror is just a 2-fold rotation followed by an inversion
    Matrix rot2 = getN_foldRotationMatrix(2, normal_axis);
    Matrix inv = getInversionMatrix();
    
    return multiplyMatrices(inv, rot2);
    }
    int main(){
    Point3D test(1,2,3);
    std::cout<<test.x<<" "<<test.y<<" "<<test.z<<std::endl;
    Point2D projected=Projectionin2D(test);
    std::cout<<"Projected 2D coordinates: "<<projected.x<<" "<<projected.y<<std::endl;
    return 0;}
  

using namespace emscripten;

#include <vector>

// Function to generate the physical mathematical path of a rotation
std::vector<Point3D> generateRotationPath(Point3D p, int n, char axis, int steps) {
    std::vector<Point3D> path;
    double totalAngleDegrees = 360.0 / n;
    double stepAngleDegrees = totalAngleDegrees / steps;

    for (int i = 0; i <= steps; ++i) {
        double currentAngle = (stepAngleDegrees * i) * (std::acos(-1.0) / 180.0);
        double c = std::cos(currentAngle);
        double s = std::sin(currentAngle);
        Matrix stepMatrix(1,0,0, 0,1,0, 0,0,1);

        if (axis == 'z') {
            stepMatrix = Matrix(c,-s,0, s,c,0, 0,0,1);
        } else if (axis == 'y') {
            stepMatrix = Matrix(c,0,s, 0,1,0, -s,0,c);
        } else if (axis == 'x') {
            stepMatrix = Matrix(1,0,0, 0,c,-s, 0,s,c);
        }

        path.push_back(operations(p, stepMatrix));
    }
    return path;
}

// Function to generate the full closed orbit under n-fold rotation
std::vector<Point3D> generateOrbit(Point3D p, int n, char axis) {
    std::vector<Point3D> orbit;
    Matrix fullRotMatrix = getN_foldRotationMatrix(n, axis);
    Point3D currentPoint = p;
    
    for (int i = 0; i < n; ++i) {
        orbit.push_back(currentPoint);
        currentPoint = operations(currentPoint, fullRotMatrix);
    }
    return orbit;
}
std::vector<Point3D> generateReflectionOrbit(Point3D p,char normal_axis){
    std::vector<Point3D> orbit;
    Matrix reflctionmatrix=getMirrorMatrix(normal_axis);
    Point3D result=operations(p,reflctionmatrix);
    orbit.push_back(p); 
    orbit.push_back(result);
    return orbit;
    
}




std::vector<Point3D> generateInversionOrbit(Point3D p){
    std::vector<Point3D> orbit;
    Matrix inversionmatrix = getInversionMatrix();
    Point3D result = operations(p, inversionmatrix);
    orbit.push_back(p);
    orbit.push_back(result);
    return orbit;
}

// Reflection and Inversion do not curve; they jump straight across space.
// Therefore, the "Path" is beautifully simple: just a straight line connecting the two points.
std::vector<Point3D> generateReflectionPath(Point3D p, char normal_axis){
    return generateReflectionOrbit(p, normal_axis);
}

std::vector<Point3D> generateInversionPath(Point3D p){
    return generateInversionOrbit(p);
}


EMSCRIPTEN_BINDINGS(my_module) {
    // Register std::vector bounds to allow JS to read paths/orbits
    register_vector<Point3D>("VectorPoint3D");
    register_vector<Point2D>("VectorPoint2D");

    // Export the Point3D struct so JS understands it
    value_object<Point3D>("Point3D")
        .field("x", &Point3D::x)
        .field("y", &Point3D::y)
        .field("z", &Point3D::z);

    class_<Matrix>("Matrix");

    // Export the Point2D struct
    value_object<Point2D>("Point2D")
        .field("x", &Point2D::x)
        .field("y", &Point2D::y);

    // Export your core functions
    function("Projectionin2D", &Projectionin2D);
    function("getInversionMatrix", &getInversionMatrix);
    function("getN_foldRotationMatrix", &getN_foldRotationMatrix);
    function("getMirrorMatrix", &getMirrorMatrix);
    function("operations", &operations); 
    function("generateRotationPath", &generateRotationPath);
    function("generateOrbit", &generateOrbit);

    // The new bindings!
    function("generateReflectionOrbit", &generateReflectionOrbit);
    function("generateInversionOrbit", &generateInversionOrbit);
    function("generateReflectionPath", &generateReflectionPath);
    function("generateInversionPath", &generateInversionPath);
}
