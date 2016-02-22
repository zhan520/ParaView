/*=========================================================================

   Program: ParaView
   Module:    pqResetScalarRangeReaction.h

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2.

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

========================================================================*/
#ifndef pqResetScalarRangeReaction_h
#define pqResetScalarRangeReaction_h

#include "pqReaction.h"
#include <QPointer>

class pqPipelineRepresentation;
class pqDataRepresentation;

/// @ingroup Reactions
/// Reaction to reset the active lookup table's range to match the active
/// representation. You can disable tracking of the active representation,
/// instead explicitly provide one using setRepresentation() by pass
/// track_active_objects as false to the constructor.
class PQAPPLICATIONCOMPONENTS_EXPORT pqResetScalarRangeReaction : public pqReaction
{
  Q_OBJECT
  typedef pqReaction Superclass;
public:
  enum Modes
    {
    DATA,
    CUSTOM,
    TEMPORAL
    };

  /// if \c track_active_objects is false, then the reaction will not track
  /// pqActiveObjects automatically.
  pqResetScalarRangeReaction(QAction* parent, bool track_active_objects=true , Modes mode=DATA);


  /// @deprecated Use resetScalarRangeToData().
  static void resetScalarRange(pqPipelineRepresentation* repr=NULL)
    { pqResetScalarRangeReaction::resetScalarRangeToData(repr); }

  /// Reset to current data range.
  static bool resetScalarRangeToData(pqPipelineRepresentation* repr=NULL);

  /// Reset range to a custom range.
  static bool resetScalarRangeToCustom(pqPipelineRepresentation* repr=NULL);

  /// Reset range to data range over time.
  static bool resetScalarRangeToDataOverTime(pqPipelineRepresentation* repr=NULL);

public slots:
  /// Updates the enabled state. Applications need not explicitly call
  /// this.
  void updateEnableState();

  /// Set the data representation explicitly when track_active_objects is false.
  void setRepresentation(pqDataRepresentation* repr);

protected:
  /// Called when the action is triggered.
  virtual void onTriggered();

private:
  Q_DISABLE_COPY(pqResetScalarRangeReaction);
  QPointer<pqPipelineRepresentation> Representation;
  Modes Mode;
};

#endif
