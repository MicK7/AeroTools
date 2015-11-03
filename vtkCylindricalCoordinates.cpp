/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCylindricalCoordinates.cpp

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//  Copyright 2014-2016 Etienne Tang
#include "vtkCylindricalCoordinates.h"

#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPointSet.h"
#include "vtkPoints.h"
#include "vtkDoubleArray.h"
#include "vtkPointData.h"
#include "vtkMath.h"

vtkStandardNewMacro(vtkCylindricalCoordinates)

vtkCylindricalCoordinates::vtkCylindricalCoordinates()
{
}

vtkCylindricalCoordinates::~vtkCylindricalCoordinates()
{

}

void vtkCylindricalCoordinates::PrintSelf(ostream& os, vtkIndent indent)
{
    os << "vtkCylindricalCoordinates\n";
}

int vtkCylindricalCoordinates::RequestData(vtkInformation* request,
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
	vtkPoints* pts = outData->GetPoints();
	
	vtkDoubleArray* R = vtkDoubleArray::New();
	vtkDoubleArray* theta = vtkDoubleArray::New();
	
	int npts = outData->GetNumberOfPoints();
	
	R->SetNumberOfComponents(1);
	theta->SetNumberOfComponents(1);
	
	R->SetNumberOfTuples(npts);
	theta->SetNumberOfTuples(npts);
	
	for(int i = 0; i < npts; i++)
	{
		double coords[3];
		pts->GetPoint(i, coords);
		
		double tmp_R = sqrt(coords[1]*coords[1] + coords[2]*coords[2]);
		double tmp_theta = atan2(coords[2], coords[1]);
		
		R->SetTuple(i, &tmp_R);
		theta->SetTuple(i, &tmp_theta);
	}
	
	R->SetName("R");
	theta->SetName("theta");

	if(outData->GetPointData()->HasArray("R"))
	{
		outData->GetPointData()->RemoveArray("R");
	}
	if(outData->GetPointData()->HasArray("theta"))
	{
		outData->GetPointData()->RemoveArray("theta");
	}
	
	outData->GetPointData()->AddArray(R);
	outData->GetPointData()->AddArray(theta);

	R->Delete();
	theta->Delete();
	
	return 1;
}
