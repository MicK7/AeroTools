/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCylindricalCoordinates2.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//  Copyright 2014-2016 Etienne Tang
#ifndef VTKCYLINDRICALCOORDINATES2_H
#define VTKCYLINDRICALCOORDINATES2_H

#include "vtkSetGet.h"
#include "vtkStdString.h"
#include "vtkMath.h"

#include "vtkPointSetAlgorithm.h"

#include <cmath>

class vtkInformation;
class vtkInformationVector;
class vtkDataSetAttributes;


class VTK_EXPORT vtkCylindricalCoordinates2 : public vtkPointSetAlgorithm
{
public:
    static vtkCylindricalCoordinates2* New();
    vtkTypeMacro(vtkCylindricalCoordinates2, vtkPointSetAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

    vtkGetMacro(ThetaRef, double);
    vtkSetMacro(ThetaRef, double);
protected:
    double ThetaRef; // Le calcul de theta donne une valeur qui est eloignee d'au plus +/- pi de ThetaRef
    
    vtkCylindricalCoordinates2();
    ~vtkCylindricalCoordinates2();

    virtual int RequestData(vtkInformation* request, vtkInformationVector** inVector, vtkInformationVector* outVector);
    
    inline double mod2pi(double a) { return a - floor(a/(2.*vtkMath::Pi()))*2.*vtkMath::Pi(); }
    
private:
    vtkCylindricalCoordinates2 operator=(const vtkCylindricalCoordinates2&);
    vtkCylindricalCoordinates2(const vtkCylindricalCoordinates2&);

};

#endif
