PULSONIX_LIBRARY_ASCII "SamacSys ECAD Model"
//191829/1231078/2.50/2/4/Switch

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "c186_h124"
		(holeDiam 1.24)
		(padShape (layerNumRef 1) (padShapeType Ellipse)  (shapeWidth 1.860) (shapeHeight 1.860))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 1.860) (shapeHeight 1.860))
	)
	(textStyleDef "Normal"
		(font
			(fontType Stroke)
			(fontFace "Helvetica")
			(fontHeight 1.27)
			(strokeWidth 0.127)
		)
	)
	(patternDef "18259651" (originalName "18259651")
		(multiLayer
			(pad (padNum 1) (padStyleRef c186_h124) (pt 0.000, 0.000) (rotation 90))
			(pad (padNum 2) (padStyleRef c186_h124) (pt 6.500, 0.000) (rotation 90))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt 3.250, 0.000) (textStyleRef "Normal") (isVisible True))
		)
		(layerContents (layerNumRef 28)
			(line (pt 0.25 1.75) (pt 6.25 1.75) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 6.25 1.75) (pt 6.25 -1.75) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 6.25 -1.75) (pt 0.25 -1.75) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 0.25 -1.75) (pt 0.25 1.75) (width 0.025))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -1.93 2.75) (pt 8.43 2.75) (width 0.1))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 8.43 2.75) (pt 8.43 -2.75) (width 0.1))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 8.43 -2.75) (pt -1.93 -2.75) (width 0.1))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -1.93 -2.75) (pt -1.93 2.75) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 0.25 1.2) (pt 0.25 1.75) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 0.25 1.75) (pt 6.25 1.75) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 6.25 1.75) (pt 6.25 1.2) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 0.25 -1.2) (pt 0.25 -1.75) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 0.25 -1.75) (pt 6.25 -1.75) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 6.25 -1.75) (pt 6.25 -1.2) (width 0.1))
		)
	)
	(symbolDef "1825965-1" (originalName "1825965-1")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -25 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 2) (pt 0 mils -100 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -125 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(line (pt 200 mils 100 mils) (pt 700 mils 100 mils) (width 6 mils))
		(line (pt 700 mils 100 mils) (pt 700 mils -200 mils) (width 6 mils))
		(line (pt 700 mils -200 mils) (pt 200 mils -200 mils) (width 6 mils))
		(line (pt 200 mils -200 mils) (pt 200 mils 100 mils) (width 6 mils))
		(attr "RefDes" "RefDes" (pt 750 mils 300 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))
		(attr "Type" "Type" (pt 750 mils 200 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))

	)
	(compDef "1825965-1" (originalName "1825965-1") (compHeader (numPins 2) (numParts 1) (refDesPrefix S)
		)
		(compPin "1" (pinName "NO") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "2" (pinName "COM") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Unknown))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "1825965-1"))
		(attachedPattern (patternNum 1) (patternName "18259651")
			(numPads 2)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
			)
		)
		(attr "TME Electronic Components Part Number" "")
		(attr "TME Electronic Components Price/Stock" "")
		(attr "Manufacturer_Name" "TE Connectivity")
		(attr "Manufacturer_Part_Number" "1825965-1")
		(attr "Description" "Switch Tactile N.O. SPST Rectangular Button PC Pins 0.05A 24VDC 20000Cycles 1.3N Thru-Hole Loose")
		(attr "<Hyperlink>" "https://www.arrow.com/en/products/1825965-1/te-connectivity")
		(attr "<Component Height>" "5")
		(attr "<STEP Filename>" "1825965-1.stp")
		(attr "<STEP Offsets>" "X=3.25;Y=0;Z=-0.1")
		(attr "<STEP Rotation>" "X=90;Y=0;Z=0")
	)

)
