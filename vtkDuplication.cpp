/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkDuplication.cpp

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//  Copyright 2014-2016 Etienne Tang
#include "vtkDuplication.h"

#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPointSet.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkPoints.h"
#include "vtkDoubleArray.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkMath.h"
#include "vtkTransform.h"

#include <iostream>
#include <list>
#include <vector>

vtkStandardNewMacro(vtkDuplication)

vtkDuplication::vtkDuplication()
{
	Nblades = 1.;
	Imin = 0;
	Imax = 0;
	
	Transform = vtkTransform::New();
}

vtkDuplication::~vtkDuplication()
{
	Transform->Delete();
}

void vtkDuplication::PrintSelf(ostream& os, vtkIndent indent)
{
    os << "vtkDuplication\n";
}

int vtkDuplication::FillInputPortInformation(int port, vtkInformation* info)
{
	info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPointSet");
	return 1;
}

int vtkDuplication::RequestData(vtkInformation* request,
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
	vtkMultiBlockDataSet* outData = vtkMultiBlockDataSet::SafeDownCast(
		outInfo->Get(vtkDataObject::DATA_OBJECT()));
	if(!outData)
	{
		return 0;
	}
	
	double dtheta = 360./Nblades;
	int nDups = Imax-Imin+1;
	if(nDups <= 0)
	{
		std::cout << "Erreur : Imax > Imin" << std::endl;
		return 0;
	}
	outData->SetNumberOfBlocks(nDups);
	for(int i = Imin; i <= Imax; i++)
	{
		vtkPointSet* curDup = inData->NewInstance();
		curDup->ShallowCopy(inData);
		if(i != 0)
		{
			Transform->Identity();
			Transform->RotateX(double(i)*dtheta);
			vtkPoints* newPts = vtkPoints::New();
			Transform->TransformPoints(inData->GetPoints(), newPts);
			curDup->SetPoints(newPts);
			newPts->Delete();
			RotateVectors(curDup->GetPointData());
			RotateVectors(curDup->GetCellData());
		}
		outData->SetBlock(i-Imin, curDup);
		curDup->Delete();
	}
	
	return 1;
}

void vtkDuplication::RotateVectors(vtkDataSetAttributes *data)
{
    // Apply the current transform to either a vtkPointData or a vtkCellData
    // First step: detect which of the variables are vectors
    std::list<std::string> names;
    for(int i = 0, n = data->GetNumberOfArrays(); i < n; i++)
    {
        vtkDataArray* ar = data->GetArray(i);
        if(ar->GetNumberOfComponents() == 3)
        {
            names.push_back(std::string(ar->GetName()));
        }
    }
    // Apply the rotation to the detected vectors
    for(auto name: names)
    {
        vtkDataArray* oldAr = data->GetArray(name.c_str());
        vtkDataArray* newAr = oldAr->NewInstance();
        newAr->SetNumberOfComponents(3);
        this->Transform->TransformVectors(oldAr, newAr);
        data->RemoveArray(name.c_str());
        newAr->SetName(name.c_str());
        data->AddArray(newAr);
        newAr->Delete();
    }
}
