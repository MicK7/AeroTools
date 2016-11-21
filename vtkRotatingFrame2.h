/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkRotatingFrame2.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//  Copyright 2014-2016 Etienne Tang
#ifndef VTKROTATINGFRAME_H
#define VTKROTATINGFRAME_H

#include "vtkSetGet.h"
#include "vtkStdString.h"

#include "vtkPointSetAlgorithm.h"

class vtkInformation;
class vtkInformationVector;
class vtkDataSetAttributes;
class vtkPointSet;


class VTK_EXPORT vtkRotatingFrame2 : public vtkPointSetAlgorithm
{
public:
    static vtkRotatingFrame2* New();
    vtkTypeMacro(vtkRotatingFrame2, vtkPointSetAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

	vtkGetMacro(Omega, double)
	vtkSetMacro(Omega, double)
	vtkGetMacro(RelativeToAbsolute, int)
	vtkSetMacro(RelativeToAbsolute, int)
	
	vtkGetMacro(Cv, double)
	vtkSetMacro(Cv, double)
	vtkGetMacro(Gamma, double)
	vtkSetMacro(Gamma, double)
	
	void SetPostfix(const char* post) { Postfix = post; }
	const char* GetPostfix() { return Postfix.c_str(); }

protected:
    vtkRotatingFrame2();
    ~vtkRotatingFrame2();
    
    std::string NewName(vtkPointSet* data, const char* name);

    virtual int RequestData(vtkInformation* request, vtkInformationVector** inVector, vtkInformationVector* outVector);

	double Omega;
	int RelativeToAbsolute;
	vtkStdString Postfix;
	
	double Cv;
	double Gamma;
	
private:
    vtkRotatingFrame2 operator=(const vtkRotatingFrame2&);
    vtkRotatingFrame2(const vtkRotatingFrame2&);

};

#endif
