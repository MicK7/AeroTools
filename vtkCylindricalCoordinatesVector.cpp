/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCylindricalCoordinatesVector.cpp

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//  Copyright 2014-2016 Etienne Tang
#include "vtkCylindricalCoordinatesVector.h"

#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPointSet.h"
#include "vtkPoints.h"
#include "vtkDoubleArray.h"
#include "vtkPointData.h"
#include "vtkMath.h"

#include <iostream>

vtkStandardNewMacro(vtkCylindricalCoordinatesVector)

vtkCylindricalCoordinatesVector::vtkCylindricalCoordinatesVector()
{
}

vtkCylindricalCoordinatesVector::~vtkCylindricalCoordinatesVector()
{

}

void vtkCylindricalCoordinatesVector::PrintSelf(ostream& os, vtkIndent indent)
{
    os << "vtkCylindricalCoordinatesVector\n";
}

int vtkCylindricalCoordinatesVector::RequestData(vtkInformation* request,
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
	
	vtkDataArray* vec = outData->GetPointData()->GetArray(this->Name);
	if((!vec) || (vec->GetNumberOfComponents() != 3))
	{
		std::cout << "Vecteur non trouve" << std::endl;
		return 0;
	}
	
	vtkPoints* pts = outData->GetPoints();
	
	vtkDoubleArray* vecR = vtkDoubleArray::New();
	vtkDoubleArray* vecTheta = vtkDoubleArray::New();
	
	int npts = vec->GetNumberOfTuples();
	
	vecR->SetNumberOfComponents(1);
	vecTheta->SetNumberOfComponents(1);
	
	vecR->SetNumberOfTuples(npts);
	vecTheta->SetNumberOfTuples(npts);
	
	for(int i = 0; i < npts; i++)
	{
		double coords[3];
		pts->GetPoint(i, coords);
		
		double theta = atan2(coords[2], coords[1]);
		double cost = cos(theta);
		double sint = sin(theta);
		
		double tmp_vec[3];
		vec->GetTuple(i, tmp_vec);
		
		double tmp_vecR = cost*tmp_vec[1]+sint*tmp_vec[2];
		double tmp_vecTheta = -sint*tmp_vec[1]+cost*tmp_vec[2];
		
		vecR->SetTuple(i, &tmp_vecR);
		vecTheta->SetTuple(i, &tmp_vecTheta);
	}
	
	std::string name_R = Name + "_r";
	std::string name_theta = Name + "_theta";
	while(outData->GetPointData()->HasArray(name_R.c_str()))
	{
		name_R = name_R + "_";
	}
	while(outData->GetPointData()->HasArray(name_theta.c_str()))
	{
		name_theta = name_theta + "_";
	}
	
	vecR->SetName(name_R.c_str());
	vecTheta->SetName(name_theta.c_str());
	
	outData->GetPointData()->AddArray(vecR);
	outData->GetPointData()->AddArray(vecTheta);

	vecR->Delete();
	vecTheta->Delete();
	
	return 1;
}
