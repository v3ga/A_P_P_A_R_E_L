
size(500, 500);
background(0);
noStroke();
int dim = 9;
float w = float(height)/dim;
float x=0, y=0;
for (float j=0;j<dim;j++)
{
  x=0;
  for (float i=0;i<dim;i++)
  {
    fill((i+j)%2 == 0 ? 255 : 0);
    rect(x, y, w, w);
    x+=w;
  }
  y+=w;
}

saveFrame("tile_"+dim+"x"+dim+".png");


