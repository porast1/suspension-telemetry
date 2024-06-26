PULSONIX_LIBRARY_ASCII "SamacSys ECAD Model"
//267787/1231078/2.50/2/2/Crystal or Oscillator

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "r190_110"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 1.100) (shapeHeight 1.900))
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
	(patternDef "ABS07" (originalName "ABS07")
		(multiLayer
			(pad (padNum 1) (padStyleRef r190_110) (pt -1.250, 0.000) (rotation 0))
			(pad (padNum 2) (padStyleRef r190_110) (pt 1.250, 0.000) (rotation 0))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt 0.000, 0.000) (textStyleRef "Normal") (isVisible True))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.6 0.75) (pt 1.6 0.75) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 1.6 0.75) (pt 1.6 -0.75) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 1.6 -0.75) (pt -1.6 -0.75) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.6 -0.75) (pt -1.6 0.75) (width 0.025))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -2.8 1.95) (pt 2.8 1.95) (width 0.1))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 2.8 1.95) (pt 2.8 -1.95) (width 0.1))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 2.8 -1.95) (pt -2.8 -1.95) (width 0.1))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -2.8 -1.95) (pt -2.8 1.95) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -0.4 0.75) (pt 0.4 0.75) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -0.4 -0.75) (pt 0.4 -0.75) (width 0.1))
		)
	)
	(symbolDef "ABS07-32.768KHZ-9-T" (originalName "ABS07-32.768KHZ-9-T")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 100 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 140 mils -15 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 2) (pt 400 mils 0 mils) (rotation 180) (pinLength 100 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 260 mils -15 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(line (pt 120 mils 100 mils) (pt 120 mils -100 mils) (width 6 mils))
		(line (pt 280 mils 100 mils) (pt 280 mils -100 mils) (width 6 mils))
		(line (pt 160 mils 140 mils) (pt 160 mils -140 mils) (width 6 mils))
		(line (pt 160 mils -140 mils) (pt 240 mils -140 mils) (width 6 mils))
		(line (pt 240 mils -140 mils) (pt 240 mils 140 mils) (width 6 mils))
		(line (pt 240 mils 140 mils) (pt 160 mils 140 mils) (width 6 mils))
		(line (pt 280 mils 0 mils) (pt 300 mils 0 mils) (width 6 mils))
		(line (pt 120 mils 0 mils) (pt 100 mils 0 mils) (width 6 mils))
		(attr "RefDes" "RefDes" (pt 350 mils 250 mils) (justify 24) (isVisible True) (textStyleRef "Normal"))
		(attr "Type" "Type" (pt 350 mils 150 mils) (justify 24) (isVisible True) (textStyleRef "Normal"))

	)
	(compDef "ABS07-32.768KHZ-9-T" (originalName "ABS07-32.768KHZ-9-T") (compHeader (numPins 2) (numParts 1) (refDesPrefix Y)
		)
		(compPin "1" (pinName "1") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "2" (pinName "2") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Unknown))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "ABS07-32.768KHZ-9-T"))
		(attachedPattern (patternNum 1) (patternName "ABS07")
			(numPads 2)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
			)
		)
		(attr "TME Electronic Components Part Number" "")
		(attr "TME Electronic Components Price/Stock" "")
		(attr "Manufacturer_Name" "ABRACON")
		(attr "Manufacturer_Part_Number" "ABS07-32.768KHZ-9-T")
		(attr "Description" "32.768 kHz +/-20ppm Crystal 9pF 70 kOhms 2-SMD, No Lead")
		(attr "<Hyperlink>" "https://abracon.com/Resonators/ABS07.pdf")
		(attr "<Component Height>" "0.9")
		(attr "<STEP Filename>" "ABS07-32.768KHZ-9-T.stp")
		(attr "<STEP Offsets>" "X=0;Y=0;Z=0")
		(attr "<STEP Rotation>" "X=90;Y=0;Z=0")
	)

)
