1				# all nanobots are the same
15
1:  abs(G0) == 1 && abs(G1) == 1 :0	# still needed in case of bad initial placing
2:  abs(G0) == 2 && abs(G1) == 2 :0	# avoid corners
3:  G0 ==  1			 :1	# info  right
4:  G0 == -1			 :2	# info left
5:  G1 == -1			 :3	# info up
6:  G1 ==  1			 :4	# info down
7:  G0 ==  2  			 :5	# found wander info right
8:  G0 == -2  			 :6	# found wander info left
9:  G1 == -2  			 :7	# found wander info up
10: G1 ==  2 			 :8	# found wander info down
11: G2 < 0			 :!	# 11 - 14 are explicit as such so all the
12: G2 > 0			 :@	# values will show up on the animation
13: G3 > 0			 :#	# file as distinct colors
14: G3 < 0			 :$
15: 1>0				 :0	# everything else doesn't matter
