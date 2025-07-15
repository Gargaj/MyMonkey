procedure loadscorez(f:string);
var mf:file;
    i:byte;
    k:word;
    s:string;
    raw:array[0..320] of byte;
begin
 assign(mf,scorefile);
 {$I-}
 reset(mf,1);
 {$I+}
 if ioresult<>0 then
 begin
  for i:=0 to 19 do
  begin
   a[i].name:=emptyname;
   a[i].score:=10*(20-i);
  end;
 end
 else
 begin
  blockread(mf,raw,filesize(mf));
  k:=0; s:='';
  for i:=0 to 19 do
  begin
   k:=i*16;
   s:='';
   repeat
    s:=s+chr(raw[k]);
    inc(k);
   until (raw[k]=0) or (k mod 16=14);

   a[i].name:=s;
   move(raw[i*16+14],a[i].score,2);
  end;
 end;

 close(mf);

end;


function savescorez(f:string):boolean;
var mf:file;
    i:byte;
    j:word;
    s:string;
    raw:array[0..320] of byte;
begin
 fillchar(raw,sizeof(raw),0);
 for i:=0 to 19 do
 begin
  for j:=1 to length(a[i].name) do
   raw[i*16+j-1]:=ord(a[i].name[j]);
  move(a[i].score,raw[i*16+14],2);
 end;

 assign(mf,scorefile);
 rewrite(mf,1);
 blockwrite(mf,raw,sizeof(raw));
 close(mf);
end;


procedure highscorelist(u:byte);
var i:byte;
    j,k,w:word;
    s:string;
    mf:file;
    raw:array[0..16*20-1] of byte;

const pos_x=60;
      pos_y=32;
begin
 loadscorez(scorefile);

 i:=0;

 colorize(0,0,63,0,63,63);
 setpal(0,0,0,0);
 setpal(255,63,63,63);
 move(bg^,vscr^,64000);
 move(vscr^,mem[$a000:0],64000);

  for i:=0 to 19 do
  begin

   s:=a[i].name;
   w:=a[i].score;

   k:=length(s);
   drawtext(pos_x+30,pos_y+i*7,s);

   s:=IntToStr(i+1)+'.';
   k:=length(s);
   drawtext(pos_x+(3-k)*6,pos_y+i*7,s);

   s:=IntToStr(w);
   k:=length(s);
   drawtext(pos_x+125+(4-k)*6,pos_y+i*7,s);

   s:=IntToStr(w div 5)+'.'+IntToStr((w mod 5)*2)+'%';
   k:=length(s);
   drawtext(pos_x+170+(4-k)*6,pos_y+i*7,s);

  end;

 move(vscr^,temp^,64000);
 j:=500;
 repeat
  move(temp^,vscr^,64000);

  for i:=1 to 50 do
  begin
{
   putdot(160,100,8,80,i,8,50,i,j);
   putdot(290, 20,10,8,i,8,8,i,j);
   putdot( 30,180,10,8,i,8,8,i,j);
   putdot(290,180,10,8,i,8,8,i,j);
}
   if j>i*10 then
    vscr^[sint[byte(j-i*10)]+sint[byte((j+64-i*10) div 5)]*320+96+37*320]:=255;

  end;

  getkey;
  getmouse;
  drawspritetranscolor(mx,my,5,7,255,@nyil,vscr);

  inc(j);
  move(vscr^,mem[$a000:0],64000);
 until (ch1=#27) or (ch1=#13) or ((lb and 1)>0) ;
 ch1:=#0;

 initgfx;
end;

procedure highscoresave;
var i,j,u:byte;
    k:word;
    st:string[18];
    s:string;
    mf:file;
    af:highscr;
    b:boolean;
    raw:array[0..16*20-1] of byte;
begin

 loadscorez(scorefile);

 b:=false;
 for i:=1 to 19 do
  if a[i].score<=point then
  begin
   b:=(point>1);
  end;

 if not b then exit;

 i:=0;

 st:=getstring(116,99,202,107,14);

 a[19].name:=st;
 a[19].score:=point;
 u:=19;

 for i:=1 to 19 do
  for j:=i-1 downto 0 do
   if a[j].score < a[j+1].score then
   begin
    af:=a[j];
    a[j]:=a[j+1];
    a[j+1]:=af;
    if u=j+1 then dec(u);
   end;

{ savescorez(scorefile);}

 highscorelist(u);
end;
