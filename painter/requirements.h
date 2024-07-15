//
// Created by Ardrass on 010, 10.07.2024.
//

#ifndef OURPAINT_REQUIREMENTS_H
#define OURPAINT_REQUIREMENTS_H

#define PARAMID double*
#include "Arry.h"
#include "objects.h"
#include <cmath>
#include "enums.h"

/* EXAMPLE
ReqPointSegDist req;
Arry<PARAMID> params = req.getParams();
Arry<double> paramValue(params.getSize());
Arry<double> derivatives(params.getSize());
// вычислить все частные производные
for(param in params) {
    derivatives[k] = req.getDerivative(param);
}
//изменили текущие состояние объектов
paramValues += (-alpha* derivatives);
// Вычислить ошибку для нового состояния
double e = req.getError();
// В зависимоти от величины ошибки продолжить или остановиться */

struct RequirementData {
    Requirement req;
    Arry<ID> objects;
    Arry<double> params;
    RequirementData();
};

struct IReq {
protected:
    Arry<ID> objects;
    Requirement req;
public:
    virtual double getError() = 0;
    virtual Arry<PARAMID> getParams() = 0;
    virtual double getDerivative(PARAMID p) = 0;
};
class ReqPointSegDist : public IReq {
    point* m_p;
    section* m_s;
    double d;
public:
    ReqPointSegDist(point* p, section* s, double dist);
    double getError() override;
    Arry<PARAMID> getParams() override;
    double getDerivative(PARAMID p) override;
};
class ReqPointOnSec : public IReq {
    point* m_p;
    section* m_s;
public:
    ReqPointOnSec(point* p, section* s);
    double getError() override;
    Arry<PARAMID> getParams() override;
    double getDerivative(PARAMID param) override;
};
class ReqPointOnPoint: public IReq {
    point* m_p1;
    point* m_p2;
public:
    ReqPointOnPoint(point* p1, point* p2);
    double getError() override;
    Arry<PARAMID> getParams() override;
    double getDerivative(PARAMID p) override;
};
class ReqPointPointDist: public IReq {
    point* m_p1;
    point* m_p2;
    double v_dist;
public:
    ReqPointPointDist(point* p1, point* p2, double dist);
    double getError() override;
    Arry<PARAMID> getParams() override;
    double getDerivative(PARAMID p) override;
};
class ReqSecCircleDist: public IReq {
    section* m_s;
    circle* m_c;
    double v_dist;
public:
    ReqSecCircleDist(section* m_s, circle* m_c, double dist);
    double getError() override;
    Arry<PARAMID> getParams() override;
    double getDerivative(PARAMID p) override;
};
class ReqSecOnCircle: public IReq {
    section* m_s;
    circle* m_c;
public:
    ReqSecOnCircle(section* m_s, circle* m_c);
    double getError() override;
    Arry<PARAMID> getParams() override;
    double getDerivative(PARAMID p) override;
};


#endif //OURPAINT_REQUIREMENTS_H