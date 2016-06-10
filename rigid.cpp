

#include "rigid.h"


Rigid::Rigid()
{
    
}

Rigid::~Rigid()
{
    
}

Rigid::Rigid(const double& m, const double& s, const double &ts)
{
    setmass(m);
    sets(s);
    setts(ts);
}

Rigid::Rigid(const Rigid& rigid)
{
    mass = rigid.mass;
    Ibody = rigid.Ibody;
    Ibodyinv = rigid.Ibodyinv;
    x = rigid.x;
    Q = rigid.Q;
    P = rigid.P;
    L = rigid.L;
    Iinv = rigid.Iinv;
    R = rigid.R;
    v = rigid.v;
    omega = rigid.omega;
    force = rigid.force;
    torque = rigid.torque;
    
}


void Rigid::setmass(const double &m)
{
    mass = m;
}

void Rigid::sets(const double &s)
{
    size = s;
}

void Rigid::setts(const double &ts)
{
    timestep = ts;
}

double& Rigid::getmass()
{
    return mass;
}

Matrix3x3& Rigid::getIbody()
{
    return Ibody;
}

Matrix3x3& Rigid::getIbodyinv()
{
    return Ibodyinv;
}

Vector3d& Rigid::getx()
{
    return x;

}

Quaternion& Rigid::getQ()
{
    return Q;
}

Vector3d& Rigid::getP()
{
    return P;
}

Vector3d& Rigid::getL()
{
    return L;
}

Matrix3x3& Rigid::getIinv()
{
    return Iinv;
}

Matrix3x3& Rigid::getR()
{
    return R;
}

Vector3d& Rigid::getv()
{
    return v;
}

Vector3d& Rigid::getomega()
{
    return omega;
}

Vector3d& Rigid::getforce()
{
    return force;
}

Vector3d& Rigid::gettorque()
{
    return torque;
}

void Rigid::calculateInertia()
{
    MOI = (mass * size * size )/6;
    Matrix3x3 Ib(MOI, 0.0, 0.0, 0.0, MOI, 0.0, 0.0, 0.0, MOI);
    Matrix3x3 Iin = Ib.inv();
    Ibody = Ib;
    Ibodyinv = Iin;
}

void Rigid::initializeRigid()
{
    Vector3d xval(-4.0,1.0,-4.0);
    Vector3d Pval(0.0,0.0,0.0);
    Vector3d Lval(80.0,0.0,0.0);
    Vector3d vval(0.0,0.0,0.0);
    Vector3d omegaval(0.0,0.0,0.0);
    Matrix3x3 rot( 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
    
    calculateInertia();
    
    x= xval;
    P = Pval;
    L= Lval;
    Q.identity();
    
    R = rot;
    
    ComputeAuxillary();
    
}

void Rigid::ComputeForceAndTorque()
{
    Vector3d fval(0.0,-0.5,0.0);
    Vector3d forcePoint(0.0,0.0,0.0);
    force = fval;
    torque = (forcePoint- x) % force;
    
}

void Rigid::drawRigid()
{
    glPushMatrix();
    glTranslatef (x.x,x.y,x.z);
    glutSolidCube(size);
    glPopMatrix ();
}

void Rigid::Update()
{
    x = x + (timestep * v);
    Q = Q + (timestep * qdot);
    P = P + (timestep * force);
    L = L + (timestep * torque);
    
}

void Rigid::ComputeAuxillary()
{
    v = P/mass;
    R = (Q.normalize()).rotation();
    Iinv = R * Ibodyinv * R.transpose();
    omega = Iinv * L;
    oq.set(omega);
    qdot = (0.5) * oq * Q;
    
}

void Rigid::runSimulation()
{
    ComputeForceAndTorque();
    Update();
    ComputeAuxillary();
    drawRigid();

}



