/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkMakeVector.cpp

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//  Copyright 2014-2016 Etienne Tang
#include "vtkMakeVector.h"

#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPointSet.h"
#include "vtkDoubleArray.h"
#include "vtkPointData.h"
#include "vtkMath.h"

vtkStandardNewMacro(vtkMakeVector)

vtkMakeVector::vtkMakeVector()
{
    this->xComponentName = "";
    this->yComponentName = "";
    this->zComponentName = "";
    this->vectorName = "";
}

vtkMakeVector::~vtkMakeVector()
{

}

void vtkMakeVector::PrintSelf(ostream& os, vtkIndent indent)
{
    os << "vtkMakeVector\n";
}

int vtkMakeVector::RequestData(vtkInformation* request,
	vtkInformationVector** inVector, vtkInformationVector* outVector)
{
	vtkInformation* inInfo = inVector[0]->GetInformationObject(0);
	if(!inInfo)
	{
		return 0;
	}
	vtkPointSet* inData = vtkPointSet::SafeDownCast(
		inInfo->Get(vtkDataObject::DATA_OBJECT()));
	if(!inData)
	{
		return 0;
	}
	
	vtkInformation* outInfo = outVector->GetInformationObject(0);
	vtkPointSet* outData = vtkPointSet::SafeDownCast(
		outInfo->Get(vtkDataObject::DATA_OBJECT()));
	if(!outData)
	{
		return 0;
	}
	
	outData->ShallowCopy(inData);
	vtkDataArray* xComp = outData->GetPointData()->GetArray(this->xComponentName);
	vtkDataArray* yComp = outData->GetPointData()->GetArray(this->yComponentName);
	vtkDataArray* zComp = outData->GetPointData()->GetArray(this->zComponentName);
	if((!xComp) || (!yComp) || (!zComp) ||
		(xComp->GetNumberOfComponents() != 1) ||
		(yComp->GetNumberOfComponents() != 1) ||
		(zComp->GetNumberOfComponents() != 1))
	{
		return 0;
	}
	
    vtkDoubleArray* vec = vtkDoubleArray::New();
	int npts = outData->GetNumberOfPoints();
    vec->SetNumberOfComponents(3);
    vec->SetNumberOfTuples(npts);
    double cur_tuple[3];
	for(int i = 0; i < npts; i++)
	{
        xComp->GetTuple(i, &(cur_tuple[0]));
        yComp->GetTuple(i, &(cur_tuple[1]));
        zComp->GetTuple(i, &(cur_tuple[2]));
        vec->SetTuple(i, cur_tuple);
	}
	vec->SetName(this->vectorName);
	
	RemoveVar(outData, this->vectorName);
	outData->GetPointData()->AddArray(vec);
    vec->Delete();

	return 1;
}

void vtkMakeVector::RemoveVar(vtkPointSet* data, const char* name)
{
	if(data->GetPointData()->HasArray(name))
	{
		data->GetPointData()->RemoveArray(name);
	}
}
