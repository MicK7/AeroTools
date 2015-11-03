/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCylindricalCoordinates.h

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

#include "vtkPointSetAlgorithm.h"

class vtkInformation;
class vtkInformationVector;
class vtkDataSetAttributes;


class VTK_EXPORT vtkCylindricalCoordinates : public vtkPointSetAlgorithm
{
public:
    static vtkCylindricalCoordinates* New();
    vtkTypeMacro(vtkCylindricalCoordinates, vtkPointSetAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

protected:
    vtkCylindricalCoordinates();
    ~vtkCylindricalCoordinates();

    virtual int RequestData(vtkInformation* request, vtkInformationVector** inVector, vtkInformationVector* outVector);

private:
    vtkCylindricalCoordinates operator=(const vtkCylindricalCoordinates&);
    vtkCylindricalCoordinates(const vtkCylindricalCoordinates&);

};

#endif
