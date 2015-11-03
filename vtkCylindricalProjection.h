/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCylindricalProjection.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//  Copyright 2014-2016 Etienne Tang
#ifndef VTKCYLINDRICALPROJECTION_H
#define VTKCYLINDRICALPROJECTION_H

#include "vtkSetGet.h"
#include "vtkStdString.h"

#include "vtkPointSetAlgorithm.h"

class vtkInformation;
class vtkInformationVector;
class vtkDataSetAttributes;


class VTK_EXPORT vtkCylindricalProjection : public vtkPointSetAlgorithm
{
public:
    static vtkCylindricalProjection* New();
    vtkTypeMacro(vtkCylindricalProjection, vtkPointSetAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);
	
	vtkGetMacro(Rref, double)
	vtkSetMacro(Rref, double)
	
protected:
    vtkCylindricalProjection();
    ~vtkCylindricalProjection();
	
	double Rref;
	
    virtual int RequestData(vtkInformation* request, vtkInformationVector** inVector, vtkInformationVector* outVector);

private:
    vtkCylindricalProjection operator=(const vtkCylindricalProjection&);
    vtkCylindricalProjection(const vtkCylindricalProjection&);

};

#endif
