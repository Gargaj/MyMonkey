procedure fill(rnd:byte);

const maxx=319;
      maxy=199;
var x:word;

 procedure subdiv(xb,yb,xe,ye:word);
 var u:byte;
     xf,yf:word;
     a:shortint;

 begin
  xf:=(xb+xe) shr 1;
  yf:=(yb+ye) shr 1;

  bg^[xb+yf*320]:=(bg^[xb+yb*320]+bg^[xb+ye*320]) shr 1;
  bg^[xf+yb*320]:=(bg^[xb+yb*320]+bg^[xe+yb*320]) shr 1;
  bg^[xe+yf*320]:=(bg^[xe+yb*320]+bg^[xe+ye*320]) shr 1;
  bg^[xf+ye*320]:=(bg^[xb+ye*320]+bg^[xe+ye*320]) shr 1;

  u:=(bg^[xb+yb*320]+
      bg^[xe+yb*320]+
      bg^[xb+ye*320]+
      bg^[xe+ye*320]) div 4;


  a:=random(rnd*2)-rnd;
       if u+a<0   then bg^[xf+yf*320]:=0
  else if u+a>255 then bg^[xf+yf*320]:=255
  else bg^[xf+yf*320]:=u+a;

{
  move(bg^,mem[$a000:0],64000);
  readln;
}

  if xe-xb<2 then exit;
  if ye-yb<2 then exit;

  subdiv(xb,yb,xf,yf);
  subdiv(xf,yb,xe,yf);
  subdiv(xb,yf,xf,ye);
  subdiv(xf,yf,xe,ye);

 end;

begin

randomize;
bg^[   0+   0*320]:=127;
bg^[   0+maxy*320]:=127;
bg^[maxx+   0*320]:=127;
bg^[maxx+maxy*320]:=127;

subdiv(0,0,maxx,maxy);

for x:=0 to 319 do
 if bg^[x]=0 then
  bg^[x]:=127;


end;


procedure blur;
var x,y:word;
begin
 for x:=1 to 318 do
  for y:=1 to 198 do
   bg^[x+y*320]:=(bg^[x  +  1+y*320]+
                  bg^[x  +(y+1)*320]+
                  bg^[x-1+    y*320]+
                  bg^[x  +(y-1)*320]) shr 2;
{
   bg^[x+y*320]:=(bg^[x  +  1+y*320]+
                  bg^[x  +(y+1)*320]+
                  bg^[x-1+    y*320]+
                  bg^[x  +(y-1)*320]+
                  bg^[x+y*320]) div 5;
}

end;

procedure colorize(r1,g1,b1,r2,g2,b2:byte);
var x:byte;
begin
 for x:=0 to 255 do
  begin
   port[$3c8]:=x;
   port[$3c9]:=((r2-r1)*x) shr 8+r1;
   port[$3c9]:=((g2-g1)*x) shr 8+g1;
   port[$3c9]:=((b2-b1)*x) shr 8+b1;
  end;
end;

procedure setpal(x,r,g,b:byte);
begin
 port[$3c8]:=x;
 port[$3c9]:=r;
 port[$3c9]:=g;
 port[$3c9]:=b;
end;

