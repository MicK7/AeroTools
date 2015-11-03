/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCellLocatorInterpolator.cpp

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//  Copyright 2014-2016 Etienne Tang
#include "vtkCellLocatorInterpolator.h"

#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPointSet.h"
#include "vtkDoubleArray.h"
#include "vtkPointData.h"
#include "vtkMath.h"
#include "vtkCellLocator.h"
#include "vtkGenericCell.h"

vtkStandardNewMacro(vtkCellLocatorInterpolator)

vtkCellLocatorInterpolator::vtkCellLocatorInterpolator()
{
    this->SetNumberOfInputPorts(2); // Input 0 : source geometrique. Input 1 : donnees dans lesquelles interpoler
    this->SetNumberOfOutputPorts(1);
    
    this->loc = vtkCellLocator::New();
}

vtkCellLocatorInterpolator::~vtkCellLocatorInterpolator()
{
    this->loc->Delete();
}

void vtkCellLocatorInterpolator::PrintSelf(ostream& os, vtkIndent indent)
{
    os << "vtkCellLocatorInterpolator\n";
}

int vtkCellLocatorInterpolator::RequestData(vtkInformation* request,
	vtkInformationVector** inVector, vtkInformationVector* outVector)
{
	vtkInformation* inInfo0 = inVector[0]->GetInformationObject(0);
	if(!inInfo0)
	{
		return 0;
	}
	vtkPointSet* inData0 = vtkPointSet::SafeDownCast(
		inInfo0->Get(vtkDataObject::DATA_OBJECT()));
	if(!inData0)
	{
		return 0;
	}
    vtkInformation* inInfo1 = inVector[1]->GetInformationObject(0);
    if(!inInfo1)
    {
        return 0;
    }
    vtkPointSet* inData1 = vtkPointSet::SafeDownCast(
        inInfo1->Get(vtkDataObject::DATA_OBJECT()));
    if(!inData1)
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
    
    outData->Initialize();
    outData->CopyStructure(inData0);
    vtkPoints* pts = outData->GetPoints();
    int nPts = outData->GetNumberOfPoints();
    
    
    
    int nAr = inData1->GetPointData()->GetNumberOfArrays();
    std::vector<vtkDoubleArray*> newArs(nAr);
    std::vector<vtkDataArray*> oldArs(nAr);
    for (int i = 0; i < nAr; i++)
    {
        oldArs[i] = inData1->GetPointData()->GetArray(i);
        newArs[i] = vtkDoubleArray::New();
        newArs[i]->SetName(inData1->GetPointData()->GetArrayName(i));
        newArs[i]->SetNumberOfComponents(inData1->GetPointData()->GetArray(i)->GetNumberOfComponents());
        newArs[i]->SetNumberOfTuples(nPts);
    }
    
    
    loc->SetDataSet(inData1);
    loc->BuildLocator();
    
    vtkGenericCell* genCell = vtkGenericCell::New();
    vtkIdType cellId;
    int subId;
    double dist2;
    double closestPt[3];
    double pt[3];
    double pCoords[3];
    std::vector<double> weights;
    
	for (int i =0; i < nPts; i++)
    {
        pts->GetPoint(i, pt);
        loc->FindClosestPoint(pt, closestPt, genCell, cellId, subId, dist2);
        if(!genCell)
        {
            return 0;
        }
        int nCellPts = genCell->GetNumberOfPoints();
        if(nCellPts > weights.size())
        {
            weights.resize(nCellPts);
        }
        genCell->EvaluatePosition(closestPt, NULL, subId, pCoords, dist2, &weights[0]);
        for (int j = 0; j < nAr; j++)
        {
            newArs[j]->InterpolateTuple(i, genCell->GetPointIds(), oldArs[j], &weights[0]);
        }
    }
    
    for (int i = 0; i < nAr; i++)
    {
        outData->GetPointData()->AddArray(newArs[i]);
        newArs[i]->Delete();
    }
        
    genCell->Delete();

	return 1;
}

