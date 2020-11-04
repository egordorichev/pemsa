pico-8 cartridge // http://www.pico-8.com
version 19
__lua__
x, y = 64, 64
u, v = 32, 32
function _update60()
	if(btn(0)) x-=1
	if(btn(1)) x+=1
	if(btn(2)) y-=1
	if(btn(3)) y+=1

	if(btn(0, 1)) u-=1
	if(btn(1, 1)) u+=1
	if(btn(2, 1)) v-=1
	if(btn(3, 1)) v+=1
end

function _draw()
 cls()
 _G[(not btn(5) and "circ" or "circfill")](x, y, 8 + cos(t() * 0.7) * 3, 10)
 _G[(not btn(5, 1) and "circ" or "circfill")](u, v, 8 + sin(t()) * 2, 8)
end