include <lib/boxes.scad>
include <lib/units.scad>
include <lib/nuts_and_bolts.scad>

// PCB parameters
boardWidth = 1.19*inch;
boardHeight = 2.31*inch;
boardThickness = 1.4;

// Sensor parameters
sensorOverhang = .2*inch + .1*inch;

// Standoff parameters
nutSize = 4;

// Misc parameters
margin = 4;			// Thickness of walls
fit = 1.5;			// Distance between PCB and housing walls
halfSpacing = 21;	// Distance between both halves when rendered side by side
ledgeOffset = 0.1;	// Offset between mating ledges of each half; adjust for tighter fit

// Housing parameters
housingWidth = boardWidth + margin*2 + fit*2;
housingHeight = boardHeight + margin*2 + sensorOverhang + fit*2;
housingThickness = 1*inch;

// Mouse ear parameters
mouseEarRadius = 10;
mouseEarHeight = .6;

// Rendering parameters
showLowerHousing = true;
showUpperHousing = true;
showMouseEars = true;
showPCB = false;
showMX12GP = false;
showUSBPort = false;

// Colors
pcbColor = [0,1,1];
sensorColor = [1,0,1];
usbColor = [1,0,0];
lowerColor = [1,.5,.2];
upperColor = [.5,1,.3];

/***************************************************
 Lower housing
 - Contains mounting hardware (M4 nuts) for PCB
****************************************************/
module lowerHousing() {
	// Carve out nut holes from shell with standoff bridges
	difference() {

		// Combine shell with bridges for standoffs
		union() {

			// Create a hollow shell =============================
			difference() {
				// Positive shell
				translate([housingWidth/2, housingHeight/2, housingThickness/2])
					roundedBox([housingWidth, housingHeight, housingThickness], 6, false);
		
				// Inner cavity
				translate([housingWidth/2, housingHeight/2, housingThickness/2])
					roundedBox([housingWidth - margin*2 + fit*2, housingHeight - margin*2, housingThickness - margin], 5, false);
		
				// Cut the top off
				translate([housingWidth/2, housingHeight/2, housingThickness])
					cube([housingWidth + margin, housingHeight + margin, housingThickness], center = true);

				// Port for MX12GP
				translate([margin + fit + .595*inch - .185*inch/2 - 1, margin + sensorOverhang - .425*inch/2, margin + 1/16*inch + boardThickness + .2*inch + 2])
					rotate([90,0,0])
						cylinder(r = .2*inch, h = .425*inch);

				// USB port
				translate([housingWidth/2, housingHeight, boardThickness + margin + 1/16*inch + 2])
					cube([14,20,11], center = true);
			}
	
			// STANDOFF BRIDGES ====================================
			// Lower standoff bridge
			translate([housingWidth/2, margin + sensorOverhang + .14*inch, 1/16*inch/2 + margin - 1])
				cube([housingWidth - margin, 11, 1/16*inch + 2], center = true);

			// Upper standoff bridge
			translate([housingWidth/2, margin + sensorOverhang + 2.165*inch, 1/16*inch/2 + margin - 1])
				cube([housingWidth - margin, 11, 1/16*inch + 2], center = true);

			// LEDGES (positive) =========================================
			// Left ledge
			translate([margin/2, housingHeight/2, housingThickness/2 + 1])
				cube([margin/4, boardHeight - fit, 2], center = true);

			// Right ledge
			translate([housingWidth - margin/2, housingHeight/2, housingThickness/2 + 1])
				cube([margin/4, boardHeight - fit, 2], center = true);

			// MOUSE EARS ===========================================
			if(showMouseEars) {
				// Bottom left
				translate([2, 2, 0])
					cylinder(r = mouseEarRadius, h = mouseEarHeight);
				
				// Bottom right
				translate([housingWidth - 2, 2, 0])
					cylinder(r = mouseEarRadius, h = mouseEarHeight);

				// Middle left
				translate([0, housingHeight/2, 0])
					cylinder(r = mouseEarRadius, h = mouseEarHeight);

				// Middle right
				translate([housingWidth, housingHeight/2, 0])
					cylinder(r = mouseEarRadius, h = mouseEarHeight);				

				// Top left
				translate([2, housingHeight - 2, 0])
					cylinder(r = mouseEarRadius, h = mouseEarHeight);
				
				// Top right
				translate([housingWidth - 2, housingHeight - 2, 0])
					cylinder(r = mouseEarRadius, h = mouseEarHeight);			
			}
		}

		// NUT HOLES =========================================
		// Bottom-left nut hole
		translate([.14*inch + margin +fit, .14*inch + sensorOverhang + margin, margin - .06*inch])
			rotate([0,0,30])
				nutHole(4);	

		// Bottom-right nut hole
		translate([1.05*inch + margin + fit, .14*inch + sensorOverhang + margin, margin - .06*inch])
			rotate([0,0,30])
				nutHole(4);

		// Top-left nut hole
		translate([.14*inch + margin + fit, 2.165*inch + sensorOverhang + margin, margin - .06*inch])
			rotate([0,0,30])
				nutHole(4);	

		// Top-right nut hole
		translate([1.05*inch + margin + fit, 2.165*inch + sensorOverhang + margin, margin - .06*inch])
			rotate([0,0,30])
				nutHole(4);
	}	
}

/**************************************************
 Upper housing
**************************************************/
module upperHousing() {
	// Rotate and translate upper half to sit next to lower half
	rotate([0,180,0]) {
		translate([-housingWidth*2 - halfSpacing,0,-housingThickness]) {

		// Add mouse ears
		union() {

			// Create the hollow shell
			difference() {
				// Positive shell
				translate([housingWidth/2, housingHeight/2, housingThickness/2])
					roundedBox([housingWidth, housingHeight, housingThickness], 6, false);
		
				// Inner cavity
				translate([housingWidth/2, housingHeight/2, housingThickness/2])
					roundedBox([housingWidth - margin*2 + fit*2, housingHeight - margin*2, housingThickness - margin], 5, false);
		
				// Cut the bottom off
				translate([housingWidth/2, housingHeight/2, 0])
					cube([housingWidth + margin, housingHeight + margin, housingThickness], center = true);

				// Port for MX12GP
				translate([margin + fit + .595*inch - .185*inch/2 - 1, margin + sensorOverhang - .425*inch/2, margin + 1/16*inch + boardThickness + .2*inch + 2])
					rotate([90,0,0])
						cylinder(r = .2*inch, h = .425*inch);

				// USB port
				translate([housingWidth/2, housingHeight, boardThickness + margin + 1/16*inch + 2])
					cube([14,20,11], center = true);

				// LEDGES (negative) =========================================
				// Left ledge
				translate([margin/2 + ledgeOffset, housingHeight/2, housingThickness/2 + 1])
					cube([margin/4, boardHeight, 2], center = true);
	
				// Right ledge
				translate([housingWidth - margin/2 - ledgeOffset, housingHeight/2, housingThickness/2 + 1])
					cube([margin/4, boardHeight, 2], center = true);
			}

			// MOUSE EARS ===========================================
			if(showMouseEars) {
				// Bottom left
				translate([2, 2, housingThickness - .6])
					cylinder(r = 10, h = .6);
				
				// Bottom right
				translate([housingWidth - 2, 2, housingThickness - .6])
					cylinder(r = 10, h = .6);
				
				// Middle left
				translate([0, housingHeight/2, housingThickness - .6])
					cylinder(r = mouseEarRadius, h = mouseEarHeight);

				// Middle right
				translate([housingWidth, housingHeight/2, housingThickness - .6])
					cylinder(r = mouseEarRadius, h = mouseEarHeight);	

				// Top left
				translate([2, housingHeight - 2, housingThickness - .6])
					cylinder(r = 10, h = .6);

				// Top right
				translate([housingWidth - 2, housingHeight - 2, housingThickness - .6])
					cylinder(r = 10, h = .6);
			}

		}
		}
	}
}

/*********************************
 Circuit board
*********************************/
module pcb() {
	translate([margin + fit, sensorOverhang + margin, margin + 1/16*inch])
		linear_extrude(height = boardThickness)
			import("Board outlines/Main-Board.dxf");
}

/*******************************
 MX12GP pressure transducer
*******************************/
module mx12gp() {
	// Port
	translate([margin + fit + .595*inch - .185*inch/2, margin + sensorOverhang - .425*inch/2, margin + 1/16*inch + boardThickness + .185*inch])
		rotate([90,0,0])
			cylinder(r = .0925*inch, h = .425*inch);
	
	// Main body
	translate([.595*inch + margin + fit, .145*inch + sensorOverhang + margin, boardThickness + margin + 1/16*inch])
		rotate([0,0,180])
			linear_extrude(height = .325*inch)
				import("Board outlines/MX12GP.dxf");
}

/********************
 USB port
********************/
module usbPort() {
	translate([housingWidth/2, sensorOverhang + boardHeight, margin + 1/16*inch + boardThickness + 1.975])
		cube([7.7, 9, 3.95], center = true);
}

/****************************************
 Render all of the modules
*****************************************/
if(showLowerHousing)	color(lowerColor)	lowerHousing();		// Lower housing
if(showUpperHousing)	color(upperColor)	upperHousing();		// Upper housing
if(showPCB)				color(pcbColor)		pcb();				// PCB
if(showMX12GP)			color(sensorColor)	mx12gp();			// MX12GP
if(showUSBPort)			color(usbColor)		usbPort();			// USB port