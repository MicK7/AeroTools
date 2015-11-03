/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkComputeVariables2.cpp

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//  Copyright 2014-2016 Etienne Tang

#include "vtkComputeVariables2.h"

#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPointSet.h"
#include "vtkDoubleArray.h"
#include "vtkPointData.h"
#include "vtkMath.h"

#include <algorithm>

vtkStandardNewMacro(vtkComputeVariables2)

vtkComputeVariables2::vtkComputeVariables2()
{
    this->Cv = 947.2046;
    this->Gamma = 1.303;

    this->rhoName = "Density";
    this->rhovName = "Momentum";
    this->rhoeName = "EnergyStagnationDensity";
    this->rhokName = "rok";
}

vtkComputeVariables2::~vtkComputeVariables2()
{

}

void vtkComputeVariables2::PrintSelf(ostream& os, vtkIndent indent)
{
    os << "vtkComputeVariables2\n";
}

#define SUTH_CONST 110.4
#define SUTH_MUREF 1.717e-05
#define SUTH_TREF 273.0

int vtkComputeVariables2::RequestData(vtkInformation* request,
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
	vtkDataArray* rho = outData->GetPointData()->GetArray(this->rhoName);
	vtkDataArray* rhov = outData->GetPointData()->GetArray(this->rhovName);
	vtkDataArray* rhoe = outData->GetPointData()->GetArray(this->rhoeName);
    vtkDataArray* rhok = outData->GetPointData()->GetArray(this->rhokName);
	if((!rho) || (!rhov) || (!rhoe) || (!rhok) ||
		(rho->GetNumberOfComponents() != 1) ||
		(rhov->GetNumberOfComponents() != 3) ||
		(rhoe->GetNumberOfComponents() != 1) ||
        (rhok->GetNumberOfComponents() != 1))
	{
		return 0;
	}
	
	vtkDoubleArray* v = vtkDoubleArray::New();
	vtkDoubleArray* ps = vtkDoubleArray::New();
	vtkDoubleArray* ts = vtkDoubleArray::New();
	vtkDoubleArray* pt = vtkDoubleArray::New();
	vtkDoubleArray* tt = vtkDoubleArray::New();
	vtkDoubleArray* s = vtkDoubleArray::New();
	vtkDoubleArray* M = vtkDoubleArray::New();
	vtkDoubleArray* mu = vtkDoubleArray::New();
	
	int npts = outData->GetNumberOfPoints();
	
	v->SetNumberOfComponents(3);
	ps->SetNumberOfComponents(1);
	ts->SetNumberOfComponents(1);
	pt->SetNumberOfComponents(1);
	tt->SetNumberOfComponents(1);
	s->SetNumberOfComponents(1);
	M->SetNumberOfComponents(1);
	mu->SetNumberOfComponents(1);
	
	v->SetNumberOfTuples(npts);
	ps->SetNumberOfTuples(npts);
	ts->SetNumberOfTuples(npts);
	pt->SetNumberOfTuples(npts);
	tt->SetNumberOfTuples(npts);
	s->SetNumberOfTuples(npts);
	M->SetNumberOfTuples(npts);
	mu->SetNumberOfTuples(npts);
	
	double rgaz = Cv*(Gamma-1.);
	double cp = Cv*Gamma;
	
	for(int i = 0; i < npts; i++)
	{
		double tmp_rho;
		double tmp_rhov[3];
		double tmp_rhoe;
		double tmp_rhok;
        
        rho->GetTuple(i, &tmp_rho);
		rhov->GetTuple(i, tmp_rhov);
		rhoe->GetTuple(i, &tmp_rhoe);
		rhok->GetTuple(i, &tmp_rhok);
        
        // Dans les extractions elsa v3.4-p1, les valeurs de rhok aux parois sont fausses...
        //if(tmp_rhok < 0.)
        //{
        //    tmp_rhok = 0.;
        //}
        
		double tmp_v[3];
		double tmp_ps, tmp_ts, tmp_pt, tmp_tt, tmp_s, tmp_M, tmp_mu, tmp_k;
		
		tmp_v[0] = tmp_rhov[0]/tmp_rho;
		tmp_v[1] = tmp_rhov[1]/tmp_rho;
		tmp_v[2] = tmp_rhov[2]/tmp_rho;
		
		double magV = vtkMath::Norm(tmp_v);
		tmp_k = tmp_rhok/tmp_rho;
        
		tmp_ts = (1./this->Cv)*((tmp_rhoe/tmp_rho)-0.5*pow(magV,2.) - tmp_k);
		tmp_ps = tmp_rho*rgaz*tmp_ts;
		tmp_s = cp*log(tmp_ts/288.15)-rgaz*log(tmp_ps/101325.0);
		tmp_M = magV/sqrt(Gamma*rgaz*tmp_ts);
		//tmp_tt = tmp_ts*(1.+0.5*(Gamma-1.)*pow(tmp_M, 2.));
		//tmp_pt = tmp_ps*pow(1.+0.5*(Gamma-1.)*pow(tmp_M, 2.), Gamma/(Gamma-1.));
        tmp_tt = tmp_ts + 0.5*magV*magV/cp + tmp_k/cp;
        tmp_pt = tmp_ps*pow(tmp_tt/tmp_ts, Gamma/(Gamma-1.));
		tmp_mu = SUTH_MUREF*sqrt(tmp_ts/SUTH_TREF)*(1.+SUTH_CONST/SUTH_TREF)/(1.+SUTH_CONST/tmp_ts);
		
		v->SetTuple(i, tmp_v);
		ps->SetTuple(i, &tmp_ps);
		ts->SetTuple(i, &tmp_ts);
		pt->SetTuple(i, &tmp_pt);
		tt->SetTuple(i, &tmp_tt);
		s->SetTuple(i, &tmp_s);
		M->SetTuple(i, &tmp_M);
		mu->SetTuple(i, &tmp_mu);
	}
	
	v->SetName("V");
	ps->SetName("Ps");
	ts->SetName("Ts");
	pt->SetName("Pt");
	tt->SetName("Tt");
	s->SetName("S");
	M->SetName("M");
	mu->SetName("mu");
	
	RemoveVar(outData, "V");
	RemoveVar(outData, "Ps");
	RemoveVar(outData, "Ts");
	RemoveVar(outData, "Pt");
	RemoveVar(outData, "Tt");
	RemoveVar(outData, "S");
	RemoveVar(outData, "M");
	RemoveVar(outData, "mu");
	
	outData->GetPointData()->AddArray(v);
	outData->GetPointData()->AddArray(ps);
	outData->GetPointData()->AddArray(ts);
	outData->GetPointData()->AddArray(pt);
	outData->GetPointData()->AddArray(tt);
	outData->GetPointData()->AddArray(s);
	outData->GetPointData()->AddArray(M);
	outData->GetPointData()->AddArray(mu);
	
	v->Delete();
	ps->Delete();
	ts->Delete();
	pt->Delete();
	tt->Delete();
	s->Delete();
	M->Delete();
	mu->Delete();
	
	return 1;
}

void vtkComputeVariables2::RemoveVar(vtkPointSet* data, const char* name)
{
	if(data->GetPointData()->HasArray(name))
	{
		data->GetPointData()->RemoveArray(name);
	}
}
