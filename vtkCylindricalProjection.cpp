/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCylindricalProjection.cpp

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//  Copyright 2014-2016 Etienne Tang
#include "vtkCylindricalProjection.h"

#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPointSet.h"
#include "vtkPoints.h"
#include "vtkDoubleArray.h"
#include "vtkPointData.h"
#include "vtkMath.h"

vtkStandardNewMacro(vtkCylindricalProjection)

vtkCylindricalProjection::vtkCylindricalProjection():
	Rref(0.089)
{
}

vtkCylindricalProjection::~vtkCylindricalProjection()
{

}

void vtkCylindricalProjection::PrintSelf(ostream& os, vtkIndent indent)
{
    os << "vtkCylindricalProjection\n";
}

int vtkCylindricalProjection::RequestData(vtkInformation* request,
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
	vtkPoints* pts = inData->GetPoints();
	int npts = pts->GetNumberOfPoints();
	
	vtkPoints* newPts = vtkPoints::New();
	newPts->SetNumberOfPoints(npts);
	
	for(int i = 0; i < npts; i++)
	{
		double curPt[3];
		double newPt[3];
		pts->GetPoint(i, curPt);
		newPt[2] = 0.0;
		//double r = sqrt(curPt[1]*curPt[1]+curPt[2]*curPt[2]);
		double theta = atan2(curPt[2], curPt[1]);
		newPt[0] = curPt[0];
		newPt[1] = -Rref*theta;
		newPts->SetPoint(i, newPt);
	}
	
	outData->SetPoints(newPts);
	newPts->Delete();
	
	return 1;
}
