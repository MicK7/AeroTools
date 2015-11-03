/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkComputeVariables.h

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


class VTK_EXPORT vtkComputeVariables : public vtkPointSetAlgorithm
{
public:
    static vtkComputeVariables* New();
    vtkTypeMacro(vtkComputeVariables, vtkPointSetAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

    vtkGetMacro(Gamma, double)
    vtkSetMacro(Gamma, double)
    vtkGetMacro(Cv, double)
    vtkSetMacro(Cv, double)

    void SetRhoName(int pipo1, int pipo2, int pipo3, int pipo4, const char* name) { this->rhoName = vtkStdString(name); }
    vtkStdString GetRhoName() { return this->rhoName; }

    void SetRhovName(int pipo1, int pipo2, int pipo3, int pipo4, const char* name) { this->rhovName = vtkStdString(name); }
    vtkStdString GetRhovName() { return this->rhovName; }

    void SetRhoeName(int pipo1, int pipo2, int pipo3, int pipo4, const char* name) { this->rhoeName = vtkStdString(name); }
    vtkStdString GetRhoeName() { return this->rhoeName; }

protected:
    vtkComputeVariables();
    ~vtkComputeVariables();

    virtual int RequestData(vtkInformation* request, vtkInformationVector** inVector, vtkInformationVector* outVector);
	
	void RemoveVar(vtkPointSet* data, const char* name);

    vtkStdString rhoName;
    vtkStdString rhovName;
    vtkStdString rhoeName;
    double Gamma;
    double Cv;

private:
    vtkComputeVariables operator=(const vtkComputeVariables&);
    vtkComputeVariables(const vtkComputeVariables&);

};

#endif
