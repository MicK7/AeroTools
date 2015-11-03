/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkConeSlice.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//  Copyright 2014-2016 Etienne Tang
#ifndef VTKCYLINDRICALCOORDINATES_H
#define VTKCYLINDRICALCOORDINATES_H

#include "vtkSetGet.h"
#include "vtkStdString.h"

#include "vtkCutter.h"
#include "vtkImplicitFunction.h"

class vtkInformation;
class vtkInformationVector;
class vtkDataSetAttributes;


class VTK_EXPORT vtkConeXrFunction : public vtkImplicitFunction
{
public:
    static vtkConeXrFunction* New();
    vtkTypeMacro(vtkConeXrFunction, vtkImplicitFunction);
    void PrintSelf(ostream& os, vtkIndent indent);
    
    double EvaluateFunction(double x[3]);
    double EvaluateFunction(double x, double y, double z)
        {return this->vtkImplicitFunction::EvaluateFunction(x, y, z); };
    void EvaluateGradient(double x[3], double g[3]);
    
    vtkGetVector2Macro(Xr1, double);
    vtkSetVector2Macro(Xr1, double);
    vtkGetVector2Macro(Xr2, double);
    vtkSetVector2Macro(Xr2, double);
    
protected:
    vtkConeXrFunction();
    ~vtkConeXrFunction() {};
    
    double Xr1[2];
    double Xr2[2];
    
private:
    vtkConeXrFunction(const vtkConeXrFunction&);
    void operator=(const vtkConeXrFunction&); 
    
};

class VTK_EXPORT vtkConeSlice : public vtkCutter
{
public:
    static vtkConeSlice* New();
    vtkTypeMacro(vtkConeSlice, vtkCutter);
    void PrintSelf(ostream& os, vtkIndent indent);
    
    vtkGetVector2Macro(Xr1, double);
    virtual void SetXr1(double arg1, double arg2)
    {
        if((this->Xr1[0] != arg1) || (this->Xr1[1] != arg2))
        {
            this->Xr1[0] = arg1;
            this->Xr1[1] = arg2;
            this->ConeFunction->SetXr1(arg1, arg2);
            this->Modified();
        }
    };
    void SetXr1(double arg[2])
    {
        this->SetXr1(arg[0], arg[1]);
    };
    
    vtkGetVector2Macro(Xr2, double);
    virtual void SetXr2(double arg1, double arg2)
    {
        if((this->Xr2[0] != arg1) || (this->Xr2[1] != arg2))
        {
            this->Xr2[0] = arg1;
            this->Xr2[1] = arg2;
            this->ConeFunction->SetXr2(arg1, arg2);
            this->Modified();
        }
    };
    void SetXr2(double arg[2])
    {
        this->SetXr2(arg[0], arg[1]);
    };
    
protected:
    vtkConeSlice();
    ~vtkConeSlice();
    
    vtkConeXrFunction* ConeFunction;
    double Xr1[2];
    double Xr2[2];

private:
    vtkConeSlice operator=(const vtkConeSlice&);
    vtkConeSlice(const vtkConeSlice&);

};

#endif
