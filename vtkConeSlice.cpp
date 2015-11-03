/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkConeSlice.cpp

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//  Copyright 2014-2016 Etienne Tang
#include "vtkConeSlice.h"

#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPointSet.h"
#include "vtkPoints.h"
#include "vtkDoubleArray.h"
#include "vtkPointData.h"
#include "vtkMath.h"

#include <cmath>

vtkStandardNewMacro(vtkConeXrFunction)
vtkStandardNewMacro(vtkConeSlice)

vtkConeXrFunction::vtkConeXrFunction()
{
    this->Xr1[0] = 0.;
    this->Xr1[1] = 0.;
    this->Xr2[0] = 0.;
    this->Xr2[1] = 1.;
}

double vtkConeXrFunction::EvaluateFunction(double pt[3])
{
    double r = sqrt(pt[1]*pt[1] + pt[2]*pt[2]);
    return (Xr1[1]-Xr2[1])*(pt[0]-Xr1[0]) + (Xr2[0]-Xr1[0])*(r - Xr1[1]);
}

void vtkConeXrFunction::EvaluateGradient(double pt[3], double g[3])
{
    double r = sqrt(pt[1]*pt[1] + pt[2]*pt[2]);
    g[0] = Xr1[1]-Xr2[1];
    g[1] = (Xr2[0]-Xr1[0])*pt[1]/r;
    g[2] = (Xr2[0]-Xr1[0])*pt[2]/r;
}

void vtkConeXrFunction::PrintSelf( ostream& os, vtkIndent indent )
{
    this->Superclass::PrintSelf( os, indent );
}


void vtkConeSlice::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
}

vtkConeSlice::vtkConeSlice():
    vtkCutter()
{
    this->ConeFunction = vtkConeXrFunction::New();
    this->SetXr1(0., 0.);
    this->SetXr2(0., 1.);
    this->SetCutFunction(this->ConeFunction);
}

vtkConeSlice::~vtkConeSlice()
{
    this->ConeFunction->Delete();
}
