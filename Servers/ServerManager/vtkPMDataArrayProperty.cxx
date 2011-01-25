/*=========================================================================

  Program:   ParaView
  Module:    $RCSfile$

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkPMDataArrayProperty.h"

#include "vtkArrayIterator.h"
#include "vtkDataArray.h"
#include "vtkDoubleArray.h"
#include "vtkFloatArray.h"
#include "vtkIdTypeArray.h"
#include "vtkIntArray.h"
#include "vtkObjectFactory.h"
#include "vtkPMProperty.h"
#include "vtkSMMessage.h"
#include "vtkStringArray.h"

vtkStandardNewMacro(vtkPMDataArrayProperty);
//----------------------------------------------------------------------------
vtkPMDataArrayProperty::vtkPMDataArrayProperty()
{
}

//----------------------------------------------------------------------------
vtkPMDataArrayProperty::~vtkPMDataArrayProperty()
{
}

//----------------------------------------------------------------------------
void vtkPMDataArrayProperty::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
//----------------------------------------------------------------------------
bool vtkPMDataArrayProperty::Pull(vtkSMMessage* msgToFill)
{
  if (!this->InformationOnly)
    {
    return false;
    }

  if (!this->GetCommand())
    {
    return false;
    }

  // Invoke property's method on the root node of the server
  vtkClientServerStream str;
  str << vtkClientServerStream::Invoke
    << this->GetVTKObject() << this->GetCommand()
    << vtkClientServerStream::End;

  this->ProcessMessage(str);

  // Get the result
  vtkAbstractArray* abstractArray = NULL;
  if (!this->GetLastResult().GetArgument(0, 0, (vtkObjectBase**)&abstractArray))
    {
    vtkErrorMacro( "Error getting return value of command: "
                   << this->GetCommand());
    return false;
    }
  vtkStringArray* stringArray = vtkStringArray::SafeDownCast(abstractArray);
  vtkDataArray* dataArray = vtkDataArray::SafeDownCast(abstractArray);

  // Create property and add it to the message
  ProxyState_Property *prop = msgToFill->AddExtension(ProxyState::property);
  prop->set_name(this->GetXMLName());
  Variant *var = prop->mutable_value();

  // If no values, then just return OK with an empty content
  if(!dataArray && !stringArray)
    {
    return true;
    }

  // Need to fill the property content with the proper type
  // Right now only those types are supported
  // - vtkDoubleArray
  // - vtkIntArray
  // - vtkStringArray
  vtkIdType numValues = abstractArray->GetNumberOfComponents()
                        * abstractArray->GetNumberOfTuples();
  if(dataArray)
    {
    vtkDoubleArray *dataDouble = NULL;
    vtkIntArray *dataInt = NULL;
    switch (dataArray->GetDataType())
      {
      case VTK_DOUBLE:
        var->set_type(Variant::FLOAT64);
        dataDouble = vtkDoubleArray::SafeDownCast(dataArray);
        for (vtkIdType cc=0; cc < numValues; cc++)
          {
          var->add_float64(dataDouble->GetValue(cc));
          }
        break;
      case VTK_INT:
        var->set_type(Variant::INT);
        dataInt = vtkIntArray::SafeDownCast(dataArray);
        for (vtkIdType cc=0; cc < numValues; cc++)
          {
          var->add_integer(dataInt->GetValue(cc));
          }
        break;
      default:
        vtkWarningMacro("The Pull method of vtkPMDataArrayProperty do not support "
                        << dataArray->GetDataTypeAsString() << " array type.");
        return false;
      }
    }
  else if(stringArray)
    {
    var->set_type(Variant::STRING);
    for (vtkIdType cc=0; cc < numValues; cc++)
      {
      var->add_txt(stringArray->GetValue(cc));
      }
    }
  return true;
}
