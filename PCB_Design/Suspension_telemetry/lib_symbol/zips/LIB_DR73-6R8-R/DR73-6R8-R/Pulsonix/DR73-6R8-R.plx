PULSONIX_LIBRARY_ASCII "SamacSys ECAD Model"
//581849/1231078/2.50/2/0/Inductor

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "r350.52_226.06"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 2.2606) (shapeHeight 3.5052))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(textStyleDef "Normal"
		(font
			(fontType Stroke)
			(fontFace "Helvetica")
			(fontHeight 1.27)
			(strokeWidth 0.127)
		)
	)
	(patternDef "INDM7676X355N" (originalName "INDM7676X355N")
		(multiLayer
			(pad (padNum 1) (padStyleRef r350.52_226.06) (pt -2.8194, 0) (rotation 0))
			(pad (padNum 2) (padStyleRef r350.52_226.06) (pt 2.8194, 0) (rotation 0))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt -4.8514, 4.3434) (textStyleRef "Normal") (isVisible True))
		)
		(layerContents (layerNumRef 18)
			(line (pt 3.81 2.0828) (pt 3.81 3.81) (width 0.1524))
		)
		(layerContents (layerNumRef 18)
			(line (pt -3.81 -2.0828) (pt -3.81 -3.81) (width 0.1524))
		)
		(layerContents (layerNumRef 18)
			(line (pt -3.6068 2.0828) (pt -1.905 3.81) (width 0.1524))
		)
		(layerContents (layerNumRef 18)
			(line (pt -3.6068 -2.0828) (pt -1.905 -3.81) (width 0.1524))
		)
		(layerContents (layerNumRef 18)
			(line (pt -3.81 -3.81) (pt 3.81 -3.81) (width 0.1524))
		)
		(layerContents (layerNumRef 18)
			(line (pt 3.81 -3.81) (pt 3.81 -2.0828) (width 0.1524))
		)
		(layerContents (layerNumRef 18)
			(line (pt 3.81 3.81) (pt -3.81 3.81) (width 0.1524))
		)
		(layerContents (layerNumRef 18)
			(line (pt -3.81 3.81) (pt -3.81 2.0828) (width 0.1524))
		)
		(layerContents (layerNumRef 28)
			(line (pt -3.81 1.905) (pt -1.905 3.81) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt -3.81 -1.905) (pt -1.905 -3.81) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt -3.81 -3.81) (pt 3.81 -3.81) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 3.81 -3.81) (pt 3.81 3.81) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 3.81 3.81) (pt -3.81 3.81) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt -3.81 3.81) (pt -3.81 -3.81) (width 0.025))
		)
	)
	(symbolDef "DR73-6R8-R" (originalName "DR73-6R8-R")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 0 mils -35 mils) (rotation 0]) (justify "UpperLeft") (textStyleRef "Normal"))
		))
		(pin (pinNum 2) (pt 800 mils 0 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 800 mils -35 mils) (rotation 0]) (justify "UpperRight") (textStyleRef "Normal"))
		))
		(arc (pt 250 mils -2 mils) (radius 50 mils) (startAngle 177.7) (sweepAngle -175.4) (width 6 mils))
		(arc (pt 350 mils -2 mils) (radius 50 mils) (startAngle 177.7) (sweepAngle -175.4) (width 6 mils))
		(arc (pt 450 mils -2 mils) (radius 50 mils) (startAngle 177.7) (sweepAngle -175.4) (width 6 mils))
		(arc (pt 550 mils -2 mils) (radius 50 mils) (startAngle 177.7) (sweepAngle -175.4) (width 6 mils))
		(attr "RefDes" "RefDes" (pt 650 mils 250 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))
		(attr "Type" "Type" (pt 650 mils 150 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))

	)
	(compDef "DR73-6R8-R" (originalName "DR73-6R8-R") (compHeader (numPins 2) (numParts 1) (refDesPrefix L)
		)
		(compPin "1" (pinName "1") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "2" (pinName "2") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Unknown))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "DR73-6R8-R"))
		(attachedPattern (patternNum 1) (patternName "INDM7676X355N")
			(numPads 2)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
			)
		)
		(attr "TTi Part Number" "")
		(attr "TTi Price/Stock" "")
		(attr "Manufacturer_Name" "COILTRONICS")
		(attr "Manufacturer_Part_Number" "DR73-6R8-R")
		(attr "Description" "POWER INDUCTOR +/-20% 6.8H 2.55A")
		(attr "<Hyperlink>" "")
		(attr "<STEP Filename>" "DR73-6R8-R.stp")
		(attr "<STEP Offsets>" "X=0;Y=0;Z=0.1")
		(attr "<STEP Rotation>" "X=90;Y=0;Z=0")
	)

)
