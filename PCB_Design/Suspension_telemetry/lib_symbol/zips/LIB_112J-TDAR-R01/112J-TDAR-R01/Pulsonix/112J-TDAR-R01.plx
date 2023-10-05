PULSONIX_LIBRARY_ASCII "SamacSys ECAD Model"
//12773305/1231078/2.50/13/4/Connector

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "r160_70"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 0.700) (shapeHeight 1.600))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(padStyleDef "r140_120"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 1.200) (shapeHeight 1.400))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(padStyleDef "c50_h100"
		(holeDiam 1)
		(padShape (layerNumRef 1) (padShapeType Ellipse)  (shapeWidth 0.500) (shapeHeight 0.500))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0.500) (shapeHeight 0.500))
	)
	(padStyleDef "r160_140"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 1.400) (shapeHeight 1.600))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(padStyleDef "r220_120"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 1.200) (shapeHeight 2.200))
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
	(patternDef "112JTDARR01" (originalName "112JTDARR01")
		(multiLayer
			(pad (padNum 1) (padStyleRef r160_70) (pt 2.250, 7.250) (rotation 0))
			(pad (padNum 2) (padStyleRef r160_70) (pt 1.150, 7.250) (rotation 0))
			(pad (padNum 3) (padStyleRef r160_70) (pt 0.050, 7.250) (rotation 0))
			(pad (padNum 4) (padStyleRef r160_70) (pt -1.050, 7.250) (rotation 0))
			(pad (padNum 5) (padStyleRef r160_70) (pt -2.150, 7.250) (rotation 0))
			(pad (padNum 6) (padStyleRef r160_70) (pt -3.250, 7.250) (rotation 0))
			(pad (padNum 7) (padStyleRef r160_70) (pt -4.350, 7.250) (rotation 0))
			(pad (padNum 8) (padStyleRef r160_70) (pt -5.450, 7.250) (rotation 0))
			(pad (padNum 9) (padStyleRef r140_120) (pt -7.750, 6.750) (rotation 0))
			(pad (padNum 10) (padStyleRef r160_70) (pt -6.550, 7.250) (rotation 0))
			(pad (padNum 11) (padStyleRef c50_h100) (pt 3.050, -3.250) (rotation 90))
			(pad (padNum 12) (padStyleRef c50_h100) (pt -4.950, -3.250) (rotation 90))
			(pad (padNum 13) (padStyleRef r160_140) (pt 6.850, 6.750) (rotation 90))
			(pad (padNum 14) (padStyleRef r220_120) (pt 7.750, -2.850) (rotation 0))
			(pad (padNum 15) (padStyleRef r220_120) (pt -7.750, -2.850) (rotation 0))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt -0.000, 0.925) (textStyleRef "Normal") (isVisible True))
		)
		(layerContents (layerNumRef 28)
			(line (pt -7.35 -7.25) (pt 7.35 -7.25) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 7.35 -7.25) (pt 7.35 7.25) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 7.35 7.25) (pt -7.35 7.25) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt -7.35 7.25) (pt -7.35 -7.25) (width 0.025))
		)
		(layerContents (layerNumRef 18)
			(line (pt 3.25 7.25) (pt 5.5 7.25) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 5.5 7.25) (pt 5.5 7.25) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 5.5 7.25) (pt 3.25 7.25) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 3.25 7.25) (pt 3.25 7.25) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 7.35 5.5) (pt 7.35 5.5) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 7.35 5.5) (pt 7.35 -1.25) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 7.35 -1.25) (pt 7.35 -1.25) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 7.35 -1.25) (pt 7.35 5.5) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 7.35 -4.5) (pt 7.35 -4.5) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 7.35 -4.5) (pt 7.35 -7.25) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 7.35 -7.25) (pt 7.35 -7.25) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 7.35 -7.25) (pt 7.35 -4.5) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 7.35 -7.25) (pt -7.35 -7.25) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -7.35 -7.25) (pt -7.35 -7.25) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -7.35 -7.25) (pt 7.35 -7.25) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 7.35 -7.25) (pt 7.35 -7.25) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -7.35 -7.25) (pt -7.35 -7.25) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -7.35 -7.25) (pt -7.35 -4.5) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -7.35 -4.5) (pt -7.35 -4.5) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -7.35 -4.5) (pt -7.35 -7.25) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -7.35 -1.25) (pt -7.35 -1.25) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -7.35 -1.25) (pt -7.35 5.5) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -7.35 5.5) (pt -7.35 5.5) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -7.35 5.5) (pt -7.35 -1.25) (width 0.1))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -9.35 10.1) (pt 9.35 10.1) (width 0.1))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 9.35 10.1) (pt 9.35 -8.25) (width 0.1))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 9.35 -8.25) (pt -9.35 -8.25) (width 0.1))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -9.35 -8.25) (pt -9.35 10.1) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 2.25 9.1) (pt 2.25 9.1) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(arc (pt 2.25, 9.05) (radius 0.05) (startAngle 90.0) (sweepAngle 180.0) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt 2.25 9) (pt 2.25 9) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(arc (pt 2.25, 9.05) (radius 0.05) (startAngle 270) (sweepAngle 180.0) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt 2.25 9.1) (pt 2.25 9.1) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(arc (pt 2.25, 9.05) (radius 0.05) (startAngle 90.0) (sweepAngle 180.0) (width 0.2))
		)
	)
	(symbolDef "112J-TDAR-R01" (originalName "112J-TDAR-R01")

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
		(pin (pinNum 6) (pt 0 mils -500 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -525 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 7) (pt 0 mils -600 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -625 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 8) (pt 1800 mils 0 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1570 mils -25 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(pin (pinNum 9) (pt 1800 mils -100 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1570 mils -125 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(pin (pinNum 10) (pt 1800 mils -200 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1570 mils -225 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(pin (pinNum 11) (pt 1800 mils -300 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1570 mils -325 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(pin (pinNum 12) (pt 1800 mils -400 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1570 mils -425 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(pin (pinNum 13) (pt 1800 mils -500 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 1570 mils -525 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(line (pt 200 mils 100 mils) (pt 1600 mils 100 mils) (width 6 mils))
		(line (pt 1600 mils 100 mils) (pt 1600 mils -700 mils) (width 6 mils))
		(line (pt 1600 mils -700 mils) (pt 200 mils -700 mils) (width 6 mils))
		(line (pt 200 mils -700 mils) (pt 200 mils 100 mils) (width 6 mils))
		(attr "RefDes" "RefDes" (pt 1650 mils 300 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))
		(attr "Type" "Type" (pt 1650 mils 200 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))

	)
	(compDef "112J-TDAR-R01" (originalName "112J-TDAR-R01") (compHeader (numPins 13) (numParts 1) (refDesPrefix J)
		)
		(compPin "1" (pinName "DAT2") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "2" (pinName "CD/DAT3") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "3" (pinName "CMD") (partNum 1) (symPinNum 3) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "4" (pinName "VDD") (partNum 1) (symPinNum 4) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "5" (pinName "CLK") (partNum 1) (symPinNum 5) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "6" (pinName "VSS") (partNum 1) (symPinNum 6) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "7" (pinName "DAT0") (partNum 1) (symPinNum 7) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "8" (pinName "DAT1") (partNum 1) (symPinNum 8) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "G1" (pinName "GROUND") (partNum 1) (symPinNum 9) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "CD1" (pinName "CARD DETECTION") (partNum 1) (symPinNum 10) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "MP1" (pinName "MP1") (partNum 1) (symPinNum 11) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "MP2" (pinName "MP2") (partNum 1) (symPinNum 12) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "MP3" (pinName "MP3") (partNum 1) (symPinNum 13) (gateEq 0) (pinEq 0) (pinType Unknown))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "112J-TDAR-R01"))
		(attachedPattern (patternNum 1) (patternName "112JTDARR01")
			(numPads 13)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
				(padNum 3) (compPinRef "3")
				(padNum 4) (compPinRef "4")
				(padNum 5) (compPinRef "5")
				(padNum 6) (compPinRef "6")
				(padNum 7) (compPinRef "7")
				(padNum 8) (compPinRef "8")
				(padNum 9) (compPinRef "G1")
				(padNum 10) (compPinRef "CD1")
				(padNum 11) (compPinRef "MP1")
				(padNum 12) (compPinRef "MP2")
				(padNum 13) (compPinRef "MP3")
			)
		)
		(attr "TME Electronic Components Part Number" "")
		(attr "TME Electronic Components Price/Stock" "")
		(attr "Manufacturer_Name" "ATTEND")
		(attr "Manufacturer_Part_Number" "112J-TDAR-R01")
		(attr "Description" "Connector: for cards; SD Micro; push-push; SMT; gold-plated")
		(attr "<Hyperlink>" "https://cdn.sos.sk/productdata/2c/8b/de324bd3/112j-tdar-r01.pdf")
		(attr "<Component Height>" "2")
		(attr "<STEP Filename>" "112J-TDAR-R01.stp")
		(attr "<STEP Offsets>" "X=0;Y=0;Z=0")
		(attr "<STEP Rotation>" "X=0;Y=0;Z=0")
	)

)
