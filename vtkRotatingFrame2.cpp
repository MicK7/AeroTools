/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkRotatingFrame2.cpp

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//  Copyright 2014-2016 Etienne Tang
#include "vtkRotatingFrame2.h"

#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPointSet.h"
#include "vtkPoints.h"
#include "vtkDoubleArray.h"
#include "vtkPointData.h"
#include "vtkMath.h"

#include <iostream>

vtkStandardNewMacro(vtkRotatingFrame2)

vtkRotatingFrame2::vtkRotatingFrame2()
{
	Omega = 5518.312216;
	RelativeToAbsolute = 1;
	
	Cv = 947.2046;
	Gamma = 1.303;
	
	Postfix = "";
}

vtkRotatingFrame2::~vtkRotatingFrame2()
{

}

void vtkRotatingFrame2::PrintSelf(ostream& os, vtkIndent indent)
{
    os << "vtkRotatingFrame2\n";
}

int vtkRotatingFrame2::RequestData(vtkInformation* request,
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
	
	vtkDataArray* V = outData->GetPointData()->GetArray("V");
	vtkDataArray* Ps = outData->GetPointData()->GetArray("Ps");
	vtkDataArray* Ts = outData->GetPointData()->GetArray("Ts");
    vtkDataArray* Tt = outData->GetPointData()->GetArray("Tt");
	
	if((!V) || (V->GetNumberOfComponents() != 3))
	{
		std::cout << "Vecteur V non trouve" << std::endl;
		return 0;
	}
	if((!Ps) || (Ps->GetNumberOfComponents() != 1))
	{
		std::cout << "Ps non trouve" << std::endl;
		return 0;
	}
	if((!Ts) || (Ts->GetNumberOfComponents() != 1))
	{
		std::cout << "Ts non trouve" << std::endl;
		return 0;
	}
    if((!Tt) || (Tt->GetNumberOfComponents() != 1))
    {
        std::cout << "Tt non trouve" << std::endl;
        return 0;
    }
	
	vtkPoints* pts = outData->GetPoints();
	
	int npts = pts->GetNumberOfPoints();
	
	vtkDoubleArray* newV = vtkDoubleArray::New();
	vtkDoubleArray* newPt = vtkDoubleArray::New();
	vtkDoubleArray* newTt = vtkDoubleArray::New();
	vtkDoubleArray* newM = vtkDoubleArray::New();
	
	newV->SetNumberOfComponents(3);
	newPt->SetNumberOfComponents(1);
	newTt->SetNumberOfComponents(1);
	newM->SetNumberOfComponents(1);
	
	newV->SetNumberOfTuples(npts);
	newPt->SetNumberOfTuples(npts);
	newTt->SetNumberOfTuples(npts);
	newM->SetNumberOfTuples(npts);
	
	double domega;
	if(RelativeToAbsolute)
	{
		domega = Omega;
	}
	else
	{
		domega = -Omega;
	}
	
	double rgaz = Cv*(Gamma-1);
    double cp = Cv*Gamma;
	
	for(int i = 0; i < npts; i++)
	{
		double coords[3];
		pts->GetPoint(i, coords);
		
		double theta = atan2(coords[2], coords[1]);
		double R = sqrt(coords[1]*coords[1]+coords[2]*coords[2]);
		
		double cost = cos(theta);
		double sint = sin(theta);
		
		double tmp_V[3];
		double tmp_Ts;
		double tmp_Ps;
        double tmp_Tt_old;
		V->GetTuple(i, tmp_V);
		Ts->GetTuple(i, &tmp_Ts);
		Ps->GetTuple(i, &tmp_Ps);
		Tt->GetTuple(i, &tmp_Tt_old);
        
		double tmp_Vr = cost*tmp_V[1]+sint*tmp_V[2];
		double tmp_Vt = -sint*tmp_V[1]+cost*tmp_V[2];
		
		tmp_Vt += R*domega;
		
		double tmp_newV[3];
		double tmp_newPt, tmp_newTt, tmp_newM;
		
		tmp_newV[0] = tmp_V[0];
		tmp_newV[1] = cost*tmp_Vr-sint*tmp_Vt;
		tmp_newV[2] = sint*tmp_Vr+cost*tmp_Vt;
		
		double magNewV = vtkMath::Norm(tmp_newV);
		double magOldV = vtkMath::Norm(tmp_V);
        /*
         * Calcul de Tt dans le nouveau repere :
         * On a Ttold = Ts+(1/cp)*0.5*magVold^2+k/cp
         * Ttnew = Ts+(1/cp)*0.5*magVnew^2+k/cp
         * donc Ttnew = Ttold + (1/cp)*0.5*(magVnew^2-magVold^2)
         */
        
		tmp_newM = magNewV/sqrt(Gamma*rgaz*tmp_Ts);
		tmp_newTt = tmp_Tt_old + (0.5/cp)*(magNewV*magNewV - magOldV*magOldV);
		tmp_newPt = tmp_Ps*pow(tmp_newTt/tmp_Ts, Gamma/(Gamma-1.));
		
		newV->SetTuple(i, tmp_newV);
		newM->SetTuple(i, &tmp_newM);
		newTt->SetTuple(i, &tmp_newTt);
		newPt->SetTuple(i, &tmp_newPt);
	}
	
	newV->SetName(NewName(outData, "V").c_str());
	newM->SetName(NewName(outData, "M").c_str());
	newTt->SetName(NewName(outData, "Tt").c_str());
	newPt->SetName(NewName(outData, "Pt").c_str());
	
	outData->GetPointData()->AddArray(newV);
	outData->GetPointData()->AddArray(newM);
	outData->GetPointData()->AddArray(newPt);
	outData->GetPointData()->AddArray(newTt);

	newV->Delete();
	newM->Delete();
	newTt->Delete();
	newPt->Delete();
	
	return 1;
}

std::string vtkRotatingFrame2::NewName(vtkPointSet* data, const char* name)
{
	std::string res = name;
	if(Postfix != "")
	{
		res = res + "_" + Postfix;
	}
	else
	{
		if(RelativeToAbsolute)
		{
			res += "_abs";
		}
		else
		{
			res += "_rel";
		}
		if(data->GetPointData()->HasArray(res.c_str()))
		{
			data->GetPointData()->RemoveArray(res.c_str());
		}
	}
	return res;
}
