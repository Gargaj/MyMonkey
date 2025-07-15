const abc='0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ%.';

FUNCTION INTTOSTR(L:LONGINT):STRING;
VAR S:STRING;
Begin
  Str(L,S);
  IntToStr:=S;
End;


procedure drawsprite(x,y,w,h:word; sprite,screen:pointer); assembler;
asm
 lds si,sprite
 les di,screen
 mov ax,y
 mov bx,320
 mul bx
 add ax,x
 mov di,ax
 mov cx,h

 @loop1:
  push cx

  mov cx,w
  rep movsb
  mov ax,320
  sub ax,w
  add di,ax

  pop cx
  loop @loop1
end;


procedure drawspritetranscolor(x,y,w,h:word; u:byte; sprite,screen:pointer); assembler;
asm
 lds si,sprite
 les di,screen
 mov ax,y
 mov bx,320
 mul bx
 add ax,x
 mov di,ax
 mov cx,h
{ mov dl,t}

 @loop1:
  push cx

  mov cx,w

  @loop2:
   cmp byte [ds:si],0
   je @ide

{   movsb}

   mov dl,u
   mov [es:di],dl
{   jmp @oda}

   @ide:

   inc si
   inc di
{   @oda:}


   loop @loop2

  mov ax,320
  sub ax,w
  add di,ax

  pop cx
  loop @loop1
end;

procedure drawspritetrans(x,y,w,h:word; t:byte; sprite,screen:pointer); assembler;
asm
 lds si,sprite
 les di,screen
 mov ax,y
 mov bx,320
 mul bx
 add ax,x
 mov di,ax
 mov cx,h
 mov dl,t

 @loop1:
  push cx

  mov cx,w

  @loop2:
   cmp byte [ds:si],dl
   je @ide

   movsb
   jmp @oda

   @ide:
   inc si
   inc di
   @oda:


   loop @loop2

  mov ax,320
  sub ax,w
  add di,ax

  pop cx
  loop @loop1
end;


Procedure drawboard;
var i,j:byte;
    p:pointer;
begin
 for i:=0 to 24 do
  for j:=0 to 19 do
  begin
   if board[i,j]<>0 then drawsprite(i*9+table_x,j*9+table_y,9,9,@kocka,vscr);
   p:=nil;
   case board[i,j] of
    1..9: drawspritetranscolor(i*9+table_x+2,j*9+table_y+2,6,6,0,@betuk[board[i,j]],vscr);
      -1: drawsprite(i*9+table_x,j*9+table_y,9,9,@fej,vscr);
      -2: drawsprite(i*9+table_x,j*9+table_y,9,9,@csillag,vscr);
   end;
   if p<>nil then
  end;

end;

procedure initgfx;
var x:byte;
    a:word;
begin
  for a:=0 to num_stars do
  begin
   stars[a].x:=random(65535);
   stars[a].y:=random(199);
   stars[a].dx:=random(max_speed shl 8);
   stars[a].plane:=random(3)+1;
  end;

  ufo.x:=330 shl 7;
  ufo.y:=random(190);
  ufo.dx:=(max_speed shl 7);

  for x:=0 to 255 do
  begin
   port[$3c8]:=x;
   port[$3c9]:=pal[x,0] shr 2;
   port[$3c9]:=pal[x,1] shr 2;
   port[$3c9]:=pal[x,2] shr 2;
  end;
end;

procedure drawstars;
var a:word;
begin
  for a:=0 to num_stars do
  begin
   vscr^[stars[a].x shr 7+(stars[a].y)*320]:=stars[a].plane;
{   stars[a].x:=(stars[a].x+stars[a].plane shl 7) mod (320 shl 7);}
   stars[a].x:=(stars[a].x+stars[a].dx shr 7) mod (320 shl 7);
  end;

{  ufo.x:=0;}
  if (ufo.x shr 7)<320 then
  begin
   drawspritetrans(ufo.x shr 7,ufo.y,9,5,0,@ufo_pic,vscr);
   ufo.x:=ufo.x+ufo.dx shr 7;
  end
  else
   if random(1000)>998 then
   begin
    ufo.x:=0;
    ufo.y:=random(190);
   end;
end;

procedure drawwindow(xb,yb,xe,ye:word);
var x,y:word;
begin
 for x:=xb to xe do
  for y:=yb to ye do
  begin
   vscr^[x+y*320]:=6;
   if (x=xb) or (y=yb) then vscr^[x+y*320]:=7;
   if (x=xe) or (y=ye) then vscr^[x+y*320]:=8;
  end;
end;

procedure drawtext(x,y:word; s:string);
var a:byte;
begin
 for a:=1 to length(s) do
  if pos(upcase(s[a]),abc)>0 then
   drawspritetranscolor(x+(a-1)*6,y,6,6,0,@betuk[pos(upcase(s[a]),abc)-1],vscr);
end;

procedure drawbuttons;
begin
 drawwindow(3, 60,40, 70); drawtext(13, 63,'New');
 drawwindow(2, 80,40, 90); drawtext( 4, 83,'Top 20');
 drawwindow(3,160,40,170); drawtext(11,163,'Exit');
end;

procedure drawscore;
var s:string;
    k:byte;
    a:byte;
begin
 drawwindow(3,28,40,50);

 s:=IntToStr(point);
 k:=length(s);
 drawtext(13+(3-k)*6,34,s);

 s:=IntToStr(point div 5)+'.'+IntToStr((point mod 5)*2)+'%';
 k:=length(s);
 drawtext(13+(4-k)*6,34+6,s);
end;

procedure putdot(x,y,x1,x2,x3,y1,y2,y3,c:word);
begin
 vscr^[round(sin(c/x1+x3)*x2+x)+round(sin(c/y1+y3)*y2+y)*320]:=255;
end;