String userinput = "";
int testcase = -1;
 float max_x, max_y;
 float min_x, min_y;
 float move_from_top = 40;//pixels
 float move_from_left = 10;//pixels
void setup(){
  size(1600,1000);
  background(0);
  println(">Enter a test case (1-30)\n");
}

void draw () {
  if (testcase >= 0) {
     clear();
  //drawRobots();
  //drawPolygons();
  drawPositions();
  drawMousePosition();
  drawPath();
  }
}

void drawPath()
{
  float x[]={255, 0, 0, 255, 0, 255, 192, 128, 128, 128, 0, 128, 0, 0,128,139,165,178,220,255,255,255,205,240,233,250,255,255,255,255,255,184,218,238,189,240,128,255,154,85,107,124,127,173,0,0,34,0,50,144,152,143,0,0,46,102,60,32,47,0,0,0,0,224,0,64,72,175,127,176,95,70,100,0,30,173,135,135,25,0,0,0,0,65,138,75,72,106,123,147,139,148,153,186,128,216,221,238,255,218,199,219,255,255,255,255,250,245,255,255,245,255,255,250,255,139,160,210,205,244,222,210,188,255,255,255,255,255,250,253,255,255,245,112,119,176,230,255,240,248,240,255,240,255,0,105,128,169,192,211,220};
  float y[]={0, 255, 0, 255, 255, 0, 192, 128, 0, 128, 128, 0, 128, 0,255,139,160,210,205,244,222,210,188,255,255,255,255,255,250,253,255,255,245,112,119,176,230,255,240,248,240,255,240,255,0,105,128,169,192,211,220,188,250,255,139,205,179,178,79,128,139,255,255,255,206,224,209,238,255,224,158,130,149,191,144,216,206,206,25,0,0,0,0,105,43,0,61,90,104,112,0,0,50,85,0,191,160,130,0,112,21,112,20,105,182,192,235,245,228,235,222,248,250,250,255,69,82,105,133,164,184,180,143,228,222,218,228,240,240,245,239,245,255,128,136,196,230,250,248,248,255,255,255,250,0,105,128,169,192,211,220};
  float z[]={0, 0, 255, 0, 255, 255, 192, 128, 0, 0, 0, 128, 128, 128,255,69,82,105,133,164,184,180,143,228,222,218,228,240,240,245,239,245,255,128,136,196,230,250,248,248,255,255,255,250,0,105,128,169,192,211,220,143,154,127,87,170,113,170,79,128,139,255,255,255,209,208,204,238,212,230,160,180,237,255,255,230,235,250,112,128,139,205,255,225,226,130,139,205,238,219,139,211,204,211,128,216,221,238,255,214,133,147,147,180,193,203,215,220,196,205,179,220,205,210,224,19,45,30,63,96,135,140,143,181,173,185,225,245,230,230,213,238,250,144,153,222,250,240,255,255,240,240,255,250,0,105,128,169,192,211,220};
  String[] robotCases = loadStrings("output.txt");
  float[] pathXY = float(robotCases[testcase].split(" "));
  int i=0, index=0;
  i=2;
  stroke(255);

  while(i + 1 < pathXY.length) {
    float robotx = pathXY[i-2]+min_x;
    float roboty = pathXY[i-1]+min_y;
    float robotx1 = pathXY[i]+min_x;
    float roboty1 = pathXY[i+1]+min_y;

    if (pathXY[i-1]!= 123456 && pathXY[i]!= 123456){
    line(robotx/max_x*(width-move_from_left)+move_from_left, roboty/max_y*(height-move_from_top)+move_from_top,
        robotx1/max_x*(width-move_from_left)+move_from_left, roboty1/max_y*(height-move_from_top)+move_from_top    );
    } 
    else { 
    stroke(x[index],y[index],z[index]); 
    index++;
    index=index%151;
    }
     i+=2;
  }
  stroke(0);
}

void drawRobot(float x, float y) {
  fill(255,0,0);
  ellipse(x, y, 7, 7);
  fill(100);
  
}

void drawMousePosition()
{
 
  textSize(15);
  fill(0,255,0);
  text("X: "+((mouseX-move_from_left)*max_x/(width-move_from_left)-min_x), 5, 15); 
  text("Y: "+((mouseY-move_from_top)*max_y/(height-move_from_top)-min_y), 150, 15);

}

void drawPositions() {
  // get robot coordinates from file and split them by test cases
  String[] robotCases = loadStrings("polyrobots.txt");
  float[] robotXY = float(robotCases[testcase].split(" "));
  
  // get polygon coordinates from file and split them by test cases
  String[] polCases = loadStrings("polygon.txt");
  float[] polXY = float(polCases[testcase].split(" "));
  
  // max x and y common to robots and obstacles
  int i = 0; 
  max_x=-9999;
  max_y=-9999;
  min_x=99999;
  min_y=99999;

  
  
  
  while(i < robotXY.length) {
    float robotx = robotXY[i];
    i++;
    float roboty = robotXY[i];
    i++;
    if (robotx<min_x) min_x = robotx;
    if (roboty<min_y) min_y = roboty;
    
  } 
  // check min x and y from polygons
  int polNumber = polXY.length;
  i=0;
  if (polNumber>=2)
    while(i < polNumber) {
      float polx = polXY[i];
      i++;
      float poly = polXY[i];
      i++;
      if (polx<min_x) min_x = polx;
      if (poly<min_y) min_y = poly;
      
    } 
  
  if (min_x<0) min_x = min_x * (-1); else min_x = 0;
  if (min_y<0) min_y = min_y * (-1); else min_y = 0;
  
  // find out maximu in robots
  i=0;
  while(i < robotXY.length) {
    float robotx = robotXY[i]+min_x;
    i++;
    float roboty = robotXY[i]+min_y;
    i++;
    if (robotx>max_x) max_x = robotx;
    if (roboty>max_y) max_y = roboty;
    
  } 
    // find out maximum in polygons too without the 999999
    i=0;
    if (polNumber>=2)
    while(i < polNumber) {
      if (polXY[i]!=999999999){
      float polx = polXY[i]+min_x;
      i++;
      float poly = polXY[i]+min_y;
      if (polx>max_x) max_x = polx;
      if (poly>max_y) max_y = poly;
      i++;
      }
      i+=2;
    } 
  // draw
  
  max_x = max_x*1.08;
  max_y = max_y*1.08;
  
  
  
  // draw polygons
  
  fill(100);
  beginShape();
  i=0;
  if (polNumber>=2)  
  while(i < polXY.length) {

    if (polXY[i] == 999999999) {
        endShape();
        //fill(random(0,200),random(0,200),random(0,200));
        i+=2;
        beginShape();
    }
      
    else {
      float polx = polXY[i]+min_x;
      i++;
      float poly = polXY[i]+min_y;
      i++;
      vertex(polx/max_x*(width-move_from_left)+move_from_left, poly/max_y*(height-move_from_top)+move_from_top);
    }
  }
  // draw robots
  i=0;
  while(i < robotXY.length) {
    float robotx = robotXY[i]+min_x;
    i++;
    float roboty = robotXY[i]+min_y;
    i++;
    drawRobot(robotx/max_x*(width-move_from_left)+move_from_left, roboty/max_y*(height-move_from_top)+move_from_top);
  }
  
 endShape();
  //exit();
  
}


void keyPressed(){
  clear();
  background(0);
  if (key == ENTER && userinput!="") {
    int keyNum = Integer.parseInt(userinput);
    userinput = "";
    
    if(keyNum<=30 && keyNum>0) {
      
      testcase = keyNum - 1;
      println("Test Case " + keyNum);
    }
  }
  else {
    if (key>='0' && key<='9') userinput = userinput + key;
  } 
  
}

/* 
   colors:
   
   Red  #FF0000  (255,0,0)
   Lime  #00FF00  (0,255,0)
   Blue  #0000FF  (0,0,255)
   Yellow  #FFFF00  (255,255,0)
   Cyan / Aqua  #00FFFF  (0,255,255)
   Magenta / Fuchsia  #FF00FF  (255,0,255)
   Silver  #C0C0C0  (192,192,192)
   Gray  #808080  (128,128,128)
   Maroon  #800000  (128,0,0)
   Olive  #808000  (128,128,0)
   Green  #008000  (0,128,0)
   Purple  #800080  (128,0,128)
   Teal  #008080  (0,128,128)
   Navy  #000080  (0,0,128)
   maroon  #800000  (128,0,0)
   dark red  #8B0000  (139,0,0)
   brown  #A52A2A  (165,42,42)
   firebrick  #B22222  (178,34,34)
   crimson  #DC143C  (220,20,60)
   red  #FF0000  (255,0,0)
   tomato  #FF6347  (255,99,71)
   coral  #FF7F50  (255,127,80)
   indian red  #CD5C5C  (205,92,92)
   light coral  #F08080  (240,128,128)
   dark salmon  #E9967A  (233,150,122)
   salmon  #FA8072  (250,128,114)
   light salmon  #FFA07A  (255,160,122)
   orange red  #FF4500  (255,69,0)
   dark orange  #FF8C00  (255,140,0)
   orange  #FFA500  (255,165,0)
   gold  #FFD700  (255,215,0)
   dark golden rod  #B8860B  (184,134,11)
   golden rod  #DAA520  (218,165,32)
   pale golden rod  #EEE8AA  (238,232,170)
   dark khaki  #BDB76B  (189,183,107)
   khaki  #F0E68C  (240,230,140)
   olive  #808000  (128,128,0)
   yellow  #FFFF00  (255,255,0)
   yellow green  #9ACD32  (154,205,50)
   dark olive green  #556B2F  (85,107,47)
   olive drab  #6B8E23  (107,142,35)
   lawn green  #7CFC00  (124,252,0)
   chart reuse  #7FFF00  (127,255,0)
   green yellow  #ADFF2F  (173,255,47)
   dark green  #006400  (0,100,0)
   green  #008000  (0,128,0)
   forest green  #228B22  (34,139,34)
   lime  #00FF00  (0,255,0)
   lime green  #32CD32  (50,205,50)
   light green  #90EE90  (144,238,144)
   pale green  #98FB98  (152,251,152)
   dark sea green  #8FBC8F  (143,188,143)
   medium spring green  #00FA9A  (0,250,154)
   spring green  #00FF7F  (0,255,127)
   sea green  #2E8B57  (46,139,87)
   medium aqua marine  #66CDAA  (102,205,170)
   medium sea green  #3CB371  (60,179,113)
   light sea green  #20B2AA  (32,178,170)
   dark slate gray  #2F4F4F  (47,79,79)
   teal  #008080  (0,128,128)
   dark cyan  #008B8B  (0,139,139)
   aqua  #00FFFF  (0,255,255)
   cyan  #00FFFF  (0,255,255)
   light cyan  #E0FFFF  (224,255,255)
   dark turquoise  #00CED1  (0,206,209)
   turquoise  #40E0D0  (64,224,208)
   medium turquoise  #48D1CC  (72,209,204)
   pale turquoise  #AFEEEE  (175,238,238)
   aqua marine  #7FFFD4  (127,255,212)
   powder blue  #B0E0E6  (176,224,230)
   cadet blue  #5F9EA0  (95,158,160)
   steel blue  #4682B4  (70,130,180)
   corn flower blue  #6495ED  (100,149,237)
   deep sky blue  #00BFFF  (0,191,255)
   dodger blue  #1E90FF  (30,144,255)
   light blue  #ADD8E6  (173,216,230)
   sky blue  #87CEEB  (135,206,235)
   light sky blue  #87CEFA  (135,206,250)
   midnight blue  #191970  (25,25,112)
   navy  #000080  (0,0,128)
   dark blue  #00008B  (0,0,139)
   medium blue  #0000CD  (0,0,205)
   blue  #0000FF  (0,0,255)
   royal blue  #4169E1  (65,105,225)
   blue violet  #8A2BE2  (138,43,226)
   indigo  #4B0082  (75,0,130)
   dark slate blue  #483D8B  (72,61,139)
   slate blue  #6A5ACD  (106,90,205)
   medium slate blue  #7B68EE  (123,104,238)
   medium purple  #9370DB  (147,112,219)
   dark magenta  #8B008B  (139,0,139)
   dark violet  #9400D3  (148,0,211)
   dark orchid  #9932CC  (153,50,204)
   medium orchid  #BA55D3  (186,85,211)
   purple  #800080  (128,0,128)
   thistle  #D8BFD8  (216,191,216)
   plum  #DDA0DD  (221,160,221)
   violet  #EE82EE  (238,130,238)
   magenta / fuchsia  #FF00FF  (255,0,255)
   orchid  #DA70D6  (218,112,214)
   medium violet red  #C71585  (199,21,133)
   pale violet red  #DB7093  (219,112,147)
   deep pink  #FF1493  (255,20,147)
   hot pink  #FF69B4  (255,105,180)
   light pink  #FFB6C1  (255,182,193)
   pink  #FFC0CB  (255,192,203)
   antique white  #FAEBD7  (250,235,215)
   beige  #F5F5DC  (245,245,220)
   bisque  #FFE4C4  (255,228,196)
   blanched almond  #FFEBCD  (255,235,205)
   wheat  #F5DEB3  (245,222,179)
   corn silk  #FFF8DC  (255,248,220)
   lemon chiffon  #FFFACD  (255,250,205)
   light golden rod yellow  #FAFAD2  (250,250,210)
   light yellow  #FFFFE0  (255,255,224)
   saddle brown  #8B4513  (139,69,19)
   sienna  #A0522D  (160,82,45)
   chocolate  #D2691E  (210,105,30)
   peru  #CD853F  (205,133,63)
   sandy brown  #F4A460  (244,164,96)
   burly wood  #DEB887  (222,184,135)
   tan  #D2B48C  (210,180,140)
   rosy brown  #BC8F8F  (188,143,143)
   moccasin  #FFE4B5  (255,228,181)
   navajo white  #FFDEAD  (255,222,173)
   peach puff  #FFDAB9  (255,218,185)
   misty rose  #FFE4E1  (255,228,225)
   lavender blush  #FFF0F5  (255,240,245)
   linen  #FAF0E6  (250,240,230)
   old lace  #FDF5E6  (253,245,230)
   papaya whip  #FFEFD5  (255,239,213)
   sea shell  #FFF5EE  (255,245,238)
   mint cream  #F5FFFA  (245,255,250)
   slate gray  #708090  (112,128,144)
   light slate gray  #778899  (119,136,153)
   light steel blue  #B0C4DE  (176,196,222)
   lavender  #E6E6FA  (230,230,250)
   floral white  #FFFAF0  (255,250,240)
   alice blue  #F0F8FF  (240,248,255)
   ghost white  #F8F8FF  (248,248,255)
   honeydew  #F0FFF0  (240,255,240)
   ivory  #FFFFF0  (255,255,240)
   azure  #F0FFFF  (240,255,255)
   snow  #FFFAFA  (255,250,250)
   black  #000000  (0,0,0)
   dim gray / dim grey  #696969  (105,105,105)
   gray / grey  #808080  (128,128,128)
   dark gray / dark grey  #A9A9A9  (169,169,169)
   silver  #C0C0C0  (192,192,192)
   light gray / light grey  #D3D3D3  (211,211,211)
   gainsboro  #DCDCDC  (220,220,220)
   */