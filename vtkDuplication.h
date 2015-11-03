/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkDuplication.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//  Copyright 2014-2016 Etienne Tang
#ifndef VTKDUPLICATION_H
#define VTKDUPLICATION_H

#include "vtkSetGet.h"
#include "vtkStdString.h"

#include "vtkMultiBlockDataSetAlgorithm.h"

class vtkInformation;
class vtkInformationVector;
class vtkDataSetAttributes;
class vtkPointSet;
class vtkTransform;


class VTK_EXPORT vtkDuplication : public vtkMultiBlockDataSetAlgorithm
{
public:
    static vtkDuplication* New();
    vtkTypeMacro(vtkDuplication, vtkMultiBlockDataSetAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

	vtkGetMacro(Nblades, double)
	vtkSetMacro(Nblades, double)
	vtkGetMacro(Imin, int)
	vtkSetMacro(Imin, int)
	vtkGetMacro(Imax, int)
	vtkSetMacro(Imax, int)
	
protected:
    vtkDuplication();
    ~vtkDuplication();
    
    virtual int FillInputPortInformation(int port, vtkInformation* info);
    virtual int RequestData(vtkInformation* request, vtkInformationVector** inVector, vtkInformationVector* outVector);

	double Nblades;
	int Imin;
	int Imax;
	vtkTransform* Transform;
	
	void RotateVectors(vtkDataSetAttributes *data);
	
private:
    vtkDuplication operator=(const vtkDuplication&);
    vtkDuplication(const vtkDuplication&);

};

#endif
