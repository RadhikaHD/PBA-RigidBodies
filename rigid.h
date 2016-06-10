
#ifndef rigid_h
#define rigid_h

#include <stdio.h>
#include "Matrix.h"
#include "Vector.h"
#include "Camera.h"
#include "Quaternion.h"


#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif


class Rigid
{
public:
    
    //constant quantities
    double mass;
    Matrix3x3 Ibody, Ibodyinv;
    
    //state variables
    Vector3d x;
    Quaternion Q;
    Quaternion qdot, oq;
    Vector3d P,L;
    
    //derived quantities
    Matrix3x3 Iinv;
    Matrix3x3 R;
    Vector3d v, omega;
    
    //computed quantities
    Vector3d force, torque;
    
    int distance, maxDistance;
    double MOI;
    double size;
    double timestep;
    
    
    Rigid();
    Rigid(const double& mass, const double& size, const double &timestep);
    Rigid(const Rigid& rigid);
    ~Rigid();
    
    void setmass(const double &mass);
    void sets(const double &size);
    void setts(const double &ts);
    
    double& getmass();
    Matrix3x3& getIbody();
    Matrix3x3& getIbodyinv();
    Vector3d& getx();
    Quaternion& getQ();
    Vector3d& getP();
    Vector3d& getL();
    Matrix3x3& getIinv();
    Matrix3x3& getR();
    Vector3d& getv();
    Vector3d& getomega();
    Vector3d& getforce();
    Vector3d& gettorque();
    
    void calculateInertia();
    void initializeRigid();
    void ComputeForceAndTorque();
    void drawRigid();
    void Update();
    void ComputeAuxillary();
    
    void runSimulation();
    
};



#endif /* rigid_h */
