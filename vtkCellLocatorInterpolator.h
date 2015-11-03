/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCellLocatorInterpolator.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//  Copyright 2014-2016 Etienne Tang
#ifndef VTKCOMPUTEVARIABLES_H
#define VTKCOMPUTEVARIABLES_H

#include "vtkSetGet.h"
#include "vtkStdString.h"

#include "vtkPointSetAlgorithm.h"

class vtkInformation;
class vtkInformationVector;
class vtkDataSetAttributes;
class vtkPointSet;
class vtkCellLocator;


class VTK_EXPORT vtkCellLocatorInterpolator : public vtkPointSetAlgorithm
{
public:
    static vtkCellLocatorInterpolator* New();
    vtkTypeMacro(vtkCellLocatorInterpolator, vtkPointSetAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

protected:
    vtkCellLocatorInterpolator();
    ~vtkCellLocatorInterpolator();

    vtkCellLocator* loc;

    virtual int RequestData(vtkInformation* request, vtkInformationVector** inVector, vtkInformationVector* outVector);

private:
    vtkCellLocatorInterpolator operator=(const vtkCellLocatorInterpolator&);
    vtkCellLocatorInterpolator(const vtkCellLocatorInterpolator&);

};

#endif
