PULSONIX_LIBRARY_ASCII "SamacSys ECAD Model"
//159518/1231078/2.50/11/3/Integrated Circuit

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "r80_30"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 0.3) (shapeHeight 0.8))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(padStyleDef "r245_175"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 1.75) (shapeHeight 2.45))
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
	(patternDef "SON50P300X300X100-11N" (originalName "SON50P300X300X100-11N")
		(multiLayer
			(pad (padNum 1) (padStyleRef r80_30) (pt -1.5, 1) (rotation 90))
			(pad (padNum 2) (padStyleRef r80_30) (pt -1.5, 0.5) (rotation 90))
			(pad (padNum 3) (padStyleRef r80_30) (pt -1.5, 0) (rotation 90))
			(pad (padNum 4) (padStyleRef r80_30) (pt -1.5, -0.5) (rotation 90))
			(pad (padNum 5) (padStyleRef r80_30) (pt -1.5, -1) (rotation 90))
			(pad (padNum 6) (padStyleRef r80_30) (pt 1.5, -1) (rotation 90))
			(pad (padNum 7) (padStyleRef r80_30) (pt 1.5, -0.5) (rotation 90))
			(pad (padNum 8) (padStyleRef r80_30) (pt 1.5, 0) (rotation 90))
			(pad (padNum 9) (padStyleRef r80_30) (pt 1.5, 0.5) (rotation 90))
			(pad (padNum 10) (padStyleRef r80_30) (pt 1.5, 1) (rotation 90))
			(pad (padNum 11) (padStyleRef r245_175) (pt 0, 0) (rotation 0))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt 0, 0) (textStyleRef "Normal") (isVisible True))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -2.125 1.75) (pt 2.125 1.75) (width 0.05))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 2.125 1.75) (pt 2.125 -1.75) (width 0.05))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 2.125 -1.75) (pt -2.125 -1.75) (width 0.05))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -2.125 -1.75) (pt -2.125 1.75) (width 0.05))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.5 1.5) (pt 1.5 1.5) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 1.5 1.5) (pt 1.5 -1.5) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 1.5 -1.5) (pt -1.5 -1.5) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.5 -1.5) (pt -1.5 1.5) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.5 0.75) (pt -0.75 1.5) (width 0.025))
		)
		(layerContents (layerNumRef 18)
			(arc (pt -1.9, 1.65) (radius 0.125) (startAngle 0.0) (sweepAngle 0.0) (width 0.25))
		)
		(layerContents (layerNumRef 18)
			(arc (pt -1.9, 1.65) (radius 0.125) (startAngle 180.0) (sweepAngle 180.0) (width 0.25))
		)
	)
	(symbolDef "MCP73114-0NSI_MF" (originalName "MCP73114-0NSI_MF")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -25 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 2) (pt 0 mils -100 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -125 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 3) (pt 0 mils -200 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -225 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 4) (pt 0 mils -300 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -325 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 5) (pt 0 mils -400 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -425 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 6) (pt 700 mils -900 mils) (rotation 90) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 725 mils -670 mils) (rotation 90]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 7) (pt 1400 mils 0 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1170 mils -25 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(pin (pinNum 8) (pt 1400 mils -100 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1170 mils -125 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(pin (pinNum 9) (pt 1400 mils -200 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1170 mils -225 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(pin (pinNum 10) (pt 1400 mils -300 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1170 mils -325 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(pin (pinNum 11) (pt 1400 mils -400 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1170 mils -425 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(line (pt 200 mils 100 mils) (pt 1200 mils 100 mils) (width 6 mils))
		(line (pt 1200 mils 100 mils) (pt 1200 mils -700 mils) (width 6 mils))
		(line (pt 1200 mils -700 mils) (pt 200 mils -700 mils) (width 6 mils))
		(line (pt 200 mils -700 mils) (pt 200 mils 100 mils) (width 6 mils))
		(attr "RefDes" "RefDes" (pt 1250 mils 300 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))
		(attr "Type" "Type" (pt 1250 mils 200 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))

	)
	(compDef "MCP73114-0NSI_MF" (originalName "MCP73114-0NSI_MF") (compHeader (numPins 11) (numParts 1) (refDesPrefix IC)
		)
		(compPin "1" (pinName "VDD_1") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "2" (pinName "VDD_2") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "3" (pinName "VBAT_1") (partNum 1) (symPinNum 3) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "4" (pinName "VBAT_2") (partNum 1) (symPinNum 4) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "5" (pinName "NC_1") (partNum 1) (symPinNum 5) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "11" (pinName "EP") (partNum 1) (symPinNum 6) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "10" (pinName "PROG") (partNum 1) (symPinNum 7) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "9" (pinName "VSS_2") (partNum 1) (symPinNum 8) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "8" (pinName "VSS_1") (partNum 1) (symPinNum 9) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "7" (pinName "STAT") (partNum 1) (symPinNum 10) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "6" (pinName "NC_2") (partNum 1) (symPinNum 11) (gateEq 0) (pinEq 0) (pinType Unknown))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "MCP73114-0NSI_MF"))
		(attachedPattern (patternNum 1) (patternName "SON50P300X300X100-11N")
			(numPads 11)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
				(padNum 3) (compPinRef "3")
				(padNum 4) (compPinRef "4")
				(padNum 5) (compPinRef "5")
				(padNum 6) (compPinRef "6")
				(padNum 7) (compPinRef "7")
				(padNum 8) (compPinRef "8")
				(padNum 9) (compPinRef "9")
				(padNum 10) (compPinRef "10")
				(padNum 11) (compPinRef "11")
			)
		)
		(attr "TME Electronic Components Part Number" "")
		(attr "TME Electronic Components Price/Stock" "")
		(attr "Manufacturer_Name" "Microchip")
		(attr "Manufacturer_Part_Number" "MCP73114-0NSI/MF")
		(attr "Description" "Single cell, 5.8V OVP battery Charger")
		(attr "<Hyperlink>" "http://ww1.microchip.com/downloads/en/DeviceDoc/20002183E.pdf")
		(attr "<Component Height>" "1")
		(attr "<STEP Filename>" "MCP73114-0NSI_MF.stp")
		(attr "<STEP Offsets>" "X=0;Y=0;Z=0")
		(attr "<STEP Rotation>" "X=0;Y=0;Z=0")
	)

)