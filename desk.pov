global_settings {
  assumed_gamma 2.5
  max_trace_level 5
}

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"
#include "woods.inc"
#include "glass.inc"
#include "metals.inc"
#include "coolcat.inc"
#include "cattext.inc"

#declare This_Brass = texture { T_Brass_4B }


#declare
RedPencil = union {
   cylinder {0, y*30, 0.5
      finish {
         crand 0.05
         ambient 0.3
         diffuse 0.7
      }
      pigment { Red }
   }

   cylinder {0, y*32, 0.5
      finish {
         crand 0.05
         ambient 0.3
         diffuse 0.7
      }
      pigment { Tan }
   }
}


#declare GreenPencil =
union {
   cylinder {0, y*30, 0.5
      finish {
         crand 0.05
         ambient 0.3
         diffuse 0.7
      }
      pigment { Green }
   }

   cylinder {0, y*32, 0.5
      finish {
         crand 0.05
         ambient 0.3
         diffuse 0.7
      }
      pigment { Tan }
   }
}


#declare BluePencil =
union {
   cylinder {0, y*30, 0.5
      finish {
         crand 0.05
         ambient 0.3
         diffuse 0.7
      }
      pigment { Blue }
   
   }

   cylinder {0, y*32, 0.5
      finish {
         crand 0.05
         ambient 0.3
         diffuse 0.7
      }
      pigment { Tan }
   }
}

#declare Back_Wall =
plane { z, 200.0
      hollow on
      finish {
      crand 0.05
      ambient 0.3
      diffuse 0.7
   }
   pigment { LightGray }
}

#declare Ceiling =
plane { y,  500.0
   hollow on
   finish {
      ambient 0.3
      diffuse 0.7
   }
   pigment { White }
}


#declare Desk_Top =
box { <-125, -2, -100> <125, 2, 100>
   translate -20.0*y
   texture {
      pigment { P_WoodGrain6A color_map { M_Wood6A }}
      #if (clock=4)
          finish { reflection 0.2 }
      #end
      rotate y*90
      translate z*30
      rotate z*5
      scale 5
   }
}

#declare Blotter =
union {
   triangle {
      <0.0, 0.0, 0.0>
      <8.5, 0.0, 0.0>
      <0.0, 0.0, -11.0>
   }
   triangle {
      <0.0, 0.0, -11.0>
      <8.5, 0.0, -11.0>
      <8.5, 0.0, 0.0>
   }

   scale <4.0, 1.0, 4.0>
   rotate -30.0*y
   translate <50.0, -17.9999, -30.0>

   finish {
      crand 0.04
      ambient 0.15
      diffuse 0.5
   }
   pigment { colour red 0.5 green 0.5 blue 0.3 }
}

#declare Pencil_Holder =
union {
   intersection {
      object { Cylinder_Y scale <5.0, 1.0, 5.0> }
      object { Cylinder_Y scale <4.8, 1.0, 4.8> inverse }
      plane { y, 0.0 inverse }
      plane { y, 15.0 rotate -45*x }
      texture { This_Brass }
   }
   object {
      RedPencil
      rotate -2*z
      translate <1.0, 0.0, 1.0>
   }
   object {
      GreenPencil
      rotate 2.0*z
      translate <-1.0, 3.0, 0.0>
   }
   object {
      BluePencil
      rotate <-2.0, 0.0, 3.0>
      translate <0.0, -2.0, -1.0>
   }
   rotate 45*y
   translate <70.0, -18.0, -20.0>
}

#declare Lamp =
union {
   object {
      cylinder { -y*18, y*40, 3 }
      texture { This_Brass }
   }
   cylinder { -y*2, y*2, 25
      translate <0.0, -16.0, -5.0>
      texture { This_Brass
         normal { bumps 0.1 }
      }
   }
   intersection {
      object { Cylinder_X scale <1.0, 10.0, 10.0> }
      object { Cylinder_X scale <1.0, 9.95, 9.95> inverse }
      plane { y, 0.0 inverse }
      plane { x, -30.0 inverse }
      plane { x, 30.0 }

      translate <0.0, 35.0, -13.0>

      finish {
         Shiny
         crand 0.05
         ambient 0.5
         diffuse 0.5
         reflection 0.3
         brilliance 4.0
      }
      pigment { DarkGreen }
   }

   union {
      intersection {
         sphere { <-30.0, 35.0, -13.0>, 10.0 }
         sphere { <-30.0, 35.0, -13.0>, 9.95 inverse }
         plane { y, 35.0 inverse }
         plane { x, -30.0 }
      }
      intersection {
         plane { y, 35.0 inverse }
         plane { x, 30.0 inverse }
         sphere { <30.0, 35.0, -13.0>, 10.0 }
         sphere { <30.0, 35.0, -13.0>, 9.95 inverse }
      }
      texture { This_Brass }
   }

   rotate 35*y
   translate <50.0, 0.0, 30.0>
}

#declare Lamp_Light_Source =
light_source { <0, 0, 0> color White

    looks_like {
       cylinder { -x*25, x*25, 2
          pigment { White filter 0 }
          finish { ambient 1 diffuse 0 }
      }
   }

   translate <0.0, 43.0, -10.0>
   rotate 35*y
   translate <50.0, 0.0, 30.0>
}
camera {
   location <0.0, 30.0, -150.0>
   up <0.0, 1.0, 0.0>
   right <4/3, 0.0, 0.0>
   look_at <0.0, 0.0, 0.0>
   rotate<0,-360*(clock+0.10),0>
}
light_source{<150,250,-250> color White*0.50} 

union{
object {
   Cat
   scale 18.3
   rotate <0,-360*(clock+0.10),0>
}

sphere{ <0,0,0>, 9
        texture { pigment{ rgb<1,0,0>}
                  finish { diffuse 0.9
                           phong 1}
                } // end of texture
        translate<31.3, 0, 0>
        rotate <0,360*clock*2,0>
}  
        translate<-20,0,0>
}

#undef BaseTex
#undef CatTex
#undef HairTex
#undef Hair
#undef PupilPos
#undef PupilDiam

object { Back_Wall }
object { Ceiling }
object { Desk_Top }
object { Blotter }
object { Lamp }
object { Lamp_Light_Source }
object { Pencil_Holder }                          
         