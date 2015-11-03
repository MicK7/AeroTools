/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCylindricalCoordinatesVector.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//  Copyright 2014-2016 Etienne Tang
#ifndef VTKCYLINDRICALCOORDINATESVECTOR_H
#define VTKCYLINDRICALCOORDINATESVECTOR_H

#include "vtkSetGet.h"
#include "vtkStdString.h"

#include "vtkPointSetAlgorithm.h"

class vtkInformation;
class vtkInformationVector;
class vtkDataSetAttributes;


class VTK_EXPORT vtkCylindricalCoordinatesVector : public vtkPointSetAlgorithm
{
public:
    static vtkCylindricalCoordinatesVector* New();
    vtkTypeMacro(vtkCylindricalCoordinatesVector, vtkPointSetAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

	void SetName(int pipo1, int pipo2, int pipo3, int pipo4, char* name) { Name = vtkStdString(name); }
	vtkStdString GetName() { return Name; }

protected:
    vtkCylindricalCoordinatesVector();
    ~vtkCylindricalCoordinatesVector();

    virtual int RequestData(vtkInformation* request, vtkInformationVector** inVector, vtkInformationVector* outVector);

	vtkStdString Name;
private:
    vtkCylindricalCoordinatesVector operator=(const vtkCylindricalCoordinatesVector&);
    vtkCylindricalCoordinatesVector(const vtkCylindricalCoordinatesVector&);

};

#endif
