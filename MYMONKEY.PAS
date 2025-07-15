Program Monkey;

uses crt;

const scorefile='MyMonkey.dat';
const emptyname='..............';
const num_stars=1000;
      table_x=47;
      table_y=10;
      max_speed=200;
      max_x=320;
      max_y=200;


type highscr=record
      Name:string[18];
      Score:word;
     end;

type tscr=array[0..63999] of byte;

type Tstar=record
      x,y:word;
      dx:word;
      plane:byte;
     end;

var board:array[0..24,0..19] of shortint;
    jx,jy,MCmd:byte;
    i,j:shortint;
    valid:boolean;
    point:word;
    vscr,bg,temp:^tscr;
    stars:array[0..num_stars] of Tstar;
    ufo:Tstar;
    ch1,ch2:char;
    mx,my,lb,rb:word;
    x,y,x_,y_:byte;
    a:array[0..19] of highscr;
    s:string;
    sint:array[0..255] of byte;


{$I gfx.inc}
{$I display.inc}
{$I input.inc}
{$I generate.inc}
{$I loadsave.inc}

procedure generate;
var i,j:byte;
begin
 randomize;
 for i:=0 to 24 do
  for j:=0 to 19 do
   board[i,j]:=random(9)+1;

 for i:=0 to random(25) do
  board[random(25),random(20)]:=-2;

 jx:=random(25);
 jy:=random(20);

 board[jx,jy]:=-1;
 point:=0;

end;

function check(x,y:shortint):boolean;
var i,j:byte;
begin
 { Check if move is valid }
 if board[jx+x,jy+y]<=0 then
 begin
  check:=false;
  exit;
 end;

 for i:=1 to board[jx+x,jy+y] do
  if ((board[jx+x*i,jy+y*i]) <= 0) or
     (jx+x*i<0) or (jx+x*i>24) or
     (jy+y*i<0) or (jy+y*i>19) then
  begin
   check:=false;
   exit;
  end;
 check:=true;
end;

procedure move_(x,y:shortint);
var i,j:byte;
begin
 for i:=0 to board[jx+x,jy+y] do
  board[jx+x*i,jy+y*i]:=0;

 board[jx+x*i,jy+y*i]:=-1;

 jx:=jx+x*i;
 jy:=jy+y*i;

end;

function range(v,l,u:word):boolean;
begin
 range:=(v>l) and (v<u);
end;

function count:word;
var i,j:byte;
    x:word;
begin
 x:=0;
 for i:=0 to 24 do
  for j:=0 to 19 do
   if (board[i,j]=0) or (board[i,j]=-1) then inc(x);

 count:=x;
end;

Begin

new(vscr);
if vscr=nil then halt;
new(bg);
if bg=nil then halt;
new(temp);
if temp=nil then halt;

for x:=0 to 255 do sint[x]:=round(sin(x*pi/128)*64+64);

asm
 mov ax,13h
 int 10h
end;

 fillchar(bg^,sizeof(bg^),0);
 fillchar(vscr^,sizeof(vscr^),0);

 generate;
 initgfx;
 fill(30);
 point:=1;
 initmouse;

  repeat

    fillchar(vscr^,sizeof(vscr^),0);
    drawstars;
    drawboard;
    point:=count;

    drawscore;
    drawbuttons;


    getkey;
    getmouse;

     if ((range(mx,3,40)) and
         (range(my,60,70)) and
         ((lb and 1)>0)) or
         ((ch1=#0) and (ch2=#60)) then
     begin
      highscoresave;
      generate;
     end;

     if (range(mx,2,40)) and
        (range(my,80,90)) and
        ((lb and 1)>0) then
         highscorelist(255);

     if ((range(mx,3,40)) and
         (range(my,160,170)) and
         ((lb and 1)>0)) or
        (ch1=#27) then
     begin
      highscoresave;
      asm
       mov ax,3
       int 10h
      end;
      dispose(vscr);
      dispose(bg);
      dispose(temp);
      halt(0);
     end;

    if ((lb and 1)>0) then
    begin
     x:=(mx-table_x) div 9;
     y:=(my-table_y) div 9;
     if (abs(jx-x)<=1) and (abs(jy-y)<=1) then
      if check(x-jx,y-jy) then
       move_(x-jx,y-jy);
    end;

    if (rb>0) then
    begin
     x:=(mx-table_x) div 9;
     y:=(my-table_y) div 9;
     if (abs(jx-x)<=1) and (abs(jy-y)<=1) then
      if check(x-jx,y-jy) then
      begin
       x_:=board[x,y]*(x-jx)+jx;
       y_:=board[x,y]*(y-jy)+jy;
       drawspritetranscolor(table_x+x_*9+2,
                            table_y+y_*9+2,
                            6,6,10,@betuk[board[x_,y_]],vscr);
      end;
    end;

    drawspritetranscolor(mx,my,5,7,4,@nyil,vscr);

    case upcase(ch1) of
      '7': if check(-1,-1) then move_(-1,-1);
      '8': if check( 0,-1) then move_( 0,-1);
      '9': if check( 1,-1) then move_( 1,-1);
      '4': if check(-1, 0) then move_(-1, 0);
      '6': if check( 1, 0) then move_( 1, 0);
      '1': if check(-1, 1) then move_(-1, 1);
      '2': if check( 0, 1) then move_( 0, 1);
      '3': if check( 1, 1) then move_( 1, 1);
      'H': highscorelist(255);
    end;

    valid:=false;

    for i:=-1 to 1 do
     for j:=-1 to 1 do
      if not ((i=0) and (j=0)) then
       valid:=valid or check(i,j);

    if (not valid) then
    begin
     point:=count;
     repeat
      fillchar(vscr^,sizeof(vscr^),0);
      drawstars;
      drawboard;
      drawbuttons;
      drawscore;

      drawwindow(126,93,193,105);
      drawtext(133,97,'Game over');

      getmouse;
      drawspritetranscolor(mx,my,5,7,4,@nyil,vscr);
      move(vscr^,mem[$a000:0],64000);
     until keypressed or ((lb and 1)>0);
     ch1:=#0; ch2:=#0;
     highscoresave;
     generate;
    end;


    move(vscr^,mem[$a000:0],64000);

  until 1=3;

end.
