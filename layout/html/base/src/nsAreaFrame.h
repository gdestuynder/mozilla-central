/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */
#ifndef nsAreaFrame_h___
#define nsAreaFrame_h___

#include "nsBlockFrame.h"
#include "nsISpaceManager.h"
#include "nsVoidArray.h"

class nsSpaceManager;

struct nsStyleDisplay;
struct nsStylePosition;

/**
 * Child list name indices
 * @see #GetAdditionalChildListName()
 */
#define NS_AREA_FRAME_ABSOLUTE_LIST_INDEX   (NS_BLOCK_FRAME_LAST_LIST_INDEX + 1)

/**
 * The area frame has an additional named child list:
 * - "Absolute-list" which contains the absolutely positioned frames
 *
 * @see nsLayoutAtoms::absoluteList
 */
class nsAreaFrame : public nsBlockFrame
{
public:
  friend nsresult NS_NewAreaFrame(nsIFrame*& aResult, PRUint32 aFlags);
  
  // nsIFrame
  NS_IMETHOD DeleteFrame(nsIPresContext& aPresContext);

  NS_IMETHOD SetInitialChildList(nsIPresContext& aPresContext,
                                 nsIAtom*        aListName,
                                 nsIFrame*       aChildList);

  NS_IMETHOD GetAdditionalChildListName(PRInt32   aIndex,
                                        nsIAtom*& aListName) const;

  NS_IMETHOD FirstChild(nsIAtom* aListName, nsIFrame*& aFirstChild) const;

  NS_IMETHOD Reflow(nsIPresContext&          aPresContext,
                    nsHTMLReflowMetrics&     aDesiredSize,
                    const nsHTMLReflowState& aReflowState,
                    nsReflowStatus&          aStatus);

#ifdef NS_DEBUG
  NS_IMETHOD Paint(nsIPresContext&      aPresContext,
                   nsIRenderingContext& aRenderingContext,
                   const nsRect&        aDirtyRect,
                   nsFramePaintLayer    aWhichLayer);
#endif

  NS_IMETHOD CreateContinuingFrame(nsIPresContext&  aPresContext,
                                   nsIFrame*        aParent,
                                   nsIStyleContext* aStyleContext,
                                   nsIFrame*&       aContinuingFrame);

  NS_IMETHOD List(FILE* out, PRInt32 aIndent, nsIListFilter* aFilter) const;
  NS_IMETHOD GetFrameName(nsString& aResult) const;

protected:
  nsAreaFrame();
  virtual ~nsAreaFrame();

  void ReflowAbsoluteItems(nsIPresContext& aPresContext,
                           const nsHTMLReflowState& aReflowState);

  void TranslatePoint(nsIFrame* aFrameFrom, nsPoint& aPoint) const;

  void ComputeAbsoluteFrameBounds(nsIPresContext&          aPresContext,
                                  nsIFrame*                aFrame,
                                  const nsHTMLReflowState& aState,
                                  const nsStylePosition*   aPosition,
                                  nsRect&                  aRect) const;

  void AddAbsoluteFrame(nsIFrame* aFrame);

private:
  nsSpaceManager* mSpaceManager;
  nsFrameList     mAbsoluteFrames;  // additional named child list

#ifdef NS_DEBUG
  struct BandData : public nsBandData {
    // Trapezoids used during band processing
    nsBandTrapezoid data[12];

    // Bounding rect of available space between any left and right floaters
    nsRect          availSpace;

    BandData() {
      size = 12;
      trapezoids = data;
    }

    /**
     * Computes the bounding rect of the available space, i.e. space
     * between any left and right floaters Uses the current trapezoid
     * data, see nsISpaceManager::GetBandData(). Also updates member
     * data "availSpace".
     */
    void ComputeAvailSpaceRect();
  };
#endif
};

#endif /* nsAreaFrame_h___ */
