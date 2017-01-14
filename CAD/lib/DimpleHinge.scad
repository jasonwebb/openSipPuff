// --------------- male side of hinge ---------------
union()
{
	cube([2,4,6]);
	translate([0,2,6]) rotate([0,90,0]) cylinder(2,2,2,$fs=.25);
	translate([1.9,2,6]) sphere(1.3,$fs=.25);	

	translate([20,0,0]) mirror([1, 0, 0 ])
	{ 
	cube([2,4,6]);
	translate([0,2,6]) rotate([0,90,0]) cylinder(2,2,2,$fs=.25);
	translate([1.9,2,6]) sphere(1.3,$fs=.25);	
	}
	translate([0,0,0]) cube([20,4,2]);
}

// ---------------  female side of hinge ---------------
translate([25,0,0]) union()
{
	difference()
	{
		union()
		{
			translate([2.2,0,0]) cube([2,4,6]);
			translate([2.2,2,6]) rotate([0,90,0]) cylinder(2,2,2,$fs=.25);
		}
		translate([1.9,2,6]) sphere(1.6,$fs=.25);
	}	
	translate([19.6,0,0]) mirror([1, 0, 0 ])
	difference()
	{
		union()
		{
			translate([2.2,0,0]) cube([2,4,6]);
			translate([2.2,2,6]) rotate([0,90,0]) cylinder(2,2,2,$fs=.25);	
		}	
		translate([1.9,2,6]) sphere(1.6,$fs=.25);
	}	
	translate([2.2,0,0]) cube([15.1,4,2]);
}
