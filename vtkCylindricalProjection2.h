/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCylindricalProjection2.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//  Copyright 2014-2016 Etienne Tang
#ifndef VTKCYLINDRICALPROJECTION2_H
#define VTKCYLINDRICALPROJECTION2_H

#include "vtkSetGet.h"
#include "vtkStdString.h"

#include "vtkPolyDataAlgorithm.h"
#include "vtkPolyData.h"
#include "vtkPoints.h"
#include "vtkMath.h"

#include <cmath>

class vtkInformation;
class vtkInformationVector;
class vtkDataSetAttributes;
class vtkAppendPolyData;
class vtkClipPolyData;
class vtkPlane;

class VTK_EXPORT vtkCylindricalProjection2 : public vtkPolyDataAlgorithm
{
public:
    static vtkCylindricalProjection2* New();
    vtkTypeMacro(vtkCylindricalProjection2, vtkPolyDataAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);
	
	vtkGetMacro(RRef, double)
	vtkSetMacro(RRef, double)
	vtkGetMacro(ThetaRef, double)
    vtkSetMacro(ThetaRef, double)
    
protected:
    vtkCylindricalProjection2();
    ~vtkCylindricalProjection2();
	
	double RRef;
    double ThetaRef;
    
    vtkAppendPolyData* AppendFilter;
    vtkClipPolyData* ClipFilter1;
    vtkClipPolyData* ClipFilter2;
    vtkPlane* CuttingPlane;
    
    virtual int RequestData(vtkInformation* request, vtkInformationVector** inVector, vtkInformationVector* outVector);
    
    void SetPlane();
    void PointProjection(vtkPolyData* orig, vtkPoints* pts, double thetaCadran);
    
    inline double mod2pi(double a)
    {
        return a - 2.*vtkMath::Pi()*std::floor(a/(2.*vtkMath::Pi()));
    };
    
private:
    vtkCylindricalProjection2 operator=(const vtkCylindricalProjection2&);
    vtkCylindricalProjection2(const vtkCylindricalProjection2&);

};

#endif
