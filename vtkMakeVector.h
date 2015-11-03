/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkMakeVector.h

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


class VTK_EXPORT vtkMakeVector : public vtkPointSetAlgorithm
{
public:
    static vtkMakeVector* New();
    vtkTypeMacro(vtkMakeVector, vtkPointSetAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

    void SetXComponentName(int pipo1, int pipo2, int pipo3, int pipo4, const char* name) { this->xComponentName = vtkStdString(name); }
    vtkStdString GetXComponentName() { return this->xComponentName; }

    void SetYComponentName(int pipo1, int pipo2, int pipo3, int pipo4, const char* name) { this->yComponentName = vtkStdString(name); }
    vtkStdString GetYComponentName() { return this->yComponentName; }

    void SetZComponentName(int pipo1, int pipo2, int pipo3, int pipo4, const char* name) { this->zComponentName = vtkStdString(name); }
    vtkStdString GetZComponentName() { return this->zComponentName; }
    
    void SetVectorName(const char* name) { this->vectorName = vtkStdString(name); }
    vtkStdString GetVectorName() { return this->vectorName; }

protected:
    vtkMakeVector();
    ~vtkMakeVector();

    virtual int RequestData(vtkInformation* request, vtkInformationVector** inVector, vtkInformationVector* outVector);
	
	void RemoveVar(vtkPointSet* data, const char* name);

    vtkStdString xComponentName;
    vtkStdString yComponentName;
    vtkStdString zComponentName;
    vtkStdString vectorName;

private:
    vtkMakeVector operator=(const vtkMakeVector&);
    vtkMakeVector(const vtkMakeVector&);

};

#endif
