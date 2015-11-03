/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkCylindricalProjection2.cpp

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//  Copyright 2014-2016 Etienne Tang
#include "vtkCylindricalProjection2.h"

#include "vtkAppendPolyData.h"
#include "vtkClipPolyData.h"
#include "vtkPlane.h"

#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPointSet.h"
#include "vtkPoints.h"
#include "vtkDoubleArray.h"
#include "vtkPointData.h"
#include "vtkMath.h"

vtkStandardNewMacro(vtkCylindricalProjection2)

vtkCylindricalProjection2::vtkCylindricalProjection2():
	RRef(0.089),
    ThetaRef(0.)
{
    AppendFilter = vtkAppendPolyData::New();
    ClipFilter1 = vtkClipPolyData::New();
    ClipFilter2 = vtkClipPolyData::New();
    CuttingPlane = vtkPlane::New();
    CuttingPlane->SetOrigin(0., 0., 0.);
    SetPlane();
    ClipFilter1->SetClipFunction(CuttingPlane);
    ClipFilter1->SetValue(0.);
    ClipFilter1->InsideOutOff();
    ClipFilter2->SetClipFunction(CuttingPlane);
    ClipFilter2->SetValue(0.);
    ClipFilter2->InsideOutOn();
}

vtkCylindricalProjection2::~vtkCylindricalProjection2()
{
    AppendFilter->Delete();
    ClipFilter1->Delete();
    ClipFilter2->Delete();
    CuttingPlane->Delete();
}

void vtkCylindricalProjection2::SetPlane()
{
    CuttingPlane->SetNormal(0., -sin(ThetaRef), cos(ThetaRef));
}

void vtkCylindricalProjection2::PointProjection(vtkPolyData* orig, vtkPoints* pts, double thetaCadran)
{
    /**
     * orig : vtkPolyData a projeter
     * pts : objet vtkPoints qui va contenir le resultat
     * thetaCadran : On utilise cette valeur de theta pour choisir le cadran dans lequel seront les points
     */
    double shift = vtkMath::Pi() - thetaCadran;
    int n = orig->GetNumberOfPoints();
    pts->SetNumberOfPoints(n);
    for(int i = 0; i < n; i++)
    {
        double curPt[3];
        orig->GetPoint(i, curPt);
        double theta = mod2pi(atan2(curPt[2], curPt[1]) + shift)-shift;
        pts->SetPoint(i, curPt[0], -RRef*theta, 0.);
    }
}

void vtkCylindricalProjection2::PrintSelf(ostream& os, vtkIndent indent)
{
    os << "vtkCylindricalProjection2\n";
}

int vtkCylindricalProjection2::RequestData(vtkInformation* request,
	vtkInformationVector** inVector, vtkInformationVector* outVector)
{
	vtkInformation* inInfo = inVector[0]->GetInformationObject(0);
	if(!inInfo)
	{
		return 0;
	}
	vtkPolyData* inData = vtkPolyData::SafeDownCast(
		inInfo->Get(vtkDataObject::DATA_OBJECT()));
	if(!inData)
	{
		return 0;
	}
	
	vtkInformation* outInfo = outVector->GetInformationObject(0);
	vtkPolyData* outData = vtkPolyData::SafeDownCast(
		outInfo->Get(vtkDataObject::DATA_OBJECT()));
	if(!outData)
	{
		return 0;
	}
    
    SetPlane();
    
    ClipFilter1->SetInputData(inData);
    ClipFilter1->Update();
    vtkPolyData* tmp1 = vtkPolyData::New();
    tmp1->ShallowCopy(ClipFilter1->GetOutput());
    vtkPoints* tmpPts1 = vtkPoints::New();
    PointProjection(tmp1, tmpPts1, ThetaRef + 0.5*vtkMath::Pi());
    tmp1->SetPoints(tmpPts1);
    tmpPts1->Delete();
    ClipFilter2->SetInputData(inData);
    ClipFilter2->Update();
    vtkPolyData* tmp2 = vtkPolyData::New();
    tmp2->ShallowCopy(ClipFilter2->GetOutput());
    vtkPoints* tmpPts2 = vtkPoints::New();
    PointProjection(tmp2, tmpPts2, ThetaRef + 1.5*vtkMath::Pi());
    tmp2->SetPoints(tmpPts2);
    tmpPts2->Delete();
    
    AppendFilter->RemoveAllInputs();
    AppendFilter->AddInputData(tmp1);
    AppendFilter->AddInputData(tmp2);
    AppendFilter->Update();
	
	outData->ShallowCopy(AppendFilter->GetOutput());
    
    AppendFilter->RemoveAllInputs();
    tmp1->Delete();
    tmp2->Delete();
    
	return 1;
}
