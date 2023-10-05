SamacSys ECAD Model
15166958/1231078/2.50/8/3/MOSFET P-Channel

DESIGNSPARK_INTERMEDIATE_ASCII

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "r152.5_70"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 0.7) (shapeHeight 1.525))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(textStyleDef "Default"
		(font
			(fontType Stroke)
			(fontFace "Helvetica")
			(fontHeight 50 mils)
			(strokeWidth 5 mils)
		)
	)
	(patternDef "SOIC127P600X175-8N" (originalName "SOIC127P600X175-8N")
		(multiLayer
			(pad (padNum 1) (padStyleRef r152.5_70) (pt -2.712, 1.905) (rotation 90))
			(pad (padNum 2) (padStyleRef r152.5_70) (pt -2.712, 0.635) (rotation 90))
			(pad (padNum 3) (padStyleRef r152.5_70) (pt -2.712, -0.635) (rotation 90))
			(pad (padNum 4) (padStyleRef r152.5_70) (pt -2.712, -1.905) (rotation 90))
			(pad (padNum 5) (padStyleRef r152.5_70) (pt 2.712, -1.905) (rotation 90))
			(pad (padNum 6) (padStyleRef r152.5_70) (pt 2.712, -0.635) (rotation 90))
			(pad (padNum 7) (padStyleRef r152.5_70) (pt 2.712, 0.635) (rotation 90))
			(pad (padNum 8) (padStyleRef r152.5_70) (pt 2.712, 1.905) (rotation 90))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt 0, 0) (textStyleRef "Default") (isVisible True))
		)
		(layerContents (layerNumRef 30)
			(line (pt -3.725 2.75) (pt 3.725 2.75) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt 3.725 2.75) (pt 3.725 -2.75) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt 3.725 -2.75) (pt -3.725 -2.75) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt -3.725 -2.75) (pt -3.725 2.75) (width 0.05))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.95 2.45) (pt 1.95 2.45) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt 1.95 2.45) (pt 1.95 -2.45) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt 1.95 -2.45) (pt -1.95 -2.45) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.95 -2.45) (pt -1.95 2.45) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.95 1.18) (pt -0.68 2.45) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -1.6 2.45) (pt 1.6 2.45) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt 1.6 2.45) (pt 1.6 -2.45) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt 1.6 -2.45) (pt -1.6 -2.45) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -1.6 -2.45) (pt -1.6 2.45) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -3.475 2.605) (pt -1.95 2.605) (width 0.2))
		)
	)
	(symbolDef "SI4435DY" (originalName "SI4435DY")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -25 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 2) (pt 0 mils -100 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -125 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 3) (pt 0 mils -200 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -225 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 4) (pt 0 mils -300 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 230 mils -325 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 5) (pt 1000 mils 0 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 770 mils -25 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(pin (pinNum 6) (pt 1000 mils -100 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 770 mils -125 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(pin (pinNum 7) (pt 1000 mils -200 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 770 mils -225 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(pin (pinNum 8) (pt 1000 mils -300 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName true)) (pinName (text (pt 770 mils -325 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(line (pt 200 mils 100 mils) (pt 800 mils 100 mils) (width 6 mils))
		(line (pt 800 mils 100 mils) (pt 800 mils -400 mils) (width 6 mils))
		(line (pt 800 mils -400 mils) (pt 200 mils -400 mils) (width 6 mils))
		(line (pt 200 mils -400 mils) (pt 200 mils 100 mils) (width 6 mils))
		(attr "RefDes" "RefDes" (pt 850 mils 300 mils) (justify Left) (isVisible True) (textStyleRef "Default"))

	)
	(compDef "SI4435DY" (originalName "SI4435DY") (compHeader (numPins 8) (numParts 1) (refDesPrefix Q)
		)
		(compPin "1" (pinName "S_1") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "2" (pinName "S_2") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "3" (pinName "S_3") (partNum 1) (symPinNum 3) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "4" (pinName "G") (partNum 1) (symPinNum 4) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "8" (pinName "D_4") (partNum 1) (symPinNum 5) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "7" (pinName "D_3") (partNum 1) (symPinNum 6) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "6" (pinName "D_2") (partNum 1) (symPinNum 7) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "5" (pinName "D_1") (partNum 1) (symPinNum 8) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "SI4435DY"))
		(attachedPattern (patternNum 1) (patternName "SOIC127P600X175-8N")
			(numPads 8)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
				(padNum 3) (compPinRef "3")
				(padNum 4) (compPinRef "4")
				(padNum 5) (compPinRef "5")
				(padNum 6) (compPinRef "6")
				(padNum 7) (compPinRef "7")
				(padNum 8) (compPinRef "8")
			)
		)
		(attr "TME Electronic Components Part Number" "")
		(attr "TME Electronic Components Price/Stock" "")
		(attr "Manufacturer_Name" "onsemi")
		(attr "Manufacturer_Part_Number" "SI4435DY")
		(attr "Description" "MOSFET P-Ch 30V 8.8A PowerTrench SOIC8")
		(attr "Datasheet Link" "https://www.onsemi.com/pdf/datasheet/si4435dy-d.pdf")
		(attr "Height" "1.75 mm")
	)

)
