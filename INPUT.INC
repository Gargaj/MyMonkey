procedure getmouse;
var mb:word;
begin
 asm
  mov ax,3
  int 33h
  mov mb,bx
  shr cx,1

  mov mx,cx
  mov my,dx
 end;

 lb:=mb and 1;
 rb:=byte(boolean(mb and 2));

 if mx>315 then mx:=315;
 if my>195 then my:=195;

 if (lb=0) then mcmd:=0;
 if (mcmd=1) then lb:=0;
 if (mcmd=0) then mcmd:=lb;
end;

procedure getkey;
var c:char;
begin
  ch1:=#0;
  ch2:=#0;
  if keypressed then
  begin
    c:=readkey;
    if c<>#0 then
    begin
     ch1:=c;
     ch2:=#0;
    end
    else
    begin
     ch1:=#0;
     c:=readkey;
     ch2:=c;
    end;
  end;
end;

function getstring(xb,yb,xe,ye:word; l:byte):string;
var x,y:word;
    c:char;
    s:string;
    b:boolean;
begin
 s:='';
 repeat
  fillchar(vscr^,sizeof(vscr^),0);
  drawstars;
  drawboard;
  drawbuttons;
  drawscore;
  drawwindow(111,89,208,111);
  drawtext(115,92,'Enter your name');

  for x:=xb to xe do
   for y:=yb to ye do
    vscr^[x+y*320]:=8;

  drawtext(xb+2,yb+2,s);
  getkey;
  if (((ch1>='a') and (ch1<='z')) or
      ((ch1>='A') and (ch1<='Z'))) and
     (l>length(s)) then
   s:=s+upcase(ch1);

  if ch1=#8 then s:=copy(s,1,length(s)-1);

  getmouse;
  drawspritetranscolor(mx,my,5,7,4,@nyil,vscr);

  for x:=0 to 4 do
   vscr^[xb+2+length(s)*6+(x+yb+2)*320]:=9;

  move(vscr^,mem[$a000:0],64000);
 until (ch1=#13) or (ch1=#27);

 if ch1=#27 then s:='';
 getstring:=s;

end;

procedure initmouse; assembler;
asm
 xor ax,ax
 int 33h
end;

